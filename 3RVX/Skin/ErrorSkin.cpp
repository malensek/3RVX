// Copyright (c) 2016, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#include "ErrorSkin.h"

#include "../Error.h"

OSDComponent *ErrorSkin::VolumeOSD() {
    Error::ErrorMessageDie(Error::SKINERR_NO_RESOURCE, L"Volume OSD");
    return nullptr;
}

OSDComponent *ErrorSkin::MuteOSD() {
    Error::ErrorMessageDie(Error::SKINERR_NO_RESOURCE, L"Mute OSD");
    return nullptr;
}

OSDComponent *ErrorSkin::EjectOSD() {
    Error::ErrorMessageDie(Error::SKINERR_NO_RESOURCE, L"Eject OSD");
    return nullptr;
}

OSDComponent *ErrorSkin::BrightnessOSD() {
    Error::ErrorMessageDie(Error::SKINERR_NO_RESOURCE, L"Brightness OSD");
    return nullptr;
}

std::vector<HICON> ErrorSkin::EjectIconset() {
    Error::ErrorMessageDie(Error::SKINERR_NO_RESOURCE, L"Eject Iconset");
    return std::vector<HICON>();
}

std::vector<HICON> ErrorSkin::VolumeIconset() {
    Error::ErrorMessageDie(Error::SKINERR_NO_RESOURCE, L"Volume Iconset");
    return std::vector<HICON>();
}

SliderComponent *ErrorSkin::VolumeSlider() {
    Error::ErrorMessageDie(Error::SKINERR_NO_RESOURCE, L"Volume Slider");
    return nullptr;
}

