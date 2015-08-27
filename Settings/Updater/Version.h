#pragma once

#include <string>

struct Version {
public:
    const int major;
    const int minor;
    const int revision;

    Version(int major, int minor = 0, int revision = 0) :
    major(major),
    minor(minor),
    revision(revision) {

    }

    std::wstring ToString() {
        return std::to_wstring(major)
            + L"."
            + std::to_wstring(minor)
            + L"."
            + std::to_wstring(revision);
    }
};