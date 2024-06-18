#ifndef UNICODE
#define UNICODE
#endif
#include <windows.h>

#define ID_UNIT_PRICE 101
#define ID_BASIS_VALUE 102

HWND hwndUnitPrice;
HWND hwndBasisValue;
HWND hwndMultiplier;
HWND hwndDiscount;
HWND hwndMarkup;
HWND hwndGrossProfit;

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void CreateLabel(HWND hwnd, LPCWSTR caption, int x, int y, int width, int height);
HWND CreateTextEdit(HWND hwnd, HMENU hMenu, int x, int y, int width, int height);
HWND CreateTextDisplay(HWND hwnd, int x, int y, int width, int height);
void CreateVerticalSeparator(HWND hwnd, int x, int top, int bottom);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    // Register window class
    const wchar_t CLASS_NAME[] = L"PricingHelper";
    const wchar_t TITLE[] = L"Pricing Helper";

    WNDCLASSW wc = { };
    
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);

    RegisterClassW(&wc);

    // Create the window
    HWND hwnd = CreateWindowExW(
        0,                              // Optional window styles.
        CLASS_NAME,                     // Window class
        TITLE,                          // Window text
        WS_OVERLAPPEDWINDOW,            // Window style
        CW_USEDEFAULT, CW_USEDEFAULT,   // Window position (x, y)
        CW_USEDEFAULT, CW_USEDEFAULT,   // Window size (width, height)
        NULL,                           // Parent window
        NULL,                           // Menu
        hInstance,                      // Instance handle
        NULL                           // Additional applicatoin data
    );

    if (hwnd == NULL) {
        MessageBoxW(hwnd, L"Call to CreateWindowExW failed", TITLE, MB_OK | MB_ICONINFORMATION);
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    // Run the message loop
    MSG msg = { };
    while (GetMessageW(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
        case WM_CREATE:
            CreateLabel(hwnd, L"Unit Price", 10, 10, 80, 20);
            CreateLabel(hwnd, L"Basis Value", 10, 30, 80, 20);
            CreateLabel(hwnd, L"Multiplier", 185, 10, 80, 20);
            CreateLabel(hwnd, L"Discount", 185, 30, 80, 20);
            CreateLabel(hwnd, L"Markup", 185, 50, 80, 20);
            CreateLabel(hwnd, L"Gross Profit", 185, 70, 80, 20);

            hwndUnitPrice = CreateTextEdit(hwnd, (HMENU)ID_UNIT_PRICE, 90, 10, 75, 20);
            hwndBasisValue = CreateTextEdit(hwnd, (HMENU)ID_BASIS_VALUE, 90, 30, 75, 20);

            CreateVerticalSeparator(hwnd, 175, 10, 80);

            hwndMultiplier = CreateTextDisplay(hwnd, 270, 10, 75, 20);
            hwndDiscount = CreateTextDisplay(hwnd, 270, 30, 75, 20);
            hwndMarkup = CreateTextDisplay(hwnd, 270, 50, 75, 20);
            hwndGrossProfit = CreateTextDisplay(hwnd, 270, 70, 75, 20);

            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        case WM_PAINT:
            {
                PAINTSTRUCT ps;
                HDC hdc = BeginPaint(hwnd, &ps);

                // All painting occurs here, between BeginPaint and EndPaint
                FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW));

                EndPaint(hwnd, &ps);
            }
            break;
    }
    
    // all unhandled msgs are handled by DefWindowProc
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void CreateLabel(HWND hwnd, LPCWSTR caption, int x, int y, int width, int height)
{
    CreateWindowExW(0, L"STATIC", caption, WS_CHILD | WS_VISIBLE, x, y, width, height, hwnd, (HMENU)NULL, NULL, NULL);
}

HWND CreateTextEdit(HWND hwnd, HMENU hMenu, int x, int y, int width, int height)
{
    return CreateWindowExW(WS_EX_CLIENTEDGE, L"EDIT", L"", WS_CHILD | WS_VISIBLE, x, y, width, height, hwnd, hMenu, NULL, NULL);
}

HWND CreateTextDisplay(HWND hwnd, int x, int y, int width, int height)
{
    CreateWindowExW(WS_EX_CLIENTEDGE, L"STATIC", L"", WS_CHILD | WS_VISIBLE, x, y, width, height, hwnd, (HMENU)NULL, NULL, NULL);
}

void CreateVerticalSeparator(HWND hwnd, int x, int top, int bottom)
{
    CreateWindowExW(0, L"STATIC", NULL, WS_VISIBLE | WS_CHILD | SS_ETCHEDVERT, x, top, 2, bottom, hwnd, NULL, NULL, NULL);
}
