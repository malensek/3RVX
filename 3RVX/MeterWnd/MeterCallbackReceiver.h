// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#pragma once

class MeterCallbackReceiver {
public:
    virtual void MeterChangeCallback(int units) = 0;
};