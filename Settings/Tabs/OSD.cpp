// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#include "OSD.h"

#include "../resource.h"

void OSD::Initialize() {
    ListBox *lb = new ListBox(LST_OSDS, *this);
    lb->AddItem(L"Volume");
    lb->AddItem(L"Brightness");
    lb->AddItem(L"Eject");
    lb->AddItem(L"Keyboard");
}

void OSD::LoadSettings() {

}

void OSD::SaveSettings() {

}