// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#include "CoreAudio.h"

#include <math.h>
#include <stdlib.h>

#include "../../Logger.h"
#include "Functiondiscoverykeys_devpkey.h"
#include "VolumeTransformation.h"

// {EC9CB649-7E84-4B42-B367-7FC39BE17806}
static const GUID G3RVXCoreAudioEvent = { 0xec9cb649, 0x7e84, 0x4b42,
    { 0xb3, 0x67, 0x7f, 0xc3, 0x9b, 0xe1, 0x78, 0x6 } };
 
HRESULT CoreAudio::Init() {
    HRESULT hr;

    hr = CoCreateInstance(
        __uuidof(MMDeviceEnumerator),
        nullptr,
        CLSCTX_INPROC_SERVER,
        IID_PPV_ARGS(&_devEnumerator));

    if (SUCCEEDED(hr)) {
        hr = _devEnumerator->RegisterEndpointNotificationCallback(this);

        if (SUCCEEDED(hr)) {
            hr = AttachDevice();
        }
    }

    return hr;
}

HRESULT CoreAudio::Init(std::wstring deviceId) {
    _devId = deviceId;
    return Init();
}

void CoreAudio::Dispose() {
    DetachDevice();
    _devEnumerator->UnregisterEndpointNotificationCallback(this);
}

HRESULT CoreAudio::AttachDevice() {
    HRESULT hr;

    if (_devId.empty()) {
        /* Use default device */
        hr = _devEnumerator->GetDefaultAudioEndpoint(eRender,
            eMultimedia, &_device);
        if (SUCCEEDED(hr)) {
            LPWSTR id = nullptr;
            hr = _device->GetId(&id);
            if (SUCCEEDED(hr) && id) {
                _devId = std::wstring(id);
                CoTaskMemFree(id);
            }
        }
    } else {
        hr = _devEnumerator->GetDevice(_devId.c_str(), &_device);
    }

    if (SUCCEEDED(hr)) {
        hr = _device->Activate(__uuidof(_volumeControl),
            CLSCTX_INPROC_SERVER, nullptr, (void **) &_volumeControl);

        CLOG(L"Attached to audio device: [%s]", DeviceName().c_str());

        if (SUCCEEDED(hr)) {
            hr = _volumeControl->RegisterControlChangeNotify(this);
            _registeredNotifications = SUCCEEDED(hr);
        }
    } else {
        CLOG(L"Failed to find audio device!");
    }

    return hr;
}

void CoreAudio::DetachDevice() {
    if (_volumeControl != nullptr) {

        if (_registeredNotifications) {
            _volumeControl->UnregisterControlChangeNotify(this);
            _registeredNotifications = false;
        }

        _volumeControl->Release();
    }

    if (_device != nullptr) {
        _device->Release();
    }
}

HRESULT CoreAudio::OnNotify(PAUDIO_VOLUME_NOTIFICATION_DATA pNotify) {
    if (pNotify->guidEventContext == G3RVXCoreAudioEvent) {
        PostMessage(_notifyHwnd, MSG_VOL_CHNG, (WPARAM) 1, 0);
    } else {
        PostMessage(_notifyHwnd, MSG_VOL_CHNG, 0, 0);
    }

    return S_OK;
}

HRESULT CoreAudio::OnDefaultDeviceChanged(
    EDataFlow flow, ERole role, LPCWSTR pwstrDefaultDeviceId) {
    if (flow == eRender) {
        PostMessage(_notifyHwnd, MSG_VOL_DEVCHNG, 0, 0);
    }

    return S_OK;
}

HRESULT CoreAudio::SelectDevice(std::wstring deviceId) {
    HRESULT hr;
    _devId = deviceId;
    DetachDevice();
    hr = AttachDevice();
    return hr;
}

HRESULT CoreAudio::SelectDefaultDevice() {
    HRESULT hr;
    _devId = L"";
    DetachDevice();
    hr = AttachDevice();
    return hr;
}

std::list<VolumeController::DeviceInfo> CoreAudio::ListDevices() {
    std::list<VolumeController::DeviceInfo> devList;
    IMMDeviceCollection *devices = nullptr;

    HRESULT hr = _devEnumerator->EnumAudioEndpoints(
        eRender,
        DEVICE_STATE_ACTIVE | DEVICE_STATE_UNPLUGGED,
        &devices);

    if (FAILED(hr)) {
        return devList;
    }

    UINT numDevices = 0;
    hr = devices->GetCount(&numDevices);
    if (FAILED(hr)) {
        return devList;
    }
    LPWSTR devId;
    for (unsigned int i = 0; i < numDevices; ++i) {
        IMMDevice *device = nullptr;
        HRESULT hr = devices->Item(i, &device);
        if (FAILED(hr)) {
            continue;
        }

        hr = device->GetId(&devId);
        if (FAILED(hr)) {
            device->Release();
            continue;
        }

        std::wstring idStr;
        if (devId) {
            idStr = std::wstring(devId);
            CoTaskMemFree(devId);
        } else {
            continue;
        }

        VolumeController::DeviceInfo devInfo = {};
        devInfo.id = idStr;
        devInfo.name = DeviceName(idStr);
        devList.push_back(devInfo);
        device->Release();
    }

    devices->Release();
    return devList;
}

std::wstring CoreAudio::DeviceId() {
    return _devId;
}

std::wstring CoreAudio::DeviceName() {
    return DeviceName(_device);
}

