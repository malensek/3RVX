#pragma once

#include <Windows.h>
#include <list>
#include "MeterWnd\Meter.h"
#include "StringUtils.h"
#include "TinyXml2\tinyxml2.h"

#define SKINS_DIR L"Skins"
#define SKIN_XML L"skin.xml"

class Skin {
public:
    Skin(std::wstring skinName) {
        _skinDir = SKINS_DIR L"\\" + skinName;
        _skinFile = _skinDir + L"\\" + SKIN_XML;

        CLOG(L"Loading skin [%s]: %s", skinName.c_str(), _skinFile.c_str());
        
        std::string u8FileName = StringUtils::Narrow(_skinFile);
        tinyxml2::XMLError result = _xml.LoadFile(u8FileName.c_str());
        if (result != tinyxml2::XMLError::XML_SUCCESS) {
            CLOG(L"Failed to read XML file!");
            //TODO throw exception?
        }
    }

    Gdiplus::Bitmap *OSDBgImg(char *osdName);
    std::list<Meter*> Meters(char *osdName);

private:
    std::wstring _skinFile;
    std::wstring _skinDir;
    tinyxml2::XMLDocument _xml;

    tinyxml2::XMLElement *OSDXMLElement(char *osdName);
    Meter *LoadMeter(tinyxml2::XMLElement *meterXMLElement);
    Gdiplus::Font *Font(tinyxml2::XMLElement *meterXMLElement);
    Gdiplus::StringAlignment Alignment(tinyxml2::XMLElement *meterXMLElement);
    std::wstring ImageName(tinyxml2::XMLElement *meterXMLElement);

    bool FileExists(std::wstring fileName);

};
