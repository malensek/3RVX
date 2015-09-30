#include "About.h"

#include "../resource.h"
#include "../Updater/Updater.h"
#include "../Updater/Version.h"

void About::Initialize() {
    INIT_CONTROL(LBL_TITLE, Label, _title);

    std::wstring version = Updater::MainAppVersion().ToString();
    _title.Text(L"3RVX " + version);
}

void About::LoadSettings() {

}

void About::SaveSettings() {

}
