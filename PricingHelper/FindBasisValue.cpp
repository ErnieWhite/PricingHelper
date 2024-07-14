#include <windows.h>
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <cstdint>
#include <regex>
#include "FindBasisValue.h"
#include "resource.h"

namespace FindBasisValue {

    HINSTANCE hInstance;

    // Global namespace variables
    const wchar_t szWndClass[] = L"FindBasisValueClass";
    const wchar_t szTitle[] = L"Find Basis Value";

    // function prototypes
    LRESULT CALLBACK ChildWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    void UpdateBasisValue();
    // void SetDecimalPlaces(std::wstringstream& stream, int decimalPlaces);
    void ClearBasisValue();
    bool ValidateDoubleText(HWND hWnd);
    bool ValidateFormulaText(HWND hWnd);

    const UINT64 IDC_UNIPRICE = 101;
    const UINT64 IDC_FORMULA = 102;
    const UINT64 IDC_DECIMALS = 103;
    const UINT64 IDC_BASISVALUE = 104;
    const UINT64 IDC_BASISVALUE_COPY = 105;

    HINSTANCE hInst;
    HWND hWnd;
    HWND hUnitPrice, hFormula, hDecimals;
    HWND hBasisValue, hBasisValueCopy;

    void Show(HWND hWndParent, HINSTANCE hInstance) {

        hInst = hInstance;

        WNDCLASSEXW wcex;

        wcex.cbSize = sizeof(WNDCLASSEXW);
        wcex.style = CS_HREDRAW | CS_VREDRAW;
        wcex.lpfnWndProc = ChildWndProc;
        wcex.cbClsExtra = 0;
        wcex.cbWndExtra = 0;
        wcex.hInstance = hInstance;
        wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
        wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
        wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 0);
        wcex.lpszMenuName = NULL;
        wcex.lpszClassName = szWndClass;
        wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

        if (!RegisterClassExW(&wcex)) {
            MessageBoxW(NULL, L"Call to RegisterClassExW failed!", szTitle, MB_OK | MB_ICONEXCLAMATION);
        }

        hWnd = CreateWindowExW(
            WS_EX_CLIENTEDGE,
            szWndClass,
            szTitle,
            WS_CHILD | WS_VISIBLE,
            0, 0,
            500, 500,
            hWndParent,
            NULL,
            hInst,
            NULL);

