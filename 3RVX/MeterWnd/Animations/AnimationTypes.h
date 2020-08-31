// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#pragma once

#include <vector>
#include <string>

#include "FadeOut.h"

class AnimationTypes {
public:
    enum HideAnimation {
        None,
        Fade
    };
    static std::vector<std::wstring> HideAnimationNames;
};