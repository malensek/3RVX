#include "MultiSkin.h"

OSDComponent * MultiSkin::VolumeOSD() {
    return nullptr;
}

OSDComponent *MultiSkin::MuteOSD() {
    return nullptr;
}

OSDComponent * MultiSkin::EjectOSD() {
    return nullptr;
}

std::vector<HICON> MultiSkin::VolumeIconset() {
    return std::vector<HICON>();
}

SliderComponent * MultiSkin::VolumeSlider() {
    return nullptr;
}
