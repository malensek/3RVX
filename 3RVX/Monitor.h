#pragma once

#include <Windows.h>
#include <list>
#include <string>
#include <unordered_map>

class Monitor {
public:
    Monitor() {

    }

    Monitor(std::wstring name, int x, int y, int width, int height) :
    _name(name),
    _x(x),
    _y(y),
    _width(width),
    _height(height) {

    }

    Monitor(std::wstring name, RECT rect) :
    _name(name),
    _x(rect.left),
    _y(rect.top),
    _width(rect.right - rect.left),
    _height(rect.bottom - rect.top) {

    }

    int X() {
        return _x;
    }

    int Y() {
        return _y;
    }

    int Width() {
        return _width;
    }

    int Height() {
        return _height;
    }

private:
    std::wstring _name;
    int _x;
    int _y;
    int _width;
    int _height;
};