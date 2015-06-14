set PATH=%PATH%;%WIX%\bin
heat dir ../Release -gg -srd -cg 3RVXComponents -dr INSTALLFOLDER -out 3RVXComponents.wxs
candle ./3RVX.wxs ./3RVXComponents.wxs
light -b ../Release -ext WixUIExtension ./3RVX.wixobj ./3RVXComponents.wixobj -o 3RVX.msi
del 3RVX.wixobj 3RVX.wixpdb 3RVXComponents.wxs 3RVXComponents.wixobj cab1.cab