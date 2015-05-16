#pragma once

#include "Control.h"
#include "../../3RVX/Logger.h"

class Label : public Control {
public:
    Label() {

    }

    Label(int id, HWND parent) :
    Control(id, parent) {

    }

    virtual bool Text(std::wstring text) {
        bool result = Control::Text(text);
        if (result == false) {
            return false;
        }

        Width(TextDimensions().cx);


//        /* Determine the width of the text */
//        HDC dc = GetDC(_hWnd);
//        SIZE sz = { 0 };
//        GetTextExtentPoint32(dc, &text[0], text.size(), &sz);

//        /* Update the window size based on the text size */
//        RECT r = Control::ScreenDimensions();
//        POINT pt = { r.left, r.top };
//        ScreenToClient(_parent, &pt);
//        MoveWindow(_hWnd, pt.x, pt.y, sz.cx, sz.cy, TRUE);
        return true;
    }
};