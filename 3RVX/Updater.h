#pragma once

#include <utility>

class Updater {
public:
    static bool NewerVersionAvailable();

private:
    static std::pair<int, int> RemoteVersion();

};