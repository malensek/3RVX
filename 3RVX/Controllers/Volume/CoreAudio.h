// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#pragma once

#include <Endpointvolume.h>
#include <Mmdeviceapi.h>
#include <string>
#include <vector>

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

    float VolumeDB();
    void VolumeDB(float volDB);

    bool Muted();
    void Muted(bool mute);

    bool DeviceEnabled();

    virtual void AddTransformation(VolumeTransformation *transform);
    virtual void RemoveTransformation(VolumeTransformation *transform);

    void CurveInfo();

    std::wstring DeviceId();
    std::wstring DeviceName();
    std::wstring DeviceName(std::wstring deviceId);
    std::wstring DeviceDesc();
    std::wstring DeviceDesc(std::wstring deviceId);

    std::vector<DeviceInfo> ListDevices();

    HRESULT SelectDevice(std::wstring deviceId);
    HRESULT SelectDefaultDevice();

    IFACEMETHODIMP_(ULONG) AddRef();
    IFACEMETHODIMP_(ULONG) Release();

private:
    std::wstring _devId;
    HWND _notifyHwnd;
    long _refCount;
    bool _registeredNotifications;
    std::vector<VolumeTransformation *> _transforms;

    IMMDevice *_device;
    IMMDeviceEnumerator *_devEnumerator;
    IAudioEndpointVolume *_volumeControl;

    ~CoreAudio() {};

    HRESULT AttachDevice();
    void DetachDevice();

    std::wstring DeviceName(IMMDevice *device);
    std::wstring DeviceDesc(IMMDevice *device);

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
    IFACEMETHODIMP QueryInterface(REFIID iid, void **ppvObject);
};