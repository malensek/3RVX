#include "About.h"

#include "../../3RVX/Updater.h"

#include "../resource.h"

void About::Initialize() {
    INIT_CONTROL(LBL_TITLE, Label, _title);

    std::wstring version = Updater::MainAppVersionString();
    _title.Text(L"3RVX " + version);
}

void About::LoadSettings() {

}

void About::SaveSettings() {

}
