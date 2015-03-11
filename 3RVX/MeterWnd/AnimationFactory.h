#pragma once

#include "Animations\AnimationTypes.h"
class Animation;

class AnimationFactory {
public:
    static Animation *Create(AnimationTypes::HideAnimation anim);
};