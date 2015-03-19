#include "MeterWndClone.h"

MeterWndClone::MeterWndClone(
    LPCWSTR className, LPCWSTR title, HINSTANCE hInstance) :
_mWnd(className, title, hInstance) {
    _mWnd.X(32);
    _mWnd.Y(32);
}

void MeterWndClone::Show() {
    _mWnd._size.cx = _mWnd._composite->GetWidth();
    _mWnd._size.cy = _mWnd._composite->GetHeight();
    _mWnd.Show();
}

void MeterWndClone::Hide() {
    _mWnd.Hide();
}

void MeterWndClone::Update(Gdiplus::Bitmap *composite) {
    _mWnd._composite = composite;
    _mWnd.UpdateLayeredWnd();
}

void MeterWndClone::Transparency(byte transparency) {
    _mWnd.Transparency(transparency);
}

void MeterWndClone::GlassMask(Gdiplus::Bitmap *mask) {
    _mWnd.GlassMask(mask);
}