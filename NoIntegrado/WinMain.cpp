#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#ifndef UNICODE
#define UNICODE
#endif

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    const wchar_t NOMBRE_CLASE[] = L"Clase ejemplo";

    WNDCLASS wc = {};

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = NOMBRE_CLASE;

    RegisterClass(&wc);

    HWND hwnd = CreateWindowExW(

        0,
        NOMBRE_CLASE,
        L"Prueba de Ventana",
        WS_OVERLAPPEDWINDOW,

        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

        NULL,
        NULL,
        hInstance,
        NULL

    );

    ShowWindow(hwnd, nCmdShow);

    MSG msg = {};
    while (GetMessageW(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    // MessageBox(NULL, L"Ola", L"Ok", MB_OK);
    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_CLOSE:
        DestroyWindow(hwnd);
        return 0;

    case WM_PAINT: {
        
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        
        //Proceso
        FillRect(hdc, &ps.rcPaint, (HBRUSH) (COLOR_WINDOW+1));
        TextOutW(hdc,10,10,L"Prueba",7);

        EndPaint(hwnd, &ps);
        ReleaseDC(hwnd,hdc);
        }
        return 0;
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return TRUE;
}