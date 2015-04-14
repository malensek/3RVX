#pragma once

#include "Control.h"

class Spinner : public Control {
public:
    Spinner() {

    }

    Spinner(int id, HWND parent) :
    Control(id, parent) {

    }

    void Buddy(int buddyId);

    /// <summary>Sets the range (min, max) for the spin control.</summary>
    /// <param name="lo">Lower bound for the spinner.</param>
    /// <param name="hi">Upper bound for the spinner.</param>
    void Range(int lo, int hi);

    std::wstring Text();
    int TextAsInt();
    bool Text(std::wstring text);
    bool Text(int value);

private:
    int _buddyId;
    HWND _buddyWnd;
};
