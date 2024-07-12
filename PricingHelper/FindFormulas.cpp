#include <windows.h>
#include "FindFormulas.h"
#include "resource.h"

namespace FindFormulas {
    const wchar_t szClassName[] = L"FindFormulasClass";
    const wchar_t szTitle[] = L"Find Formulas";

    LRESULT CALLBACK ChildWndProc(HWND, UINT, WPARAM, LPARAM);


    HINSTANCE hInst;
    HWND hWnd;
    HWND hButton;

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
        wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        wcex.lpszMenuName = NULL;
        wcex.lpszClassName = szClassName;
        wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

        if (!RegisterClassExW(&wcex)) {
            MessageBoxW(NULL, L"Call to RegisterClassExW failed!", szTitle, MB_OK | MB_ICONEXCLAMATION);
        }

        hWnd = CreateWindowExW(
            0,
            szClassName,
            szTitle,
            WS_CHILD | WS_VISIBLE,
            0, 0, 500, 500,
            hWndParent,
            NULL,
            hInstance,
            NULL
        );

        if (!hWnd) {
            MessageBoxW(NULL, L"Call to CreateWindowExW failed!", szTitle, MB_OK | MB_ICONEXCLAMATION);
        }
    }

    LRESULT CALLBACK ChildWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

        switch (uMsg) {
        case WM_COMMAND:
            if (LOWORD(wParam) == IDC_BUTTON_FINDFORMULAS) {
                MessageBoxW(hWnd, L"Find Formulas", FindFormulas::szTitle, MB_OK);
            }
            break;
        case WM_CREATE:
            hButton = CreateWindowExW(0, L"BUTTON", L"Find Formulas", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                50, 50, 150, 30, hWnd, (HMENU)IDC_BUTTON_FINDFORMULAS, hInst, NULL);
            break;
        default:
            return DefWindowProc(hWnd, uMsg, wParam, lParam);
        }
        return 0;
    }

    void Hide() {
        DestroyWindow(hButton);
        DestroyWindow(hWnd);
        UnregisterClassW(szClassName, hInst);
    }
}