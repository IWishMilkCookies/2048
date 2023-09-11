// NN_CreateTrainingFile.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "NN_CreateTrainingFile.h"
#include <NeuralNetwork.h>
#include <TrainingData.h>

#include <windows.h>
#include <shobjidl.h> 
#include "commdlg.h"
#include <shlwapi.h>

#include <fstream>
#include <string>
#include <memory>
#include <regex>
#include <cassert>

#define MAX_LOADSTRING 100
#define IDC_SELECT_PATH_BUTTON 1001
#define IDC_SELECT_FILE_BUTTON 1002
#define IDC_TRAIN_NETWORK_BUTTON 1003

namespace TextBoxes
{
	HWND g_TopologyTextBox;
	HWND g_NrOfTrainingTextBox;
}

namespace NeuralNetwork
{
	std::unique_ptr<TrainingDataW> g_TrainingData = nullptr;
	std::unique_ptr<Net> g_NeuralNetwork = nullptr;
}

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

std::wstring OpenSaveDialogBox(HWND hWnd, const wchar_t* extFilter, const wchar_t* defExtenstion);
std::wstring SelectFileDialogBox(HWND hWnd, const wchar_t* extFilter);
void TrainNeuralNet(std::wstring filePath);


void WriteToFile(HWND hWnd, std::wstring filePath);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.

	// Initialize global strings
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_NNCREATETRAININGFILE, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_NNCREATETRAININGFILE));

	MSG msg;

	// Main message loop:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_NNCREATETRAININGFILE));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_NNCREATETRAININGFILE);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Store instance handle in our global variable
	
	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

