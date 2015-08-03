#pragma once

#include "MeterComponent.h"

#include "../Slider/SliderKnob.h"

struct SliderComponent : public MeterComponent {
    SliderKnob *knob;

    virtual ~SliderComponent() {
        delete knob;
    }
};