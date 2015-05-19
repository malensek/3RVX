#pragma once

#include <string>
#include <vector>

#include "TinyXml2\tinyxml2.h"

class LanguageTranslator {
public:
    LanguageTranslator();
    LanguageTranslator(std::wstring langFileName);

    std::wstring Translate(std::wstring str);

public:
    static std::vector<std::wstring> CurrentLocale();

private:
    tinyxml2::XMLDocument _xml;
    tinyxml2::XMLElement *_root;
};