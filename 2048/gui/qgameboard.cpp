#include "gui/qgameboard.h"
#include "core/board.h"
#include "core/game.h"
#include "gui/qtile.h"
#include "core/tile.h"
#include "gui/qresetbutton.h"

#include <QVBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QKeyEvent>
#include <QVBoxLayout>
#include <QString>

#include <QDebug>
#include "qgameboard.h"


QGameBoard::~QGameBoard()
{
	delete game;
}

QGameBoard::QGameBoard(QWidget* parent) :
	QWidget(parent)
{
	// set default size
	resize(650, 450);

	// create the main layout
	mainLayout = new QVBoxLayout();
	setLayout(mainLayout);

	// will be created in drawBoard()
	boardLayout = NULL;

	// create the game and register as observer
	game = new Game(4);
	game->registerObserver(this);

	// create the gui board and draw it
	gui_board.resize(game->getGameBoard()->getDimension());
	for (int i = 0; i < game->getGameBoard()->getDimension(); ++i)
		gui_board[i].resize(game->getGameBoard()->getDimension());
	for (int i = 0; i < game->getGameBoard()->getDimension(); ++i)
		for (int j = 0; j < game->getGameBoard()->getDimension(); ++j)
			gui_board[i][j] = NULL;
	drawBoard();

	// create the score widget and add it to the board
	score = new QLabel(QString("SCORE: %1").arg(game->getScore()));
	score->setStyleSheet("QLabel { color: rgb(235,224,214); font: 16pt; }");
	score->setFixedHeight(50);
	mainLayout->insertWidget(1, score, 0, Qt::AlignRight);

	// style sheet of the board
	setStyleSheet("QGameBoard { background-color: rgb(187,173,160) }");

	connect(gameOverWindow.getResetBtn(), SIGNAL(clicked()), this, SLOT(resetGame()));

	neuralNetwork = std::make_unique<Net>(neuralNetTopology);
}

void QGameBoard::keyPressEvent(QKeyEvent* event)
{
	//All this will be done on a seperate thread.
	//NN Variable needed.
	//Topology: 16-??-4
	//Method that will read the game board state and determine the input for the NN.
	//Method to emulate keypress via a NN.
	switch (event->key()) {
	case Qt::Key_Up:
		game->move(UP);
		break;
	case Qt::Key_Left:
		game->move(LEFT);
		break;
	case Qt::Key_Right:
		game->move(RIGHT);
		break;
	case Qt::Key_Down:
		game->move(DOWN);
		break;
	case Qt::Key_Space:
		LoopNeuralNet();
		break;
	}
}

void QGameBoard::notify()
{
	if (game->isGameOver())
		gameOverWindow.show();

	if (game->won())
		score->setText(QString("You hit 2048, congratulations! Keep playing to increase your score.\t\t SCORE: %1").arg(game->getScore()));
	else
		score->setText(QString("SCORE: %1").arg(game->getScore()));

	drawBoard();
}

void QGameBoard::LoopNeuralNet()
{
	auto gameState = game->getGameBoard()->GetBoardState();

	//Converting the game state to a std::vector for the FeedForward.
	std::vector<double> neuralInputs{};
	SanitizeInputs(gameState, neuralInputs);
	neuralNetwork->FeedForward(neuralInputs);

	//These are the results, the network came up with.
	std::vector<double> resultValues;												
	neuralNetwork->GetResults(resultValues);		  								

	std::vector<double> targetOutputs(4, 0);

	for (int i = 0; i < targetOutputs.size(); ++i)
	{
		std::unique_ptr<Board> boardCopy = std::make_unique<Board>(*game->getGameBoard());
		targetOutputs[i] = SimulateMovement(static_cast<Direction>(i), boardCopy.get()) / static_cast<double>(gameState.size());
	}

	SetAllButHighestToZero(targetOutputs);

	//Telling the network what the outputs should have been.											  
	neuralNetwork->BackProp(targetOutputs);


	ApplyNeuralNetworkMove(resultValues);

}

void QGameBoard::ApplyNeuralNetworkMove(const std::vector<double>& targetOutput)
{
	int idxMaxVal{ 0 };
	for (int i = 0; i < targetOutput.size(); ++i)
	{
		if (i + 1 < targetOutput.size())
		{
			if (targetOutput[i] > targetOutput[i + 1])
				idxMaxVal = i;
		}
	}

	game->move(static_cast<Direction>(idxMaxVal));
}

