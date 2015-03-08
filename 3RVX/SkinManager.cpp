#include "SkinManager.h"

#include "Skin.h"

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
    _skin = new Skin(skinXML);
}

Skin *SkinManager::CurrentSkin() {
    return _skin;
}