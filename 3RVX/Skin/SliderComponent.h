#pragma once

#include "SkinComponent.h"

class SliderKnob;

struct SliderComponent : public SkinComponent {
    SliderKnob *knob;
};