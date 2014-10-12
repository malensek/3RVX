#include "3RVX.h"
#include "Controllers\Volume\IVolume.h"
#include "Controllers\Volume\CoreAudio.h"
#include "LayeredWnd\LayeredWnd.h"
#include "MeterWnd\Meters\HorizontalBar.h"
#include "MeterWnd\Meters\HorizontalEndcap.h"
#include "MeterWnd\MeterWnd.h"
#include "MeterWnd\Meter.h"
#include <Wtsapi32.h>

int APIENTRY
wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
LPTSTR lpCmdLine, int nCmdShow) {
    using namespace Gdiplus;

    hInst = hInstance;

    GdiplusStartupInput gdiplusStartupInput;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    mainWnd = CreateMainWnd(hInstance);
    if (mainWnd == NULL)
        return EXIT_FAILURE;

    AllocConsole();
    FILE *in, *out, *err;
    freopen_s(&in, "CONIN$", "r", stdin);
    freopen_s(&out, "CONOUT$", "w", stdout);
    freopen_s(&err, "CONOUT$", "w", stderr);

    printf("Starting...\n");

    HRESULT hr = CoInitializeEx(NULL,
        COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

    /* Tell the program to initialize */
    PostMessage(mainWnd, WM_3RVX_CONTROL, MSG_LOAD, NULL);

    /* Start the event loop */
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    GdiplusShutdown(gdiplusToken);
    return (int)msg.wParam;
}

void Init() {
    ca = new CoreAudio(mainWnd);
    ca->Init();

    Meter *m = new HorizontalEndcap();
    std::wstring meterbmp(L"meter.png");
    m->SetBitmap(Gdiplus::Bitmap::FromFile(meterbmp.c_str(), false));
    m->Init();
    m->SetLocation(71, 29);
    m->SetUnits(28);
    MeterWnd *mW = new MeterWnd(hInst, L"testtest", L"what what");
    mW->AddMeter(m);
    std::wstring bgbmp(L"bg.png");
    mW->SetBackgroundImage(Gdiplus::Bitmap::FromFile(bgbmp.c_str(), true));
    mW->SetMeters(0.35);
    mW->Update();
    mW->Show();

    WTSRegisterSessionNotification(mainWnd, NOTIFY_FOR_THIS_SESSION);

    HotkeyManager *hkManager = HotkeyManager::Instance();
    hkManager->Register(mainWnd, HKM_MOD_WIN + VK_BACK);
    hkManager->Register(mainWnd, HKM_MOD_WIN + HKM_MOUSE_WHUP);
}

HWND CreateMainWnd(HINSTANCE hInstance) {
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = NULL;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = NULL;
    wcex.cbWndExtra = NULL;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = L"3RVX";
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassEx(&wcex)) {
        return NULL;
    }

    HWND hWnd = CreateWindowEx(
        NULL,
        L"3RVX", L"3RVX",
        NULL, NULL, NULL, //your boat, gently down the stream
        NULL, NULL, HWND_MESSAGE, NULL, hInstance, NULL);

    return hWnd;
}

LRESULT CALLBACK WndProc(
    HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

    if (message == MSG_VOLCHNG) {
        printf("Volume level: %.0f\n", ca->Volume() * 100.0f);
    }

    if (message == MSG_DEVCHNG) {
        printf("Device change detected.\n");
        ca->ReattachDefaultDevice();
    }

    if (message == WM_3RVX_CONTROL) {
        switch (wParam) {
        case MSG_LOAD:
            Init();
            break;

        case 101:
            printf("%x\n", lParam);
            break;
        }
    }

    if (message == WM_HOTKEY) {
        printf("Hotkey: %d\n", (int) wParam);
    }

    if (message == WM_WTSSESSION_CHANGE) {
        printf("session change\n");
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}