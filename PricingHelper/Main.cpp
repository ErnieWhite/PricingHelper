#ifndef UNICODE
#define UNICODE
#endif

#ifndef _UNICODE
#define _UNICODE
#endif

#include <windows.h>
#include "FindFormulas.h"
#include "FindBasisValue.h"
#include "ConvertFormula.h"


// Menu IDs
const UINT16 IDM_FINDFORMULAS = 101;
const UINT16 IDM_FINDBASISVALUE = 102;
const UINT16 IDM_CONVERTFORMULA = 103;
const UINT16 IDM_EXIT = 199;

enum View { FINDFORMULAVIEW, FINDBASISVALUEVIEW, CONVERTFORMULAVIEW };
View currentView = FINDFORMULAVIEW;

// Forward declarations of functions in this code module:
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void ShowView(View view);
void SwitchView(View view);

// Global variables
HWND hWnd;
HINSTANCE hInst;                                        // Current instance
const wchar_t szWindowClass[] = L"PricingHelperClass";  // The main window class
const wchar_t szTitle[] = L"Pricing Helper";            // The title bar text

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    hInst = hInstance;  // Store instance handle in our global variable

    // Create the main menu
    HMENU hMenu = CreateMenu();

    // Create the File submenu
    HMENU hFileMenu = CreatePopupMenu();
    AppendMenu(hFileMenu, MF_STRING, IDM_EXIT, L"E&xit");

    // Create the View submenu
    HMENU hViewMenu = CreatePopupMenu();
    AppendMenu(hViewMenu, MF_STRING, IDM_FINDFORMULAS, L"&Find Formulas");
    AppendMenu(hViewMenu, MF_STRING, IDM_FINDBASISVALUE, L"Find &Basis Value");
    AppendMenu(hViewMenu, MF_STRING, IDM_CONVERTFORMULA, L"&Convert Formula");

    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hFileMenu, L"File");
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hViewMenu, L"View");

    // Register the main window class
    WNDCLASSEXW wcex;
    wcex.cbSize = sizeof(WNDCLASSEXW);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

    if (!RegisterClassExW(&wcex)) {
        MessageBox(NULL, L"Call to RegisterClassExW failed!", szTitle, MB_OK | MB_ICONINFORMATION);
        return 1;
    }

    hWnd = CreateWindowExW(
        0,
        szWindowClass,
        szTitle,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        // TODO: change to actual window size needed
        CW_USEDEFAULT, CW_USEDEFAULT,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (!hWnd) {
        MessageBoxW(NULL, L"Call to CreateWindowExW failed!", szTitle, MB_OK | MB_ICONINFORMATION);
        return 1;
    }

    SetMenu(hWnd, hMenu);

    // Show the main window
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    ShowView(currentView);

    // Main message loop
    MSG msg;
    while (GetMessageW(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

void ShowView(View view) {
    switch (view) {
    case FINDFORMULAVIEW:
        FindFormulas::Show(hWnd, hInst);
        break;
    case FINDBASISVALUEVIEW:
        FindBasisValue::Show(hWnd, hInst);
        break;
    case CONVERTFORMULAVIEW:
        ConvertFormula::Show(hWnd, hInst);
        break;
    }
}

void SwitchView(View view) {

    if (currentView == view) {
        return;
    }

    switch (currentView) {
    case FINDFORMULAVIEW:
        FindFormulas::Hide();
        break;
    case FINDBASISVALUEVIEW:
        FindBasisValue::Hide();
        break;
    case CONVERTFORMULAVIEW:
        ConvertFormula::Hide();
        break;
    }

    currentView = view;
    ShowView(view);
}


// Main window prodedure
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    static HWND hFindFormulas, hFindBasisValue, hConvertFormula;

    switch (uMsg) {
    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case IDM_EXIT: // Exit
            PostQuitMessage(0);
            break;
        case IDM_FINDFORMULAS:  // Find Formulas View
            SwitchView(FINDFORMULAVIEW);
            break;
        case IDM_FINDBASISVALUE:  // Find Formulas View
            SwitchView(FINDBASISVALUEVIEW);
            break;
        case IDM_CONVERTFORMULA:  // Find Formulas View
            SwitchView(CONVERTFORMULAVIEW);
            break;
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
    return 0;
}