#include "SliderKnob.h"

SliderKnob::SliderKnob(
        std::wstring bitmapName, int x, int y, int width, int height) :
Meter(bitmapName, x, y, 1), /* units=1 temporarily; calculated by children */
_track(x, y, width, height) {

}

int SliderKnob::TrackX() const {
    return _track.X;
}

int SliderKnob::TrackY() const {
    return _track.Y;
}

int SliderKnob::TrackWidth() const {
    return _track.Width;
}

int SliderKnob::TrackHeight() const {
    return _track.Height;
}
