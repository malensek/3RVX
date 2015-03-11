#pragma once

#include <vector>

#include "FadeOut.h"

class AnimationTypes {
public:
    enum HideAnimation {
        None,
        Fade
    };
    static std::vector<std::wstring> HideAnimationNames;
};