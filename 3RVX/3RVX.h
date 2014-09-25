#pragma once

#include <windows.h>
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")
#include <string>

#include "Controllers\Volume\CoreAudio.h"
#include "HotkeyManager.h"

static const UINT WM_3RVX_CONTROL = RegisterWindowMessage(L"WM_3RVX_CONTROL");
#define MSG_LOAD WM_APP + 100

HINSTANCE hInst;
ULONG_PTR gdiplusToken;

HWND mainWnd;
HWND CreateMainWnd(HINSTANCE hInstance);

LRESULT CALLBACK CbtProc(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

CoreAudio *ca;

void Init();