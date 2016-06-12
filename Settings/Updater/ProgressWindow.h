// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#pragma once

#include <thread>

#include "../Controls/Dialog.h"

#include "Updater.h"
#include "Version.h"

class ProgressBar;
class Button;

class ProgressWindow : public Dialog {
public:
    ProgressWindow(HWND parent, Version version);

private:
    Version _version;
    Button *_cancel;
    ProgressBar *_progress;

    void Initialize();

    std::thread _dlThread;
    void Download();

};