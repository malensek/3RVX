#include "Skin.h"

#include <algorithm>
#include <CommCtrl.h>
#include <memory>
#include <vector>
#include <Shlwapi.h>

#include "CommCtl.h"
#include "Error.h"
#include "MeterWnd/Meters/MeterTypes.h"
#include "StringUtils.h"
#include "Slider/SliderKnob.h"
#include "SoundPlayer.h"

Skin::Skin(std::wstring skinXML) :
SkinInfo(skinXML) {
    volumeBackground = OSDBgImg("volume");
    volumeMask = OSDMask("volume");
    volumeMeters = OSDMeters("volume");
    volumeIconset = Iconset("volume");
    volumeSound = OSDSound("volume");

    muteBackground = OSDBgImg("mute");
    muteMask = OSDMask("mute");

    ejectBackground = OSDBgImg("eject");
    ejectMask = OSDMask("eject");

    volumeSliderBackground = SliderBgImg("volume");
    volumeSliderMask = SliderMask("volume");
    volumeSliderMeters = SliderMeters("volume");
    volumeSliderKnob = Knob("volume");
}

Skin::~Skin() {
    delete volumeBackground;
    delete volumeMask;
    for (Meter *meter : volumeMeters) {
        delete meter;
    }
    for (HICON icon : volumeIconset) {
        DestroyIcon(icon);
    }

    delete muteBackground;
    delete muteMask;

    delete ejectBackground;
    delete ejectMask;

    delete volumeSliderBackground;
    delete volumeSliderMask;
    for (Meter *meter : volumeSliderMeters) {
        delete meter;
    }
    delete volumeSliderKnob;
}

int Skin::DefaultVolumeUnits() {
    return DefaultOSDUnits("volume");
}

int Skin::DefaultOSDUnits(char *osdName) {
    int defaultUnits = SKIN_DEFAULT_UNITS;

    tinyxml2::XMLElement *osdElem = OSDXMLElement(osdName);
    if (osdElem == NULL) {
        return defaultUnits;
    }

    osdElem->QueryIntAttribute("defaultUnits", &defaultUnits);
    return defaultUnits;
}

bool Skin::HasOSD(char *osdName) {
    return (OSDXMLElement(osdName) != NULL);
}

Gdiplus::Bitmap *Skin::OSDBgImg(char *osdName) {
    tinyxml2::XMLElement *osd = OSDXMLElement(osdName);
    if (osd == NULL) {
        Error::ErrorMessageDie(SKINERR_INVALID_OSD,
            StringUtils::Widen(osdName));
    }
    return Image(osd, "background");
}

Gdiplus::Bitmap *Skin::OSDMask(char *osdName) {
    tinyxml2::XMLElement *osd = OSDXMLElement(osdName);
    if (osd == NULL) {
        return NULL;
    }

    return Image(osd, "mask");
}

Gdiplus::Bitmap *Skin::SliderBgImg(char *sliderName) {
    tinyxml2::XMLElement *sliderElement = SliderXMLElement(sliderName);
    if (sliderElement == NULL) {
        Error::ErrorMessageDie(
            SKINERR_INVALID_BG, StringUtils::Widen(sliderName));
    }
    return Image(sliderElement, "background");
}

Gdiplus::Bitmap *Skin::SliderMask(char *sliderName) {
    tinyxml2::XMLElement *slider = SliderXMLElement(sliderName);
    if (slider == NULL) {
        return NULL;
    }

    return Image(slider, "mask");
}

Gdiplus::Bitmap *Skin::Image(tinyxml2::XMLElement *element, char *attName) {

    if (element == NULL) {
        CLOG(L"XML Element is NULL!");
        return NULL;
    }

    const char *imgFile = element->Attribute(attName);
    if (imgFile == NULL) {
        std::wstring aName = StringUtils::Widen(attName);
        CLOG(L"Could not find XML attribute: %s", aName.c_str());
        return NULL;
    }

    std::wstring wImgFile = _skinDir + L"\\" + StringUtils::Widen(imgFile);
    if (PathFileExists(wImgFile.c_str()) == FALSE) {
        Error::ErrorMessageDie(SKINERR_NOTFOUND, wImgFile);
    }

    Gdiplus::Bitmap *bg = Gdiplus::Bitmap::FromFile(wImgFile.c_str());
    return bg;
}

