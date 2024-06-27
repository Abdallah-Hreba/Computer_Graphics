#include <windows.h> 
#include <cmath> 

void drawLine(HDC hdc, int x1, int y1, int x2, int y2) {
    int dx = abs(x2 - x1), dy = abs(y2 - y1); 
    int sx = (x1 < x2) ? 1 : -1, sy = (y1 < y2) ? 1 : -1; 
    int err = dx - dy; 
    while (true) {
        SetPixel(hdc, x1, y1, RGB(255, 0, 0)); 
        if (x1 == x2 && y1 == y2) break; 
        int e2 = 2 * err; 
        if (e2 > -dy) { err -= dy; x1 += sx; } 
        if (e2 < dx) { err += dx; y1 += sy; } 
    }
}

void drawTriangle(HDC hdc, int x1, int y1, int x2, int y2, int x3, int y3) {
    drawLine(hdc, x1, y1, x2, y2); 
    drawLine(hdc, x2, y2, x3, y3); 
    drawLine(hdc, x3, y3, x1, y1); 
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_PAINT: { 
        PAINTSTRUCT ps; 
        HDC hdc = BeginPaint(hwnd, &ps); 

        int x1 = 200, y1 = 100;
        int x2 = 100, y2 = 200;
        int x3 = 300, y3 = 200;

        drawTriangle(hdc, x1, y1, x2, y2, x3, y3); 

        EndPaint(hwnd, &ps); 
        break;
    }
    case WM_CLOSE: 
        PostQuitMessage(0); 
        break;
    default: 
        return DefWindowProc(hwnd, uMsg, wParam, lParam); 
    }
    return 0; 
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    const wchar_t CLASS_NAME[] = L"Sample Window Class"; 

    WNDCLASSW wc = { 0 }; 
    wc.lpfnWndProc = WindowProc; 
    wc.hInstance = hInstance; 
    wc.lpszClassName = CLASS_NAME; 

    RegisterClassW(&wc); 

    HWND hwnd = CreateWindowExW(
        0, 
        CLASS_NAME, 
        L"Triangle Drawing", 
        WS_OVERLAPPEDWINDOW, 
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 
        NULL, 
        NULL, 
        hInstance, 
        NULL 
    );

    if (hwnd == NULL) { 
        return 0; 
    }

    ShowWindow(hwnd, nCmdShow); 

    MSG msg = { 0 }; 
    while (GetMessage(&msg, NULL, 0, 0)) { 
        TranslateMessage(&msg); 
        DispatchMessage(&msg); 
    }

    return msg.wParam; 
}
