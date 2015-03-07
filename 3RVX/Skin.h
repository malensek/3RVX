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

    void Load();
    ~Skin();

    bool HasOSD(char *osdName);

    SliderKnob *Knob(char *sliderName);
    std::vector<HICON> Iconset(char *osdName);

    int DefaultVolumeUnits();

public:
    Gdiplus::Bitmap *volumeBackground;
    Gdiplus::Bitmap *volumeMask;
    std::list<Meter *> volumeMeters;

    Gdiplus::Bitmap *muteBackground;
    Gdiplus::Bitmap *muteMask;

    Gdiplus::Bitmap *ejectBackground;
    Gdiplus::Bitmap *ejectMask;

    Gdiplus::Bitmap *volumeSliderBackground;
    Gdiplus::Bitmap *volumeSliderMask;
    std::list<Meter *> volumeSliderMeters;
    SliderKnob *volumeSliderKnob;

private:

    Gdiplus::Bitmap *OSDBgImg(char *osdName);
    Gdiplus::Bitmap *OSDMask(char *osdName);
    std::list<Meter *> OSDMeters(char *osdName);
    tinyxml2::XMLElement *OSDXMLElement(char *osdName);

    Gdiplus::Bitmap *SliderBgImg(char *sliderName);
    Gdiplus::Bitmap *SliderMask(char *sliderName);
    std::list<Meter *> SliderMeters(char *osdName);   
    tinyxml2::XMLElement *SliderXMLElement(char *sliderName);

    Meter *LoadMeter(tinyxml2::XMLElement *meterXMLElement);

    Gdiplus::Bitmap *Image(tinyxml2::XMLElement *element, char *attrName);
    std::wstring ImageName(tinyxml2::XMLElement *meterXMLElement);
    Gdiplus::Font *Font(tinyxml2::XMLElement *meterXMLElement);
    Gdiplus::StringAlignment Alignment(tinyxml2::XMLElement *meterXMLElement);

    int DefaultOSDUnits(char *osdName);
};
