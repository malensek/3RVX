#include "CoreAudioVolume.h"
#include <MMDeviceApi.h>

CoreAudioVolume::CoreAudioVolume()
{
    CoInitialize(NULL);
    m_epVol = NULL;
}

CoreAudioVolume::~CoreAudioVolume()
{
    m_epVol->Release();
    CoUninitialize();
}

bool CoreAudioVolume::Init()
{
    if(m_epVol != NULL)
        m_epVol->Release();

    IMMDeviceEnumerator *devEnum = NULL;
    HRESULT hRes;

    hRes = CoCreateInstance(
        __uuidof(MMDeviceEnumerator), NULL, CLSCTX_INPROC_SERVER, 
        __uuidof(IMMDeviceEnumerator), (LPVOID *)&devEnum);

    if(FAILED(hRes))
        return false;

    IMMDevice *defaultDev = NULL; // 0 = default device
    hRes = devEnum->GetDefaultAudioEndpoint(
        eRender, eConsole, &defaultDev);

    if(FAILED(hRes))
        return false;

    devEnum->Release();
    devEnum = NULL;

    hRes = defaultDev->Activate(__uuidof(IAudioEndpointVolume), 
        CLSCTX_INPROC_SERVER, NULL, (LPVOID *)&m_epVol);

    if(FAILED(hRes))
        return false;

    defaultDev->Release();
    defaultDev = NULL;

    return true;
}

float CoreAudioVolume::Volume()
{
    float vol = 0;
    m_epVol->GetMasterVolumeLevelScalar(&vol);
    return vol;
}

void CoreAudioVolume::SetVolume(float vol)
{
    if(vol > 1.0f)
        vol = 1.0f;

    if(vol < 0.0f)
        vol = 0.0f;

    m_epVol->SetMasterVolumeLevelScalar(vol, NULL);
}

bool CoreAudioVolume::Muted()
{
    BOOL mute;
    m_epVol->GetMute(&mute);
    return (mute == TRUE) ? true : false;
}

void CoreAudioVolume::SetMute(bool mute)
{
    m_epVol->SetMute(mute, NULL);
}