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

    int dispChars = 1;
    if (digits[1] > 0) {
        dispChars = 2;
    } else if (digits[2] > 0) {
        dispChars = 3;
    }

    int drawX = _charWidth * 2;
    if (_align == Left) {
        drawX = (dispChars - 1) * _charWidth;
    } else if (_align == Center) {
        drawX = (_charWidth * 2) - ((3 - dispChars) * (_charWidth / 2));
    }

    for (int i = 0, x = drawX; i < dispChars; ++i, x -= _charWidth) {
        int digit = digits[i];
        CLOG(L"Drawing digit [%d]: (%d, %d); %dx%d", digit,
            _rect.X + x, _rect.Y, _charWidth, _rect.Height);

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