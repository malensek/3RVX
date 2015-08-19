// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#pragma once

#include <string>
#include <unordered_map>

#define GENERR  0x100
#define SKINERR 0x200
#define SYSERR  0x400

#define GENERR_NOTFOUND            GENERR + 1
#define GENERR_SETTINGSFILE        GENERR + 2
#define GENERR_MISSING_XML         GENERR + 3

#define SKINERR_INVALID_SKIN       SKINERR + 1
#define SKINERR_INVALID_OSD        SKINERR + 2
#define SKINERR_INVALID_METER      SKINERR + 3
#define SKINERR_INVALID_SLIDER     SKINERR + 4
#define SKINERR_INVALID_BG         SKINERR + 5
#define SKINERR_INVALID_SLIDERTYPE SKINERR + 7
#define SKINERR_NOTFOUND           SKINERR + 8
#define SKINERR_MISSING_XML        SKINERR + 9
#define SKINERR_READERR            SKINERR + 10

#define SYSERR_REGISTERCLASS       SYSERR + 1
#define SYSERR_CREATEWINDOW        SYSERR + 2

class Error {
public:
    static void ErrorMessage(unsigned int error, std::wstring detail = L"");
    static void ErrorMessageDie(unsigned int error, std::wstring detail = L"");

private:
    static std::unordered_map<int, std::wstring> errorMap;

    static wchar_t *ErrorType(unsigned int error);
};