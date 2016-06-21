// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#include "SkinManager.h"

#include "Shlwapi.h"

#include "../Error.h"
#include "../Settings.h"

#include "ErrorSkin.h"
#include "MeterComponent.h"
#include "OSDComponent.h"
#include "Skin.h"
#include "SkinV2.h"
#include "SkinV3.h"
#include "SliderComponent.h"

SkinManager *SkinManager::instance;

SkinManager *SkinManager::Instance() {
    if (instance == NULL) {
        instance = new SkinManager();
    }
    return instance;
}

void SkinManager::LoadSkin(std::wstring skinXML) {
    DisposeComponents();

    /* First, make sure the skin directory exists. */
    Settings *settings = Settings::Instance();
    std::wstring skinDir = settings->SkinDir();
    if (PathFileExists(skinDir.c_str()) == FALSE) {
        Error::ErrorMessageDie(Error::SKINERR_SKINDIR, skinDir);
    }

    Skin *skin;
    SkinInfo info(skinXML, false);
    if (info.FormatVersion() == 2) {
        skin = new SkinV2(skinXML);
    } else {
        skin = new SkinV3(skinXML);
    }

    std::vector<Skin *> skins;
    skins.push_back(skin);
    skins.push_back(new SkinV3(settings->SkinXML(L"Classic")));
    skins.push_back(new ErrorSkin());

    for (Skin *skin : skins) {
        if (_volumeOSD == nullptr) {
            _volumeOSD = skin->VolumeOSD();
        }

        if (_volumeIconset.size() == 0) {
            _volumeIconset = skin->VolumeIconset();
        }

        if (_volumeSlider == nullptr) {
            _volumeSlider = skin->VolumeSlider();
        }

        if (_muteOSD == nullptr) {
            _muteOSD = skin->MuteOSD();
        }

        if (_ejectOSD == nullptr) {
            _ejectOSD = skin->EjectOSD();
        }

        if (_ejectIcon == nullptr) {
            _ejectIcon = skin->EjectIcon();
        }

        if (_brightnessOSD == nullptr) {
            _brightnessOSD = skin->BrightnessOSD();
        }
    }

    for (Skin *skin : skins) {
        delete skin;
    }
}

OSDComponent *SkinManager::VolumeOSD() {
    return _volumeOSD;
}

std::vector<HICON> &SkinManager::VolumeIconset() {
    return _volumeIconset;
}

SliderComponent *SkinManager::VolumeSlider() {
    return _volumeSlider;
}

OSDComponent *SkinManager::MuteOSD() {
    return _muteOSD;
}

OSDComponent *SkinManager::EjectOSD() {
    return _ejectOSD;
}

HICON &SkinManager::EjectIcon() {
    return _ejectIcon;
}

OSDComponent * SkinManager::BrightnessOSD() {
    return _brightnessOSD;
}

SkinManager::~SkinManager() {
    DisposeComponents();
}

void SkinManager::DisposeComponents() {
    delete _volumeOSD;
    _volumeOSD = nullptr;
    for (HICON icon : _volumeIconset) {
        DestroyIcon(icon);
    }
    _volumeIconset.clear();
    delete _volumeSlider;
    _volumeSlider = nullptr;

    delete _muteOSD;
    _muteOSD = nullptr;

    delete _ejectOSD;
    _ejectOSD = nullptr;
    DestroyIcon(_ejectIcon);
    _ejectIcon = nullptr;

    delete _brightnessOSD;
    _brightnessOSD = nullptr;
}