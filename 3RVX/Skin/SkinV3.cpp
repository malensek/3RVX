// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#include "SkinV3.h"

#include <algorithm>
#include <memory>
#include <vector>
#include <Shlwapi.h>

#include "../CommCtl.h"
#include "../Error.h"
#include "../MeterWnd/Meters/MeterTypes.h"
#include "../StringUtils.h"
#include "../Slider/SliderKnob.h"
#include "../SoundPlayer.h"
#include "OSDComponent.h"
#include "MeterComponent.h"
#include "SliderComponent.h"

SkinV3::SkinV3(std::wstring skinXML) :
SkinInfo(skinXML) {

}

SkinV3::~SkinV3() {

}

OSDComponent *SkinV3::VolumeOSD() {
    return CreateOSDComponent("volume");
}

OSDComponent *SkinV3::MuteOSD() {
    return CreateOSDComponent("mute");
}

OSDComponent *SkinV3::EjectOSD() {
    return CreateOSDComponent("eject");
}

OSDComponent *SkinV3::BrightnessOSD() {
    return CreateOSDComponent("brightness");
}

HICON SkinV3::EjectIcon() {
    XMLElement *eject = SubElement("osds", "eject");
    return Icon(eject);
}

std::vector<HICON> SkinV3::VolumeIconset() {
    std::vector<HICON> iconList;

    XMLElement *volumeIconElem = SubElement("osds", "volume");
    if (volumeIconElem == NULL) {
        return iconList;
    }

    return Iconset(volumeIconElem);
}

SliderComponent *SkinV3::VolumeSlider() {
    return CreateSliderComponent("volume");
}

