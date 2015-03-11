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