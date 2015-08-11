#include "SkinV2.h"

#include "../Logger.h"
#include "../StringUtils.h"
#include "MeterComponent.h"
#include "SkinUtils.h"

SkinV2::SkinV2(std::wstring skinXML) :
SkinInfo(skinXML) {

}

SkinV2::~SkinV2() {

}

OSDComponent *SkinV2::VolumeOSD() {
    return nullptr;
}

OSDComponent *SkinV2::MuteOSD() {
    return nullptr;
}

OSDComponent *SkinV2::EjectOSD() {
    return nullptr;
}

std::vector<HICON> SkinV2::VolumeIconset() {
    std::wstring iconDir = _skinDir + L"\\Notification Icons\\";
    return SkinUtils::ReadIconDirectory(iconDir);
}

SliderComponent *SkinV2::VolumeSlider() {
    return nullptr;
}

OSDComponent *SkinV2::CreateOSDComponent(char *osdName) {
//    XMLElement *osdElem = SubElement("osds", osdName);
//    if (osdElem == NULL) {
//        return NULL;
//    }

//    OSDComponent *osd = new OSDComponent;
//    if (PopulateMeterComponent(osd, osdElem) == false) {
//        /* Could not fully populate the OSD */
//        delete osd;
//        return NULL;
//    }
//    osd->defaultUnits = DefaultUnits(osdElem);

//    return osd;
    return nullptr;
}

bool SkinV2::PopulateMeterComponent(
        MeterComponent *component, XMLElement *elem) {
//    component->background = Image(elem, "background");
//    component->mask = Image(elem, "mask");
//    component->meters = Meters(elem);

    std::wstring soundName = _skinDir + L"\\sound.wav";
    SoundPlayer *player = new SoundPlayer(soundName);
    if (player->Ready() == false) {
        delete player;
        player = NULL;
    }
    component->sound = player;

    if (component->background == NULL) {
        /* A meter skin component has to have a background image */
        return false;
    }

    return true;
}

