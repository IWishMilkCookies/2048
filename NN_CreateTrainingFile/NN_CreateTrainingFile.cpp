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

#define MAX_LOADSTRING 100
#define IDC_SELECT_PATH_BUTTON 1001
#define IDC_SELECT_FILE_BUTTON 1002

namespace TextBoxes
{
	HWND g_TextBox;
}

namespace NeuralNetwork
{
	std::unique_ptr<TrainingDataW> g_TrainingData;
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

std::wstring OpenSaveDialogBox(HWND hWnd);
std::wstring SelectFileDialogBox(HWND hWnd);


void WriteToFile(std::wstring filePath);

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

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

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
		L"Select Trainig File",      // Button text 
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
		350,         // x position 
		670,         // y position 
		300,        // Button width
		50,        // Button height
		hWnd,     // Parent window
		(HMENU)IDC_SELECT_FILE_BUTTON,       // Unique button identifier.
		(HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
		NULL);      // Pointer not needed.


	TextBoxes::g_TextBox = CreateWindow(L"EDIT", 0, WS_BORDER | WS_CHILD | WS_VISIBLE, 10 /*x pos*/, 40 /*y pos*/, 400 /*width*/, 20 /*height*/, hWnd, 0, hInst, 0);
	//Add text box for number of tests -> User fills in how many tests need to be performed.
	
	CreateWindow(L"STATIC",
		L"Fill in the topology of your network seperated by spaces: 20 10 10 5.",
		WS_BORDER | WS_CHILD | WS_VISIBLE,
		10 /*x pos*/, 10 /*y pos*/, 450 /*width*/, 20 /*height*/,
		hWnd, 0, hInst, 0);

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
				auto filePath = OpenSaveDialogBox(hWnd);
				WriteToFile(filePath);
			}
			break;
		case IDC_SELECT_FILE_BUTTON:
			if (HIWORD(wParam) == BN_CLICKED)
			{
				auto filePath = SelectFileDialogBox(hWnd);
				NeuralNetwork::g_TrainingData = std::make_unique<TrainingDataW>(filePath);
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

std::wstring OpenSaveDialogBox(HWND hWnd)
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
	ofn.lpstrFilter = L"Training Data Files (*.TD)\0*.TD\0";
	ofn.nFilterIndex = 1;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
	ofn.lpstrDefExt = L".TD";


	if (GetSaveFileNameW(&ofn) == TRUE)
		return ofn.lpstrFile;
	else
		return std::wstring{};



	//char* resultStr = new char[wcslen(ofn.lpstrFile) + 1];
	//WriteFile(hWnd, resultStr);
}

std::wstring SelectFileDialogBox(HWND hWnd)
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
	ofn.lpstrFilter = L"Training Data Files (*.TD)\0*.TD\0";
	ofn.nFilterIndex = 1;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;


	if (GetOpenFileNameW(&ofn) == TRUE)
		return ofn.lpstrFile;
	else
		return std::wstring{};
}

void WriteToFile(std::wstring filePath)
{
	std::wofstream sampleData;
	sampleData.open(filePath);

	sampleData << L"topology: ";
	//Sampla data << 'numbers of topology' << std::endl;
	std::wstring inputOutputText{};
	int textLength = GetWindowTextLengthW(TextBoxes::g_TextBox) + 1;
	std::wstring topologyText(textLength, L' ');
	
	GetWindowTextW(TextBoxes::g_TextBox, &topologyText[0], textLength);
	//Check if topology is valid, with regex : \d+\s\d+\s\d+(?:\s\d+)*
	
	sampleData << topologyText << '\n';
	for (int i = 2000; i >= 0; --i)
	{
		int n1 = static_cast<int>(2.f * rand() / static_cast<unsigned>(RAND_MAX));
		int n2 = static_cast<int>(2.f * rand() / static_cast<unsigned>(RAND_MAX));
		int t = n1 ^ n2;

		sampleData << L"in: " << n1 << L".0 " << n2 << L".0 " << std::endl;
		sampleData << L"out: " << t << L".0 " << std::endl;
	}
	sampleData.close();
}
