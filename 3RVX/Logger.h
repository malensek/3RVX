/*
Controlling The Logger
----------------------

- Logging is enabled whenever _DEBUG is defined
- By default, log entries are printed to the console window
- To force logging (even when _DEBUG is not defined): 
    define (/D) FORCE_3RVX_LOG
- To log to a file instead of the console:
    define (/D) ENABLE_3RVX_LOGTOFILE

*/

#pragma once

#include <Windows.h>
#include <Wincon.h>
#include <tchar.h>

#ifdef _DEBUG
    #define ENABLE_3RVX_LOG 1
#else
    #ifdef FORCE_3RVX_LOG
        #define ENABLE_3RVX_LOG 1
    #else
        #define ENABLE_3RVX_LOG 0
    #endif
#endif

#define FOREGROUND_WHITE (FOREGROUND_RED \
    | FOREGROUND_GREEN \
    | FOREGROUND_BLUE)

#define COLORIZE \
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), \
        FOREGROUND_RED | FOREGROUND_INTENSITY);

#define DECOLORIZE \
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), \
        FOREGROUND_WHITE);

#define CLOG(fmt, ...) \
do { \
    if (ENABLE_3RVX_LOG) { \
        COLORIZE \
        wprintf(L"[%s:%d]\n", __FUNCTIONW__, __LINE__); \
        DECOLORIZE \
        wprintf(fmt L"\n", __VA_ARGS__); \
    } \
} while (0)

#define QCLOG(fmt, ...) \
if (ENABLE_3RVX_LOG) { wprintf(fmt L"\n", __VA_ARGS__); }

class Logger {
public:
    static void Start();
    static void Stop();

private:
    static FILE *in, *out, *err;
};