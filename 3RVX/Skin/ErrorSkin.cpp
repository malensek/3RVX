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

std::vector<HICON> ErrorSkin::VolumeIconset() {
    Error::ErrorMessageDie(Error::SKINERR_NO_RESOURCE, L"Volume Iconset");
    return std::vector<HICON>();
}

SliderComponent *ErrorSkin::VolumeSlider() {
    Error::ErrorMessageDie(Error::SKINERR_NO_RESOURCE, L"Volume Slider");
    return nullptr;
}

