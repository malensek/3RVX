// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#pragma once

#include <Windows.h>
#include <list>
#include <string>
#include "../TinyXml2/tinyxml2.h"

class SkinInfo {
public:
    SkinInfo(std::wstring skinName, bool reportErrors = true);

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