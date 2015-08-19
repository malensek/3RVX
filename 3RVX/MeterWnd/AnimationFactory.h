// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#pragma once

#include "Animations\AnimationTypes.h"
class Animation;

class AnimationFactory {
public:
    static Animation *Create(AnimationTypes::HideAnimation anim, int speed);
};