// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#pragma once

#include "MeterComponent.h"

#include "../MeterWnd/Meters/SliderKnob.h"

struct SliderComponent : public MeterComponent {
    SliderKnob *knob;

    virtual ~SliderComponent() {
        delete knob;
    }
};