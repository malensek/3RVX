// Copyright (c) 2016, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#pragma once

#include <Windows.h>
#include <d3d9.h>

class D3DDevice {
public:
    D3DDevice(HWND hWnd);
    ~D3DDevice();

    bool Occluded();

private:
    IDirect3D9Ex *_pDirect3DEx;
    LPDIRECT3DDEVICE9EX _pDeviceEx;
};