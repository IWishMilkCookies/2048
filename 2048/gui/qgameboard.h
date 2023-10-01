#ifndef QGAMEBOARD_H
#define QGAMEBOARD_H

#include "core/observer.h"
#include "qgameoverwindow.h"

#include <QVector>
#include <QWidget>

//Required for NN
#include <memory>
#include <NeuralNetwork.h>
#include <vector>

class QResetButton;
class Game;
class QKeyEvent;
class QTile;
class QGridLayout;
class QVBoxLayout;
class QLabel;
class Board;

enum Direction : int;


class QGameBoard : public QWidget, public Observer
{
    Q_OBJECT
public:
    explicit QGameBoard(QWidget *parent = 0);
    ~QGameBoard();

    void notify();

    //Method(s) for NN
    void LoopNeuralNet();
    void ApplyNeuralNetworkMove(const std::vector<double>& targetOutput);

    int SimulateMovement(Direction direction, Board* board);
    void SetAllButHighestToZero(std::vector<double>& targetOutputs);
    void CombineAdjacentElements(int& elem1, int& elem2, int& elem3, int& elem4);
    void MoveElements(int& elem1, int& elem2, int& elem3, int& elem4);
    int SolveRow(int elem1, int elem2, int elem3, int elem4);

    void SanitizeInputs(const QVector<int>& inputVector, std::vector<double>& outputVector);

private:

    Game* game;
    // gui representation of board
    QVector<QVector<QTile*> > gui_board;
    // main layout
    QVBoxLayout *mainLayout;
    // grid layout of board
    QGridLayout *boardLayout;
    // score widget
    QLabel *score;
    // game over widget
    QGameOverWindow gameOverWindow;
    // winner widget
    QLabel *youwin;

    void drawBoard();


    //Variable(s) for NN
    std::vector<unsigned> neuralNetTopology{ 16,10,4 };
    std::unique_ptr<Net> neuralNetwork;
    Direction previousDirection;

protected:
    void keyPressEvent(QKeyEvent *event);

signals:

public slots:
    void resetGame();

};

#endif // QGAMEBOARD_H