OSDComponent *SkinV3::CreateOSDComponent(char *osdName) {
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

SliderComponent *SkinV3::CreateSliderComponent(char *sliderName) {
    XMLElement *sliderElem = SubElement("sliders", sliderName);
    if (sliderElem == NULL) {
        return NULL;
    }

    SliderComponent *slider = new SliderComponent;
    if (PopulateMeterComponent(slider, sliderElem) == false) {
        /* Could not fully populate the slider */
        delete slider;
        return NULL;
    }
    slider->knob = Knob(sliderElem);

    return slider;
}

bool SkinV3::PopulateMeterComponent(
        MeterComponent *component, XMLElement *elem) {
    component->background = Image(elem, "background");
    component->mask = Image(elem, "mask");
    component->meters = Meters(elem);
    component->sound = Sound(elem);

    if (component->background == NULL) {
        /* A meter skin component has to have a background image */
        return false;
    }

    return true;
}

int SkinV3::DefaultUnits(XMLElement *elem) {
    int defaultUnits = DEFAULT_UNITS;
    elem->QueryIntAttribute("defaultUnits", &defaultUnits);
    return defaultUnits;
}

Gdiplus::Bitmap *SkinV3::Image(XMLElement *elem, char *attName) {
    const char *imgFile = elem->Attribute(attName);
    if (imgFile == NULL) {
        std::wstring aName = StringUtils::Widen(attName);
        CLOG(L"Could not find XML attribute: %s", aName.c_str());
        return NULL;
    }

    std::wstring wImgFile = _skinDir + L"\\" + StringUtils::Widen(imgFile);
    if (PathFileExists(wImgFile.c_str()) == FALSE) {
        Error::ErrorMessageDie(Error::SKINERR_NOTFOUND, wImgFile);
    }

    Gdiplus::Bitmap *bg = Gdiplus::Bitmap::FromFile(wImgFile.c_str());
    return bg;
}

std::wstring SkinV3::ImageName(XMLElement *meterXMLElement) {
    const char *imgName = meterXMLElement->Attribute("image");
    if (imgName == NULL) {
        return NULL;
    }
    return _skinDir + L"\\" + StringUtils::Widen(imgName);
}

HICON SkinV3::Icon(XMLElement *elem) {
    XMLElement *icon = elem->FirstChildElement("icon");
    if (icon == nullptr) {
        return nullptr;
    }

    const char *file = icon->Attribute("file");
    if (file == nullptr) {
        return nullptr;
    }

    return Skin::ReadIcon(_skinDir + L"\\" + StringUtils::Widen(file));
}

std::vector<HICON> SkinV3::Iconset(XMLElement *elem) {
    XMLElement *set = elem->FirstChildElement("iconset");
    if (set == NULL) {
        return std::vector<HICON>();
    }

    const char *loc = set->Attribute("location");
    if (loc == NULL) {
        CLOG(L"Unknown iconset location");
        return std::vector<HICON>();
    }

    std::wstring iconDir = _skinDir + L"\\" + StringUtils::Widen(loc) + L"\\";
    return Skin::ReadIconDirectory(iconDir);
}

SoundPlayer *SkinV3::Sound(XMLElement *elem) {
    XMLElement *sound = elem->FirstChildElement("sound");
    if (sound == NULL) {
        return NULL;
    }

    const char *fileName = sound->Attribute("file");
    if (fileName == NULL) {
        CLOG(L"OSD has <sound> tag but no file specified");
        return NULL;
    }

    std::wstring wFileName = _skinDir + L"\\" + StringUtils::Widen(fileName);
    if (PathFileExists(wFileName.c_str()) == FALSE) {
        Error::ErrorMessage(Error::SKINERR_NOTFOUND, wFileName);
    }

    SoundPlayer *player = new SoundPlayer(wFileName);
    if (player->Ready() == false) {
        CLOG(L"Failed to initialize sound player");
        delete player;
        return NULL;
    }

    return player;
}

std::vector<Meter *> SkinV3::Meters(XMLElement *parentElement) {
    std::vector<Meter*> meters;

    XMLElement *meter = parentElement->FirstChildElement("meter");
    for (; meter != NULL; meter = meter->NextSiblingElement("meter")) {
        Meter *m = LoadMeter(meter);
        if (m != NULL) {
            meters.push_back(m);
        }
    }

    return meters;
}

Meter *SkinV3::LoadMeter(XMLElement *meterXMLElement) {
    const char *meterType = meterXMLElement->Attribute("type");
    if (meterType == NULL) {
        /* If we dont' know the meter type, we can't proceed. */
        CLOG(L"Unknown meter type!");
        return NULL;
    }

    std::string type(meterType);
    std::transform(type.begin(), type.end(), type.begin(), ::tolower);

    /* IntAttribute() returns 0 on error, the default position. */
    int x = meterXMLElement->IntAttribute("x");
    int y = meterXMLElement->IntAttribute("y");

    int units = 10;
    meterXMLElement->QueryIntAttribute("units", &units);

    if (units > 100) {
        units = 100;
    }
    if (units < 1) {
        units = 1;
    }

    /* Several of the meters can be inverted (flipped), so we check for this
     * flag now. */
    bool inverted = false;
    meterXMLElement->QueryBoolAttribute("inverted", &inverted);

    /* Check for meter background image. 'text' is the only meter
     * that does not require an image. */
    std::wstring img;
    if (type != "text") {
        img = ImageName(meterXMLElement);
        if (PathFileExists(img.c_str()) == FALSE) {
            Error::ErrorMessageDie(Error::SKINERR_NOTFOUND, img);
        }
    }

    Meter *m = NULL;
    if (type == "bitstrip") {
        m = new Bitstrip(img, x, y, units);
    } else if (type == "horizontalbar") {
        m = new HorizontalBar(img, x, y, units);
    } else if (type == "horizontalendcap") {
        m = new HorizontalEndcap(img, x, y, units);
    } else if (type == "horizontaltile") {
        m = new HorizontalTile(img, x, y, units, inverted);
    } else if (type == "image") {
        m = new StaticImage(img, x, y);
    } else if (type == "numberstrip") {
        Gdiplus::StringAlignment align = Alignment(meterXMLElement);
        m = new NumberStrip(img, x, y, units, align);
    } else if (type == "text") {
        int width = meterXMLElement->IntAttribute("width");
        int height = meterXMLElement->IntAttribute("height");

        Gdiplus::Font *font = Font(meterXMLElement);
        Gdiplus::StringAlignment align = Alignment(meterXMLElement);

        const char *fontColor = meterXMLElement->Attribute("color");
        std::wstring color(L"FFFFFF");
        if (fontColor != NULL) {
            color = std::wstring(StringUtils::Widen(fontColor));
        }
        int itrans = 255;
        meterXMLElement->QueryIntAttribute("transparency", &itrans);
        byte transparency = (byte) itrans;

        const char* stringFormat = meterXMLElement->Attribute("format");
        std::wstring format(L"[[PERC]]%");
        if (stringFormat != NULL) {
            format = std::wstring(StringUtils::Widen(stringFormat));
        }

        m = new Text(x, y, width, height, font, align, color,
            transparency, format);

        delete font;

    } else if (type == "verticalbar") {
        m = new VerticalBar(img, x, y, units, inverted);
    } else {
        CLOG(L"Unknown meter type: %s", StringUtils::Widen(type).c_str());
        return NULL;
    }

    CLOG(L"Created meter [%s]:\n%s",
        StringUtils::Widen(type).c_str(), m->ToString().c_str());

    return m;
}

Gdiplus::Font *SkinV3::Font(XMLElement *meterXMLElement) {
    const char *fontName = meterXMLElement->Attribute("font");
    std::wstring name(L"Arial");
    if (fontName != NULL) {
        name = std::wstring(StringUtils::Widen(fontName));
    }

    float size = 10;
    meterXMLElement->QueryFloatAttribute("size", &size);

    int styleFlags = 0;
    const char *fontStyle = meterXMLElement->Attribute("style");
    if (fontStyle != NULL) {
        std::wstring style(StringUtils::Widen(fontStyle));
        std::transform(style.begin(), style.end(), style.begin(), ::tolower);

        std::wstring::size_type sz;
        sz = style.find(L"bold", 0);
        if (sz != std::wstring::npos) {
            styleFlags |= Gdiplus::FontStyleBold;
        }

        sz = style.find(L"italic", 0);
        if (sz != std::wstring::npos) {
            styleFlags |= Gdiplus::FontStyleItalic;
        }

        sz = style.find(L"underline", 0);
        if (sz != std::wstring::npos) {
            styleFlags |= Gdiplus::FontStyleUnderline;
        }

        sz = style.find(L"strike", 0);
        if (sz != std::wstring::npos) {
            styleFlags |= Gdiplus::FontStyleStrikeout;
        }
    }

    Gdiplus::Font *font = new Gdiplus::Font(name.c_str(), size, styleFlags);
    return font;
}

Gdiplus::StringAlignment
SkinV3::Alignment(XMLElement *meterXMLElement) {
    const char *align = meterXMLElement->Attribute("align");
    if (align == NULL) {
        align = "left";
    }

    std::string alignStr(align);
    std::transform(alignStr.begin(), alignStr.end(),
        alignStr.begin(), ::tolower);

    Gdiplus::StringAlignment alignment = Gdiplus::StringAlignmentNear;
    if (alignStr == "right") {
        alignment = Gdiplus::StringAlignmentFar;
    } else if (alignStr == "center") {
        alignment = Gdiplus::StringAlignmentCenter;
    }

    return alignment;
}

SliderKnob *SkinV3::Knob(XMLElement *elem) {
    XMLElement *slider = elem->FirstChildElement("slider");
    if (slider == nullptr) {
        Error::ErrorMessageDie(Error::SKINERR_MISSING_XML, L"<slider>");
        return nullptr;
    }

    std::wstring img = ImageName(slider);
    if (PathFileExists(img.c_str()) == FALSE) {
        Error::ErrorMessageDie(Error::SKINERR_NOTFOUND, img);
    }

    const char *type = slider->Attribute("type");
    if (type == nullptr) {
        type = "vertical";
    }

    std::string typeStr(type);
    std::transform(typeStr.begin(), typeStr.end(),
        typeStr.begin(), ::tolower);

    if (typeStr != "vertical" && typeStr != "horizontal") {
        Error::ErrorMessageDie(
            Error::SKINERR_INVALID_SLIDERTYPE,
            StringUtils::Widen(typeStr));
    }

    bool vertical = true;
    if (typeStr == "horizontal") {
        vertical = false;
    }

    int x = slider->IntAttribute("x");
    int y = slider->IntAttribute("y");
    int w = slider->IntAttribute("width");
    int h = slider->IntAttribute("height");

    SliderKnob *knob = new SliderKnob(img, x, y, w, h, vertical);
    return knob;
}