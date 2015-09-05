#pragma once

#include <Windows.h>
#include <functional>
#include <string>
#include <vector>

class Dialog;

/// <summary>
/// Base class for Windows forms control implementations (buttons, checkboxes,
/// etc). Many common features, like control dimensions, window text, and styles
/// are managed by this class.
/// </summary>
class Control {
public:
    Control();
    Control(int id, HWND parent);
    Control(int id, Dialog &parent, bool translate = true);
    ~Control();

    int ID();

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

    virtual SIZE TextDimensions();
    virtual SIZE TextDimensions(std::wstring &text);

    virtual void Enable();
    virtual void Disable();
    virtual bool Enabled();
    virtual void Enabled(bool enabled);

    virtual int EmSize();

    virtual void Move(int x, int y);
    virtual void PlaceRightOf(Control &control);
    virtual void Resize(int width, int height);

    virtual int X();
    virtual int Y();
    virtual int Width();
    virtual int Height();
    virtual void X(int x);
    virtual void Y(int y);
    virtual void Width(int width);
    virtual void Height(int height);

    virtual std::wstring Text();
    virtual int TextAsInt();
    virtual bool Text(std::wstring text);
    virtual bool Text(int value);

    virtual void Translate();

    virtual bool Visible();
    virtual void Visible(bool visible);

    /// <summary>Retrieve the extended styles for this control.</summary>
    long WindowExStyle();

    /// <summary>Set the extended styles for this control.</summary>
    void WindowExStyle(long exStyle);

    /// <summary>Adds an extended style to this control.</summary>
    void AddWindowExStyle(long exStyle);

    /// <summary>Removes an extended style from this control.</summary>
    void RemoveWindowExStyle(long exStyle);

    /// <summary>Handles WM_COMMAND messages.</summary>
    /// <param name="nCode">Control-defined notification code</param>
    virtual BOOL CALLBACK Command(unsigned short nCode);

    /// <summary>Handles WM_NOTIFY messages.</summary>
    /// <param name="nHdr">Notification header structure</param>
    virtual BOOL CALLBACK Notification(NMHDR *nHdr);

protected:
    int _id;
    HWND _hWnd;
    HWND _parent;
    Dialog *_parentDlg;

protected:
    static const int MAX_EDITSTR = 4096;
};