// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#pragma once

#include <Windows.h>

#include <string>
#include <vector>

class Skin;
struct OSDComponent;
struct SliderComponent;

class SkinManager {
public:
    static SkinManager *Instance();
    void LoadSkin(std::wstring skinXML);

    OSDComponent *VolumeOSD();
    std::vector<HICON> &VolumeIconset();
    SliderComponent *VolumeSlider();

    OSDComponent *MuteOSD();

    OSDComponent *EjectOSD();
    std::vector<HICON> &EjectIconset();

    OSDComponent *BrightnessOSD();

private:
    OSDComponent *_volumeOSD;
    std::vector<HICON> _volumeIconset;
    SliderComponent *_volumeSlider;
    OSDComponent *_muteOSD;

    OSDComponent *_ejectOSD;
    std::vector<HICON> _ejectIconset;

    OSDComponent *_brightnessOSD;

    ~SkinManager();
    void DisposeComponents();

private:
    static SkinManager *instance;

};