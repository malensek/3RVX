#pragma once

#include <list>
#include <vector>

#include "../TinyXml2/tinyxml2.h"
#include "SkinInfo.h"

class Meter;
class SliderKnob;
class SoundPlayer;

struct OSDComponent;
struct SkinComponent;
struct SliderComponent;

using tinyxml2::XMLElement;

class Skin : public SkinInfo {
public:
    Skin(std::wstring skinXML);

public:
    virtual OSDComponent *VolumeOSD() = 0;
    virtual std::vector<HICON> VolumeIconset() = 0;
    virtual SliderComponent *VolumeSlider() = 0;

    virtual OSDComponent *MuteOSD() = 0;

    virtual OSDComponent *EjectOSD() = 0;

protected:
    XMLElement *SubElement(char *parent, char *child);

protected:
    static const int DEFAULT_UNITS = 10;
};
