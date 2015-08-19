// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#pragma once

#include <Windows.h>
#include <string>

/// <summary>
/// Contains functionality for manipulating and converting strings. One
/// important function provided by this class is converting standard C++ strings
/// to and from Windows' wide character strings -- especially useful for reading
/// XML.
/// </summary>
class StringUtils {
public:
    /// <summary>
    /// Converts a standard character string to a wide string. This is required
    /// to properly convert strings from UTF-8 sources to Windows-compatible
    /// strings.
    /// </summary>
    static std::wstring Widen(const char *str);

    /// <summary>Converts a standard string to a wide string.</summary>
    static std::wstring Widen(const std::string &str);

    /// <summary>
    /// Converts a wide string to a standard string. Required for exporting
    /// Windows strings to UTF-8 sources.
    /// </summary>
    static std::string Narrow(const std::wstring &str);

    /// <summary>
    /// Retrieves the file extension of the given file name. This is done by
    /// locating the substring after the last dot '.' in the file name; the dot
    /// character is not included in the string returned by this function.
    /// </summary>
    static std::wstring FileExtension(const std::wstring &fileName);
};