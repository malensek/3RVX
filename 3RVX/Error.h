// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#pragma once

#include <string>
#include <unordered_map>

class Error {
public:
    static void ErrorMessage(unsigned int error, std::wstring detail = L"");
    static void ErrorMessageDie(unsigned int error, std::wstring detail = L"");

private:
    static std::unordered_map<int, std::wstring> errorMap;

    static wchar_t *ErrorType(unsigned int error);

public:
    static const int GENERR = 0x100;
    static const int SKINERR = 0x200;
    static const int SYSERR = 0x300;

    static const int GENERR_NOTFOUND = GENERR + 1;
    static const int GENERR_MISSING_XML = GENERR + 3;

    static const int SKINERR_INVALID_SKIN = SKINERR + 1;
    static const int SKINERR_INVALID_SLIDERTYPE = SKINERR + 7;
    static const int SKINERR_NOTFOUND = SKINERR + 8;
    static const int SKINERR_MISSING_XML = SKINERR + 9;
    static const int SKINERR_XMLPARSE = SKINERR + 10;

    static const int SYSERR_REGISTERCLASS = SYSERR + 1;
    static const int SYSERR_CREATEWINDOW = SYSERR + 2;
};