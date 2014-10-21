#include "Logger.h"
#include <windows.h>
#include <string>

void Logger::OpenConsole() {
    AllocConsole();
    FILE *in, *out, *err;
    freopen_s(&in, "CONIN$", "r", stdin);
    freopen_s(&out, "CONOUT$", "w", stdout);
    freopen_s(&err, "CONOUT$", "w", stderr);
}