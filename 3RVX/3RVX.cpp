#include "3RVX.h"
#include "Controllers\Volume\IVolume.h"
#include "Controllers\Volume\CoreAudio.h"
#include "LayeredWnd\LayeredWnd.h"

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

    /* Show a layered window */
    LayeredWnd *lw = new LayeredWnd(hInst, L"testing", L"test test");
    lw->Init();
    std::wstring filename(L"m.gif");
    Gdiplus::Bitmap *buff = Gdiplus::Bitmap::FromFile(filename.c_str(), false);
    printf("Last status: %d\n", buff->GetLastStatus());
    lw->Image(buff);
    lw->Show();
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

    return DefWindowProc(hWnd, message, wParam, lParam);
}