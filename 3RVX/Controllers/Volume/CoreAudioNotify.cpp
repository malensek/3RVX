#include "CoreAudioNotify.h"

HRESULT CoreAudioNotify::Init(HWND notifyHwnd)
{
    m_notifyHwnd = notifyHwnd;

	HRESULT hRes;
	hRes = m_devEnumerator.CoCreateInstance(__uuidof(MMDeviceEnumerator));
	if(hRes >= 0)
    {
		hRes = m_devEnumerator->RegisterEndpointNotificationCallback(this);
		if(hRes >= 0)
		{
			hRes = AttachDefaultDevice();
		}
	}

	return hRes;
}

void CoreAudioNotify::Dispose()
{
    DetachDevice();
}

void CoreAudioNotify::Update()
{
    DetachDevice();
    AttachDefaultDevice();
}

HRESULT CoreAudioNotify::AttachDefaultDevice()
{
    m_deviceLock.Enter();

	HRESULT hRes;
	hRes = m_devEnumerator->GetDefaultAudioEndpoint(eRender, 
		eMultimedia, &m_device);

	if(hRes >= 0)
	{
        hRes = m_device->Activate(__uuidof(m_volumeControl), 
            CLSCTX_INPROC_SERVER, NULL, (void**)&m_volumeControl);
        if(hRes >= 0)
        {
            hRes = m_volumeControl->RegisterControlChangeNotify(this);
            m_registeredNotifications = SUCCEEDED(hRes);
        }
	}

    m_deviceLock.Leave();

    return hRes;
}

void CoreAudioNotify::DetachDevice()
{
    m_deviceLock.Enter();

    if(m_volumeControl != NULL)
    {
        if(m_registeredNotifications)
        {
            m_volumeControl->UnregisterControlChangeNotify(this);
            m_registeredNotifications = false;
        }

        m_volumeControl.Release();
    }

    if(m_device != NULL)
        m_device.Release();

    m_deviceLock.Leave();
}

HRESULT CoreAudioNotify::OnNotify(PAUDIO_VOLUME_NOTIFICATION_DATA pNotify)
{
    PostMessage(m_notifyHwnd, WM_APP+1, 0, 0);
	return S_OK;
}


// - IMMNotificationClient ------------------------------------------------- //

HRESULT CoreAudioNotify::OnDefaultDeviceChanged(EDataFlow flow, 
    ERole role, LPCWSTR pwstrDefaultDeviceId)
{
    if (flow == eRender)
    {   
        PostMessage(m_notifyHwnd, WM_APP+2, 0, 0);
    }

    return S_OK;
}


// - IUnknown -------------------------------------------------------------- //

ULONG CoreAudioNotify::AddRef()
{
    return InterlockedIncrement(&m_refCount);
}

ULONG CoreAudioNotify::Release()
{
    long lRef = InterlockedDecrement(&m_refCount);
    if (lRef == 0)
    {
        delete this;
    }
    return lRef;
}

HRESULT CoreAudioNotify::QueryInterface(REFIID iid, void** ppUnk)
{
    if ((iid == __uuidof(IUnknown)) ||
        (iid == __uuidof(IMMNotificationClient)))
    {
        *ppUnk = static_cast<IMMNotificationClient*>(this);
    }
    else if (iid == __uuidof(IAudioEndpointVolumeCallback))
    {
        *ppUnk = static_cast<IAudioEndpointVolumeCallback*>(this);
    }
    else
    {
        *ppUnk = NULL;
        return E_NOINTERFACE;
    }

    AddRef();
    return S_OK;
}