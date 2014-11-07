#pragma once

#include <unordered_map>

#include "Logger.h"
#include "StringUtils.h"
#include "TinyXml2\tinyxml2.h"

class Settings {
public:
    Settings(std::wstring file) :
    _file(file) {
        CLOG(L"Loading settings file [%s]", file.c_str());
        
        std::string u8FileName = StringUtils::Narrow(file);
        tinyxml2::XMLError result = _xml.LoadFile(u8FileName.c_str());
        if (result != tinyxml2::XMLError::XML_SUCCESS) {
            throw std::runtime_error("Failed to parse XML file");
        }

        _root = _xml.GetDocument()->FirstChildElement("settings");
        if (_root == NULL) {
            throw std::runtime_error("Could not find root XML element");
        }
    }

    std::wstring SkinName();
    std::unordered_map<int, int> Hotkeys();
    bool IsEnabled(std::string elementName);
    std::wstring GetText(std::string elementName);

private:
    std::wstring _file;
    tinyxml2::XMLDocument _xml;
    tinyxml2::XMLElement *_root;
};