@ECHO OFF
TITLE 3RVX Installer Builder
ECHO "   _____ ______     ____  _______   "
ECHO "  |___ /|  _ \ \   / /\ \/ /___ /   "
ECHO "    |_ \| |_) \ \ / /  \  /  |_ \   "
ECHO "   ___) |  _ < \ V /   /  \ ___) |  "
ECHO "  |____/|_| \_\ \_/   /_/\_\____/   "
ECHO "                                    "

ECHO This script will generate a 3RVX installer.
set /p version= "Enter 3RVX Version (X.X): "
ECHO.

ECHO Setting Version: %version%
ECHO.

ECHO Setting PATH...
ECHO ON

set PATH=%PATH%;%WIX%\bin
heat dir ../Release -gg -srd -cg 3RVXComponents -dr INSTALLFOLDER -out 3RVXComponents.wxs
candle -pedantic -dProductVersion=%version%.0.0 ./3RVX.wxs ./3RVXComponents.wxs
light -b ../Release -ext WixUIExtension ./3RVX.wixobj ./3RVXComponents.wixobj -o 3RVX-%version%.msi
del 3RVX.wixobj 3RVX.wixpdb 3RVXComponents.wxs 3RVXComponents.wixobj

@ECHO OFF
ECHO.
ECHO.
IF EXIST "7za.exe" (
	ECHO Creating .zip distribution...
	ECHO.
	ECHO ON
	del 3RVX-%version%.zip
	copy "..\Assets\EmptySettings.xml" "..\Release\Settings.xml"
	7za a -tzip -mx9 3RVX-%version%.zip ../Release/*
)

@ECHO OFF
ECHO.

PAUSE