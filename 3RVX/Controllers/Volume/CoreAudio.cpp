#include "CoreAudio.h"

HRESULT CoreAudio::Init()
{
	HRESULT hr;

	hr = m_devEnumerator.CoCreateInstance(__uuidof(MMDeviceEnumerator));
	if (SUCCEEDED(hr)) {
		hr = m_devEnumerator->RegisterEndpointNotificationCallback(this);

		if (SUCCEEDED(hr)) {
			hr = AttachDefaultDevice();
		}
	}

	return hr;
}

void CoreAudio::Dispose()
{
	DetachCurrentDevice();
	m_devEnumerator->UnregisterEndpointNotificationCallback(this);
}

HRESULT CoreAudio::AttachDefaultDevice()
{
	m_critSect.Enter();

	HRESULT hr;
	hr = m_devEnumerator->GetDefaultAudioEndpoint(eRender,
		eMultimedia, &m_device);

	if (SUCCEEDED(hr)) {
		hr = m_device->Activate(__uuidof(m_volumeControl),
			CLSCTX_INPROC_SERVER, NULL, (void**)&m_volumeControl);

		if (SUCCEEDED(hr)) {
			hr = m_volumeControl->RegisterControlChangeNotify(this);
			m_registeredNotifications = SUCCEEDED(hr);
		}
	}

	m_critSect.Leave();
	return hr;
}

void CoreAudio::DetachCurrentDevice()
{
	m_critSect.Enter();

	if (m_volumeControl != NULL) {

		if (m_registeredNotifications) {
			m_volumeControl->UnregisterControlChangeNotify(this);
			m_registeredNotifications = false;
		}

		m_volumeControl.Release();
	}

	if (m_device != NULL) {
		m_device.Release();
	}

	m_critSect.Leave();
}

HRESULT CoreAudio::OnNotify(PAUDIO_VOLUME_NOTIFICATION_DATA pNotify)
{
	PostMessage(m_notifyHwnd, WM_APP + 1, 0, 0);
	return S_OK;
}

HRESULT CoreAudio::OnDefaultDeviceChanged(
	EDataFlow flow, ERole role, LPCWSTR pwstrDefaultDeviceId)
{
	if (flow == eRender) {
		PostMessage(m_notifyHwnd, WM_APP + 2, 0, 0);
	}

	return S_OK;
}

void CoreAudio::ReattachDefaultDevice()
{
	DetachCurrentDevice();
	AttachDefaultDevice();
}

float CoreAudio::Volume()
{
	float vol = 0.0f;
	m_volumeControl->GetMasterVolumeLevelScalar(&vol);
	return vol;
}

void CoreAudio::Volume(float vol)
{
	if (vol > 1.0f) {
		vol = 1.0f;
	}

	if (vol < 0.0f) {
		vol = 0.0f;
	}

	m_volumeControl->SetMasterVolumeLevelScalar(vol, NULL);
}

bool CoreAudio::Muted()
{
	BOOL muted;
	m_volumeControl->GetMute(&muted);

	return (muted == TRUE) ? true : false;
}

void CoreAudio::Muted(bool muted)
{
	m_volumeControl->SetMute(muted, NULL);
}

ULONG CoreAudio::AddRef()
{
	return InterlockedIncrement(&m_refCount);
}

ULONG CoreAudio::Release()
{
	long lRef = InterlockedDecrement(&m_refCount);
	if (lRef == 0) {
		delete this;
	}
	return lRef;
}

HRESULT CoreAudio::QueryInterface(REFIID iid, void** ppUnk)
{
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