        if (!hWnd) {
            MessageBoxW(NULL, L"Call to CreateWindowExW failed!", szTitle, MB_OK | MB_ICONEXCLAMATION);
        }

    }

    LRESULT CALLBACK ChildWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

        switch (uMsg) {
        case WM_COMMAND:
            if (LOWORD(wParam) == IDC_BUTTON_FINDBASISVALUE) {
                MessageBoxW(hWnd, L"Find Basis Value", szTitle, MB_OK);
            }
            break;
        case WM_CREATE:
            // TODO: extract this to a function
            // Create the labels for the inputs
            CreateWindowW(L"STATIC", L"Unit Price", WS_CHILD | WS_VISIBLE, 10, 10, 80, 20, hWnd, NULL, hInst, NULL);
            CreateWindowW(L"STATIC", L"Formula", WS_CHILD | WS_VISIBLE, 10, 50, 80, 20, hWnd, NULL, hInst, NULL);
            CreateWindowW(L"STATIC", L"Decimals", WS_CHILD | WS_VISIBLE, 10, 90, 80, 20, hWnd, NULL, hInst, NULL);

            // Create the input controls
            hUnitPrice = CreateWindowExW(WS_EX_CLIENTEDGE, L"EDIT", L"", WS_CHILD | WS_VISIBLE, 100, 10, 80, 20, hWnd, (HMENU)IDC_UNIPRICE, hInst, NULL);
            hFormula = CreateWindowExW(WS_EX_CLIENTEDGE, L"EDIT", L"", WS_CHILD | WS_VISIBLE, 100, 50, 80, 20, hWnd, (HMENU)IDC_FORMULA, hInst, NULL);
            hDecimals = CreateWindowExW(0, L"COMBOBOX", L"", CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE, 100, 90, 80, 200, hWnd, (HMENU)IDC_DECIMALS, hInst, NULL);

            // Add items to the combo box
            SendMessageW(hDecimals, CB_ADDSTRING, 0, (LPARAM)L"0");
            SendMessageW(hDecimals, CB_ADDSTRING, 0, (LPARAM)L"1");
            SendMessageW(hDecimals, CB_ADDSTRING, 0, (LPARAM)L"2");
            SendMessageW(hDecimals, CB_ADDSTRING, 0, (LPARAM)L"3");
            SendMessageW(hDecimals, CB_ADDSTRING, 0, (LPARAM)L"4");
            SendMessageW(hDecimals, CB_ADDSTRING, 0, (LPARAM)L"5");
            SendMessageW(hDecimals, CB_ADDSTRING, 0, (LPARAM)L"6");
            SendMessageW(hDecimals, CB_ADDSTRING, 0, (LPARAM)L"Auto");
            SendMessageW(hDecimals, CB_SETCURSEL, 0, 0); // Default selection

            // Create a vertical separator
            CreateWindowW(L"STATIC", L"", WS_CHILD | WS_VISIBLE | SS_ETCHEDVERT, 190, 10, 2, 110, hWnd, NULL, hInst, NULL);

            // Create the labels for the formula displays
            CreateWindowW(L"STATIC", L"Basis Value", WS_CHILD | WS_VISIBLE, 200, 10, 80, 20, hWnd, NULL, hInst, NULL);

            // Create the formula display controls
            hBasisValue = CreateWindowExW(WS_EX_STATICEDGE, L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_DISABLED, 285, 10, 75, 20, hWnd, (HMENU)IDC_BASISVALUE, hInst, nullptr);
            hBasisValueCopy = CreateWindowExW(0, L"BUTTON", L"Copy", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 360, 10, 75, 20, hWnd, (HMENU)IDC_BASISVALUE_COPY, hInst, NULL);

            // Set initial values
            UpdateBasisValue();

            break;
        case WM_KEYDOWN:
        case WM_CHAR: 
            if (LOWORD(wParam) == IDC_UNIPRICE) {
                MessageBeep(5);
            }
            // if the keypress as the return, tab or backspace keys let them through
            if (LOWORD(wParam) == VK_RETURN || LOWORD(wParam) == VK_TAB || LOWORD(wParam) == VK_BACK) {
                return DefWindowProc(hWnd, uMsg, wParam, lParam);
            }

            // if this is the unit price and the value validates let the key pass through
            if (GetFocus() == hUnitPrice && ValidateDoubleText(hUnitPrice)) {
                return DefWindowProc(hWnd, uMsg, wParam, lParam);
            }

            // if this is the formula and the value validates let the key pass through
            if (GetFocus() == hFormula && ValidateFormulaText(hFormula)) {
                return DefWindowProc(hWnd, uMsg, wParam, lParam);
            }
            break;
        default:
            return DefWindowProc(hWnd, uMsg, wParam, lParam);
        }

        return 0;
    }

    void UpdateBasisValue() {

        // Get the text from the controls
        wchar_t unitPriceText[101];
        wchar_t formulaText[101];

        GetWindowTextW(hUnitPrice, unitPriceText, 100);
        GetWindowTextW(hFormula, formulaText, 100);

        // convert the unitriceText to a double value;
        double unitPrice = _wtof(unitPriceText);
        double multiplier = 1.0;

        if (formulaText[0] == '*') {
            multiplier = _wtof(&formulaText[1]);
        }
        else if (formulaText[0] == '+' || formulaText[0] == '-') {
            double number = _wtof(&formulaText[1]);
            multiplier = 1 - number / 100.0;
        }
        else if (formulaText[0] == 'D') {
            double number = _wtof(&formulaText[1]);
            if (number == 0) {  // cannot divide by zero
                ClearBasisValue();
                return;
            }
            multiplier = 1 / number;
        }
        else if (formulaText[0] == 'G' && formulaText[1] == 'P') {
            double number = _wtof(&formulaText[2]);
            multiplier = 1 / (1 - number / 100);
        }

        if (multiplier == 0) {  // cannot divide by zero
            ClearBasisValue();
            return;
        }

        double basisValue = unitPrice / multiplier;

        if (basisValue == 0) {
            ClearBasisValue();
        }

        int decimalPlaces = (int)SendMessage(hDecimals, CB_GETCURSEL, 0, 0);

        std::wstringstream stream;
        if (decimalPlaces < 7) {
            stream << std::fixed << std::setprecision(decimalPlaces) << basisValue;
        }
        else {
            stream << basisValue;
        }

        std::wstring wstr = stream.str();
        SetWindowTextW(hBasisValue, stream.str().c_str());


    }

    // void SetDecimalPlaces(std::wstringstream& stream, int decimalPlaces) {
    //     if (decimalPlaces == 7) {   // 7 corresponds to "Auto"
    //         stream << std::defaultfloat;
    //     } else {
    //         stream << std::fixed << std::setprecision(decimalPlaces);
    //     }
    // }


    void ClearBasisValue() {
        SetWindowTextW(hBasisValue, L"");
    }

     bool ValidateDoubleText(HWND hWnd) {
         wchar_t buffer[100];
         GetWindowTextW(hWnd, buffer, 100);

         std::wstring text(buffer);
         std::wregex float_regex(LR"(^[-+]?\d*\.?\d*$)");
         MessageBeep(5);

         return !std::regex_match(text, float_regex);
     }

     bool ValidateFormulaText(HWND hWnd) {
         wchar_t buffer[100];
         GetWindowTextW(hWnd, buffer, 100);

         std::wstring text(buffer);
         std::wregex pattern_regex(LR"(^(\*|D|\+|-|GP[-+]?\d*\.?\d*$)");

         return !std::regex_match(text, pattern_regex);
     }

    void Hide() {
        DestroyWindow(hWnd);
        UnregisterClassW(szWndClass, hInst);
    }
}