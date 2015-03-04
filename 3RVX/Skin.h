#pragma once

#include <Windows.h>
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")

#include <list>
#include <vector>

#include "SkinInfo.h"
#include "TinyXml2/tinyxml2.h"

class Meter;
class SliderKnob;

#define SKINS_DIR L"Skins"
#define SKIN_XML L"skin.xml"
#define SKIN_DEFAULT_UNITS 10

class Skin : SkinInfo {
public:
    Skin(std::wstring skinName) :
        SkinInfo(skinName) {

    }

    bool HasOSD(char *osdName);

    Gdiplus::Bitmap *OSDBgImg(char *osdName);
    Gdiplus::Bitmap *SliderBgImg(char *sliderName);

    Gdiplus::Bitmap *OSDMask();
    Gdiplus::Bitmap *SliderMask();

    std::list<Meter *> VolumeMeters();
    std::list<Meter *> VolumeSliderMeters();
    SliderKnob *Knob(char *sliderName);
    std::vector<HICON> Iconset(char *osdName);

    int DefaultVolumeUnits();

private:
    std::list<Meter *> _volumeMeters;
    std::list<Meter *> _volumeSliderMeters;

    Gdiplus::Bitmap *BgImg(tinyxml2::XMLElement *element);

    std::list<Meter *> OSDMeters(char *osdName);
    tinyxml2::XMLElement *OSDXMLElement(char *osdName);
    Meter *LoadMeter(tinyxml2::XMLElement *meterXMLElement);

    std::list<Meter *> SliderMeters(char *osdName);   
    tinyxml2::XMLElement *SliderXMLElement(char *sliderName);

    Gdiplus::Font *Font(tinyxml2::XMLElement *meterXMLElement);
    Gdiplus::StringAlignment Alignment(tinyxml2::XMLElement *meterXMLElement);
    std::wstring ImageName(tinyxml2::XMLElement *meterXMLElement);

    int DefaultOSDUnits(char *osdName);
};
