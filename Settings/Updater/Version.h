#pragma once

#include <string>

struct Version {
public:
    Version(int major = 0, int minor = 0, int revision = 0) :
    _major(major),
    _minor(minor),
    _revision(revision) {

    }

    const int Major() { return _major; }
    const int Minor() { return _minor; }
    const int Revision() { return _revision; }

    unsigned int ToInt() {
        return (_major << 16) | (_minor << 8) | (_revision);
    }

    std::wstring ToString() {
        return std::to_wstring(_major)
            + L"."
            + std::to_wstring(_minor)
            + L"."
            + std::to_wstring(_revision);
    }

private:
    int _major;
    int _minor;
    int _revision;
};