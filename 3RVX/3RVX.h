#pragma once

#include <windows.h>
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")
#include <string>
#include <unordered_map>

#include "Controllers\Volume\VolumeController.h"
#include "Controllers\Volume\CoreAudio.h"
#include "HotkeyManager.h"
#include "VolumeOSD.h"

#define CLASS_3RVX L"3RVXv3"

static const UINT WM_3RVX_CONTROL = RegisterWindowMessage(L"WM_3RVX_CONTROL");
#define MSG_LOAD WM_APP + 100

HINSTANCE hInst;
ULONG_PTR gdiplusToken;

HWND mainWnd;
HWND CreateMainWnd(HINSTANCE hInstance);

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

CoreAudio *volCtrl;
VolumeOSD *vOsd;

std::unordered_map<int, int> hotkeys;

void Init();