#include "HideWin10VolumeOSD.h"

static HWND hWndInject = 0;

bool HideWin10VolumeOSD::Init()
{
  if (hWndInject != 0)
  {
      return true;
  }

  hWndInject = FindOSDWindow(true);

  if (hWndInject == 0)
  {
    return false;
  }

  HideOSD();

  return true;
}

HWND HideWin10VolumeOSD::FindOSDWindow(bool bSilent)
{
  HWND hwndRet = 0;
  HWND hwndHost = 0;

  int pairCount = 0;

  // search for all windows with class 'NativeHWNDHost'

  while ((hwndHost = FindWindowEx(0, hwndHost, L"NativeHWNDHost", L"")) != 0)
  {
    // if this window has a child with class 'DirectUIHWND' it might be the volume OSD

    if (FindWindowEx(hwndHost, 0, L"DirectUIHWND", L"") != 0)
    {
      // if this is the only pair we are sure

      if (pairCount == 0)
      {
        hwndRet = hwndHost;
      }

      pairCount++;

      // if there are more pairs the criteria has failed...

      if (pairCount > 1)
      {
        throw(L"Severe error: Multiple pairs found!");
        return 0;
      }
    }
  }

  // if no window found yet, there is no OSD window at all

  if (hwndRet == 0 && !bSilent)
  {
    throw(L"Severe error: OSD window not found!");
  }

  return hwndRet;
}

void HideWin10VolumeOSD::HideOSD()
{
  ShowWindow(hWndInject, 6); // SW_MINIMIZE
}

void HideWin10VolumeOSD::ShowOSD()
{
  ShowWindow(hWndInject, 9); // SW_RESTORE

  // show window on the screen

  keybd_event(VK_VOLUME_UP, 0, 0, 0);
  keybd_event(VK_VOLUME_DOWN, 0, 0, 0);
}
