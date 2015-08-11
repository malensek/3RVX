#include "SkinV2.h"

#include "../Logger.h"
#include "../StringUtils.h"
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
    XMLElement *osdElem = SubElement("osds", osdName);
    if (osdElem == NULL) {
        return NULL;
    }

    OSDComponent *osd = new OSDComponent;
    if (PopulateMeterComponent(osd, osdElem) == false) {
        /* Could not fully populate the OSD */
        delete osd;
        return NULL;
    }
    osd->defaultUnits = DefaultUnits(osdElem);

    return osd;
}


