#pragma once

#include <string>

class Skin;

class SkinManager {
public:
    static SkinManager *Instance();
    void LoadSkin(std::wstring skinXML);
    Skin *CurrentSkin();

private:
    static SkinManager *instance;
    Skin *_skin;

    ~SkinManager();
};