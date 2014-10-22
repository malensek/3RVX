#include "Skin.h"

Gdiplus::Bitmap *Skin::BackgroundImage(char *osdName) {
    tinyxml2::XMLElement *osd = OSDElement(osdName);
    const char *bgFile = osd->Attribute("background");
    std::wstring wBgFile = _skinDir + L"\\" + Widen(bgFile);
    Gdiplus::Bitmap *bg = Gdiplus::Bitmap::FromFile(wBgFile.c_str());
    CLOG(L"%d", bg->GetLastStatus());
    return bg;
}

void Skin::Meters(char *osdName) {
    tinyxml2::XMLElement *osd = OSDElement(osdName);
    osd->Attribute("background");
    std::list<Meter> l();
}

tinyxml2::XMLElement *Skin::OSDElement(char *osdName) {
    tinyxml2::XMLElement *osd = _xml.GetDocument()
        ->FirstChildElement("skin")
        ->FirstChildElement("osds")
        ->FirstChildElement(osdName);
    return osd;
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