#pragma once

#include <Windows.h>
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")

class LayeredWnd {
public:
    LayeredWnd(LPCWSTR className, LPCWSTR title, HINSTANCE hInstance = NULL,
        Gdiplus::Bitmap *bitmap = NULL, DWORD exStyles = NULL);
    ~LayeredWnd();

    virtual bool AlwaysOnTop();
    virtual void AlwaysOnTop(bool onTop);

    virtual Gdiplus::Bitmap *Bitmap();
    virtual void Bitmap(Gdiplus::Bitmap *bitmap);

    /// <summary>
    /// Enables the blurred 'glass' background available on Vista and Windows 7.
    /// Has no effect on other Windows versions.
    /// </summary>
    /// <param name="mask">
    /// Bitmap that defines the region that should show glass. Black pixels will
    /// reveal the glass effect underneath, whereas white pixels obscure the
    /// glass.
    /// </param>
    /// <returns>true if successful, false otherwise.</returns>
    virtual bool EnableGlass(Gdiplus::Bitmap *mask);

    /// <summary>Disables the blurred glass effect.</summary>
    virtual bool DisableGlass();

    virtual byte Transparency();
    virtual void Transparency(byte transparency);

    virtual void Show();
    virtual void Hide();

    int X();
    int Y();
    void X(int x);
    void Y(int y);

    int Width();
    int Height();

    POINT Position();
    void Position(int x, int y);

protected:
    HINSTANCE _hInstance;
    LPCWSTR _className;
    LPCWSTR _title;
    HWND _hWnd;

    bool _visible;
    POINT _location;
    SIZE _size;
    byte _transparency;

    Gdiplus::Bitmap *_bitmap;
    Gdiplus::Bitmap *_glassMask;

    /// <summary>
    /// Updates layered window properties. Called after the window bitmap
    /// changes.
    /// </summary>
    /// <param name="dirtyRect">
    /// If non-NULL, specifies a 'dirty region' of the window to update.
    /// Otherwise, the entire window is updated.
    /// </param>
    void UpdateWindow(RECT *dirtyRect = NULL);

    /// <summary>
    /// Updates the transparency level (0 - 255) of the window.
    /// </summary>
    void UpdateTransparency();
    void UpdateWindowPosition();

    static LRESULT CALLBACK StaticWndProc(HWND hWnd, UINT message,
        WPARAM wParam, LPARAM lParam);
    virtual LRESULT WndProc(UINT message, WPARAM wParam, LPARAM lParam);
};