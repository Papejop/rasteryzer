#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include "window.h"
#include "windowbuffer.h"

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    const wchar_t CLASS_NAME[] = L"training wheels";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    DWORD dwStyle = WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME;
    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        L"training wheels part 2",
        dwStyle,
        CW_USEDEFAULT, CW_USEDEFAULT, 1920, 1080,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if(hwnd == NULL)
    {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    MSG msg = {};

    while(GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static WindowBuffer buffer;

    switch(uMsg)
    {  
        
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        case WM_CLOSE:
            if(MessageBox(hwnd, L"Really quit?", L"small window", MB_OKCANCEL) == IDOK)
            {
                DestroyWindow(hwnd);
            }
            return 0;
        case WM_SIZE:
            {
                RECT rect;
                GetClientRect(hwnd, &rect);
                int width = rect.right - rect.left;
                int height = rect.bottom - rect.top;
                InitializeBuffer(&buffer, width, height);
                FillBuffer(&buffer);
                return 0;
                // initialize buffer here and do some other stuff too
            }
        case WM_PAINT:
            {
                PAINTSTRUCT ps;
                HDC hdc = BeginPaint(hwnd, &ps);
                //TextOut(hdc, 0, 0, L"witaj okrutny swiecie", 21);
                //FillRect(hdc, &ps.rcPaint, (HBRUSH) (COLOR_WINDOW+1));
                RenderBitmap(&buffer, hdc);
                EndPaint(hwnd, &ps);
                return 0;
            }

        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}