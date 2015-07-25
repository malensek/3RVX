#pragma once

#pragma comment(lib, "gdiplus.lib")

#include <Windows.h>
#include <gdiplus.h>

#include <list>
#include <vector>

#include "SkinInfo.h"
#include "../TinyXml2/tinyxml2.h"

class Meter;
class SliderKnob;
class SoundPlayer;

#define SKIN_DEFAULT_UNITS 10

class Skin : SkinInfo {
public:
    Skin(std::wstring skinXML);
    ~Skin();

    bool HasOSD(char *osdName);
    int DefaultVolumeUnits();

public:
    Gdiplus::Bitmap *volumeBackground;
    Gdiplus::Bitmap *volumeMask;
    std::vector<Meter *> volumeMeters;
    std::vector<HICON> volumeIconset;
    SoundPlayer *volumeSound;

    Gdiplus::Bitmap *muteBackground;
    Gdiplus::Bitmap *muteMask;

    Gdiplus::Bitmap *ejectBackground;
    Gdiplus::Bitmap *ejectMask;

    Gdiplus::Bitmap *volumeSliderBackground;
    Gdiplus::Bitmap *volumeSliderMask;
    std::vector<Meter *> volumeSliderMeters;
    SliderKnob *volumeSliderKnob;

private:
    Gdiplus::Bitmap *OSDBgImg(char *osdName);
    Gdiplus::Bitmap *OSDMask(char *osdName);
    std::vector<Meter *> OSDMeters(char *osdName);
    tinyxml2::XMLElement *OSDXMLElement(char *osdName);
    SoundPlayer *OSDSound(char *osdName);

    std::vector<HICON> Iconset(char *osdName);

    Gdiplus::Bitmap *SliderBgImg(char *sliderName);
    Gdiplus::Bitmap *SliderMask(char *sliderName);
    std::vector<Meter *> SliderMeters(char *osdName);   
    tinyxml2::XMLElement *SliderXMLElement(char *sliderName);
    SliderKnob *Knob(char *sliderName);

    Meter *LoadMeter(tinyxml2::XMLElement *meterXMLElement);

    Gdiplus::Bitmap *Image(tinyxml2::XMLElement *element, char *attrName);
    std::wstring ImageName(tinyxml2::XMLElement *meterXMLElement);
    Gdiplus::Font *Font(tinyxml2::XMLElement *meterXMLElement);
    Gdiplus::StringAlignment Alignment(tinyxml2::XMLElement *meterXMLElement);

    int DefaultOSDUnits(char *osdName);
};
