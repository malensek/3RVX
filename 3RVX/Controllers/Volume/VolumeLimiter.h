#pragma once

#include "VolumeTransformation.h"

/// <summary>
/// Scales volume based on a maximum value; for instance, if a limit of 0.8 is
/// given, an input volume of 0.8 outputs 1.0, and an input of 0.4 would output
/// 0.5 (50% of .8).
/// </summary>
class VolumeLimiter : public VolumeTransformation {
public:
    /// <summary>
    /// Creates a new VolumeLimiter, where the maximum volume level is the value
    /// provided.
    /// </summary>
    /// <param name="limit">New maximum volume level</param>
    VolumeLimiter(float limit) :
    _limit(limit) {

    }

    virtual float Apply(float vol) {
        return vol * _limit;
    }

    virtual float Revert(float vol) {
        float newVal = vol / _limit;
        if (newVal > 1.0f) {
            newVal = 1.0f;
        }

        return newVal;
    }

private:
    float _limit;
};