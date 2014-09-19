#include "CoreAudio.h"
#include "CoreAudioNotify.h"
#include "CoreAudioVolume.h"

bool CoreAudio::Init()
{
    m_volume = new CoreAudioVolume;
    return m_volume->Init();
}

CoreAudio::~CoreAudio()
{
    if(m_notify != NULL)
        m_notify->Dispose();

    if(m_volume)
        delete m_volume;
}

float CoreAudio::Volume()
{
    return m_volume->Volume();
}

void CoreAudio::SetVolume(float vol)
{
    m_volume->SetVolume(vol);
}

bool CoreAudio::Muted()
{
    return m_volume->Muted();
}

void CoreAudio::SetMute(bool mute)
{
    m_volume->SetMute(mute);
}

void CoreAudio::ToggleMute()
{
    m_volume->SetMute(!m_volume->Muted());
}

void CoreAudio::SetNotifyWnd(HWND hWnd)
{
    m_notify = new CoreAudioNotify;
    m_notify->Init(hWnd);
}

void CoreAudio::DeviceChanged()
{
    m_notify->Update();
}