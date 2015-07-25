#pragma once

#pragma comment(lib, "gdiplus.lib")

#include <Windows.h>
#include <gdiplus.h>

#include <string>
#include <vector>

#include "../TinyXml2/tinyxml2.h"
#include "Skin.h"

class Meter;
class OSDSkin;
class SliderKnob;
class SoundPlayer;

class SkinV3 : public Skin {
public:
    SkinV3(std::wstring skinXML);
    ~SkinV3();

private:
    OSDComponent *_volumeOSD;
    OSDComponent *_muteOSD;
    OSDComponent *_ejectOSD;

};
