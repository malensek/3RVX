#pragma once
#include <Windows.h>
#include <Wincon.h>
#include <tchar.h>

#ifdef _DEBUG
#define PRINT_LOG 1
#else
#define PRINT_LOG 0
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
    if (PRINT_LOG) { \
        COLORIZE \
        wprintf(L"[%s:%d]\n", __FUNCTIONW__, __LINE__); \
        DECOLORIZE \
        wprintf(fmt L"\n", __VA_ARGS__); \
    } \
} while (0)

#define QCLOG(fmt, ...) \
if (PRINT_LOG) { wprintf(fmt L"\n", __VA_ARGS__); }

class Logger {
public:
    static void OpenConsole();
    static void CloseConsole();
};