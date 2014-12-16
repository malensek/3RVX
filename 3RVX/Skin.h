#pragma once

#include <Windows.h>
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")
#include <list>
#include <vector>

#include "SkinInfo.h"
#include "TinyXml2\tinyxml2.h"

class Meter;

#define SKINS_DIR L"Skins"
#define SKIN_XML L"skin.xml"

class Skin : SkinInfo {
public:
    Skin(std::wstring skinName) :
    SkinInfo(skinName) {
    }

    Gdiplus::Bitmap *BgImg(tinyxml2::XMLElement *element);

    bool HasOSD(char *osdName);
    Gdiplus::Bitmap *OSDBgImg(char *osdName);
    std::list<Meter *> Meters(char *osdName);
    std::vector<HICON> Iconset(char *osdName);

    Gdiplus::Bitmap *ControllerBgImg(char *controllerName);

private:
    tinyxml2::XMLElement *OSDXMLElement(char *osdName);
    Meter *LoadMeter(tinyxml2::XMLElement *meterXMLElement);
    Gdiplus::Font *Font(tinyxml2::XMLElement *meterXMLElement);
    Gdiplus::StringAlignment Alignment(tinyxml2::XMLElement *meterXMLElement);
    std::wstring ImageName(tinyxml2::XMLElement *meterXMLElement);
    bool FileExists(std::wstring fileName);
};
