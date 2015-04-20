#pragma once

#include <Windows.h>
#include <CommCtrl.h>

#include "Control.h"

class ListView : public Control {
public:
    ListView() :
    _columns(0) {

    }

    ListView(int id, HWND parent) :
    Control(id, parent),
    _columns(0) {

    }

    void AddListExStyle(DWORD style);

    void AddColumn(std::wstring name, int width);
    void InsertColumn(int index, std::wstring name, int width);

    int AddItem(std::wstring item);
    int InsertItem(int index, std::wstring item);

    void ItemText(int index, int subIndex, std::wstring str);

    int Selection();
    int Size();

public:
    /* Event Handlers */
    std::function<void(NMLISTVIEW *lv)> OnItemChange;
private:
    int _columns;
};