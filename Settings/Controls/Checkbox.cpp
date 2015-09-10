// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#include "Checkbox.h"

bool Checkbox::Checked() {
    return IsDlgButtonChecked(_parent, _id) == BST_CHECKED;
}

bool Checkbox::Checked(bool checked) {
    return CheckDlgButton(
        _parent,_id, checked ? BST_CHECKED : BST_UNCHECKED) == TRUE;
}

