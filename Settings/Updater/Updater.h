// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#pragma once

#include <string>

class StatusCallback;

struct Version;

/// <summary>
/// Handles update functionality: checking for updates online, and downloading
/// new versions of the software.
/// <p />
/// The latest version number of 3RVX can be retrieved from the URL specified
/// by the LATEST_URL constant string.
/// <p />
/// Versions can be downloaded from:
/// DOWNLOAD_URL + "3RVX-X.Y.{zip, msi}"
/// Where X is the major version, Y is the minor version, and the type of
/// installation is determined by the package extension:
/// <ul>
///   <li>.msi - Installer based version</li>
///   <li>.zip - Portable (non-installer) version</li>
/// </ul>
/// </summary>
class Updater {
public:
    /// <summary>
    /// Determines whether a new version of the program is available online
    /// or not.
    /// </summary>
    static bool NewerVersionAvailable();

    /// <summary>
    /// Retrieves the latest version of the program availabile online.
    /// </summary>
    static Version RemoteVersion();

    /// <summary>
    /// Retrieves the version number of the main application (3RVX.exe).
    /// </summary>
    static Version MainAppVersion();

    /// <summary>
    /// Downloads the specified version of the software. If a DownloadStatus
    /// is provided, the download will be performed asynchronously and updates
    /// will be sent to the DownloadStatus instance. Otherwise, this function is
    /// synchronous.
    /// <summary>
    /// <param name="version">Version to download</param>
    /// <param name="ds">
    /// The DownloadStatus instance to report progress to. If this parameter is
    /// specified, the method will run asynchronously.
    /// </param>
    static std::wstring DownloadVersion(
        Version version, StatusCallback *ds = nullptr);

private:
    /// <summary>
    /// Retrieves the file name of the specified version of the program stored
    /// on the remote server (example: 3RVX-3.4.msi).
    /// </summary>
    static std::wstring DownloadFileName(Version version);

private:
    /// <summary>
    /// Specifies the URL to check to determine the latest version online. The
    /// file at this address is a plain text file that contains a single line
    /// with the currently-available version number (example: 3.2).
    /// </summary>
    static const std::wstring LATEST_URL;

    /// <summary>
    /// Specifies the base URL to use when downloading new versions of the
    /// program.
    /// </summary>
    static const std::wstring DOWNLOAD_URL;
};