std::vector<HICON> Skin::Iconset(char *osdName) {
    std::vector<HICON> iconset;

    tinyxml2::XMLElement *osd = OSDXMLElement(osdName);
    if (osd == NULL) {
        Error::ErrorMessageDie(SKINERR_INVALID_OSD);
    }

    tinyxml2::XMLElement *set = osd->FirstChildElement("iconset");
    if (set == NULL) {
        return iconset;
    }

    const char *loc = set->Attribute("location");
    if (loc == NULL) {
        CLOG(L"Unknown iconset location");
        return iconset;
    }

    std::wstring iconDir = _skinDir + L"\\" + StringUtils::Widen(loc) + L"\\";
    CLOG(L"Reading icons from: %s", iconDir.c_str());

    HANDLE hFind;
    WIN32_FIND_DATA fd = {};
    hFind = FindFirstFile((iconDir + L"*").c_str(), &fd);
    if (hFind == INVALID_HANDLE_VALUE) {
        CLOG(L"Could not read icon directory");
        return iconset;
    }

    do {
        std::wstring iconName(fd.cFileName);
        if (iconName == L"." || iconName == L"..") {
            continue;
        }

        std::wstring iconPath = iconDir + iconName;
        std::wstring ext = StringUtils::FileExtension(iconName);
        if (ext != L"ico") {
            QCLOG(L"Ignoring non-ico file: %s", iconPath.c_str());
            continue;
        }

        HICON icon = NULL;
        HRESULT hr = LoadIconMetric(
            NULL,
            iconPath.c_str(),
            LIM_SMALL,
            &icon);

        if (icon != NULL) {
            QCLOG(L"%s", iconPath.c_str());
            iconset.push_back(icon);
        }

    } while (FindNextFile(hFind, &fd));
    FindClose(hFind);
 
    return iconset;
}

SoundPlayer *Skin::OSDSound(char *osdName) {
    tinyxml2::XMLElement *osd = OSDXMLElement(osdName);
    if (osd == NULL) {
        Error::ErrorMessageDie(SKINERR_INVALID_OSD,
            StringUtils::Widen(osdName));
    }

    tinyxml2::XMLElement *sound = osd->FirstChildElement("sound");
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
        Error::ErrorMessage(SKINERR_NOTFOUND, wFileName);
    }

    SoundPlayer *player = NULL;
    try {
        player = new SoundPlayer(wFileName);
    } catch (std::runtime_error e) {
        Error::ErrorMessage(SKINERR_READERR, wFileName);
    }

    return player;
}

std::list<Meter *> Skin::OSDMeters(char *osdName) {
    std::list<Meter*> meters;

    tinyxml2::XMLElement *osd = OSDXMLElement(osdName);
    tinyxml2::XMLElement *meter = osd->FirstChildElement("meter");
    for (; meter != NULL; meter = meter->NextSiblingElement("meter")) {
        Meter *m = LoadMeter(meter);
        if (m != NULL) {
            meters.push_back(m);
        }
    }

    return meters;
}

std::list<Meter *> Skin::SliderMeters(char *osdName) {
    std::list<Meter *> meters;

    tinyxml2::XMLElement *osd = SliderXMLElement(osdName);
    tinyxml2::XMLElement *meter = osd->FirstChildElement("meter");
    for (; meter != NULL; meter = meter->NextSiblingElement("meter")) {
        Meter *m = LoadMeter(meter);
        if (m != NULL) {
            meters.push_back(m);
        }
    }

    return meters;
}

Meter *Skin::LoadMeter(tinyxml2::XMLElement *meterXMLElement) {
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
            Error::ErrorMessageDie(SKINERR_NOTFOUND, img);
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

Gdiplus::Font *Skin::Font(tinyxml2::XMLElement *meterXMLElement) {
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
Skin::Alignment(tinyxml2::XMLElement *meterXMLElement) {
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

SliderKnob *Skin::Knob(char *sliderName) {
    tinyxml2::XMLElement *controller = SliderXMLElement(sliderName);
    if (controller == NULL) {
        Error::ErrorMessageDie(
            SKINERR_INVALID_SLIDER, StringUtils::Widen(sliderName));
    }

    tinyxml2::XMLElement *slider = controller->FirstChildElement("slider");
    if (slider == NULL) {
        Error::ErrorMessageDie(SKINERR_MISSING_XML, L"<slider>");
    }

    std::wstring img = ImageName(slider);
    if (PathFileExists(img.c_str()) == FALSE) {
        Error::ErrorMessageDie(SKINERR_NOTFOUND, img);
    }

    const char *type = slider->Attribute("type");
    if (type == NULL) {
        type = "vertical";
    }

    std::string typeStr(type);
    std::transform(typeStr.begin(), typeStr.end(),
        typeStr.begin(), ::tolower);

    if (typeStr != "vertical" && typeStr != "horizontal") {
        Error::ErrorMessageDie(
            SKINERR_INVALID_SLIDERTYPE,
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

std::wstring Skin::ImageName(tinyxml2::XMLElement *meterXMLElement) {
    const char *imgName = meterXMLElement->Attribute("image");
    if (imgName == NULL) {
        return NULL;
    }
    return _skinDir + L"\\" + StringUtils::Widen(imgName);
}

tinyxml2::XMLElement *Skin::OSDXMLElement(char *osdName) {
    tinyxml2::XMLHandle xmlHandle(_root);
    tinyxml2::XMLElement *osd = xmlHandle
        .FirstChildElement("osds")
        .FirstChildElement(osdName)
        .ToElement();
    return osd;
}

tinyxml2::XMLElement *Skin::SliderXMLElement(char *sliderName) {
    tinyxml2::XMLHandle xmlHandle(_root);
    tinyxml2::XMLElement *controller = xmlHandle
        .FirstChildElement("sliders")
        .FirstChildElement(sliderName)
        .ToElement();
    return controller;
}