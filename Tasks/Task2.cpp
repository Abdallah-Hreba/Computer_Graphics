#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include <math.h>

void Draw8Points(HDC hdc, int xc, int yc, int x, int y, COLORREF c)
{
    SetPixel(hdc, xc + x, yc + y, c);
    SetPixel(hdc, xc - x, yc + y, c);
    SetPixel(hdc, xc - x, yc - y, c);
    SetPixel(hdc, xc + x, yc - y, c);
    SetPixel(hdc, xc + y, yc + x, c);
    SetPixel(hdc, xc - y, yc + x, c);
    SetPixel(hdc, xc - y, yc - x, c);
    SetPixel(hdc, xc + y, yc - x, c);
}

void PolarCircle(HDC hdc, int xc, int yc, int RAD, COLORREF c)
{
    int x = RAD;
    int y = 0;
    int d = 1 - RAD;

    Draw8Points(hdc, xc, yc, x, y, c);

    while (x > y)
    {
        if (d < 0)
        {
            d += 2 * y + 3;
        }
        else
        {
            d += 2 * (y - x) + 5;
            x--;
        }
        y++;
        Draw8Points(hdc, xc, yc, x, y, c);
    }
}

LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

TCHAR szClassName[] = _T("CodeBlocksWindowsApp");

int WINAPI WinMain (HINSTANCE hThisInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,
                    int nCmdShow)
{
    HWND hwnd;
    MSG messages;
    WNDCLASSEX wincl;

    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;
    wincl.style = CS_DBLCLKS;
    wincl.cbSize = sizeof(WNDCLASSEX);
    wincl.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor(NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;
    wincl.cbClsExtra = 0;
    wincl.cbWndExtra = 0;
    wincl.hbrBackground = (HBRUSH)COLOR_BACKGROUND;

    if (!RegisterClassEx(&wincl))
        return 0;

    hwnd = CreateWindowEx(
               0,
               szClassName,
               _T("Code::Blocks Template Windows App"),
               WS_OVERLAPPEDWINDOW,
               CW_USEDEFAULT,
               CW_USEDEFAULT,
               544,
               375,
               HWND_DESKTOP,
               NULL,
               hThisInstance,
               NULL
           );

    ShowWindow(hwnd, nCmdShow);

    while (GetMessage(&messages, NULL, 0, 0))
    {
        TranslateMessage(&messages);
        DispatchMessage(&messages);
    }

    return messages.wParam;
}

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    static int xc, yc, y, x, c, RAD;
    switch (message)
    {
    case WM_LBUTTONDOWN:
        xc = LOWORD(lParam);
        yc = HIWORD(lParam);
        break;

    case WM_LBUTTONUP:
        hdc = GetDC(hwnd);
        x = LOWORD(lParam);
        y = HIWORD(lParam);
        RAD = sqrt(pow(xc - x, 2.0) + pow(yc - y, 2.0));
        PolarCircle(hdc, xc, yc, RAD, c);
        PolarCircle(hdc, xc, yc + 50, 20, c);
        PolarCircle(hdc, xc - 40, yc - 40, 10, c);
        PolarCircle(hdc, xc + 40, yc - 40, 10, c);
        ReleaseDC(hwnd, hdc);
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, message, wParam, lParam);
    }

    return 0;
}
