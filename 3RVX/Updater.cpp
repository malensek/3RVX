#include "Updater.h"

#pragma comment(lib, "Version.lib")
#pragma comment(lib, "wininet.lib")
#include <Windows.h>
#include <WinInet.h>
#include <sstream>

#include "Settings.h"
#include "StringUtils.h"
#include "Logger.h"

const std::wstring Updater::LATEST_URL
    = DOWNLOAD_URL + L"latest_version";

const std::wstring Updater::DOWNLOAD_URL
    = L"https://3rvx.com/releases/";

bool Updater::NewerVersionAvailable() {
    std::pair<int, int> remote = RemoteVersion();
    std::pair<int, int> local = MainAppVersion();
    CLOG(L"Remote version: %d.%d\n Local version: %d.%d",
        remote.first, remote.second,
        local.first, local.second);

    if (remote.first == 0 && remote.second == 0
        || local.first == 0 && local.second == 0) {
        /* One of the version checks failed, so say that there is no new
         * version. No need to bother the user with (hopefully) temporary
         * errors. */
        return false;
    }

    if (remote.first > local.first || remote.second > local.second) {
        return true;
    }

    return false;
}

std::pair<int, int> Updater::MainAppVersion() {
    std::wstring mainExe = Settings::Instance()->MainApp();
    BOOL result;
    std::pair<int, int> version = std::pair<int, int>(0, 0);

    DWORD size = GetFileVersionInfoSize(mainExe.c_str(), NULL);
    if (size == 0) {
        CLOG(L"Could not determine version info size");
        return version;
    }

    unsigned char *block = new unsigned char[size];
    result = GetFileVersionInfo(mainExe.c_str(), NULL, size, block);
    if (result == 0) {
        CLOG(L"Failed to retrieve file version info");
        delete[] block;
        return version;
    }

    unsigned int dataSz;
    VS_FIXEDFILEINFO *vers;
    result = VerQueryValue(block, L"\\", (void **) &vers, &dataSz);
    if (result == 0) {
        CLOG(L"Could not query root block for version info");
        delete[] block;
        return version;
    }

    if (vers->dwSignature != 0xFEEF04BD) {
        CLOG(L"Invalid version signature");
        delete[] block;
        return version;
    }

    unsigned long verl = vers->dwProductVersionMS;
    int hi = (verl >> 16) & 0xFF;
    int lo = verl & 0xFF;
    version = std::pair<int, int>(hi, lo);

    delete[] block;
    return version;
}

std::wstring Updater::MainAppVersionString() {
    std::pair<int, int> vers = MainAppVersion();
    return std::to_wstring(vers.first) + L"." + std::to_wstring(vers.second);
}

std::pair<int, int> Updater::RemoteVersion() {
    HINTERNET internet = InternetOpen(
        L"3RVX Updater",
        INTERNET_OPEN_TYPE_PRECONFIG,
        NULL,
        NULL,
        NULL);

    HINTERNET connection = InternetOpenUrl(
        internet,
        LATEST_URL.c_str(),
        NULL,
        0,
        INTERNET_FLAG_NO_CACHE_WRITE | INTERNET_FLAG_PRAGMA_NOCACHE,
        0);

    if (connection == NULL) {
        CLOG(L"Could not connect to URL!");
        return std::pair<int, int>(0, 0);
    }

    std::string str("");
    char buf[32];
    DWORD read;
    while (InternetReadFile(connection, buf, 16, &read) == TRUE && read != 0) {
        str.append(buf);
    }

    /* Only consider the first line */
    str.erase(str.find('\n'), str.size() - 1);

    int dot = str.find('.');
    std::string major = str.substr(0, dot);
    std::string minor = str.substr(dot + 1, str.size());

    std::pair<int, int> version;
    std::istringstream ss;

    ss = std::istringstream(major);
    ss >> version.first;
    ss = std::istringstream(minor);
    ss >> version.second;

    return version;
}