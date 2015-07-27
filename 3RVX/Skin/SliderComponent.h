#pragma once

#include "MeterComponent.h"

class SliderKnob;

struct SliderComponent : public MeterComponent {
    SliderKnob *knob;

    virtual ~SliderComponent() {
        delete knob;
    }
};