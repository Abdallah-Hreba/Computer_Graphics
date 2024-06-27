#include <Windows.h>
#include <cmath>
#include <stack>

using namespace std;

struct Vertex
{
    int x, y;
    Vertex(int x, int y) : x(x), y(y) {}
};

void floodFill(HDC hdc, int x, int y, COLORREF Cb, COLORREF Cf)
{
    stack<Vertex> S;
    S.push(Vertex(x, y));
    while (!S.empty())
    {
        Vertex v = S.top();
        S.pop();
        COLORREF c = GetPixel(hdc, v.x, v.y);
        if (c == Cb || c == Cf) continue;
        SetPixel(hdc, v.x, v.y, Cf);
        S.push(Vertex(v.x + 1, v.y));
        S.push(Vertex(v.x - 1, v.y));
        S.push(Vertex(v.x, v.y + 1));
        S.push(Vertex(v.x, v.y - 1));
    }
}

void Draw8Points(HDC hdc, int xc, int yc, int a, int b, COLORREF color)
{
    SetPixel(hdc, xc + a, yc + b, color);
    SetPixel(hdc, xc - a, yc + b, color);
    SetPixel(hdc, xc - a, yc - b, color);
    SetPixel(hdc, xc + a, yc - b, color);
    SetPixel(hdc, xc + b, yc + a, color);
    SetPixel(hdc, xc - b, yc + a, color);
    SetPixel(hdc, xc - b, yc - a, color);
    SetPixel(hdc, xc + b, yc - a, color);
}

void DrawCircle(HDC hdc, int xc, int yc, int R, COLORREF color)
{
    int x = 0, y = R;
    int d = 1 - R;
    int c1 = 3, c2 = 5 - 2 * R;
    Draw8Points(hdc, xc, yc, x, y, color);
    while (x < y)
    {
        if (d < 0)
        {
            d += c1;
            c2 += 2;
        }
        else
        {
            d += c2;
            c2 += 4;
            y--;
        }
        c1 += 2;
        x++;
        Draw8Points(hdc, xc, yc, x, y, color);
    }
}

void Draw8(HDC hdc, int x, int y, COLORREF color) {
    int radius = 50;
    DrawCircle(hdc, x, y, radius, color);
    DrawCircle(hdc, x, y + 100, radius, color);
    floodFill(hdc, x, y, color, RGB(255, 255, 255));
    floodFill(hdc, x, y + 100, color, RGB(255, 255, 255));
    DrawCircle(hdc, x, y, radius / 2, color);
    DrawCircle(hdc, x, y + 100, radius / 2, color);
    floodFill(hdc, x, y, color, RGB(0, 0, 0));
    floodFill(hdc, x, y + 100, color, RGB(0, 0, 0));
}

LRESULT WINAPI WndProc(HWND hwnd, UINT m, WPARAM wp, LPARAM lp)
{
    HDC hdc;
    int x, y;
    switch (m)
    {
    case WM_LBUTTONDOWN:
        hdc = GetDC(hwnd);
        x = LOWORD(lp);
        y = HIWORD(lp);
        Draw8(hdc, x, y, RGB(0, 0, 255));
        ReleaseDC(hwnd, hdc);
        break;
    case WM_CLOSE:
        DestroyWindow(hwnd);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default: return DefWindowProc(hwnd, m, wp, lp);
    }
    return 0;
}

int APIENTRY WinMain(HINSTANCE hi, HINSTANCE pi, LPSTR c, int ns)
{
    WNDCLASS wc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hbrBackground = CreateSolidBrush(RGB(50, 50, 50));
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
    wc.hInstance = hi;
    wc.lpfnWndProc = WndProc;
    wc.lpszClassName = "MyClass";
    wc.lpszMenuName = NULL;
    wc.style = CS_HREDRAW | CS_VREDRAW;
    RegisterClass(&wc);
    HWND hwnd = CreateWindow("MyClass", "Hello World", WS_OVERLAPPEDWINDOW, 15, 15, 1200, 1200, NULL, NULL, hi, 0);
    ShowWindow(hwnd, ns);
    UpdateWindow(hwnd);
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}
