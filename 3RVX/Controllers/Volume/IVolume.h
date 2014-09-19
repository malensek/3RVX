#ifndef IVOLUME_H
#define IVOLUME_H

#include "Windows.h"

class IVolume {
public:
    virtual bool Init() = 0;
    virtual float Volume() = 0;
    virtual void SetVolume(float vol) = 0;

    virtual bool Muted() = 0; 
    virtual void SetMute(bool mute) = 0;
    virtual void ToggleMute() = 0;

    virtual void SetNotifyWnd(HWND hWnd) = 0;
};

#endif /* IVOLUME_H */