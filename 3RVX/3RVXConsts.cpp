#include "3RVX.h"

const wchar_t *_3RVX::CLASS_3RVX = L"3RVXv3";
const wchar_t *_3RVX::CLASS_3RVX_SETTINGS = L"3RVXv3-Settings";

const UINT _3RVX::WM_3RVX_CTRL
    = RegisterWindowMessage(L"WM_3RVX_CTRL");
const UINT _3RVX::WM_3RVX_SETTINGSCTRL
    = RegisterWindowMessage(L"WM_3RVX_SETTINGSCTRL");
