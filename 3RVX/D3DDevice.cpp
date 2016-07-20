// Copyright (c) 2016, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#include "D3DDevice.h"

#include <d3d9.h>
#pragma comment(lib, "d3d9.lib")

#include "Logger.h"

D3DDevice::D3DDevice(HWND hWnd) {
    DWORD behaviorFlags = D3DCREATE_HARDWARE_VERTEXPROCESSING;
    D3DPRESENT_PARAMETERS d3dpp = { 0 };
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;

    Direct3DCreate9Ex(D3D_SDK_VERSION, &_pDirect3DEx);
    HRESULT hr = _pDirect3DEx->CreateDeviceEx(
        D3DADAPTER_DEFAULT,
        D3DDEVTYPE_HAL,
        hWnd,
        behaviorFlags,
        &d3dpp,
        NULL,
        &_pDeviceEx);
}

D3DDevice::~D3DDevice() {
    if (_pDeviceEx) {
        _pDeviceEx->Release();
    }
    if (_pDirect3DEx) {
        _pDirect3DEx->Release();
    }
}

bool D3DDevice::Occluded() {
    HRESULT hr = _pDeviceEx->CheckDeviceState(NULL);
    return (hr == S_PRESENT_OCCLUDED);
}
