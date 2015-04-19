#pragma once

#include "Control.h"

class ListBox : public Control {
public:
    ListBox() :
    _columns(0) {

    }

    ListBox(int id, HWND parent) :
    Control(id, parent),
    _columns(0) {

    }

    void AddColumn(std::wstring name, int width);
    void InsertColumn(int index, std::wstring name, int width);

    void AddItem(std::wstring);

private:
    int _columns;
};