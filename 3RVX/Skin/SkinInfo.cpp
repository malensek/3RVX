// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#include "SkinInfo.h"

#include "../Error.h"
#include "../Logger.h"
#include "../Settings.h"
#include "../StringUtils.h"

SkinInfo::SkinInfo(std::wstring skinFile) :
_skinFile(skinFile) {
    CLOG(L"Loading skin XML: %s", _skinFile.c_str());

    /* Remove the '/skin.xml' portion from the file name to get the dir name. */
    std::wstring xmlName = std::wstring(Settings::SKIN_XML);
    _skinDir = _skinFile.substr(0, _skinFile.length() - (xmlName.length() + 1));

    FILE *fp;
    _wfopen_s(&fp, _skinFile.c_str(), L"rb");
    if (fp == NULL) {
        QCLOG(L"Failed to open file!");
        return;
    }

    tinyxml2::XMLError result = _xml.LoadFile(fp);
    fclose(fp);
    if (result != tinyxml2::XMLError::XML_SUCCESS) {
        if (result == tinyxml2::XMLError::XML_ERROR_FILE_NOT_FOUND) {
            Error::ErrorMessageDie(SKINERR_INVALID_SKIN);
        }
        throw std::logic_error("Failed to read XML file!");
    }

    _root = _xml.GetDocument()->FirstChildElement("skin");
    if (_root != NULL) {
        _version = 3;
    } else {
        _root = _xml.GetDocument()->FirstChildElement("XMLskinSettings");

        if (_root != NULL) {
            _version = 2;
        } else {
            throw std::runtime_error("Could not find root XML element");
        }
    }
}

std::wstring SkinInfo::Author() {
    tinyxml2::XMLHandle xmlHandle(_root);
    tinyxml2::XMLElement *author = xmlHandle
        .FirstChildElement("info")
        .FirstChildElement("author")
        .ToElement();

    const char *authorText;
    if (author == NULL || (authorText = author->GetText()) == NULL) {
        return L"";
    }

    return std::wstring(StringUtils::Widen(authorText));
}

std::wstring SkinInfo::URL() {
    tinyxml2::XMLHandle xmlHandle(_root);
    tinyxml2::XMLElement *site = xmlHandle
        .FirstChildElement("info")
        .FirstChildElement("website")
        .ToElement();

    const char *text;
    if (site == NULL || (text = site->GetText()) == NULL) {
        return L"";
    }
    return std::wstring(StringUtils::Widen(text));
}

int SkinInfo::FormatVersion() {
    return _version;
}

tinyxml2::XMLElement *SkinInfo::SubElement(char *parent, char *child) {
    tinyxml2::XMLHandle xmlHandle(_root);
    tinyxml2::XMLElement *elem = xmlHandle
        .FirstChildElement(parent)
        .FirstChildElement(child)
        .ToElement();
    return elem;
}