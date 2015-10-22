#pragma once

class VolumeTransformation {
public:
    /// <summary>
    /// Transforms a given volume level to a new ("virtual") level based on a
    /// formula or set of rules (e.g., a volume curve transformation).
    /// </summary>
    virtual float ToVirtual(float vol) = 0;

    /// <summary>
    /// Given a transformed ("virtual") volume value, this function reverts it 
    /// back to its original value (assuming the given value was produced
    /// by the ToVirtual() function).
    /// </summary>
    virtual float FromVirtual(float vol) = 0;
};