#pragma once

#include <Windows.h>
#include <list>
#include "../TinyXml2/tinyxml2.h"

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
    int _version;

    tinyxml2::XMLElement *SubElement(char *parent, char *child);

protected:
    static const int DEFAULT_UNITS = 10;
};