std::wstring CoreAudio::DeviceDesc() {
    return DeviceDesc(_device);
}

std::wstring CoreAudio::DeviceName(std::wstring deviceId) {
    IMMDevice *device;
    HRESULT hr = _devEnumerator->GetDevice(deviceId.c_str(), &device);
    if (FAILED(hr)) {
        return L"";
    }
    std::wstring name = DeviceName(device);
    device->Release();
    return name;
}

std::wstring CoreAudio::DeviceDesc(std::wstring deviceId) {
    IMMDevice *device;
    HRESULT hr = _devEnumerator->GetDevice(deviceId.c_str(), &device);
    if (FAILED(hr)) {
        return L"";
    }
    std::wstring name = DeviceName(device);
    device->Release();
    return name;
}

std::wstring CoreAudio::DeviceName(IMMDevice *device) {
    if (device == nullptr) {
        return L"";
    }

    IPropertyStore *props = nullptr;
    HRESULT hr = device->OpenPropertyStore(STGM_READ, &props);
    if (FAILED(hr)) {
        return L"";
    }

    PROPVARIANT pvName;
    PropVariantInit(&pvName);
    hr = props->GetValue(PKEY_Device_FriendlyName, &pvName);
    if (FAILED(hr)) {
        props->Release();
        return L"";
    }

    std::wstring str(pvName.pwszVal);
    PropVariantClear(&pvName);
    props->Release();

    return str;
}

std::wstring CoreAudio::DeviceDesc(IMMDevice *device) {
    HRESULT hr;

    if (device == nullptr) {
        return L"";
    }

    IPropertyStore *props = nullptr;
    hr = device->OpenPropertyStore(STGM_READ, &props);
    if (FAILED(hr)) {
        return L"";
    }

    PROPVARIANT pvDesc;
    PropVariantInit(&pvDesc);
    hr = props->GetValue(PKEY_Device_DeviceDesc, &pvDesc);
    if (FAILED(hr)) {
        props->Release();
        return L"";
    }

    std::wstring str(pvDesc.pwszVal);
    PropVariantClear(&pvDesc);
    props->Release();
    return str;
}

float CoreAudio::Volume() {
    float vol = 0.0f;
    if (_volumeControl) {
        _volumeControl->GetMasterVolumeLevelScalar(&vol);

        if (_transform) {
            vol = _transform->FromVirtual(vol);
        }
    }
    return vol;
}

void CoreAudio::Volume(float vol) {
    if (vol > 1.0f) {
        vol = 1.0f;
    }

    if (vol < 0.0f) {
        vol = 0.0f;
    }

    if (_volumeControl) {
        if (_transform) {
            vol = _transform->ToVirtual(vol);
        }

        _volumeControl->SetMasterVolumeLevelScalar(vol, &G3RVXCoreAudioEvent);
    }
}

float CoreAudio::VolumeDB() {
    float vol = 0.0f;
    if (_volumeControl) {
        _volumeControl->GetMasterVolumeLevel(&vol);
    }
    return vol;
}

void CoreAudio::VolumeDB(float volDB) {
    if (_volumeControl) {
        _volumeControl->SetMasterVolumeLevel(volDB, &G3RVXCoreAudioEvent);
    }
}

bool CoreAudio::Muted() {
    if (_volumeControl == nullptr) {
        return true;
    }

    BOOL muted = FALSE;
    HRESULT hr = _volumeControl->GetMute(&muted);
    if (FAILED(hr)) {
        return false;
    }

    return (muted == TRUE) ? true : false;
}

void CoreAudio::Muted(bool muted) {
    if (_volumeControl) {
        _volumeControl->SetMute(muted, &G3RVXCoreAudioEvent);
    }
}

void CoreAudio::Transformation(VolumeTransformation* transform) {
    _transform = transform;
}

VolumeTransformation* CoreAudio::Transformation() {
    return _transform;
}

void CoreAudio::CurveInfo() {
    if (_volumeControl == nullptr) {
        return;
    }

    float min, max, inc;
    _volumeControl->GetVolumeRange(&min, &max, &inc);
    CLOG(L"Volume Range (min, max, increment): %f, %f, %f", min, max, inc);
    float range = abs(max - min);
    float steps = range / inc;
    CLOG(L"Full range: %f (%f steps)", range, steps);
    float orig = Volume();
    for (float f = 0; f < 100.1f; ++f) {
        Volume(f / 100.0f);
        QCLOG("%f    %f", Volume(), VolumeDB());
    }
    Volume(orig);
}

ULONG CoreAudio::AddRef() {
    return InterlockedIncrement(&_refCount);
}

ULONG CoreAudio::Release() {
    long lRef = InterlockedDecrement(&_refCount);
    if (lRef == 0) {
        delete this;
    }
    return lRef;
}

HRESULT CoreAudio::QueryInterface(REFIID iid, void **ppvObject) {
    if ((iid == __uuidof(IUnknown)) ||
        (iid == __uuidof(IMMNotificationClient))) {
        *ppvObject = static_cast<IMMNotificationClient*>(this);
    } else if (iid == __uuidof(IAudioEndpointVolumeCallback)) {
        *ppvObject = static_cast<IAudioEndpointVolumeCallback*>(this);
    } else {
        *ppvObject = nullptr;
        return E_NOINTERFACE;
    }

    AddRef();
    return S_OK;
}