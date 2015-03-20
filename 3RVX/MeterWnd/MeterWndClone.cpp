#include "MeterWndClone.h"

MeterWndClone::MeterWndClone(
    LPCWSTR className, LPCWSTR title, HINSTANCE hInstance) :
_mWnd(className, title, hInstance) {
}

MeterWndClone::~MeterWndClone() {

}

void MeterWndClone::Show() {
    _mWnd.Show();
}

void MeterWndClone::Hide() {
    _mWnd.Hide();
}

void MeterWndClone::Update(Gdiplus::Bitmap *composite) {
    _mWnd._composite = composite;
    _mWnd._size.cx = composite->GetWidth();
    _mWnd._size.cy = composite->GetHeight();
    _mWnd.UpdateLayeredWnd();
}

void MeterWndClone::Transparency(byte transparency) {
    _mWnd.Transparency(transparency);
}

void MeterWndClone::GlassMask(Gdiplus::Bitmap *mask) {
    _mWnd.GlassMask(mask);
}

MeterWnd &MeterWndClone::MeterWindow() {
    return _mWnd;
}