#include "SkinManager.h"

#include "Skin.h"
#include "SkinV2.h"
#include "SkinV3.h"

#include "../Settings.h"
#include "MeterComponent.h"
#include "OSDComponent.h"
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

    Skin *skin;
    SkinInfo info(skinXML);
    if (info.FormatVersion() == 2) {
        skin = new SkinV2(skinXML);
    } else {
        skin = new SkinV3(skinXML);
    }

    std::vector<Skin *> skins;
    skins.push_back(skin);
    skins.push_back(new SkinV3(Settings::Instance()->SkinXML(L"Classic")));

    for (Skin *skin : skins) {
        if (_volumeOSD == NULL) {
            _volumeOSD = skin->VolumeOSD();
        }

        if (_volumeIconset.size() == 0) {
            _volumeIconset = skin->VolumeIconset();
        }

        if (_volumeSlider == NULL) {
            _volumeSlider = skin->VolumeSlider();
        }

        if (_muteOSD == NULL) {
            _muteOSD = skin->MuteOSD();
        }

        if (_ejectOSD == NULL) {
            _ejectOSD = skin->EjectOSD();
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

SkinManager::~SkinManager() {
    DisposeComponents();
}

void SkinManager::DisposeComponents() {
    delete _volumeOSD;
    _volumeOSD = NULL;
    for (HICON icon : _volumeIconset) {
        DestroyIcon(icon);
    }
    _volumeIconset.clear();
    delete _volumeSlider;
    _volumeSlider = NULL;

    delete _muteOSD;
    _muteOSD = NULL;

    delete _ejectOSD;
    _ejectOSD = NULL;
}