# Read

A little database to keep track of books read.  The database includes the author(s), protagonist(s) and
titles of the books read.  The display is sorted by people names (authors first and then protagonists).

## Getting Started

This version was created with Visual Studion 2022 (VS22).  It was compiled with the following properties:
  o Windows SDK Version: 10.0.xxxxx (latest)
  o Platfor Toolset: visual Studio 2022 (latest)
  o MFC: Use MFC in a Shared DLL
  o Character Set:  Use Unicode Character Set
  o Precompiled Header:  Not Using Precompiled Headers
  o Linker/Additional Dependencies:  Htmlhelp.lib

The HTML Help Workshop (HHW), Version 4.74.8702.0 (the last one produced) was used to prepare the help
file (WixApp.chm).  It is
copied into the Release directory.  I used Dreamweaver (DW) to do most of the content production of the
help files that the HTML Help Workshop produces (i.e. HHW is used to produce the pages/files needed
and DW is used to fill in the content).

Wix is used to create the &lt;AppName&gt;.msi file.  WixApp (one of my applications, see git) was used
to produce the product.wxs file.

Note:  In order to avoid hickups during the compilation/link process make sure the VS22 dependencies are
set so the projects are produced in the proper order.

## Prerequisites

Windows 7 or above.  Visual Studio 2022 or above.  The WiX Toolset must be installed in Visual Studio.
The "HTML Help Workshop" (google it) must be installed on the PC.

### Installing

Execute the msi file to install the app.

## Updates

### Update 6/29/24

Add MIT License to installation.

### Update 6/14/24

Reorganize toolbar and update WinPos.

### Update 2/18/24

Reorganized the Library so that it compiles once into Debug and Release directories under the Library
directory.

### Update 1/8/24

Library and other changes.

### Update 7/7/23

Library Update and resulting app changes.

### Update 11/10/22

Update the toolbar background.  Update the help file icons.  Replace stdafx.h with pch.h/framework.h.
And other changes.

## Authors

Robert R. Van Tuyl

Contact:  romeo romeo victor tango alpha tango sam whisky delta echo dot charlie oscar mike

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details

