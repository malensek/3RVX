#pragma once

#include <Windows.h>
#include <functional>
#include <string>

/// <summary>
/// Base class for Windows forms control implementations (buttons, checkboxes,
/// etc). Many common features, like control dimensions, window text, and styles
/// are managed by this class.
/// </summary>
class Control {
public:
    Control();
    Control(int id, HWND parent);
    ~Control();

    /// <summary>
    /// Retrieves the dimensions of this control's window based on screen
    /// coordinates.
    /// </summary>
    virtual RECT ScreenDimensions();

    /// <summary>
    /// Retrieves the dimensions of this control's window based its client
    /// coordinates (position relative to the parent window). Note: If only
    /// interested in the width and height of the control, use
    /// ScreenDimensions().
    /// </summary>
    virtual RECT ClientDimensions();

    virtual void Enable();
    virtual void Disable();
    virtual bool Enabled();
    virtual void Enabled(bool enabled);

    virtual std::wstring Text();
    virtual int TextAsInt();
    virtual bool Text(std::wstring text);
    virtual bool Text(int value);

    virtual bool Visible();
    virtual void Visible(bool visible);

    void WindowExStyle();
    void WindowExStyle(long exStyle);
    void AddWindowExStyle(long exStyle);
    void RemoveWindowExStyle(long exStyle);

    /// <summary>Handles WM_COMMAND messages.</summary>
    /// <param name="nCode">Control-defined notification code</param>
    virtual DLGPROC Command(unsigned short nCode);

    /// <summary>Handles WM_NOTIFY messages.</summary>
    /// <param name="nHdr">Notification header structure</param>
    virtual DLGPROC Notification(NMHDR *nHdr);

protected:
    int _id;
    HWND _hWnd;
    HWND _parent;

protected:
    static const int MAX_EDITSTR = 4096;
};