#pragma region Buttons


	CreateWindow(
		L"BUTTON",  // Predefined class; Unicode assumed 
		L"Select Path",      // Button text 
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
		10,         // x position 
		670,         // y position 
		300,        // Button width
		50,        // Button height
		hWnd,     // Parent window
		(HMENU)IDC_SELECT_PATH_BUTTON,       // Unique button identifier.
		(HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
		NULL);      // Pointer not needed.

	CreateWindow(
		L"BUTTON",  // Predefined class; Unicode assumed 
		L"Select Training File",      // Button text 
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
		340,         // x position 
		670,         // y position 
		300,        // Button width
		50,        // Button height
		hWnd,     // Parent window
		(HMENU)IDC_SELECT_FILE_BUTTON,       // Unique button identifier.
		(HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
		NULL);      // Pointer not needed.

	CreateWindow(
		L"BUTTON",  // Predefined class; Unicode assumed 
		L"Train selected Neural Net.",      // Button text 
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
		670,         // x position 
		670,         // y position 
		300,        // Button width
		50,        // Button height
		hWnd,     // Parent window
		(HMENU)IDC_TRAIN_NETWORK_BUTTON,       // Unique button identifier.
		(HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
		NULL);      // Pointer not needed.
#pragma endregion


#pragma region TextBoxes

	TextBoxes::g_TopologyTextBox = CreateWindow(L"EDIT", L"Topology", WS_BORDER | WS_CHILD | WS_VISIBLE, 10 /*x pos*/, 70 /*y pos*/, 400 /*width*/, 20 /*height*/, hWnd, 0, hInst, 0);
	TextBoxes::g_NrOfTrainingTextBox = CreateWindow(L"EDIT", L"0", WS_BORDER | WS_CHILD | WS_VISIBLE | ES_NUMBER, 350 /*x pos*/, 100 /*y pos*/, 50 /*width*/, 20 /*height*/, hWnd, 0, hInst, 0);
	//Add text box for number of tests -> User fills in how many tests need to be performed.
	
#pragma endregion

#pragma region Static Text

	CreateWindow(L"STATIC",
		L"Fill in the topology of your network seperated by spaces: 20 10 10 5.",
		WS_BORDER | WS_CHILD | WS_VISIBLE,
		10 /*x pos*/, 10 /*y pos*/, 450 /*width*/, 20 /*height*/,
		hWnd, 0, hInst, 0);

	CreateWindow(L"STATIC",
		L"Then click 'Select Path' to save the Training data file.",
		WS_BORDER | WS_CHILD | WS_VISIBLE,
		10 /*x pos*/, 40 /*y pos*/, 450 /*width*/, 20 /*height*/,
		hWnd, 0, hInst, 0);

	CreateWindow(L"STATIC",
		L"Amount of times you want to train your neural net.",
		WS_BORDER | WS_CHILD | WS_VISIBLE,
		10 /*x pos*/, 100 /*y pos*/, 330 /*width*/, 20 /*height*/,
		hWnd, 0, hInst, 0);

#pragma endregion


	if (!hWnd)
		return FALSE;

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case IDC_SELECT_PATH_BUTTON:
			if (HIWORD(wParam) == BN_CLICKED) //Check if button is clicked
			{
				auto filePath = OpenSaveDialogBox(hWnd, L"Training Data Files (*.TD)\0*.TD\0", L".TD");
				if (filePath.empty()) //If path doesn't exist, break out of switch case.
				{
					MessageBox(hWnd, L"No valid path has been given.", NULL, MB_OK);
					break;
				}

				WriteToFile(hWnd, filePath);
			}
			break;
		case IDC_SELECT_FILE_BUTTON:
			if (HIWORD(wParam) == BN_CLICKED)
			{
				auto filePath = SelectFileDialogBox(hWnd, L"Training Data Files (*.TD)\0*.TD\0");
				
				if (filePath.empty()) //If path doesn't exist, break out of switch case.
				{
					MessageBox(hWnd, L"No valid path has been given.", NULL, MB_OK);
					break;
				}

				NeuralNetwork::g_TrainingData = std::make_unique<TrainingDataW>(filePath);

				NeuralNetwork::g_TrainingData->fillTopology();
				NeuralNetwork::g_NeuralNetwork = std::make_unique<Net>(NeuralNetwork::g_TrainingData->getTopology());

			}
			break;
		case IDC_TRAIN_NETWORK_BUTTON:
			if (HIWORD(wParam) == BN_CLICKED)
			{		

				if (!NeuralNetwork::g_TrainingData)
				{
					MessageBox(hWnd, L"No prior training data file has been selected.", NULL, MB_OK | MB_ICONERROR);
					break;
				}

				std::wstring filePath = OpenSaveDialogBox(hWnd, L"Text Files (*.txt)\0*.txt\0", L".txt");

				if (filePath.empty()) //If path doesn't exist, break out of switch case.
				{
					MessageBox(hWnd, L"No valid training file was selected.", NULL, MB_OK);
					break;
				}

				TrainNeuralNet(filePath);
			}
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code that uses hdc here...
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

std::wstring OpenSaveDialogBox(HWND hWnd, const wchar_t* extFilter, const wchar_t* defExtenstion) //Add Filter and extension as parameter
{
	//Open dialog box
	OPENFILENAME ofn;

	WCHAR file_Name[260];

	ZeroMemory(&ofn, sizeof(OPENFILENAME));

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFile = file_Name;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(file_Name);
	ofn.lpstrFilter = extFilter;
	ofn.nFilterIndex = 1;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
	ofn.lpstrDefExt = defExtenstion;


	if (GetSaveFileNameW(&ofn) == TRUE)
		return ofn.lpstrFile;
	else
		return std::wstring{};

}

std::wstring SelectFileDialogBox(HWND hWnd, const wchar_t* extFilter)
{
	//Open dialog box
	OPENFILENAME ofn;

	WCHAR file_Name[260];

	ZeroMemory(&ofn, sizeof(OPENFILENAME));

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFile = file_Name;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(file_Name);
	ofn.lpstrFilter = extFilter;
	ofn.nFilterIndex = 1;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;


	if (GetOpenFileNameW(&ofn) == TRUE)
		return ofn.lpstrFile;
	else
		return std::wstring{};
}

void WriteToFile(HWND hWnd, std::wstring filePath)
{
	int textLength = GetWindowTextLengthW(TextBoxes::g_TopologyTextBox) + 1;
	std::wstring topologyText(textLength, L' ');
	
	GetWindowTextW(TextBoxes::g_TopologyTextBox, &topologyText[0], textLength);

	//Check if topology is valid, with regex : \d+\s\d+\s\d+(?:\s\d+)*
	std::wregex regexPattern(L"\\d+\\s\\d+\\s\\d+(?:\\s\\d+)*.");
	if (!std::regex_match(topologyText, regexPattern))
	{
		MessageBox(hWnd, L"This topology is not valid.", NULL, MB_OK | MB_ICONERROR);
		return;
	}

	std::wofstream sampleData;
	sampleData.open(filePath);
	sampleData << L"topology: ";
	sampleData << topologyText << '\n';



	int trainingAmount = GetWindowTextLengthW(TextBoxes::g_NrOfTrainingTextBox) + 1;
	std::wstring trainingAmountText(trainingAmount, L' ');
	
	GetWindowTextW(TextBoxes::g_NrOfTrainingTextBox, &trainingAmountText[0], trainingAmount);
	int actualTrainingAmount = std::stoi(trainingAmountText);
	

	for (int i = actualTrainingAmount; i >= 0; --i) //2000 -> Number given in by user.
	{
		int n1 = static_cast<int>(2.f * rand() / static_cast<unsigned>(RAND_MAX));
		int n2 = static_cast<int>(2.f * rand() / static_cast<unsigned>(RAND_MAX));
		int t = n1 ^ n2;

		sampleData << L"in: " << n1 << L".0 " << n2 << L".0 " << std::endl;
		sampleData << L"out: " << t << L".0 " << std::endl;
	}

	sampleData.close();
}

void TrainNeuralNet(std::wstring filePath) //Write couts to file, file writing Path given as parameter.
{
	std::vector<double> inputVals, targetVals, resultVals;
	int trainingPass = 0;

	std::wofstream trainingData;
	trainingData.open(filePath);
	
	while (NeuralNetwork::g_TrainingData->isEof())
	{

		++trainingPass;
		trainingData << std::endl << "Pass " << trainingPass;

		//Get new input data and feed it forward
		if (NeuralNetwork::g_TrainingData->getNextInputs(inputVals) != NeuralNetwork::g_TrainingData->getTopology()[0])
			break;

		NeuralNetwork::g_NeuralNetwork->FeedForward(inputVals);

		//Collect the net's actual results
		NeuralNetwork::g_NeuralNetwork->GetResults(resultVals);

		//Train the net what the outputs should have been:
		NeuralNetwork::g_TrainingData->getTargetOutputs(targetVals);
		assert(targetVals.size() == NeuralNetwork::g_TrainingData->getTopology().back());

		NeuralNetwork::g_NeuralNetwork->BackProp(targetVals);

		//Report how well the training is working.
		trainingData << "Net recent average error: " << NeuralNetwork::g_NeuralNetwork->GetRecentAverageError() << std::endl;

	}

	trainingData.close();
}