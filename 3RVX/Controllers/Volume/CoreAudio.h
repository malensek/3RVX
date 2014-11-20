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
        _notifyHwnd(hWnd),
        _refCount(1) { }

    HRESULT Init();
    HRESULT Init(std::wstring deviceId);
    void Dispose();

    float Volume();
    void Volume(float vol);

    bool Muted();
    void Muted(bool mute);

    std::wstring DeviceName();

    HRESULT SelectDevice(std::wstring deviceId);
    HRESULT SelectDefaultDevice();

    IFACEMETHODIMP_(ULONG) AddRef();
    IFACEMETHODIMP_(ULONG) Release();

private:
    std::wstring _devId;
    CCriticalSection _critSect;
    HWND _notifyHwnd;
    long _refCount;
    bool _registeredNotifications;

    CComPtr<IMMDevice> _device;
    CComPtr<IMMDeviceEnumerator> _devEnumerator;
    CComPtr<IAudioEndpointVolume> _volumeControl;

    ~CoreAudio() {};

    HRESULT AttachDevice();
    void DetachDevice();

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