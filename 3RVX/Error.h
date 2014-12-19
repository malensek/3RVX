#pragma once

#include <string>

#define GENERR  (0x1 << 7)
#define SKINERR (GENERR << 8)

#define SKINERR_INVALID_SKIN       SKINERR + 1
#define SKINERR_INVALID_OSD        SKINERR + 2
#define SKINERR_INVALID_METER      SKINERR + 3
#define SKINERR_INVALID_CONT       SKINERR + 4
#define SKINERR_INVALID_OSD_BG     SKINERR + 5
#define SKINERR_INVALID_CONT_BG    SKINERR + 6
#define SKINERR_INVALID_SLIDERTYPE SKINERR + 7
#define SKINERR_NOTFOUND           SKINERR + 8

class Error {
public:
    static void ErrorMessage(unsigned int error, std::wstring detail = L"") {
        std::wstring msg(L"");

        msg = msg + L"Unknown error occurred: " + std::to_wstring(error);
        if (detail != L"") {
            msg = msg + L"\n" + detail;
        }

        MessageBox(NULL, msg.c_str(), Error::ErrorType(error), MB_ICONERROR);
    }

    static void ErrorMessageDie(unsigned int error, std::wstring detail = L"") {
        ErrorMessage(error);
        exit(EXIT_FAILURE);
    }

private:
    static wchar_t *ErrorType(unsigned int error) {
        if ((error & GENERR) == GENERR) {
            return L"3RVX Error";
        } else if ((error & SKINERR) == SKINERR) {
            return L"Skin Error";
        }

        return L"Error";
    }
};