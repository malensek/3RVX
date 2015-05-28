#include "LanguageTranslator.h"

#include <sstream>

#include "Logger.h"
#include "StringUtils.h"

std::vector<std::wstring> LanguageTranslator::CurrentLocale() {
    wchar_t buf[LOCALE_NAME_MAX_LENGTH];
    GetUserDefaultLocaleName(buf, LOCALE_NAME_MAX_LENGTH);
    CLOG(L"Locale: %s", buf);

    std::wstring locale(buf);

    /* Remove custom supplemental locales */
    int custom = locale.find(L"-x-");
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
        int sort = sub.find(L"_");
        if (sort > 0) {
            sub.erase(sort, sub.size() - 1);
        }

        finalLocale.push_back(sub);
    }

    return finalLocale;
}

LanguageTranslator::LanguageTranslator() {

}

LanguageTranslator::LanguageTranslator(std::wstring langFileName) {
    CLOG(L"Loading language XML: %s", langFileName.c_str());
    std::string u8FileName = StringUtils::Narrow(langFileName);
    tinyxml2::XMLError result = _xml.LoadFile(u8FileName.c_str());
    if (result != tinyxml2::XMLError::XML_SUCCESS) {
        if (result == tinyxml2::XMLError::XML_ERROR_FILE_NOT_FOUND) {
            //Error::ErrorMessageDie(SKINERR_INVALID_SKIN);
        }
        throw std::logic_error("Failed to read XML file!");
    }

    _root = _xml.GetDocument()->FirstChildElement("translation");
    if (_root == NULL) {
        throw std::runtime_error("Could not find root XML element");
    }

    _author = StringUtils::Widen(_root->Attribute("author", ""));
    tinyxml2::XMLElement *trans = _root->FirstChildElement("language");
    if (trans == NULL) {
        CLOG(L"No <language> tag.");
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

    tinyxml2::XMLElement *string = _root->FirstChildElement("string");
    for (; string != NULL; string = string->NextSiblingElement()) {
        string->FirstChildElement("resource");
        /* parse translation */
    }


}

std::wstring LanguageTranslator::Translate(std::wstring str) {
    return str;
}