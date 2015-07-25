#pragma once

#pragma comment(lib, "gdiplus.lib")

#include <Windows.h>
#include <gdiplus.h>

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

class Skin : SkinInfo {
public:
    Skin(std::wstring skinXML);
    ~Skin();

public:
    virtual OSDComponent *VolumeOSD();
    virtual OSDComponent *MuteOSD();
    virtual OSDComponent *EjectOSD();

    virtual std::vector<HICON> VolumeIconset();

    virtual SliderComponent *VolumeSlider();

private:
    OSDComponent *_volumeOSD;
    OSDComponent *_muteOSD;
    OSDComponent *_ejectOSD;

    std::vector<HICON> _volumeIcons;

    SliderComponent *_volumeSlider;

    void PopulateComponent(
        SkinComponent *component, char *componentClass, char *componentName);
    void DestroyComponent(SkinComponent *component);

    Gdiplus::Bitmap *BackgroundImage(char *parent, char *child);
    Gdiplus::Bitmap *MaskImage(char *parent, char *child);
    Gdiplus::Bitmap *Image(tinyxml2::XMLElement *element, char *attrName);
    std::wstring ImageName(tinyxml2::XMLElement *meterXMLElement);

    int DefaultUnits(char *parent, char *child);
    std::vector<HICON> Iconset(char *osdName);
    SliderKnob *Knob(char *sliderName);
    SoundPlayer *Sound(char *parent, char *child);

    std::vector<Meter *> Meters(tinyxml2::XMLElement *parentElement);
    Meter *LoadMeter(tinyxml2::XMLElement *meterXMLElement);
    Gdiplus::Font *Font(tinyxml2::XMLElement *meterXMLElement);
    Gdiplus::StringAlignment Alignment(tinyxml2::XMLElement *meterXMLElement);

    tinyxml2::XMLElement *SubElement(char *parent, char *child);

protected:
    static const int DEFAULT_UNITS = 10;
};
