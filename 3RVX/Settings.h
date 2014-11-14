#pragma once

#include <unordered_map>

#include "TinyXml2\tinyxml2.h"

class Settings {
public:
    Settings(std::wstring file);


    std::wstring SkinName();
    std::unordered_map<int, int> Hotkeys();
    bool IsEnabled(std::string elementName);
    std::wstring GetText(std::string elementName);

private:
    std::wstring _file;
    tinyxml2::XMLDocument _xml;
    tinyxml2::XMLElement *_root;
};