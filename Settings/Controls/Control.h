// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

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
    Control(int id, Dialog &parent, bool translate = true);
    ~Control();

    int ID();
    HWND Handle();

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

    /// <summary>
    /// Retrieve the window attributes of this control. See the GetWindowLongPtr
    /// Win32 function for index values.
    /// </summary>
    /// <param name="index">
    /// The index of the attribute to retrieve. Common indexes include
    /// GWL_STYLE, GWL_EXSTYLE, etc.
    /// </param>
	LONG_PTR WindowAttributes(int index);

    /// <summary>
    /// Sets the window attributes of this control. See the SetWindowLongPtr
    /// Win32 function for index values.
    /// </summary>
    /// <param name="index">
    /// The index of the attribute to retrieve. Common indexes include
    /// GWL_STYLE, GWL_EXSTYLE, etc.
    /// </param>
    /// <param name="value">New value of the attribute</param>
    void WindowAttributes(int index, LONG_PTR value);

    /// <summary>
    /// Adds a window attribute to this control; same as retrieving the window
    /// attributes, ORing the attribute with the original value, and then
    /// updating with the new window attributes.
    /// </summary>
    void AddWindowAttribute(int index, long attribute);

    /// <summary>
    /// Removes a window attribute from this control; same as retrieving the
    /// window attributes, NOTing the attribute with the original value, and
    /// then updating with the new window attributes.
    /// </summary>
    void RemoveWindowAttribute(int index, long attribute);

    /// <summary>Handles WM_COMMAND messages.</summary>
    /// <param name="nCode">Control-defined notification code</param>
    virtual BOOL CALLBACK Command(unsigned short nCode);

    /// <summary>Handles WM_NOTIFY messages.</summary>
    /// <param name="nHdr">Notification header structure</param>
    virtual BOOL CALLBACK Notification(NMHDR *nHdr);

    /// <summary>Handles WM_HSCROLL/WM_VSCROLL messages.</summary>
    /// <param name="horizontal">
    /// Set to true if this is a horizontal scroll (WM_HSCROLL) message.
    /// </param>
    /// <param name="request">
    /// The scroll request. See SB_* constants defined for
    /// WM_HSCROLL/WM_VSCROLL.
    /// </param>
    /// <param name="position">
    /// The scroll position. Only valid when the scroll request is
    /// SB_THUMBPOSITION or SB_THUMBTRACK.
    /// </param>
    virtual BOOL Scroll(bool horizontal, WORD request, WORD position);

protected:
    int _id;
    HWND _hWnd;
    HWND _parent;
    Dialog *_parentDlg;

protected:
    static const int MAX_EDITSTR = 4096;
};