// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#pragma once

class MeterWnd;

class Animation {
public:
    Animation(int speed) :
        _speed(speed) {

    }

    virtual bool Animate(MeterWnd *meterWnd) = 0;
    virtual void Reset(MeterWnd *meterWnd) = 0;

    virtual int UpdateInterval() = 0;

protected:
    int _speed;
};