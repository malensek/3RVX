#include "CoreAudio.h"
#include "Functiondiscoverykeys_devpkey.h"
#include "../../Logger.h"

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
    PostMessage(_notifyHwnd, MSG_VOL_CHNG, 0, 0);
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

    CComPtr<IMMDevice> device;
    LPWSTR devId;

    std::list<VolumeController::DeviceInfo> devList;

    for (unsigned int i = 0; i < numDevices; ++i) {
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

std::wstring CoreAudio::DeviceName(std::wstring deviceId) {
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

float CoreAudio::Volume() {
    float vol = 0.0f;
    _volumeControl->GetMasterVolumeLevelScalar(&vol);
    return vol;
}

void CoreAudio::Volume(float vol) {
    if (vol > 1.0f) {
        vol = 1.0f;
    }

    if (vol < 0.0f) {
        vol = 0.0f;
    }

    _volumeControl->SetMasterVolumeLevelScalar(vol, NULL);
}

bool CoreAudio::Muted() {
    BOOL muted = FALSE;
    _volumeControl->GetMute(&muted);

    return (muted == TRUE) ? true : false;
}

void CoreAudio::Muted(bool muted) {
    _volumeControl->SetMute(muted, NULL);
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