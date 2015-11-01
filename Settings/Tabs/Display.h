#pragma once

#include "SettingsTab.h"

#include <CommCtrl.h>

class Display : public SettingsTab {
public:
    Display() :
    SettingsTab(NULL, MAKEINTRESOURCE(IDD_DISPLAY), L"Display") {

    }

    virtual void SaveSettings();

protected:
    virtual void Initialize();
    virtual void LoadSettings();

private:
    bool OnAnimationChange();
    bool OnAnimationSpin(NMUPDOWN *ud);
    bool OnCustomCheckChange();
    bool OnPositionChange();

private:
    /* Controls: */
    GroupBox *_visibilityGroup;;
    Checkbox *_onTop;
    Checkbox *_hideFullscreen;
    Checkbox *_hideDirectX;

    GroupBox *_positionGroup;
    ComboBox *_position;
    Label *_customX;
    EditBox *_positionX;
    Label *_customY;
    EditBox *_positionY;
    Checkbox *_customDistance;
    Spinner *_edgeSpinner;
    Label *_displayDevLabel;
    ComboBox *_displayDevice;

    GroupBox *_animationGroup;
    ComboBox *_hideAnimation;
    Label *_hideDelayLabel;
    Spinner *_hideDelay;
    Label *_hideSpeedLabel;
    Spinner *_hideSpeed;

private:
    /* Strings: */
    std::wstring _allMonitorStr = L"All Monitors";
    std::wstring _primaryMonitorStr = L"Primary Monitor";
    std::wstring _customPositionStr = L"Custom";
    std::wstring _noAnimStr = L"None";

private:
    /* Constants: */
    const int MIN_EDGE = -65535;
    const int MAX_EDGE = 65535;
    const int MIN_MS = USER_TIMER_MINIMUM;
    const int MAX_MS = 60000;
    const int ANIM_SPIN_INCREMENT = 100;
};