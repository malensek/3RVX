#include "Updater.h"

#pragma comment(lib, "wininet.lib")
#include <Windows.h>
#include <WinInet.h>
#include <sstream>

#include "StringUtils.h"
#include "Logger.h"

bool Updater::NewerVersionAvailable() {
    std::pair<int, int> version = RemoteVersion();
    CLOG(L"Remote version: %d.%d", version.first, version.second);
    return false;
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
        L"http://matthew.malensek.net/projects/3RVX/latest_version",
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