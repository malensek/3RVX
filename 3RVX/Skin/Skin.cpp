#include "Skin.h"

#include <Shlwapi.h>

Gdiplus::Bitmap * Skin::LoadImg(std::wstring fileName) {
    if (PathFileExists(fileName.c_str()) == FALSE) {
        return nullptr;
    }

    Gdiplus::Bitmap *bg = Gdiplus::Bitmap::FromFile(fileName.c_str());
    return bg;
}
