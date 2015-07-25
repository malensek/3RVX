#include "SkinManager.h"

#include "Skin.h"
#include "SkinV3.h"

SkinManager *SkinManager::instance;

SkinManager *SkinManager::Instance() {
    if (instance == NULL) {
        instance = new SkinManager();
    }
    return instance;
}

SkinManager::~SkinManager() {
    delete _skin;
}

void SkinManager::LoadSkin(std::wstring skinXML) {
    delete _skin;
    _skin = new SkinV3(skinXML);
}

Skin *SkinManager::CurrentSkin() {
    return _skin;
}