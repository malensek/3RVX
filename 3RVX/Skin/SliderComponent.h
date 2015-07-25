#pragma once

#include "SkinComponent.h"

class SliderKnob;

struct OSDComponent : public SkinComponent {
    SliderKnob *knob;
};