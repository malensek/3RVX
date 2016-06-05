3RVX Release Procedure
======================

Items to complete when creating a new release:

* Update version information for the main application (3RVX.exe) and the Settings UI. Versions should be identical.
* Build release binaries (Rebuild All)
* The *Release* folder is used to produce the installer and zip packages. Remove object files and debug databases for production releases.
* Remove the *Languages* and *Skins* junctions (shortcuts) and replace with production language files and skins (if necessary)
* Run *BuildInstaller.bat* in the *Installer* folder
* Upload the installer and portable versions of the application
* Update the latest_version record for online updates
