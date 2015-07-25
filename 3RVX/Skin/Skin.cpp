#include "Skin.h"

Skin::Skin(std::wstring skinXML) :
SkinInfo(skinXML) {

}

XMLElement *Skin::SubElement(char *parent, char *child) {
    tinyxml2::XMLHandle xmlHandle(_root);
    tinyxml2::XMLElement *elem = xmlHandle
        .FirstChildElement(parent)
        .FirstChildElement(child)
        .ToElement();
    return elem;
}