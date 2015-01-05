#include "MiscUtils.h"

#include <Windows.h>

bool MiscUtils::FileExists(std::wstring fileName) {
    DWORD attr = GetFileAttributes(fileName.c_str());
    return (attr != INVALID_FILE_ATTRIBUTES
        && !(attr & FILE_ATTRIBUTE_DIRECTORY)
        && GetLastError() != ERROR_FILE_NOT_FOUND);
}