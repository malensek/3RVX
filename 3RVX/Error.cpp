// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#include "Error.h"

#include <Windows.h>

#include "LanguageTranslator.h"
#include "Settings.h"

std::unordered_map<int, std::wstring> Error::errorMap = {
    { GENERR_NOTFOUND, L"File not found:\n{1}" },
    { GENERR_MISSING_XML, L"Could not locate XML tag: {1}" },
    { GENERR_UNKNOWN, L"An unknown error occurred: ID #{1}"},
    { SKINERR_INVALID_SKIN, L"Could not find specified skin:\n{1}" },
    { SKINERR_INVALID_SLIDERTYPE, L"Invalid slider type: {1}" },
    { SKINERR_MISSING_XML, L"Could not locate required skin XML tag: {1}" },
    { SKINERR_MISSING_XMLROOT, L"Could not locate root XML element" },
    { SKINERR_NO_RESOURCE, L"Skin resource '{1}' was not found in any of the "
        "available skins." },
    { SKINERR_SKINDIR, L"Could not locate skin directory:\n{1}" },
    { SKINERR_XMLPARSE, L"Failed to parse skin XML file:\n{1}" },
    { SYSERR_REGISTERCLASS, L"Could not register window class: {1}" },
    { SYSERR_CREATEWINDOW, L"Could not create window: {1}" },
};

void Error::ErrorMessage(unsigned int error, std::wstring detail) {
    std::wstring errType = ErrorType(error);
    std::wstring errMsg = errorMap[error];

    if (errMsg == L"") {
        errMsg = errorMap[GENERR_UNKNOWN];
        detail = std::to_wstring(error);
    }

    /* Check if a translator instance is available; if so, use it.
     * Otherwise, we keep the original english message. */
    Settings *settings = Settings::Instance();
    LanguageTranslator *translator = nullptr;
    if (settings) {
        translator = settings->Translator();
    }
    if (translator) {
        errMsg = translator->TranslateAndReplace(errMsg, detail);
        errType = translator->Translate(errType);
    }

    MessageBox(NULL, errMsg.c_str(), errType.c_str(), MB_ICONERROR);
}

void Error::ErrorMessageDie(unsigned int error, std::wstring detail) {
    ErrorMessage(error, detail);
    exit(EXIT_FAILURE);
}

std::wstring Error::LastErrorString() {
    DWORD error = GetLastError();
    if (error == 0) {
        return L"";
    }

    std::wstring str;
    LPTSTR msg;
    DWORD msgLen = FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER
        | FORMAT_MESSAGE_FROM_SYSTEM
        | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        error,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR) &msg,
        0,
        NULL);
    if (msgLen > 0) {
        str = std::wstring(msg);
        LocalFree(msg);
    }

    return str;
}

wchar_t *Error::ErrorType(unsigned int error) {
    if (error & SKINERR) {
        return L"3RVX Skin Error";
    } else if (error & SYSERR) {
        return L"3RVX System Error";
    }

    return L"3RVX Error";
}