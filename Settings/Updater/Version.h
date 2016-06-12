// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#pragma once

#include <string>

/// <summary>
/// Encapsulates version information for 3RVX releases. A release version is
/// composed of a major, minor, and revision number -- we don't use the fourth
/// version number supported by Windows.
/// </summary>
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

    /// <summary>
    /// Determines whether this Version is newer than another Version (the
    /// version number is higher than the other).
    /// <returns>true if this version is newer than the other.</returns>
    /// </summary>
    bool NewerThan(Version &otherVersion) {
        return ToInt() > otherVersion.ToInt();
    }

    /// <summary>
    /// Converts this version to a single integer that can be used for comparing
    /// versions. NOTE: This is 3RVX-specific because we assume version numbers
    /// (major, minor, revision) are 8 bits each. This is not necessarily true
    /// for all Windows software.
    /// </summary>
    unsigned int ToInt() {
        return (_major << 16) | (_minor << 8) | (_revision);
    }

    /// <summary>
    /// Creates a string-based representation of this version identifier; for
    /// example "3.6.2"
    /// </summary>
    std::wstring ToString() {
        return std::to_wstring(_major)
            + L"."
            + std::to_wstring(_minor)
            + L"."
            + std::to_wstring(_revision);
    }

private:
    /// <summary>Major version: large feature changes</summary>
    int _major;

    /// <summary>Minor version: small features/improvements</summary>
    int _minor;

    /// <summary>Revision number: bugfixes to a previous release</summary>
    int _revision;
};