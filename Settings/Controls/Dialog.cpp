// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#include "Dialog.h"

#include "../../3RVX/Logger.h"
#include "../UITranslator.h"
#include "Control.h"

Dialog::Dialog(LPCWSTR className, LPCWSTR dlgTemplate) :
Window(className) {
    _dlgHwnd = CreateDialogParam(
        Window::InstanceHandle(),
        dlgTemplate,
        Window::Handle(),
        StaticDialogProc,
        (LPARAM) this);

    UITranslator::TranslateWindowText(_dlgHwnd);
}
