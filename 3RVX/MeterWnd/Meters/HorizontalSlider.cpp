#include "HorizontalSlider.h"

HorizontalSlider::HorizontalSlider(
    std::wstring bitmapName, int x, int y, int width, int height) :
Meter(bitmapName, x, y, 1), /* units=1 temporarily; calculated in constr. */
_track(x, y, width, height) {
    _units = _track.Width - _rect.Width;
}

int HorizontalSlider::TrackX() const {
    return _track.X;
}

int HorizontalSlider::TrackY() const {
    return _track.Y;
}

int HorizontalSlider::TrackWidth() const {
    return _track.Width;
}

int HorizontalSlider::TrackHeight() const {
    return _track.Height;
}
