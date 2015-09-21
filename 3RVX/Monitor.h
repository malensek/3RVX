// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#pragma once

#include <Windows.h>
#include <list>
#include <string>
#include <unordered_map>

class Monitor {
public:
    Monitor() {

    }

    Monitor(HMONITOR handle, std::wstring name,
        int x, int y, int width, int height) :
    _handle(handle),
    _name(name),
    _x(x),
    _y(y),
    _width(width),
    _height(height) {

    }

    Monitor(HMONITOR handle, std::wstring name, RECT rect) :
    _handle(handle),
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

    HMONITOR Handle() {
        return _handle;
    }

    std::wstring Name() {
        return _name;
    }

private:
    HMONITOR _handle;
    std::wstring _name;
    int _x;
    int _y;
    int _width;
    int _height;
};