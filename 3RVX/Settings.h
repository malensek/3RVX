#pragma once

#include <unordered_map>
#include <string>

#include "TinyXml2\tinyxml2.h"

class Settings {
public:
    Settings(std::wstring file);

    static std::wstring AppDir();

    std::wstring SkinName();
    std::wstring GetText(std::string elementName);
    std::unordered_map<int, int> Hotkeys();
    bool IsEnabled(std::string elementName);

private:
    static std::wstring _appDir;

    std::wstring _file;
    tinyxml2::XMLDocument _xml;
    tinyxml2::XMLElement *_root;
};