// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#include "OSD.h"

#include "../resource.h"

#include "../../3RVX/Window.h"

void OSD::Initialize() {
    _osdList = new ListView(LST_OSDS, *this);
    _osdList->AddListExStyle(LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT);
    _osdList->AddColumn(L"OSD", (int) (_osdList->Width() * .97f));
    _osdList->AddItem(L"Volume");
    _osdList->AddItem(L"Brightness");
    _osdList->AddItem(L"Eject");
    _osdList->AddItem(L"Keyboard");
    _monitorVolEvents = new Checkbox(CHK_MONITORVOL, *this);
    _volumeGroup = new GroupBox(GRP_VOLUME, *this);
    _volumeGroup->AddChildren({ _monitorVolEvents });

}

void OSD::LoadSettings() {

}

void OSD::SaveSettings() {

}