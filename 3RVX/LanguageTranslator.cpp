#include "LanguageTranslator.h"

#include "Logger.h"
#include "StringUtils.h"

LanguageTranslator::LanguageTranslator() {

}

LanguageTranslator::LanguageTranslator(std::wstring langFileName) {
    CLOG(L"Loading language XML: %s", langFileName.c_str());
    /*
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
    */

    wchar_t buf[LOCALE_NAME_MAX_LENGTH];
    GetUserDefaultLocaleName(buf, LOCALE_NAME_MAX_LENGTH);
    CLOG(L"Locale: %s", buf);
}

std::wstring LanguageTranslator::Translate(std::wstring str) {
    return str;
}