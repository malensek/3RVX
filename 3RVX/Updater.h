#pragma once

#include <utility>

class Updater {
public:
    static bool NewerVersionAvailable();
    static std::pair<int, int> MainAppVersion();

private:
    static std::pair<int, int> RemoteVersion();

};