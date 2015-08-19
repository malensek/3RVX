// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#include "SkinUtils.h"

#include "../CommCtl.h"
#include "../StringUtils.h"

std::vector<HICON> SkinUtils::ReadIconDirectory(std::wstring iconDir) {
    std::vector<HICON> iconset;
    CLOG(L"Reading icons from: %s", iconDir.c_str());

    HANDLE hFind;
    WIN32_FIND_DATA fd = {};
    hFind = FindFirstFile((iconDir + L"*").c_str(), &fd);
    if (hFind == INVALID_HANDLE_VALUE) {
        CLOG(L"Could not read icon directory");
        return iconset;
    }

    do {
        std::wstring iconName(fd.cFileName);
        if (iconName == L"." || iconName == L"..") {
            continue;
        }

        std::wstring iconPath = iconDir + iconName;
        std::wstring ext = StringUtils::FileExtension(iconName);
        if (ext != L"ico") {
            QCLOG(L"Ignoring non-ico file: %s", iconPath.c_str());
            continue;
        }

        HICON icon = NULL;
        HRESULT hr = LoadIconMetric(
            NULL,
            iconPath.c_str(),
            LIM_SMALL,
            &icon);

        if (SUCCEEDED(hr) && icon != NULL) {
            QCLOG(L"%s", iconPath.c_str());
            iconset.push_back(icon);
        }

    } while (FindNextFile(hFind, &fd));
    FindClose(hFind);
 
    return iconset;
}
