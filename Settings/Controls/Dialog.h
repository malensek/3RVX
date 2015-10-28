// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#pragma once

#include <unordered_map>

#include "../../3RVX/Window.h"
#include "DialogBase.h"

class Control;

class Dialog : public DialogBase, public Window {
public:
    Dialog(LPCWSTR className, LPCWSTR dlgTemplate);
};
