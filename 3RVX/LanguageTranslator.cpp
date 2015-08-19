// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#include "LanguageTranslator.h"

#include <sstream>

#include "Logger.h"
#include "StringUtils.h"

LanguageTranslator::LanguageTranslator() {

}

LanguageTranslator::LanguageTranslator(std::wstring langFileName) {
    CLOG(L"Loading language XML: %s", langFileName.c_str());

    FILE *fp;
    _wfopen_s(&fp, langFileName.c_str(), L"rb");
    if (fp == NULL) {
        CLOG(L"Failed to open file!");
        return;
    }

    tinyxml2::XMLError result = _xml.LoadFile(fp);
    fclose(fp);
    if (result != tinyxml2::XMLError::XML_SUCCESS) {
        CLOG(L"Failed to read language file!");
        return;
    }

    _root = _xml.GetDocument()->FirstChildElement("translation");
    if (_root == NULL) {
        return;
    }

    /*
    _author = StringUtils::Widen(_root->Attribute("author", ""));
    */

    tinyxml2::XMLElement *trans = _root->FirstChildElement("language");
    if (trans == NULL) {
        CLOG(L"No <language> tag.");
        return;
    }

    CLOG(L"Loading translation header");
    _name = StringUtils::Widen(trans->Attribute("name"));
    _id = StringUtils::Widen(trans->Attribute("id"));

    if (_name == L"" || _id == L"") {
        CLOG(L"whoops");
    }

    QCLOG(L"Language name: %s", _name.c_str());
    QCLOG(L"Locale identifier: %s", _id.c_str());
    const char *regions = trans->Attribute("regions");
    if (regions != NULL) {
        std::string region;
        std::istringstream ss(regions);
        while (std::getline(ss, region, ',')) {
            std::wstring regionStr = StringUtils::Widen(region);
            _regions.push_back(regionStr);
            QCLOG(L"Region: %s", regionStr.c_str());
        }
    }
}

void LanguageTranslator::LoadTranslations() {
    if (_root == NULL) {
        return;
    }

    tinyxml2::XMLElement *string = _root->FirstChildElement("string");
    for (; string != NULL; string = string->NextSiblingElement()) {
        const char *originalText = NULL;
        const char *translatedText = NULL;

        tinyxml2::XMLElement *orig = string->FirstChildElement("original");
        if (orig) {
            originalText = orig->GetText();
        }
        tinyxml2::XMLElement *trans = string->FirstChildElement("translation");
        if (trans) {
            translatedText = trans->GetText();
        }

        if (originalText && translatedText) {
            std::wstring origStr = StringUtils::Widen(originalText);
            std::wstring transStr = StringUtils::Widen(translatedText);

            if (_translations.find(origStr) != _translations.end()) {
                CLOG(L"WARNING: Duplicate translation found!");
                QCLOG(L"[%s] -> [%s]", origStr.c_str(), transStr.c_str());
                return;
            }

            _translations[origStr] = transStr;
        }
    }
}

const std::wstring &LanguageTranslator::Translate(const std::wstring &str) {
    if (str == L"") {
        return str;
    }

    if (_translations.find(str) == _translations.end()) {
        /* If the translation isn't found, return the original string. */
        CLOG(L"No translation found: [%s]", str.c_str());
        return str;
    }
    return _translations[str];
}

const std::wstring LanguageTranslator::TranslateAndReplace(
        const std::wstring &str, const std::wstring &arg) {

    std::wstring trans(Translate(str));
    return Replace(trans, arg);
}

const std::wstring LanguageTranslator::Replace(
        const std::wstring &str, const std::wstring &arg) {

    std::wstring repStr(str);
    size_t strloc = repStr.find(L"{1}");
    if (strloc == std::wstring::npos) {
        return str;
    }
    repStr.replace(strloc, 3, arg);
    return repStr;
}

const std::wstring &LanguageTranslator::LanguageName() {
    return _name;
}

const std::wstring &LanguageTranslator::Author() {
    return _author;
}

std::vector<std::wstring> LanguageTranslator::CurrentLocale() {
    wchar_t buf[LOCALE_NAME_MAX_LENGTH];
    GetUserDefaultLocaleName(buf, LOCALE_NAME_MAX_LENGTH);
    CLOG(L"Locale: %s", buf);

    std::wstring locale(buf);

    /* Remove custom supplemental locales */
    size_t custom = locale.find(L"-x-");
    if (custom > 0) {
        locale.erase(custom, locale.size() - 1);
    }

    /* Split up parts of the locale string */
    std::vector<std::wstring> parts;
    std::wstring sub;
    std::wistringstream ss(locale);
    while (std::getline(ss, sub, L'-')) {
        parts.push_back(sub);
    }

    std::vector<std::wstring> finalLocale;
    for (std::wstring sub : parts) {
        /* Remove sort orders */
        size_t sort = sub.find(L"_");
        if (sort > 0) {
            sub.erase(sort, sub.size() - 1);
        }

        finalLocale.push_back(sub);
    }

    return finalLocale;
}

