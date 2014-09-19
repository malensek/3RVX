#include "3RVX.h"

int APIENTRY 
wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, 
          LPTSTR lpCmdLine, int nCmdShow)
{
    using namespace Gdiplus;

    hInst = hInstance;

    GdiplusStartupInput gdiplusStartupInput;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    mainWnd = CreateMainWnd(hInstance);
    if(mainWnd == NULL)
        return EXIT_FAILURE;

    AllocConsole();
	FILE *in, *out, *err;
	freopen_s(&in, "CONIN$", "r", stdin);
	freopen_s(&out, "CONOUT$", "w", stdout);
	freopen_s(&err, "CONOUT$", "w", stderr);

	printf("Starting up");

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

	return (int) msg.wParam;
}

void Init()
{

}

HWND CreateMainWnd(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize         = sizeof(WNDCLASSEX);
	wcex.style			= NULL;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= NULL;
	wcex.cbWndExtra		= NULL;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(NULL, IDI_APPLICATION);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= L"3RVX";
	wcex.hIconSm		= LoadIcon(NULL, IDI_APPLICATION);

	if(!RegisterClassEx(&wcex))
	{
		return NULL;
	}

	HWND hWnd = CreateWindowEx(
		NULL,
		L"3RVX", L"3RVX",
		NULL, NULL, NULL, //your boat, gently down the stream
        NULL, NULL, HWND_MESSAGE, NULL, hInstance, NULL);

	return hWnd;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if(message == WM_3RVX_CONTROL)
    {
        switch(wParam)
        {
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