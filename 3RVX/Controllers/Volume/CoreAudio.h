#pragma once

#include <atlbase.h>
#include <atlsync.h>
#include <Endpointvolume.h>
#include <Mmdeviceapi.h>
#include <string>

#include "VolumeController.h"

class CoreAudio : IAudioEndpointVolumeCallback, IMMNotificationClient,
    public VolumeController {
public:
    CoreAudio(HWND hWnd) :
        m_notifyHwnd(hWnd),
        m_refCount(1) {
    };

    HRESULT Init();

    float Volume();
    void Volume(float vol);

    bool Muted();
    void Muted(bool mute);

    void ReattachDefaultDevice();
    std::wstring DeviceName();

    void Dispose();
    IFACEMETHODIMP_(ULONG) AddRef();
    IFACEMETHODIMP_(ULONG) Release();

private:
    ~CoreAudio() {};
    long m_refCount;
    CCriticalSection m_critSect;

    CComPtr<IMMDeviceEnumerator> m_devEnumerator;
    CComPtr<IMMDevice> m_device;
    CComPtr<IAudioEndpointVolume> m_volumeControl;

    bool m_registeredNotifications;

    HWND m_notifyHwnd;

    HRESULT AttachDefaultDevice();
    void DetachCurrentDevice();

    /* IAudioEndpointVolumeCallback */
    IFACEMETHODIMP OnNotify(PAUDIO_VOLUME_NOTIFICATION_DATA pNotify);

    /* IMMNotificationClient */
    IFACEMETHODIMP OnDefaultDeviceChanged(
        EDataFlow flow, ERole role, LPCWSTR pwstrDefaultDeviceId);

    IFACEMETHODIMP OnDeviceStateChanged(
        LPCWSTR pwstrDeviceId, DWORD dwNewState) {
        return S_OK;
    }

    IFACEMETHODIMP OnPropertyValueChanged(
        LPCWSTR pwstrDeviceId, const PROPERTYKEY key) {
        return S_OK;
    }

    IFACEMETHODIMP OnDeviceAdded(LPCWSTR pwstrDeviceId) {
        return S_OK;
    }

    IFACEMETHODIMP OnDeviceRemoved(LPCWSTR pwstrDeviceId) {
        return S_OK;
    }

    IFACEMETHODIMP OnDeviceQueryRemove() {
        return S_OK;
    }

    IFACEMETHODIMP OnDeviceQueryRemoveFailed() {
        return S_OK;
    }

    IFACEMETHODIMP OnDeviceRemovePending() {
        return S_OK;
    }

    /* IUnknown */
    IFACEMETHODIMP QueryInterface(const IID& iid, void** ppUnk);
};