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
    using namespace Gdiplus;

    printf("Updating meter window\n");

    if (m_wndImg == NULL || m_bgImg == NULL) {
        /* should return a flag */
        return;
    }

    for (Meter *meter : m_meters) {
        if (meter->Dirty() == true) {
            m_dirty = true;
            break;
        }
    }

    if (m_dirty) {
        printf("dirty; redraw\n");
        delete m_wndImg;
        Rect bgRect(0, 0, m_bgImg->GetWidth(), m_bgImg->GetHeight());
        m_wndImg = m_bgImg->Clone(bgRect, PixelFormat32bppARGB);

        Graphics graphics(m_wndImg);

        for (Meter *meter : m_meters) {
            printf("drawing meter\n");
            meter->Draw(m_wndImg, &graphics);
        }

        m_dirty = false;
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

LRESULT MeterWnd::WndProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    return DefWindowProc(m_hWnd, message, wParam, lParam);
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