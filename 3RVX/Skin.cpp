#include "Skin.h"

#include <algorithm>
#include <vector>

#include "MeterWnd/Meters/MeterTypes.h"
#include "StringUtils.h"

Gdiplus::Bitmap *Skin::OSDBgImg(char *osdName) {
    tinyxml2::XMLElement *osd = OSDXMLElement(osdName);
    return BgImg(osd);
}

Gdiplus::Bitmap *Skin::ControllerBgImg(char *controllerName) {
    tinyxml2::XMLHandle xmlHandle(_root);
    tinyxml2::XMLElement *controller = xmlHandle
        .FirstChildElement("controllers")
        .FirstChildElement(controllerName)
        .ToElement();
    return BgImg(controller);
}

Gdiplus::Bitmap *Skin::BgImg(tinyxml2::XMLElement *element) {
    const char *bgFile = element->Attribute("background");
    if (bgFile == NULL) {
        CLOG(L"Could not load background image.");
        return NULL;
    }
    std::wstring wBgFile = _skinDir + L"\\" + StringUtils::Widen(bgFile);
    Gdiplus::Bitmap *bg = Gdiplus::Bitmap::FromFile(wBgFile.c_str());
    return bg;
}

std::vector<HICON> Skin::Iconset(char *osdName) {
    std::vector<HICON> iconset;

    tinyxml2::XMLElement *osd = OSDXMLElement(osdName);
    const char *loc = osd->FirstChildElement("iconset")->Attribute("location");
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

        CLOG(L"Loading icon: %s", (iconDir + iconName).c_str());
        Gdiplus::Bitmap *iconBmp = Gdiplus::Bitmap::FromFile(
            (iconDir + iconName).c_str());
        if (iconBmp == NULL) {
            CLOG(L"Failed to load icon!");
        }

        HICON icon;
        if (iconBmp->GetHICON(&icon) == Gdiplus::Status::Ok) {
            iconset.push_back(icon);
        }

        delete iconBmp;

    } while (FindNextFile(hFind, &fd));
    FindClose(hFind);
 
    return iconset;
}


std::list<Meter *> Skin::Meters(char *osdName) {
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
        if (FileExists(img) == false) {
            CLOG(L"Could not find meter bitmap: %s", img.c_str());
            return NULL;
        }
    }

    Meter *m = NULL;
    if (type == "bitstrip") {
        m = new Bitstrip(img, x, y, units);
    } else if (type == "horizontalendcap") {
        m = new HorizontalEndcap(img, x, y, units);
    } else if (type == "horizontalbar") {
        m = new HorizontalBar(img, x, y, units);
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

bool Skin::FileExists(std::wstring fileName) {
    DWORD attr = GetFileAttributes(fileName.c_str());
    return (attr != INVALID_FILE_ATTRIBUTES
        && !(attr & FILE_ATTRIBUTE_DIRECTORY)
        && GetLastError() != ERROR_FILE_NOT_FOUND);
}