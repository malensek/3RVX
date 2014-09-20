#pragma once
#include <EndpointVolume.h>

class CoreAudioVolume {
public:
    CoreAudioVolume();
    ~CoreAudioVolume();

    bool Init();

    float Volume();
    void SetVolume(float vol);
    bool Muted();
    void SetMute(bool mute);

private:
    IAudioEndpointVolume *m_epVol;
};