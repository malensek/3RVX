#pragma once

#include <math.h>

#include "VolumeTransformation.h"

/// <summary>
/// Transforms volume inputs to adjust for the system/hardware audio taper.
/// </summary>
class CurveTransformer : public VolumeTransformation {
public:
    /// <summary>
    /// Creates a new CurveTransformer with the given scale factor. The scale
    /// determines the exponent used when reshaping the volume curve:
    /// base = 2^scale
    /// </summary>
    CurveTransformer(float scale) {
        base = pow(2.0f, scale);
        logBase = log(base);
    }

    virtual float Apply(float vol) {
        return (pow(base, vol) - 1) / (base - 1);
    }

    virtual float Revert(float vol) {
        return log(vol * (base - 1) + 1) / logBase;
    }

private:
    float base;
    float logBase;
};