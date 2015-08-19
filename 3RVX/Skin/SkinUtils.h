// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#pragma once

#include <Windows.h>
#include <vector>

#include "../Logger.h"

class SkinUtils {
public:
    static std::vector<HICON> ReadIconDirectory(std::wstring iconDir);
};