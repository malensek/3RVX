#include "Updater.h"

#pragma comment(lib, "wininet.lib")
#include <Windows.h>
#include <WinInet.h>

#include "Logger.h"

bool Updater::NewerVersionAvailable() {
    HINTERNET internet = InternetOpen(
        L"3RVX-Updater",
        INTERNET_OPEN_TYPE_PRECONFIG,
        NULL,
        NULL,
        NULL);

    HINTERNET connection = InternetOpenUrl(
        internet,
        L"http://matthew.malensek.net",
        NULL,
        0,
        INTERNET_FLAG_NO_CACHE_WRITE | INTERNET_FLAG_PRAGMA_NOCACHE,
        0);

    if (connection == NULL) {
        CLOG(L"Could not connect to URL!");
        return false;
    }

    char buf[16];
    DWORD read;
    InternetReadFile(connection, buf, 16, &read);
}