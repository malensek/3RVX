#pragma once

#include "Control.h"

class ListBox : public Control {
public:
    ListBox() {

    }

    ListBox(int id, HWND parent) :
    Control(id, parent) {

    }

    void AddColumn(std::wstring name, int width);
    void InsertColumn(int index, std::wstring name, int width);

    void AddItem(std::wstring);

private:
    int _columns;
};