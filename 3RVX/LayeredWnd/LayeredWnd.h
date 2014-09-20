#pragma once

#include <windows.h>
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")

class LayeredWnd {

public:
    LayeredWnd(HINSTANCE hInstance, LPCWSTR className, LPCWSTR title)
        : m_hInstance(hInstance),
        m_className(className),
        m_title(title),
        m_buffer(NULL),
        m_dirtyRect(NULL) {
    };

    ~LayeredWnd();

    bool Init();
    void Show();
    void Hide();

    byte Transparency() const;
    void Transparency(byte transparency);

    int X() const;
    int Y() const;
    void X(int x);
    void Y(int y);

    int Width() const;
    int Height() const;

    Gdiplus::Bitmap *Image();
    void Image(Gdiplus::Bitmap *image);

    HWND Hwnd() const;

private:
    HINSTANCE m_hInstance;
    LPCWSTR m_className;
    LPCWSTR m_title;
    HWND m_hWnd;

    POINT m_location;
    SIZE m_size;

    byte m_transparency;
    RECT *m_dirtyRect;

    Gdiplus::Bitmap *m_buffer;

    void UpdateLayeredWnd();
    void UpdateLocation();
    void UpdateTransparency();

    static LRESULT CALLBACK StaticWndProc(HWND hWnd, UINT message,
        WPARAM wParam, LPARAM lParam);
    virtual LRESULT WndProc(UINT message, WPARAM wParam, LPARAM lParam);
};