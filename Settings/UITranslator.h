#pragma once

#include <Windows.h>
#include <string>

/// <summary>
/// Handles translation of UI elements defined as Win32 controls.
/// </summary>
class UITranslator {
public:
    /// <summary>
    /// Reads a control's text, translates it, and updates the text.
    /// </summary>
    static void TranslateControlText(HWND hWnd, int dlgItem);

    /// <summary>
    /// Sets a control's text to the translation of the provided string.
    /// </summary>
    static void TranslateControlText(HWND hWnd, int dlgItem, std::wstring &str);
    static void TranslateWindowText(HWND hWnd);
};