void QGameBoard::SetAllButHighestToZero(std::vector<double>& targetOutputs) {
	// Check if the vector is empty
	if (targetOutputs.empty()) {
		return; // Nothing to do
	}

	// Find the highest value in the vector
	double highestValue = targetOutputs[0];
	for (int i = 1; i < targetOutputs.size(); ++i) {
		if (targetOutputs[i] > highestValue) {
			highestValue = targetOutputs[i];
		}
	}


	bool highestValueSet{ false };
	// Set all values other than the highest one to 0
	for (int i = 0; i < targetOutputs.size(); ++i) {
		if (targetOutputs[i] != highestValue) {
			targetOutputs[i] = 0;
		}
		else if (!highestValueSet)
		{
			highestValueSet = true;
			targetOutputs[i] = 1;
			previousDirection = static_cast<Direction>(i);
		}
		else
		{
			targetOutputs[i] = 0;
		}

	}
}


int QGameBoard::SimulateMovement(Direction direction, Board* board)
{

	if (previousDirection == direction)
		return 0;


	board->move(direction);


	int emptySpaceAmount{0};
	auto boardState = board->GetBoardState();
	for (auto& idx : boardState)
	{
		if (idx == 0)
			++emptySpaceAmount;
	}

	return emptySpaceAmount;
}


int QGameBoard::SolveRow(int elem1, int elem2, int elem3, int elem4)
{
	if (elem1 == 0 && elem2 == 0 && elem3 == 0 && elem4 == 0)
		return 4;

	CombineAdjacentElements(elem1, elem2, elem3, elem4);

	MoveElements(elem1, elem2, elem3, elem4);

	int nullSpaceAmount{ 0 };
	if (elem1 == 0)
		++nullSpaceAmount;
	if (elem2 == 0)
		++nullSpaceAmount;
	if (elem3 == 0)
		++nullSpaceAmount;
	if (elem4 == 0)
		++nullSpaceAmount;

	return nullSpaceAmount;
}

void QGameBoard::SanitizeInputs(const QVector<int>& inputVector, std::vector<double>& outputVector)
{
	outputVector.clear();
	outputVector.resize(inputVector.size());
	
	double maxValue = *std::max_element(inputVector.begin(), inputVector.end());

	for (int i = 0; i < inputVector.size(); ++i)
	{
		outputVector[i] = inputVector[i] / maxValue;
	}
}

void QGameBoard::CombineAdjacentElements(int& elem1, int& elem2, int& elem3, int& elem4)
{
	if (elem1 == elem2)						 
	{										 
		elem1 *= 2;							 
		elem2 = elem3;						 
		elem3 = elem4;						 
		elem4 = 0;							 
	}										 
	else if (elem2 == elem3)				 
	{										 
		elem2 *= 2;							 
		elem3 = elem4;						 
		elem4 = 0;							 
	}
	else if (elem3 == elem4)
	{
		elem3 *= 2;
		elem4 = 0;
	}
}

void QGameBoard::MoveElements(int& elem1, int& elem2, int& elem3, int& elem4)
{
	// Move elements towards the right
	if (elem3 == 0)
	{
		elem3 = elem4;
		elem4 = 0;
	}
	if (elem2 == 0)
	{
		elem2 = elem3;
		elem3 = elem4;
		elem4 = 0;
	}
	if (elem1 == 0)
	{
		elem1 = elem2;
		elem2 = elem3;
		elem3 = elem4;
		elem4 = 0;
	}
}


void QGameBoard::drawBoard()
{
	delete boardLayout;
	boardLayout = new QGridLayout();
	for (int i = 0; i < game->getGameBoard()->getDimension(); ++i) {
		for (int j = 0; j < game->getGameBoard()->getDimension(); ++j) {
			delete gui_board[i][j];
			gui_board[i][j] = new QTile(game->getGameBoard()->getTile(i, j));
			boardLayout->addWidget(gui_board[i][j], i, j);
			gui_board[i][j]->draw();
		}
	}
	mainLayout->insertLayout(0, boardLayout);
}


void QGameBoard::resetGame()
{
	game->restart();
	drawBoard();
	score->setText(QString("SCORE: %1").arg(game->getScore()));
	gameOverWindow.hide();



}
