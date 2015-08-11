#pragma once

#include <Windows.h>
#include <vector>

#include "../Logger.h"

class SkinUtils {
public:
    static std::vector<HICON> ReadIconDirectory(std::wstring iconDir);
};