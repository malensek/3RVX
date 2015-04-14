#include "Checkbox.h"

bool Checkbox::Checked() {
    return IsDlgButtonChecked(_parent, _id) == BST_CHECKED;
}

bool Checkbox::Checked(bool checked) {
    return CheckDlgButton(
        _parent,_id, checked ? BST_CHECKED : BST_UNCHECKED) == TRUE;
}

