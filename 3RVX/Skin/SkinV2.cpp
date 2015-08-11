#include "SkinV2.h"

#include "../Logger.h"
#include "../MeterWnd/Meters/MeterTypes.h"
#include "../StringUtils.h"
#include "MeterComponent.h"
#include "OSDComponent.h"
#include "SkinUtils.h"

SkinV2::SkinV2(std::wstring skinXML) :
SkinInfo(skinXML) {

}

SkinV2::~SkinV2() {

}

OSDComponent *SkinV2::VolumeOSD() {
    OSDComponent *volume = new OSDComponent;

    /* Images */
    volume->background = LoadImg(_skinDir + L"\\OSD\\back.png");
    volume->mask = LoadImg(_skinDir + L"\\OSD\\glassMask.png");

    /* Sound */
    std::wstring soundName = _skinDir + L"\\sound.wav";
    SoundPlayer *player = new SoundPlayer(soundName);
    if (player->Ready() == false) {
        delete player;
        player = NULL;
    }
    volume->sound = player;

    /* Determine the number of units */
    int units = 10;
    tinyxml2::XMLElement *meterMax = SubElement("osd", "meterMax");
    if (meterMax) {
        meterMax->QueryIntText(&units);
    }
    volume->defaultUnits = units;

    /* Load the meter(s) */
    const char *meterType = nullptr;
    tinyxml2::XMLElement *meterOrientation
        = SubElement("osd", "meterOrientation");
    if (meterOrientation) {
        meterType = meterOrientation->GetText();
    }
    int x = 0;
    int y = 0;
    tinyxml2::XMLElement *pos = SubElement("osd", "meterPosition");
    if (pos) {
        tinyxml2::XMLElement *xelem = pos->FirstChildElement("X");
        tinyxml2::XMLElement *yelem = pos->FirstChildElement("Y");
        if (xelem) {
            xelem->QueryIntText(&x);
        }
        if (yelem) {
            yelem->QueryIntText(&y);
        }
    }

    std::wstring meterImg = _skinDir + L"\\OSD\\meter.png";
    if (meterType == "vertical") {
        volume->meters.push_back(
            new VerticalBar(meterImg, x, y, units));
    } else if (meterType == "bitstrip") {
        volume->meters.push_back(
            new Bitstrip(meterImg, x, y, units));
    } else {
        /* Horizontal meter is the default */
        volume->meters.push_back(
            new HorizontalBar(meterImg, x, y, units));
    }

    return volume;
}

OSDComponent *SkinV2::MuteOSD() {
    OSDComponent *mute = new OSDComponent;
    mute->background = LoadImg(_skinDir + L"\\OSD\\mute.png");
    mute->mask = LoadImg(_skinDir + L"\\OSD\\glassMask.png");
    return mute;
}

OSDComponent *SkinV2::EjectOSD() {
    OSDComponent *eject = new OSDComponent;
    eject->background = LoadImg(_skinDir + L"\\OSD\\eject.png");
    eject->mask = LoadImg(_skinDir + L"\\OSD\\glassMask.png");
    return eject;
}

std::vector<HICON> SkinV2::VolumeIconset() {
    std::wstring iconDir = _skinDir + L"\\Notification Icons\\";
    return SkinUtils::ReadIconDirectory(iconDir);
}

SliderComponent *SkinV2::VolumeSlider() {
    return nullptr;
}