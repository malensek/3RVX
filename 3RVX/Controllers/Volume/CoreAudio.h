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

    CoreAudio(HWND hWnd, LPWSTR deviceId) :
        _devId(deviceId),
        _notifyHwnd(hWnd),
        _refCount(1) { }

    HRESULT Init();
    void Dispose();

    float Volume();
    void Volume(float vol);

    bool Muted();
    void Muted(bool mute);

    void ReattachDevice();
    std::wstring DeviceName();

    IFACEMETHODIMP_(ULONG) AddRef();
    IFACEMETHODIMP_(ULONG) Release();

private:
    LPWSTR _devId;
    CCriticalSection _critSect;
    HWND _notifyHwnd;
    long _refCount;
    bool _registeredNotifications;

    CComPtr<IMMDevice> _device;
    CComPtr<IMMDeviceEnumerator> _devEnumerator;
    CComPtr<IAudioEndpointVolume> _volumeControl;

    ~CoreAudio() {};

    HRESULT AttachDevice();
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