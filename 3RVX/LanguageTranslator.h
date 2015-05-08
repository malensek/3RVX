#pragma once

#include <string>

#include "TinyXml2\tinyxml2.h"

class LanguageTranslator {
public:
    LanguageTranslator(std::wstring langFileName);

    std::wstring Translate(std::wstring str);

private:
    tinyxml2::XMLDocument _xml;
    tinyxml2::XMLElement *_root;
};