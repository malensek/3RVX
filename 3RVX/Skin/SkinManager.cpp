#include "SkinManager.h"

#include "Skin.h"
#include "SkinV3.h"

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

    Skin *skin = new SkinV3(skinXML);

    _volumeOSD = skin->VolumeOSD();
    _volumeIconset = skin->VolumeIconset();
    _volumeSlider = skin->VolumeSlider();
    _muteOSD = skin->MuteOSD();
    _ejectOSD = skin->EjectOSD();
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

    delete _muteOSD;
    _muteOSD = NULL;

    delete _ejectOSD;
    _ejectOSD = NULL;
}