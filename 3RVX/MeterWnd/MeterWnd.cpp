// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#include "MeterWnd.h"
#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")
#include <VersionHelpers.h>
#include <sstream>

#include "Animation.h"
#include "AnimationFactory.h"
#include "../DisplayManager.h"

MeterWnd::MeterWnd(LPCWSTR className, LPCWSTR title, HINSTANCE hInstance) :
LayeredWnd(className, title, hInstance, NULL, WINDOW_STYLES) {

}

MeterWnd::~MeterWnd() {
    delete _hideAnimation;
    delete _composite;
    DeleteClones();
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

bool MeterWnd::DisableFullscreen() {
    return _disableFullscreen;
}

void MeterWnd::DisableFullscreen(bool disable) {
    _disableFullscreen = disable;
}

bool MeterWnd::DisableDirectX() {
    return _disableDirectX;
}

void MeterWnd::DisableDirectX(bool disable) {
    _disableDirectX = disable;
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

        bool disabled = false;
        if (_disableFullscreen
                && DisplayManager::IsFullscreen(Window::Handle())) {
            disabled = true;
        }

        if (_disableDirectX
                && DisplayManager::Direct3DOccluded(Window::Handle())) {
            disabled = true;
        }


        if (disabled == false) {
            ShowWindow(Window::Handle(), SW_SHOW);
            _visible = true;
        }
    }

    ShowClones();

    if (_visibleDuration > 0) {
        SetTimer(Window::Handle(), TIMER_HIDE, _visibleDuration, NULL);
        KillTimer(Window::Handle(), TIMER_OUT);

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
        SetTimer(Window::Handle(),
            TIMER_OUT, _hideAnimation->UpdateInterval(), NULL);
    } else {
        ShowWindow(Window::Handle(), SW_HIDE);
        _visible = false;
        HideClones();
    }
}

void MeterWnd::AnimateOut() {
    bool animOver = _hideAnimation->Animate(this);
    if (animOver) {
        CLOG(L"Finished hide animation.");
        KillTimer(Window::Handle(), TIMER_OUT);
        ShowWindow(Window::Handle(), SW_HIDE);
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
    size_t numClones = _clones.size() + 1;
    std::wstringstream cloneClass;
    cloneClass << Window::ClassName() << L":" << numClones;
    std::wstringstream cloneTitle;
    cloneTitle << Window::Title() << L":" << numClones;

    LayeredWnd *clone = new LayeredWnd(
        cloneClass.str().c_str(),
        cloneTitle.str().c_str(),
        Window::InstanceHandle(),
        _composite,
        GetWindowLong(Window::Handle(), GWL_EXSTYLE));

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

void MeterWnd::DeleteClones() {
    for (LayeredWnd *clone : _clones) {
        delete clone;
    }
    _clones.clear();
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
        if (_disableFullscreen
                && DisplayManager::IsFullscreen(clone->Handle())) {
            continue;
        }
        if (_disableDirectX
                && DisplayManager::Direct3DOccluded(clone->Handle())) {
            continue;
        }

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

LRESULT MeterWnd::WndProc(
        HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

    if (message == WM_TIMER) {
        switch (wParam) {
        case TIMER_HIDE:
            CLOG(L"Display duration has elapsed. Hiding window.");
            Hide();
            KillTimer(hWnd, TIMER_HIDE);
            break;

        case TIMER_OUT:
            AnimateOut();
            break;
        }
    }

    return LayeredWnd::WndProc(hWnd, message, wParam, lParam);
}

