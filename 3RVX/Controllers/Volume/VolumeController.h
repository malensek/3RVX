#pragma once

#define MSG_VOL_CHNG WM_APP + 1080
#define MSG_VOL_DEVCHNG WM_APP + 1081

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
};