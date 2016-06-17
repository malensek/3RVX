3RVX Release Procedure
======================

Items to complete when creating a new release:

* Update version information for the main application (3RVX.exe) and the Settings UI. Versions should be identical.
* Build release binaries (Rebuild All)
* The *Release* folder is used to produce the installer and zip packages. Remove object files and debug databases for production releases.
* Remove the *Languages* and *Skins* junctions (shortcuts) and replace with production language files and skins. Note that eventually the production release and source tree will match up and this step will not be required.
* Run *BuildInstaller.bat* in the *Installer* folder
  * Note: The [WiX Toolset](http://wixtoolset.org) and 7za.exe from the [7-zip Extras](http://www.7-zip.org/download.html) are required to build the installer and portable zip distributions.
* Upload the installer and portable versions of the application
* Update the latest_version record for online updates
