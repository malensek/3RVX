#pragma once

class COMUtil {
public:
    template <class T> static void SafeRelease(T *p) {
        if (p) {
            p->Release();
            p = NULL;
        }
    }

    template <class T> static void SafeRelease(T **ppT) {
        if (*ppT) {
            (*ppT)->Release();
            *ppT = NULL;
        }
    }
};

