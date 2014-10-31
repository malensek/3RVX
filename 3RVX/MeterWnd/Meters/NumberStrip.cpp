#include "NumberStrip.h"

#include <sstream>

void NumberStrip::Draw(Gdiplus::Bitmap *buffer, Gdiplus::Graphics *graphics) {
    int units = CalcUnits();
    int perc = units * (100 / _units);

    int digits[] = {
        perc % 10,
        (perc / 10) % 10,
        perc / 100,
    };

    int chars = 1;
    if (digits[1] > 0) {
        chars = 2;
    } else if (digits[2] > 0) {
        chars = 3;
    }

    int drawX = _charWidth * 2;
    if (_align == Gdiplus::StringAlignmentNear) {
        drawX = (chars - 1) * _charWidth;
    } else if (_align == Gdiplus::StringAlignmentFar) {
        drawX = (_charWidth * 2) - ((3 - chars) * (_charWidth / 2));
    }

    for (int i = 0, x = drawX; i < chars; ++i, x -= _charWidth) {
        int digit = digits[i];
        QCLOG(L"Drawing digit [%d]; x-offset: %d", digit, _rect.X + x);

        Gdiplus::Rect destRect(_rect.X + x, _rect.Y, _charWidth, _rect.Height);
        graphics->DrawImage(_bitmap, destRect,
            0, digit * _rect.Height, _charWidth, _rect.Height,
            Gdiplus::UnitPixel, NULL, NULL, NULL);
    }

    _lastValue = Value();
    _lastUnits = units;
}

std::wstring NumberStrip::ToString() {
    std::wstringstream ss;
    ss << std::endl;
    ss << "Text Alignment: " << _align;
    return Meter::ToString() + ss.str();
}