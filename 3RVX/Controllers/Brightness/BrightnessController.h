// Copyright (c) 2017, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#pragma once

class BrightnessController {
public:
    virtual float Brightness() = 0;
    virtual void Brightness(float level) = 0;
};