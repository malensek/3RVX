#pragma once
#include <tchar.h>

#ifdef _DEBUG
#define PRINT_LOG 1
#else
#define PRINT_LOG 0
#endif

#define CLOG(fmt, ...) \
do { \
    if (PRINT_LOG) { \
        wprintf(L"[%s:%d]\n" fmt L"\n", \
                __FUNCTIONW__, __LINE__, __VA_ARGS__); \
    } \
} while (0)

#define QCLOG(fmt, ...) \
if (PRINT_LOG) { wprintf(fmt L"\n", __VA_ARGS__); }

class Logger {
public:
    static void OpenConsole();
};