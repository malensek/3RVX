#pragma once

#include <windows.h>
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")

#include <iostream>
#include <list>
#include <string>
#include <hash_map>

struct HotkeyData
{
    std::wstring handler;
    int action;
    std::wstring arg;
};

// Forward Declarations -----------
void Init();

HWND CreateMainWnd(HINSTANCE hInstance);
HHOOK CreateCbtHook();

LRESULT CALLBACK CbtProc(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


// Globals ------------------------
HINSTANCE        hInst;
HWND             mainWnd;
ULONG_PTR        gdiplusToken;


// Message-related ----------------
static const UINT WM_3RVX_CONTROL = RegisterWindowMessage(L"WM_3RVX_CONTROL");
#define MSG_LOAD 100