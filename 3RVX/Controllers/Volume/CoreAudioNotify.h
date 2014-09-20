// CoreAudioNotify
// ------------------------------------------------------------------------- //
// Class for reading/modifying the system volume level, as well as notifying
// the main window when volume changes occur.
// 
// Based on sample code provided in the MS Windows SDK 7.0.
// ------------------------------------------------------------------------- //

#pragma once

#include <Endpointvolume.h>
#include <Mmdeviceapi.h>
#include <atlbase.h>
#include <atlsync.h>

class CoreAudioNotify : IAudioEndpointVolumeCallback, IMMNotificationClient {
public:
    CoreAudioNotify() : m_refCount(1) {};

	IFACEMETHODIMP_(ULONG) AddRef();
	IFACEMETHODIMP_(ULONG) Release();

	HRESULT Init(HWND notifyHwnd);

	void Dispose();
    void Update();

private:
    long m_refCount;
    CCriticalSection m_deviceLock;

	CComPtr<IMMDeviceEnumerator> m_devEnumerator;
	CComPtr<IMMDevice> m_device;
    CComPtr<IAudioEndpointVolume> m_volumeControl;

    bool m_registeredNotifications;

    HWND m_notifyHwnd;

    ~CoreAudioNotify() {};

	HRESULT AttachDefaultDevice();
    void DetachDevice();

    // IAudioEndpointVolumeCallback
	IFACEMETHODIMP OnNotify(PAUDIO_VOLUME_NOTIFICATION_DATA pNotify);

    // IMMNotificationClient
    IFACEMETHODIMP OnDefaultDeviceChanged(EDataFlow flow, ERole role, LPCWSTR pwstrDefaultDeviceId);

    IFACEMETHODIMP OnDeviceStateChanged(LPCWSTR pwstrDeviceId, DWORD dwNewState) {
		return S_OK;
	}

    IFACEMETHODIMP OnPropertyValueChanged(LPCWSTR pwstrDeviceId, const PROPERTYKEY key) {
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

    // IUnknown
    IFACEMETHODIMP QueryInterface(const IID& iid, void** ppUnk);
};