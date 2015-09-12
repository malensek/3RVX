// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#include "SkinV2.h"

#include <algorithm>
#include <iomanip>
#include <sstream>

#include "../Logger.h"
#include "../MeterWnd/Meters/MeterTypes.h"
#include "../StringUtils.h"
#include "MeterComponent.h"
#include "OSDComponent.h"
#include "SliderComponent.h"

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
    const char *type = nullptr;
    tinyxml2::XMLElement *meterOrientation
        = SubElement("osd", "meterOrientation");
    if (meterOrientation) {
        type = meterOrientation->GetText();
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
    std::string meterType(type);
    if (meterType == "vertical") {
        volume->meters.push_back(
            new VerticalBar(meterImg, x, y, units));
    } else if (meterType == "bitstrip") {
        /* For some reason, the number of units in v2 for bit strips was
         * (meterMax + 1), so we update the number of units here. */
        volume->meters.push_back(
            new Bitstrip(meterImg, x, y, units + 1));
    } else {
        /* Horizontal meter is the default */
        volume->meters.push_back(
            new HorizontalBar(meterImg, x, y, units));
    }

    tinyxml2::XMLElement *drawText = SubElement("osd", "drawPercentage");
    if (drawText) {
        bool textEnabled = false;
        drawText->QueryBoolText(&textEnabled);
        if (textEnabled) {
            Text *t = CreateText(volume->background);
            if (t) {
                volume->meters.push_back(t);
            }
        }
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
    return Skin::ReadIconDirectory(iconDir);
}

SliderComponent *SkinV2::VolumeSlider() {
    SliderComponent *slider = new SliderComponent;
    slider->background = LoadImg(_skinDir + L"\\Control\\back.png");
    slider->mask = LoadImg(_skinDir + L"\\Control\\glassMask.png");
    slider->knob = CreateKnob();

    return slider;
}

Text *SkinV2::CreateText(Gdiplus::Bitmap *baseBitmap) {
    tinyxml2::XMLElement *fontTag = SubElement("osd", "font");
    if (fontTag == nullptr) {
        return nullptr;
    }

    const char *fontName = "Arial";
    tinyxml2::XMLElement *nameTag = fontTag->FirstChildElement("fontName");
    if (nameTag) {
        fontName = nameTag->GetText();
        if (fontName == nullptr) {
            fontName = "Arial";
        }
    }

    int styleFlags = 0;
    tinyxml2::XMLElement *styleTag = fontTag->FirstChildElement("fontStyle");
    if (styleTag) {
        std::string style(styleTag->GetText());
        std::transform(style.begin(), style.end(), style.begin(), ::tolower);
        /* Only one font style can be applied (ie no bold-italic, etc) */
        if (style == "bold") {
            styleFlags = Gdiplus::FontStyleBold;
        } else if (style == "italic") {
            styleFlags = Gdiplus::FontStyleItalic;
        }
    }

    std::wstring hexColor = L"000000";
    int transparency = 255;
    tinyxml2::XMLElement *colorTag = fontTag->FirstChildElement("fontColor");
    if (colorTag) {
        const char *color = colorTag->GetText();
        if (color == nullptr) {
            color = "255,0,0,0";
        }

        std::string cc;
        std::istringstream iss(color);
        std::stringstream ss;
        std::getline(iss, cc, ',');
        try {
            transparency = std::stoi(cc);
        } catch (std::exception) { }
        ss << std::hex << std::setfill('0') << std::setw(2);
        while (std::getline(iss, cc, ',')) {
            int byte = 0;
            try {
                byte = std::stoi(cc);
            } catch (std::exception) { }
            ss << byte << std::setw(2);
        }
        hexColor = StringUtils::Widen(ss.str());
    }

    float size = 10.0f;
    tinyxml2::XMLElement *sizeTag = fontTag->FirstChildElement("fontSize");
    if (sizeTag) {
        sizeTag->QueryFloatText(&size);
    }

    tinyxml2::XMLHandle fontHandle(fontTag);
    int x = 0;
    tinyxml2::XMLElement *xt = fontHandle
        .FirstChildElement("fontLocation")
        .FirstChildElement("X")
        .ToElement();
    if (xt) {
        xt->QueryIntText(&x);
    }

    int y = 0;
    tinyxml2::XMLElement *yt = fontHandle
        .FirstChildElement("fontLocation")
        .FirstChildElement("Y")
        .ToElement();
    if (yt) {
        yt->QueryIntText(&y);
    }

    /* v2 didn't support text alignment, so we assume left alignment and then
     * expand the text's bounding box to stretch to the edges of the OSD. */
    Gdiplus::StringAlignment align = Gdiplus::StringAlignmentNear;
    int width = baseBitmap->GetWidth() - x;
    int height = baseBitmap->GetHeight() - y;

    Gdiplus::Font font(StringUtils::Widen(fontName).c_str(), size, styleFlags);
    Text *text = new Text(x, y, width, height,
        &font, align, hexColor, transparency, L"[[PERC]]%");
    return text;
}

SliderKnob *SkinV2::CreateKnob() {
    tinyxml2::XMLElement *controlTag = _root->FirstChildElement("control");
    if (controlTag == nullptr) {
        return nullptr;
    }
    tinyxml2::XMLHandle controlHandle(controlTag);

    bool vertical = true;
    const char *orientation = "vertical";
    tinyxml2::XMLElement *orTag = controlHandle
        .FirstChildElement("orientation")
        .ToElement();
    if (orTag) {
        orientation = orTag->GetText();
    }
    std::string orStr(orientation);
    if (orStr == "horizontal") {
        vertical = false;
    }

    int x = 0;
    tinyxml2::XMLElement *xt = controlHandle
        .FirstChildElement("sliderPosition")
        .FirstChildElement("Location")
        .FirstChildElement("X")
        .ToElement();
    if (xt) {
        xt->QueryIntText(&x);
    }

    int y = 0;
    tinyxml2::XMLElement *yt = controlHandle
        .FirstChildElement("sliderPosition")
        .FirstChildElement("Location")
        .FirstChildElement("Y")
        .ToElement();
    if (yt) {
        yt->QueryIntText(&y);
    }

    int w = 0;
    tinyxml2::XMLElement *wt = controlHandle
        .FirstChildElement("sliderPosition")
        .FirstChildElement("Size")
        .FirstChildElement("Width")
        .ToElement();
    if (wt) {
        wt->QueryIntText(&w);
    }

    int h = 0;
    tinyxml2::XMLElement *ht = controlHandle
        .FirstChildElement("sliderPosition")
        .FirstChildElement("Size")
        .FirstChildElement("Height")
        .ToElement();
    if (ht) {
        ht->QueryIntText(&h);
    }

    SliderKnob *knob = new SliderKnob(
        _skinDir + L"\\Control\\knob.png",
        x, y, w, h,
        vertical);
    return knob;
}
