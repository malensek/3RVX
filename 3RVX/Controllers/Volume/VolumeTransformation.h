#pragma once

class VolumeTransformation {
public:
    /// <summary>
    /// Transforms a given volume level to a new ("virtual") level based on a
    /// formula or set of rules (e.g., a volume curve transformation).
    /// </summary>
    virtual float Apply(float vol) = 0;

    /// <summary>
    /// Given a transformed ("virtual") volume value, this function reverts it 
    /// back to its original value (assuming the given value was produced
    /// by the ToVirtual() function).
    /// </summary>
    virtual float Revert(float vol) = 0;

    /// <summary>
    /// Applies several transformations supplied as a vector to the initial
    /// value provided. The transformations are applied in order of appearance 
    /// in the vector.
    /// </summary>
    static float ApplyTransformations(
            std::vector<VolumeTransformation *> &transforms, float initialVal) {
        float newVal = initialVal;
        for (VolumeTransformation *trans : transforms) {
            newVal = trans->Apply(newVal);
        }
        return newVal;
    }

    /// <summary>
    /// Reverts several transformations, starting with a transformed ("virtual")
    /// value. This method reverts the transformations in reverse order.
    /// </summary>
    static float RevertTransformations(
            std::vector<VolumeTransformation *> &transforms, float virtVal) {
        float newVal = virtVal;
        for (auto it = transforms.rbegin(); it != transforms.rend(); ++it) {
            newVal = (*it)->Revert(newVal);
        }
        return newVal;
    }
};