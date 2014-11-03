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
#define MSG_SETTINGS WM_APP + 101

HANDLE mutex;
HINSTANCE hInst;
ULONG_PTR gdiplusToken;
HWND mainWnd;

CoreAudio *volCtrl;
VolumeOSD *vOsd;
std::unordered_map<int, int> hotkeys;

HWND CreateMainWnd(HINSTANCE hInstance);
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void Init();