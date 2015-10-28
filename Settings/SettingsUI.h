#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <malloc.h>
#include <memory.h>
#include <prsht.h>
#include <stdlib.h>
#include <tchar.h>

#include "../3RVX/3RVX.h"
#include "resource.h"


class SettingsUI : public Window {
public:
    SettingsUI(HINSTANCE hInstance);

    INT_PTR LaunchPropertySheet();

    virtual LRESULT WndProc(HWND hWnd, UINT message,
        WPARAM wParam, LPARAM lParam);


private:

};

/* Forward Declarations */
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
int CALLBACK PropSheetProc(HWND hwndDlg, UINT uMsg, LPARAM lParam);

BOOL CALLBACK GeneralTabProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DisplayTabProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK OSDTabProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK HotkeyTabProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK AboutTabProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);