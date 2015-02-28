#include "CoreAudio.h"
#include "Functiondiscoverykeys_devpkey.h"
#include "../../Logger.h"

// {EC9CB649-7E84-4B42-B367-7FC39BE17806}
static const GUID G3RVXCoreAudioEvent = { 0xec9cb649, 0x7e84, 0x4b42,
    { 0xb3, 0x67, 0x7f, 0xc3, 0x9b, 0xe1, 0x78, 0x6 } };

 
HRESULT CoreAudio::Init() {
    HRESULT hr;

    hr = _devEnumerator.CoCreateInstance(__uuidof(MMDeviceEnumerator));
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
    _critSect.Enter();

    HRESULT hr;

    if (_devId.empty()) {
        /* Use default device */
        hr = _devEnumerator->GetDefaultAudioEndpoint(eRender,
            eMultimedia, &_device);
        if (SUCCEEDED(hr)) {
            LPWSTR id;
            _device->GetId(&id);
            _devId = std::wstring(id);
        }
    } else {
        hr = _devEnumerator->GetDevice(_devId.c_str(), &_device);
    }

    if (SUCCEEDED(hr)) {
        hr = _device->Activate(__uuidof(_volumeControl),
            CLSCTX_INPROC_SERVER, NULL, (void **) &_volumeControl);

        CLOG(L"Attached to audio device: [%s]", DeviceName().c_str());

        if (SUCCEEDED(hr)) {
            hr = _volumeControl->RegisterControlChangeNotify(this);
            _registeredNotifications = SUCCEEDED(hr);
        }
    } else {
        CLOG(L"Failed to find audio device!");
    }

    _critSect.Leave();
    return hr;
}

void CoreAudio::DetachDevice() {
    _critSect.Enter();

    if (_volumeControl != NULL) {

        if (_registeredNotifications) {
            _volumeControl->UnregisterControlChangeNotify(this);
            _registeredNotifications = false;
        }

        _volumeControl.Release();
    }

    if (_device != NULL) {
        _device.Release();
    }

    _critSect.Leave();
}

HRESULT CoreAudio::OnNotify(PAUDIO_VOLUME_NOTIFICATION_DATA pNotify) {
    if (pNotify->guidEventContext == G3RVXCoreAudioEvent) {
        PostMessage(_notifyHwnd, MSG_VOL_CHNG, (WPARAM) 1, NULL);
    } else {
        PostMessage(_notifyHwnd, MSG_VOL_CHNG, NULL, NULL);
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
    _critSect.Enter();

    CComPtr<IMMDeviceCollection> devices;
    _devEnumerator->EnumAudioEndpoints(
        eRender,
        DEVICE_STATE_ACTIVE | DEVICE_STATE_UNPLUGGED,
        &devices);

    UINT numDevices = 0;
    devices->GetCount(&numDevices);

    LPWSTR devId;

    std::list<VolumeController::DeviceInfo> devList;

    for (unsigned int i = 0; i < numDevices; ++i) {
        CComPtr<IMMDevice> device;
        devices->Item(i, &device);
        device->GetId(&devId);

        std::wstring idStr(devId);
        VolumeController::DeviceInfo devInfo = {};
        devInfo.id = idStr;
        devInfo.name = DeviceName(idStr);
        devList.push_back(devInfo);
    }

    return devList;
    _critSect.Leave();
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
    CComPtr<IMMDevice> device;
    _devEnumerator->GetDevice(deviceId.c_str(), &device);
    return DeviceName(device);
}

std::wstring CoreAudio::DeviceDesc(std::wstring deviceId) {
    CComPtr<IMMDevice> device;
    _devEnumerator->GetDevice(deviceId.c_str(), &device);
    return DeviceName(device);
}

std::wstring CoreAudio::DeviceName(CComPtr<IMMDevice> device) {
    IPropertyStore *props = NULL;
    HRESULT hr = device->OpenPropertyStore(STGM_READ, &props);

    if (FAILED(hr)) {
        return L"";
    }

    PROPVARIANT pvName;
    PropVariantInit(&pvName);
    props->GetValue(PKEY_Device_FriendlyName, &pvName);

    std::wstring str(pvName.pwszVal);
    PropVariantClear(&pvName);
    props->Release();

    return str;
}

std::wstring CoreAudio::DeviceDesc(CComPtr<IMMDevice> device) {
    if (device == NULL) {
        return L"";
    }

    IPropertyStore *props = NULL;
    HRESULT hr = device->OpenPropertyStore(STGM_READ, &props);

    if (FAILED(hr)) {
        return L"";
    }

    PROPVARIANT pvDesc;
    PropVariantInit(&pvDesc);
    props->GetValue(PKEY_Device_DeviceDesc, &pvDesc);

    std::wstring str(pvDesc.pwszVal);
    PropVariantClear(&pvDesc);
    props->Release();

    return str;
}

float CoreAudio::Volume() {
    float vol = 0.0f;
    if (_volumeControl) {
        _volumeControl->GetMasterVolumeLevelScalar(&vol);
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
        _volumeControl->SetMasterVolumeLevelScalar(vol, &G3RVXCoreAudioEvent);
    }
}

bool CoreAudio::Muted() {
    if (_volumeControl == NULL) {
        return true;
    }

    BOOL muted = FALSE;
    _volumeControl->GetMute(&muted);

    return (muted == TRUE) ? true : false;
}

void CoreAudio::Muted(bool muted) {
    if (_volumeControl) {
        _volumeControl->SetMute(muted, &G3RVXCoreAudioEvent);
    }
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

HRESULT CoreAudio::QueryInterface(REFIID iid, void **ppUnk) {
    if ((iid == __uuidof(IUnknown)) ||
        (iid == __uuidof(IMMNotificationClient))) {
        *ppUnk = static_cast<IMMNotificationClient*>(this);
    } else if (iid == __uuidof(IAudioEndpointVolumeCallback)) {
        *ppUnk = static_cast<IAudioEndpointVolumeCallback*>(this);
    } else {
        *ppUnk = NULL;
        return E_NOINTERFACE;
    }

    AddRef();
    return S_OK;
}