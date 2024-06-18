#ifndef UNICODE
#define UNICODE
#endif
#include <windows.h>

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    // Register window class
    const wchar_t CLASS_NAME[] = L"PricingHelper";
    const wchar_t TITLE[] = L"Pricing Helper";

    WNDCLASSW wc = { };
    
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

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
    UpdateWindow(hwnd);

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
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        case WM_PAINT:
            {
                PAINTSTRUCT ps;
                HDC hdc = BeginPaint(hwnd, &ps);

                // All painting occurs here, between BeginPaint and EndPaint
                FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

                EndPaint(hwnd, &ps);
            }
            return 0;
    }
    
    // all unhandled msgs are handled by DefWindowProc
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}