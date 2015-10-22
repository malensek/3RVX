// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#pragma once

class VolumeTransformation;

class VolumeController {
public:
    struct DeviceInfo {
        std::wstring name;
        std::wstring id;
    };

    /// <summary>
    /// Retrieves the current volume level as a float, ranging from 0.0 - 1.0
    /// </summary>
    virtual float Volume() = 0;

    /// <summary>Sets the volume level. Valid range: 0.0 - 1.0</summary>
    virtual void Volume(float vol) = 0;

    virtual bool Muted() = 0;
    virtual void Muted(bool mute) = 0;
    virtual void ToggleMute() {
        (Muted() == true) ? Muted(false) : Muted(true);
    }

    virtual void AddTransformation(VolumeTransformation *transform) = 0;
    virtual void RemoveTransformation(VolumeTransformation *transform) = 0;

public:
    static const int MSG_VOL_CHNG = WM_APP + 1080;
    static const int MSG_VOL_DEVCHNG = WM_APP + 1081;

};