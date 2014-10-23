#include "Skin.h"

#include <algorithm>
#include "MeterWnd/Meters/MeterTypes.h"

Gdiplus::Bitmap *Skin::OSDBgImg(char *osdName) {
    tinyxml2::XMLElement *osd = OSDXMLElement(osdName);
    const char *bgFile = osd->Attribute("background");
    std::wstring wBgFile = _skinDir + L"\\" + Widen(bgFile);
    Gdiplus::Bitmap *bg = Gdiplus::Bitmap::FromFile(wBgFile.c_str());
    return bg;
}

void Skin::Meters(char *osdName) {
    tinyxml2::XMLElement *osd = OSDXMLElement(osdName);
    tinyxml2::XMLElement *meter = osd->FirstChildElement("meter");
    LoadMeter(meter);
    const char *img = meter->Attribute("image");
    printf("%s\n", img);
}

Meter *Skin::LoadMeter(tinyxml2::XMLElement *meterXMLElement) {
    std::string type(meterXMLElement->Attribute("type"));
    std::transform(type.begin(), type.end(), type.begin(), ::tolower);
    int x = meterXMLElement->IntAttribute("x");
    int y = meterXMLElement->IntAttribute("y");
    int units = meterXMLElement->IntAttribute("units");

    Meter *m;
    if (type == "horizontalendcap") {
        m = new HorizontalEndcap(ImageName(meterXMLElement), x, y, units);
    } else if (type == "horizontalbar") {
        m = new HorizontalEndcap(ImageName(meterXMLElement), x, y, units);
    } else if (type == "horizontaltile") {
        m = new HorizontalTile(ImageName(meterXMLElement), x, y, units);
    }
}

std::wstring Skin::ImageName(tinyxml2::XMLElement *meterXMLElement) {
    return Widen(meterXMLElement->Attribute("image"));
}

tinyxml2::XMLElement *Skin::OSDXMLElement(char *osdName) {
    tinyxml2::XMLElement *osd = _xml.GetDocument()
        ->FirstChildElement("skin")
        ->FirstChildElement("osds")
        ->FirstChildElement(osdName);
    return osd;
}

bool Skin::FileExists(std::wstring fileName) {
    DWORD attr = GetFileAttributes(fileName.c_str());
    return (attr != INVALID_FILE_ATTRIBUTES
        && !(attr & FILE_ATTRIBUTE_DIRECTORY)
        && GetLastError() != ERROR_FILE_NOT_FOUND);
}

std::wstring Skin::Widen(const char *str) {
    std::string sstr(str);
    return Widen(sstr);
}

std::wstring Skin::Widen(std::string &str) {
    int size = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int) str.size(),
        NULL, 0);
    std::wstring buf(size, 0);
    MultiByteToWideChar(CP_UTF8, 0, &str[0], (int) str.size(), &buf[0], size);
    return buf;
}

std::string Skin::Narrow(std::wstring &str) {
    int size = WideCharToMultiByte(CP_UTF8, 0, &str[0], (int) str.size(),
        NULL, 0, NULL, NULL);
    std::string buf(size, 0);
    WideCharToMultiByte(CP_UTF8, 0, &str[0], (int) str.size(),
        &buf[0], size, NULL, NULL);
    return buf;
}