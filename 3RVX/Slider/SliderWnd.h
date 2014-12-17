#pragma once

#include "..\MeterWnd\MeterWnd.h"

class SliderKnob;

class SliderWnd : public MeterWnd {
public:
    SliderWnd(LPCWSTR className, LPCWSTR title, HINSTANCE hInstance = NULL);

    virtual void Show();
    void Knob(SliderKnob *knob);

private:
    SliderKnob *_knob;
    bool _dragging;
    int _dragOffset;

    void PositionWindow();
    bool MouseOverKnob(int x, int y);
    bool MouseOverTrack(int x, int y);
    void UpdateKnob(int x, int y);

    virtual LRESULT WndProc(UINT message, WPARAM wParam, LPARAM lParam);
};