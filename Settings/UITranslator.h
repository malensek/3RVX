#pragma once

#include <Windows.h>
#include <string>

class UITranslator {
public:
    static void TranslateControlText(HWND hWnd, int dlgItem);
    static void TranslateControlText(HWND hWnd, int dlgItem, std::wstring &str);
    static void TranslateWindowText(HWND hWnd);
};