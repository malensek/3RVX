#include "MeterWnd.h"
#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")
#include <VersionHelpers.h>
#include <sstream>

#include "Animation.h"
#include "AnimationFactory.h"

MeterWnd::MeterWnd(LPCWSTR className, LPCWSTR title, HINSTANCE hInstance) :
LayeredWnd(className, title, hInstance, NULL, WINDOW_STYLES) {

}

MeterWnd::~MeterWnd() {
    delete _hideAnimation;
    delete _composite;

    for (LayeredWnd *clone : _clones) {
        delete clone;
    }
}

void MeterWnd::Update() {
    CLOG(L"Updating meter window");
    using namespace Gdiplus;

    bool dirty = (_composite == NULL);

    for (Meter *meter : _meters) {
        if (meter->Dirty() == true) {
            dirty = true;
            break;
        }
    }

    if (dirty) {
        QCLOG(L"Contents have changed; redrawing");

        if (_composite) {
            delete _composite;
        }

        Rect bgRect(0, 0, _background->GetWidth(), _background->GetHeight());
        _composite = _background->Clone(bgRect, PixelFormat32bppARGB);
        Graphics graphics(_composite);

        for (Meter *meter : _meters) {
            CLOG(L"Drawing meter:\n%s", meter->ToString().c_str());
            meter->Draw(_composite, &graphics);
        }
    }

    Bitmap(_composite);
    UpdateClones();
}

void MeterWnd::AddMeter(Meter *meter) {
    _meters.push_back(meter);
}

void MeterWnd::MeterLevels(float value) {
    for (Meter *meter : _meters) {
        meter->Value(value);
    }
}

void MeterWnd::HideAnimation(AnimationTypes::HideAnimation anim, int speed) {
    delete _hideAnimation;
    _hideAnimation = AnimationFactory::Create(anim, speed);
}

void MeterWnd::VisibleDuration(int duration) {
    _visibleDuration = duration;
}

void MeterWnd::BackgroundImage(Gdiplus::Bitmap *background) {
    _background = background;
}

bool MeterWnd::EnableGlass(Gdiplus::Bitmap *mask) {
    bool result = LayeredWnd::EnableGlass(mask);
    ApplyClonesGlass();
    return result;
}

void MeterWnd::Show(bool animate) {
    if (_visible == false) {
        UpdateWindowPosition();
        ShowWindow(_hWnd, SW_SHOW);
        _visible = true;
    }

    ShowClones();

    if (_visibleDuration > 0) {
        SetTimer(_hWnd, TIMER_HIDE, _visibleDuration, NULL);
        KillTimer(_hWnd, TIMER_OUT);

        if (_hideAnimation) {
            _hideAnimation->Reset(this);
        }
    }
}

void MeterWnd::Hide(bool animate) {
    if (_visible == false) {
        return;
    }

    if (animate && _hideAnimation) {
        SetTimer(_hWnd, TIMER_OUT, _hideAnimation->UpdateInterval(), NULL);
    } else {
        ShowWindow(_hWnd, SW_HIDE);
        _visible = false;
        HideClones();
    }
}

void MeterWnd::AnimateOut() {
    bool animOver = _hideAnimation->Animate(this);
    if (animOver) {
        CLOG(L"Finished hide animation.");
        KillTimer(_hWnd, TIMER_OUT);
        ShowWindow(_hWnd, SW_HIDE);
        _visible = false;
        HideClones();
    }
}

byte MeterWnd::Transparency() {
    return LayeredWnd::Transparency();
}

void MeterWnd::Transparency(byte transparency) {
    LayeredWnd::Transparency(transparency);
    UpdateClonesTransparency(transparency);
}

LayeredWnd *MeterWnd::Clone() {
    int numClones = _clones.size() + 1;
    std::wstringstream cloneClass;
    cloneClass << _className << L":" << numClones;
    std::wstringstream cloneTitle;
    cloneTitle << _title << L":" << numClones;

    LayeredWnd *clone = new LayeredWnd(
        cloneClass.str().c_str(),
        cloneTitle.str().c_str(),
        _hInstance,
        _composite,
        GetWindowLong(_hWnd, GWL_EXSTYLE));

    if (_glassMask) {
        clone->EnableGlass(_glassMask);
    }

    _clones.push_back(clone);
    CLOG(L"Created meter window clone: %s/%s",
        cloneClass.str().c_str(), cloneTitle.str().c_str());
    return clone;
}

std::vector<LayeredWnd *> MeterWnd::Clones() {
    return _clones;
}

void MeterWnd::UpdateClones() {
    for (LayeredWnd *clone : _clones) {
        clone->Bitmap(_composite);
    }
}

void MeterWnd::UpdateClonesTransparency(byte transparency) {
    for (LayeredWnd *clone : _clones) {
        clone->Transparency(transparency);
    }
}

void MeterWnd::ShowClones() {
    for (LayeredWnd *clone : _clones) {
        clone->Show();
    }
}

void MeterWnd::HideClones() {
    for (LayeredWnd *clone : _clones) {
        clone->Hide();
    }
}

void MeterWnd::ApplyClonesGlass() {
    if (_glassMask == NULL) {
        return;
    }

    for (LayeredWnd *clone : _clones) {
        clone->EnableGlass(_glassMask);
    }
}

LRESULT MeterWnd::WndProc(UINT message, WPARAM wParam, LPARAM lParam) {
    if (message == WM_TIMER) {
        switch (wParam) {
        case TIMER_HIDE:
            CLOG(L"Display duration has elapsed. Hiding window.");
            Hide();
            KillTimer(_hWnd, TIMER_HIDE);
            break;

        case TIMER_OUT:
            AnimateOut();
            break;
        }
    }

    return LayeredWnd::WndProc(message, wParam, lParam);
}

