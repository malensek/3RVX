#include "HorizontalSlider.h"

HorizontalSlider::HorizontalSlider(
    std::wstring bitmapName, int x, int y, int width, int height) :
Meter(bitmapName, x, y, 1), /* units=1 temporarily; calculated in constr. */
_track(x, y, width, height) {
    _units = _track.Width - _rect.Width;
}
