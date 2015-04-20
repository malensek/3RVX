#pragma once

#include "Control.h"

class ListView : public Control {
public:
    ListView() {

    }

    ListView(int id, HWND parent) :
    Control(id, parent) {

    }

    void AddColumn(std::wstring name, int width);
    void InsertColumn(int index, std::wstring name, int width);

private:
    int _columns;
};