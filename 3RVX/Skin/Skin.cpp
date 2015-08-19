// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#include "Skin.h"

#include <Shlwapi.h>

Gdiplus::Bitmap * Skin::LoadImg(std::wstring fileName) {
    if (PathFileExists(fileName.c_str()) == FALSE) {
        return nullptr;
    }

    Gdiplus::Bitmap *bg = Gdiplus::Bitmap::FromFile(fileName.c_str());
    return bg;
}
