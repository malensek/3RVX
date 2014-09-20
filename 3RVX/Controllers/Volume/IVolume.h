#pragma once

#define MSG_VOLCHNG WM_APP + 1080
#define MSG_DEVCHNG WM_APP + 1081

class IVolume {
public:
    virtual float Volume() = 0;
    virtual void Volume(float vol) = 0;



    virtual bool Muted() = 0; 
    virtual void Muted(bool mute) = 0;
	virtual void ToggleMute() {
		(Muted() == true) ? Muted(false) : Muted(true);
	}
};