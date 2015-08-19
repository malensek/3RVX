// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#include "AnimationFactory.h"

Animation *AnimationFactory::Create(
        AnimationTypes::HideAnimation anim, int speed) {

    Animation *animation;

    switch (anim) {
    case AnimationTypes::Fade:
        animation = new FadeOut(speed);
        break;

    case AnimationTypes::None:
    default:
        animation = NULL;
    }

    return animation;
}