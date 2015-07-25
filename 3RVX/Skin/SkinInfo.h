#pragma once

#include <Windows.h>
#include <list>
#include "../TinyXml2/tinyxml2.h"

#define SKINS_DIR L"Skins"
#define SKIN_XML L"skin.xml"

class SkinInfo {
public:
    SkinInfo(std::wstring skinName);

    std::wstring Author();
    std::wstring URL();
    int FormatVersion();

protected:
    std::wstring _skinFile;
    std::wstring _skinDir;
    tinyxml2::XMLDocument _xml;
    tinyxml2::XMLElement *_root;
};