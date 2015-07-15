#pragma once

class VolumeController {
public:
    struct DeviceInfo {
        std::wstring name;
        std::wstring id;
    };

    virtual float Volume() = 0;
    virtual void Volume(float vol) = 0;

    virtual bool Muted() = 0;
    virtual void Muted(bool mute) = 0;
    virtual void ToggleMute() {
        (Muted() == true) ? Muted(false) : Muted(true);
    }

public:
    static const int MSG_VOL_CHNG = WM_APP + 1080;
    static const int MSG_VOL_DEVCHNG = WM_APP + 1081;

};