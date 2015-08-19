// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

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

    /// <summary>
    /// Translates the provided string to the current locale. If no translation
    /// is available, the original string is returned.
    /// </summary>
    const std::wstring &Translate(const std::wstring &str);

    const std::wstring TranslateAndReplace(
        const std::wstring &str, const std::wstring &arg);

    /// <summary>
    /// Replaces the placeholder(s) in str with args, but does not perform a
    /// translation lookup.
    /// </summary>
    const std::wstring Replace(
        const std::wstring &str, const std::wstring &arg);

    const std::wstring &LanguageName();
    const std::wstring &Author();

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