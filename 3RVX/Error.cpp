// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#include "Error.h"

#include <Windows.h>

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
    { SYSERR_REGISTERCLASS, L"Could not register window class." },
    { SYSERR_CREATEWINDOW, L"Could not create window." },
};

void Error::ErrorMessage(unsigned int error, std::wstring detail) {
    std::wstring msg(L"");

    msg = msg + L"Unknown error occurred: " + std::to_wstring(error);
    if (detail != L"") {
        msg = msg + L"\n" + detail;
    }

    MessageBox(NULL, msg.c_str(), Error::ErrorType(error), MB_ICONERROR);
}

void Error::ErrorMessageDie(unsigned int error, std::wstring detail) {
    ErrorMessage(error, detail);
    exit(EXIT_FAILURE);
}

wchar_t *Error::ErrorType(unsigned int error) {
    if (error & GENERR) {
        return L"3RVX Error";
    } else if (error & SKINERR) {
        return L"Skin Error";
    } else if (error & SYSERR) {
        return L"System Error";
    }

    return L"Error";
}