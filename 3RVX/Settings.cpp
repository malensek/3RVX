#include "Settings.h"

#include "Hotkeys.h"

std::wstring Settings::SkinName() {
    const char* skinName = _root->FirstChildElement("skin")->GetText();
    if (skinName == NULL) {
        return L"Default";
    } else {
        return StringUtils::Widen(skinName);
    }
}

std::unordered_map<int, int> Settings::Hotkeys() {
    std::unordered_map<int, int> keyMappings;

    tinyxml2::XMLElement *hotkeys = _root->FirstChildElement("hotkeys");
    tinyxml2::XMLElement *hotkey = hotkeys->FirstChildElement("hotkey");

    for (; hotkey != NULL; hotkey = hotkey->NextSiblingElement()) {
        int action = -1;
        hotkey->QueryIntAttribute("action", &action);
        if (action == -1) {
            CLOG(L"No action provided for hotkey; skipping");
            continue;
        }

        int combination = -1;
        hotkey->QueryIntAttribute("combination", &combination);
        if (combination == -1) {
            CLOG(L"No key combination provided for hotkey; skipping");
            continue;
        }
        
        /* Whew, we made it! */
        CLOG(L"Adding hotkey mapping: %d -> %d", combination, action);
        keyMappings[combination] = action;
    }

    return keyMappings;
}
