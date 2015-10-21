#pragma once

#include <math.h>

#include "VolumeTransformation.h"

class CurveTransformer : public VolumeTransformation {
public:

    CurveTransformer(float exp) {
        base = pow(2.0f, exp);
    }

    virtual float Transform(float vol) {
        return (pow(base, vol) - 1) / (base - 1);
    }

private:
    float base;
};