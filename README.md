# wxWidgets Example Applications

## Introduction
This repository contains example applications demonstrating various features of wxWidgets. These applications serve as additional materials for the book **"Cross-Platform Programming with C++ and wxWidgets 3"** by **Bartosz Warzocha**, published in 2018. The book can be found at:
[https://ksiegarnia.pwn.pl/Programowanie-wieloplatformowe-z-C-i-wxWidgets-3,750527898,p.html](https://ksiegarnia.pwn.pl/Programowanie-wieloplatformowe-z-C-i-wxWidgets-3,750527898,p.html)

All examples are licensed under the wxWidgets license.

The source code has been updated and tested on:
- **Windows 11** (Visual Studio 2022)
- **Linux Mint 22**

Additionally, all example applications have been fully translated into English. Originally, they were only available in Polish.

## Upgrade Script: upgrade_wxwidgets.py

The `upgrade_wxwidgets.py` script allows automatic migration of wxWidgets versions in projects. It replaces references to older library versions with newer ones and updates Makefile configurations.

### Usage
1. Edit scirpt with your wxWidgets versions.
2. Run the script:
   ```sh
   python upgrade_wxwidgets.py
   ```
3. Enter the directory path containing the project files (`./`).
4. The script will automatically update all `.vcxproj`, `.sln`, and `Makefile` files.

## Directory Structure

| Name | Description | Screenshot |
|------|------------|------------|
| **Base app** | Base application with some examples about mouse and keyboard events using. | |
| **Chess timer - wxPropertyGrid example** | Chess timer with properties realised with wxPropertyGrid. | ![Chess timer](Chess%20timer%20-%20wxPropertyGrid%20example/screenshot.png) |
| **Combo list** | The example shows how you can implement combobox controls with your own advanced lists in a deopdown part. | ![Combo list](Combo%20list/screenshot.png) |
| **Command line** | This simple application shows the way of working with default wxWidgets command line parser. | ![Command line](Command%20line/screenshot.png) |
| **Console app** | Base console app. | |
| **Custom controls** | This programme demonstrates how to make some of your own GUI controls - from a simple graphical button, to a progress bar, to an advanced calendar. You simply have to see how easy it is with wxWidgets! | ![Custom controls](Custom%20controls/screenshot.png) |
| **Custom splashscreen** | Splashcreen with your own text or counter. | |
| **DB (Soci + Firebird)** | The application shows how to easily combine working with wxWidgets and libraries for the popular Firebird database server. Please note that you must be familiar with SQL. | |
| **Dialogs** | This programme demonstrates all standard windows and dialogue functions available in wxWidgets. | |
| **DocMDI** | | ![DocMDI](DocMDI/screenshot.png) |
| **Drawing** | A simple application which show you how to draw with wxWidgets. | ![Drawing](Drawing/screenshot.png) |
| **File system** | | ![File system](File%20system/screenshot.png) |
| **FTP** | | ![FTP](FTP/screenshot.png) |
| **Hangman game** | | ![Hangman](Hangman%20game/screenshot.png) |
| **HTTP** | | ![HTTP](HTTP/screenshot.png) |
| **Internationalization** | | ![Internationalization](Internationalization/screenshot.png) |
| **Log** | | ![Log](Log/screenshot.png) |
| **MDI** | | ![MDI](MDI/screenshot.png) |
| **Multithreading** | | ![Multithreading](Multithreading/screenshot.png) |
| **OpenGL** | | ![OpenGL](OpenGL/screenshot.png) |
| **Socket** | | ![Socket](Socket/screenshot.png) |
| **Text editor - wxRibbonBar example** | | ![Text editor](Text%20editor%20-%20wxRibbonBar%20example/screenshot.png) |
| **Text editor - wxRichTextCtrl example** | | ![Text editor](Text%20editor%20-%20wxRichTextCtrl%20example/screenshot.png) |
| **Time** | | ![Time](Time/screenshot.png) |
| **Useful utilities** | Different files useful with some projects. | ![Useful utilities](Useful%20utilities/screenshot.png) |
| **Validators** | How to use validators? This app will give you an answer... | ![Validators](Validators/screenshot.png) |
| **wxAUI example** | | ![wxAUI](wxAUI%20example/screenshot.png) |
| **wxC++ (Your own IDE!)** | A simple, but fully-functional integrated development environment (IDE) with support for creating and organising a development project. The programme demonstrates working with wxScintiila and teaches how to implement not only code folding, but also how to perform syntax highlighting specific to selected keywords. | ![wxC++](wxC++%20(Your%20own%20IDE!)/screenshot.png) |

## Copyright
**Bartosz Warzocha**, 2018. All rights reserved.

