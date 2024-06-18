#ifndef UNICODE
#define UNICODE
#endif
#include <windows.h>

#define ID_UNIT_PRICE 101
#define ID_BASIS_VALUE 102

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void CreateLabel(HWND hwnd, LPCWSTR caption, int x, int y, int width, int height);
HWND CreateTextEdit(HWND hwnd, HMENU hMenu, int x, int y, int width, int height);

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
            CreateLabel(hwnd, L"Unit Price", 10, 10, 75, 20);
            CreateTextEdit(hwnd, (HMENU)ID_UNIT_PRICE, 85, 10, 75, 20);

            CreateLabel(hwnd, L"Basis Value", 10, 30, 75, 20);
            CreateTextEdit(hwnd, (HMENU)ID_BASIS_VALUE, 85, 30, 75, 20);

            CreateLabel(hwnd, L"Multiplier", 170, 10, 75, 20);
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
