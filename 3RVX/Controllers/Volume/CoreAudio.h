#pragma once

class CoreAudioNotify;
class CoreAudioVolume;
#include "IVolume.h"

class CoreAudio : public IVolume {
public:
    CoreAudio() : m_notify(NULL), m_volume(NULL) {};
    ~CoreAudio();

    virtual bool Init();

    virtual float Volume();
    virtual void SetVolume(float vol);

    virtual bool Muted(); 
    virtual void SetMute(bool mute);
    virtual void ToggleMute();

    virtual void SetNotifyWnd(HWND hWnd);
    virtual void DeviceChanged();

protected:
    CoreAudioNotify *m_notify;
    CoreAudioVolume *m_volume;
};