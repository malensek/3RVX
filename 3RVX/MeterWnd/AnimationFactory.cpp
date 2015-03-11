#include "AnimationFactory.h"

Animation *AnimationFactory::Create(AnimationTypes::HideAnimation anim) {
    Animation *animation;

    switch (anim) {
    case AnimationTypes::Fade:
        animation = new FadeOut();
        break;

    case AnimationTypes::None:
    default:
        animation = NULL;
    }

    return animation;
}