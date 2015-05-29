#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "TinyXml2\tinyxml2.h"

class LanguageTranslator {
public:
    LanguageTranslator();
    LanguageTranslator(std::wstring langFileName);

    void LoadTranslations();
    std::wstring &Translate(std::wstring &str);

public:
    static std::vector<std::wstring> CurrentLocale();

private:
    tinyxml2::XMLDocument _xml;
    tinyxml2::XMLElement *_root;
    std::unordered_map<std::wstring, std::wstring> _translations;

    std::wstring _author;
    std::wstring _name;
    std::wstring _id;
    std::vector<std::wstring> _regions;
};