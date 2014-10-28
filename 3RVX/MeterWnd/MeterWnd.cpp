#include "MeterWnd.h"

MeterWnd *MeterWnd::Clone(HINSTANCE hInstance, LPCWSTR className, LPCWSTR title)
{
/*    MeterWnd *newWnd = new MeterWnd(hInstance, className, title);

    newWnd->m_parent = this;
    newWnd->m_buffer = m_buffer;
    newWnd->m_size = m_size;
    newWnd->m_location = m_location;

    if(m_showAnim)
    {
        newWnd->m_showAnim = m_showAnim->Clone();
        newWnd->m_showAnim->SetLayeredWnd(newWnd);
    }

    if(m_hideAnim)
    {
        newWnd->m_hideAnim = m_hideAnim->Clone();
        newWnd->m_hideAnim->SetLayeredWnd(newWnd);
    }

    return newWnd;
    */
    return NULL;
}

void MeterWnd::Update()
{
    CLOG(L"Updating meter window");
    using namespace Gdiplus;

    if (m_wndImg == NULL || m_bgImg == NULL) {
        CLOG(L"Window images have not been initialized");
        /* should return a flag */
        return;
    }

    bool dirty = false;
    for (Meter *meter : m_meters) {
        if (meter->Dirty() == true) {
            dirty = true;
            break;
        }
    }

    if (dirty) {
        CLOG(L"Contents have changed; redrawing");

        delete m_wndImg;
        Rect bgRect(0, 0, m_bgImg->GetWidth(), m_bgImg->GetHeight());
        m_wndImg = m_bgImg->Clone(bgRect, PixelFormat32bppARGB);
        Graphics graphics(m_wndImg);

        for (Meter *meter : m_meters) {
            CLOG(L"Drawing meter:\n%s", meter->ToString().c_str());
            meter->Draw(m_wndImg, &graphics);
        }
    }

    m_lWnd.Image(m_wndImg);
}

void MeterWnd::AddMeter(Meter *meter)
{
    m_meters.push_back(meter);
}

void MeterWnd::Show()
{
    /*
    KillTimer(m_hWnd, TIMER_HIDEANIM);

    if(m_showAnim)
    {
        StartAnimation(m_showAnim, TIMER_SHOWANIM);
    }
    else
    {
        LayeredWnd::Show();
        SetTransparency(255);
    }

    SetTimer(m_hWnd, TIMER_DELAY, 600, NULL);
    */
    m_lWnd.Show();
}

void MeterWnd::Hide()
{
    /*
    KillTimer(m_hWnd, TIMER_SHOWANIM);

    if(m_hideAnim)
        StartAnimation(m_hideAnim, TIMER_HIDEANIM);
    else
        LayeredWnd::Hide();
    */
    m_lWnd.Hide();
}

void MeterWnd::SetBackgroundImage(Gdiplus::Bitmap *backgroundImage)
{
	using namespace Gdiplus;

    if (m_wndImg) {
        delete m_wndImg;
    }

	m_bgImg = backgroundImage;
	m_wndImg = new Bitmap(
		m_bgImg->GetWidth(),
		m_bgImg->GetHeight(), 
		PixelFormat32bppARGB);
}

void MeterWnd::MeterLevels(float value)
{
    for (Meter *meter : m_meters) {
        meter->Value(value);
    }
}

int MeterWnd::X() const {
    return m_lWnd.X();
}

void MeterWnd::X(int x) {
    m_lWnd.X(x);
}

int MeterWnd::Y() const {
    return m_lWnd.Y();
}

void MeterWnd::Y(int y) {
    m_lWnd.Y(y);
}

int MeterWnd::Height() const {
    return m_lWnd.Height();
}

int MeterWnd::Width() const {
    return m_lWnd.Width();
}

byte MeterWnd::Transparency() const {
    return m_lWnd.Transparency();
}

void MeterWnd::Transparency(byte transparency) {
    m_lWnd.Transparency(transparency);
}