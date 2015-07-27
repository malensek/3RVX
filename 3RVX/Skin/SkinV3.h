#pragma once

#pragma comment(lib, "gdiplus.lib")

#include <Windows.h>
#include <gdiplus.h>

#include <string>
#include <vector>

#include "Skin.h"

class Meter;
class MeterComponent;
class SliderKnob;
class SoundPlayer;

class SkinV3 : public Skin {
public:
    SkinV3(std::wstring skinXML);
    ~SkinV3();

    virtual OSDComponent *VolumeOSD();
    virtual OSDComponent *MuteOSD();
    virtual OSDComponent *EjectOSD();

    virtual std::vector<HICON> VolumeIconset();

    virtual SliderComponent *VolumeSlider();

private:
    OSDComponent *CreateOSDComponent(char *osdType);
    SliderComponent *CreateSliderComponent(char *sliderType);
    bool PopulateMeterComponent(MeterComponent *component, XMLElement *elem);

    int DefaultUnits(XMLElement *elem);
    Gdiplus::Bitmap *Image(XMLElement *element, char *attrName);
    std::wstring ImageName(XMLElement *elem);

    std::vector<HICON> Iconset(XMLElement *elem);
    SliderKnob *Knob(XMLElement *elem);
    SoundPlayer *Sound(XMLElement *elem);

    std::vector<Meter *> Meters(XMLElement *parentElement);
    Meter *LoadMeter(XMLElement *meterXMLElement);
    Gdiplus::Font *Font(XMLElement *meterXMLElement);
    Gdiplus::StringAlignment Alignment(XMLElement *meterXMLElement);
};
