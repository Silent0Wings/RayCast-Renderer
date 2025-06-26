#include "guiwindow.h"

// global image to show on screen
HBITMAP hBitmap = nullptr;
image *g_imgPtr = nullptr; // global pointer to current image

// create HBITMAP from image class
HBITMAP imageToHBITMAP(const image &img)
{
    int width = img.getwidth();
    int height = img.getheight();

    BITMAPINFO bmi = {};
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = width;
    bmi.bmiHeader.biHeight = -height;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 24;
    bmi.bmiHeader.biCompression = BI_RGB;

    void *pixels = nullptr;
    HDC screen = GetDC(NULL);
    HBITMAP hBmp = CreateDIBSection(screen, &bmi, DIB_RGB_COLORS, &pixels, NULL, 0);
    ReleaseDC(NULL, screen);

    if (!pixels)
        return NULL;

    unsigned char *dst = (unsigned char *)pixels;
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            const color &c = img.get(y, x);
            int i = (y * width + x) * 3;
            dst[i + 0] = c.b();
            dst[i + 1] = c.g();
            dst[i + 2] = c.r();
        }
    }

    return hBmp;
}

// this is the function that handles window events
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        HDC memDC = CreateCompatibleDC(hdc);
        SelectObject(memDC, hBitmap);
        BitBlt(hdc, 0, 0, 640, 480, memDC, 0, 0, SRCCOPY);
        DeleteDC(memDC);
        EndPaint(hwnd, &ps);
        return 0;
    }
    case WM_TIMER:
        if (g_imgPtr)
        {
            DeleteObject(hBitmap);
            hBitmap = imageToHBITMAP(*g_imgPtr);
        }
        InvalidateRect(hwnd, NULL, FALSE); // trigger redraw
        return 0;

    case WM_DESTROY:
        DeleteObject(hBitmap);
        PostQuitMessage(0);
        return 0;

    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
}

// fallback test image
void createImageInMemory(int width, int height)
{
    BITMAPINFO bmi = {};
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = width;
    bmi.bmiHeader.biHeight = -height;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 24;
    bmi.bmiHeader.biCompression = BI_RGB;

    void *pixels = nullptr;
    HDC screen = GetDC(NULL);
    hBitmap = CreateDIBSection(screen, &bmi, DIB_RGB_COLORS, &pixels, NULL, 0);
    ReleaseDC(NULL, screen);

    if (pixels)
    {
        unsigned char *p = (unsigned char *)pixels;
        for (int y = 0; y < height; ++y)
        {
            for (int x = 0; x < width; ++x)
            {
                int idx = (y * width + x) * 3;
                p[idx + 0] = x % 256;
                p[idx + 1] = y % 256;
                p[idx + 2] = (x + y) % 256;
            }
        }
    }
}

// main run of window
int guiwindow::run(HINSTANCE hInstance, int nCmdShow)
{
    if (m_img.getheight() == 0 || m_img.getwidth() == 0)
        createImageInMemory(640, 480);
    else
        hBitmap = imageToHBITMAP(m_img);

    if (!hBitmap)
        MessageBoxW(NULL, L"Bitmap creation failed", L"Error", MB_OK);

    g_imgPtr = &m_img;

    WNDCLASSW wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"MyWindowClass";
    RegisterClassW(&wc);

    HWND hwnd = CreateWindowW(L"MyWindowClass", L"Image Window", WS_OVERLAPPEDWINDOW,
                              CW_USEDEFAULT, CW_USEDEFAULT, 660, 520, NULL, NULL, hInstance, NULL);
    ShowWindow(hwnd, nCmdShow);

    // set 60 FPS timer
    SetTimer(hwnd, 1, 16, NULL);

    MSG msg;
    while (GetMessageW(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    return 0;
}