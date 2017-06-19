#pragma once

#include <string>
#include <Windows.h>

class HideWin10VolumeOSD
{
public:
  static bool Init();

private:
  static HWND FindOSDWindow(bool bSilent);

  /*
  private void Application_ApplicationExit(object sender, EventArgs e)
  {
    ShowOSD();
  }
  */

public:
  static void HideOSD();

  static void ShowOSD();
};
