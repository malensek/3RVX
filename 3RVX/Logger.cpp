// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#include "Logger.h"
#include <string>

void Logger::Start() {
#ifdef ENABLE_3RVX_LOGTOFILE
    /* Disable buffering */
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);

    FILE *out, *err;
    _wfreopen_s(&out, L"3RVX_Log.txt", L"w", stdout);
    _wfreopen_s(&err, L"3RVX_Log.txt", L"w", stderr);
#else
#if ENABLE_3RVX_LOG != 0
    AllocConsole();
    FILE *in, *out, *err;
    freopen_s(&in, "CONIN$", "r", stdin);
    freopen_s(&out, "CONOUT$", "w", stdout);
    freopen_s(&err, "CONOUT$", "w", stderr);
#endif
#endif
}

void Logger::Stop() {
#ifdef ENABLE_3RVX_LOGTOFILE
    CLOG(L"Logger stopped.");
    fclose(stdout);
    fclose(stderr);
#else
#if ENABLE_3RVX_LOG != 0
    FreeConsole();
#endif
#endif
}