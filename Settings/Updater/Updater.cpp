// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#include "Updater.h"

#pragma comment(lib, "Urlmon.lib")
#pragma comment(lib, "Version.lib")
#pragma comment(lib, "wininet.lib")

#include <Windows.h>
#include <Shlobj.h> // Has to be included before WinInet to avoid warning
#include <WinInet.h>
#include <sstream>

#include "../../3RVX/Error.h"
#include "../../3RVX/Settings.h"
#include "../../3RVX/StringUtils.h"
#include "../../3RVX/Logger.h"
#include "../Controls/StatusCallback.h"
#include "Version.h"

const std::wstring Updater::DOWNLOAD_URL
    = L"https://3rvx.com/releases/";

/* Since this const depends on DOWNLOAD_URL, it needs to be defined after it. */
const std::wstring Updater::LATEST_URL
    = Updater::DOWNLOAD_URL + L"latest_version";

const std::wstring Updater::RELEASE_NOTES_URL
    = L"https://3rvx.com/release-notes/";

bool Updater::NewerVersionAvailable() {
    Version remote = RemoteVersion();
    Version local = MainAppVersion();
    CLOG(L"Remote version: %s\n Local version: %s",
        remote.ToString().c_str(),
        local.ToString().c_str());

    if (remote.ToInt() == 0 || local.ToInt() == 0) {
        /* One of the version checks failed, so say that there is no new
         * version. No need to bother the user with (hopefully) temporary
         * errors. */
        return false;
    }

    return remote.NewerThan(local);
}

Version Updater::MainAppVersion() {
    std::wstring mainExe = Settings::Instance()->MainApp();
    BOOL result;

    DWORD size = GetFileVersionInfoSize(mainExe.c_str(), NULL);
    if (size == 0) {
        CLOG(L"Could not determine version info size");
        return { 0, 0, 0 };
    }

    unsigned char *block = new unsigned char[size];
    result = GetFileVersionInfo(mainExe.c_str(), NULL, size, block);
    if (result == 0) {
        CLOG(L"Failed to retrieve file version info");
        delete[] block;
        return { 0, 0, 0 };
    }

    unsigned int dataSz;
    VS_FIXEDFILEINFO *vers;
    result = VerQueryValue(block, L"\\", (void **) &vers, &dataSz);
    if (result == 0) {
        CLOG(L"Could not query root block for version info");
        delete[] block;
        return { 0, 0, 0 };
    }

    if (vers->dwSignature != 0xFEEF04BD) {
        CLOG(L"Invalid version signature");
        delete[] block;
        return { 0, 0, 0 };
    }

    unsigned long verms = vers->dwProductVersionMS;
    int hi = (verms >> 16) & 0xFF;
    int lo = verms & 0xFF;

    unsigned long verls = vers->dwProductVersionLS;
    int rev = (verls >> 16) & 0xFF;

    delete[] block;
    return Version(hi, lo, rev);
}

std::wstring Updater::DownloadVersion(Version version, StatusCallback *cb) {
    std::wstring localDir = L"";

    if (Settings::Portable()) {
        PWSTR dlPath = nullptr;
        SHGetKnownFolderPath(FOLDERID_Downloads, 0, NULL, &dlPath);
        localDir = std::wstring(dlPath);
        CoTaskMemFree(dlPath);
    } else {
        wchar_t tmpPath[MAX_PATH];
        DWORD result = GetTempPath(MAX_PATH, tmpPath);
        if (result == 0) {
            CLOG(L"Could not get temp download path");
            return L"";
        }
        localDir = std::wstring(tmpPath);
    }

    if (localDir == L"") {
        Error::ErrorMessage(Error::GENERR_UPDATEDL,
            L"Local download directory could not be found");
        return L"";
    }

    std::wstring fname = DownloadFileName(version);
    std::wstring url = DOWNLOAD_URL + fname;
    std::wstring localFile = localDir + L"\\" + fname;

    /* Make sure we can actually write to the file */
    HANDLE fileHandle = CreateFile(
        localFile.c_str(),
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL);
    if (fileHandle == INVALID_HANDLE_VALUE) {
        Error::ErrorMessage(Error::GENERR_UPDATEDL,
            Error::LastErrorString());
    }
    CloseHandle(fileHandle);

    CLOG(L"Downloading %s to %s...", url.c_str(), localFile.c_str());
    DeleteUrlCacheEntry(url.c_str());
    HRESULT hr = URLDownloadToFile(
        NULL,
        url.c_str(),
        localFile.c_str(),
        0,
        cb);

    if (hr == S_OK) {
        return localFile;
    } else {
        return L"";
    }
}

std::wstring Updater::DownloadFileName(Version version) {
    std::wstring ext;
    if (Settings::Portable()) {
        ext = L".zip";
    } else {
        ext = L".msi";
    }
    return std::wstring(L"3RVX-" + version.ToString() + ext);
}

Version Updater::RemoteVersion() {
    HINTERNET internet = InternetOpen(
        L"3RVX Updater",
        INTERNET_OPEN_TYPE_PRECONFIG,
        NULL,
        NULL,
        NULL);

    CLOG(L"Opening URL: %s", LATEST_URL.c_str());
    HINTERNET connection = InternetOpenUrl(
        internet,
        LATEST_URL.c_str(),
        NULL,
        0,
        INTERNET_FLAG_NO_CACHE_WRITE | INTERNET_FLAG_PRAGMA_NOCACHE,
        0);

    if (connection == NULL) {
        CLOG(L"Could not connect to URL!");
        return { 0, 0, 0 };
    }

    std::string str("");
    char buf[32];
    DWORD read;
    while (InternetReadFile(connection, buf, 16, &read) == TRUE && read != 0) {
        str.append(buf);
    }

    /* Only consider the first line */
    str.erase(str.find('\n'), str.size() - 1);

    size_t dot = str.find('.');
    size_t dot2 = str.find('.', dot + 1);
    std::string major = str.substr(0, dot);
    std::string minor = str.substr(dot + 1, dot2);
    std::string rev = str.substr(dot2 + 1, str.size());
    return Version(std::stoi(major), std::stoi(minor), std::stoi(rev));
}