#pragma once

#include <list>
#include <vector>

struct OSDComponent;
struct SkinComponent;
struct SliderComponent;

class Skin {
public:
    virtual OSDComponent *VolumeOSD() = 0;
    virtual std::vector<HICON> VolumeIconset() = 0;
    virtual SliderComponent *VolumeSlider() = 0;
    virtual OSDComponent *MuteOSD() = 0;

    virtual OSDComponent *EjectOSD() = 0;
};
