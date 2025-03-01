# wxWidgets Example Applications

## From base application to your own IDE!

## Introduction
The repository contains a selection of sample [wxWidgets](https://www.wxwidgets.org/) applications, which are supplementary material to my book **"Cross-Platform Programming with C++ and wxWidgets 3"**. The book can be found at:
[https://ksiegarnia.pwn.pl/Programowanie-wieloplatformowe-z-C-i-wxWidgets-3,750527898,p.html](https://ksiegarnia.pwn.pl/Programowanie-wieloplatformowe-z-C-i-wxWidgets-3,750527898,p.html)

All examples are licensed under the [wxWidgets License](https://www.wxwidgets.org/about/licence/).

The source code has been updated and tested on:
- **Windows 11** (Visual Studio 2022) with wxWidgets 3.3.x
- **Linux Mint 22** with wxWidgets 3.2.6

In every application folder you can find **Visual Studio solution & project** file, **Code::Blocks project** file and **makefile**, which allow you to build the app in every Windows or Linux OS.

_Additionally, all example applications have been fully translated into English. Originally, they were only available in Polish._

## Upgrade projects to your version of the wxWidgets: upgrade_wxwidgets.py

The `upgrade_wxwidgets.py` script allows automatic migration of wxWidgets versions in projects. It replaces references to older library versions with newer ones and updates Makefile configurations.

### Usage
1. Edit scirpt with your wxWidgets versions.
2. Run the script:
   ```sh
   python upgrade_wxwidgets.py
   ```
3. Enter the directory path containing the project files (`./`).
4. The script will automatically update all `.vcxproj`, `.sln`, and `Makefile` files.

## Directory Structure & Examples list

Here is a list of example apllications (as well as other add-ons) included in the repository.
- Note that some projects require additional files, which are usually placed in the Release_linux and Release_msw_x64 directories. If you are building the Debug version, you need to make sure that the additional files are also placed in the target directories.
- On some Linux distributions, it may be necessary to manually hardcode the Release_linux/obj and Debug_linux/obj directories before starting the build.

| Name (in alphabetical order) | Description | Screenshot |
|------|------------|------------|
| [**Base app**](Base%20app/) | Base application with some examples about mouse and keyboard events using. | |
| [**Chess timer - wxPropertyGrid example**](Chess%20timer%20-%20wxPropertyGrid%20example/) | Chess timer with properties realised with wxPropertyGrid. | ![Chess timer](Chess%20timer%20-%20wxPropertyGrid%20example/screenshot.png) |
| [**Combo list**](Combo%20list/) | The example shows how you can implement combobox controls with your own advanced lists in a dropdown part. | ![Combo list](Combo%20list/screenshot.png) |
| [**Command line**](Command%20line/) | This simple application shows the way of working with default wxWidgets command line parser. | ![Command line](Command%20line/screenshot.png) |
| [**Console app**](Console%20app/) | Base console app. | |
| [**Custom controls**](Custom%20controls/) | This program demonstrates how to make some of your own GUI controls - from a simple graphical button, to a progress bar, to an advanced calendar. You simply have to see how easy it is with wxWidgets! | ![Custom controls](Custom%20controls/screenshot.png) |
| [**Custom splashscreen**](Custom%20splashscreen/) | Splashscreen with your own text or counter. | ![Custom splashscreen](Custom%20splashscreen/screenshot.png) |
| [**DB (Soci + Firebird)**](DB%20%28Soci%20+%20Firebird%29/) | The application shows how to easily combine working with wxWidgets and libraries for the popular Firebird database server. Please note that you must be familiar with SQL. | |
| [**Dialogs**](Dialogs/) | This program demonstrates all standard windows and dialogue functions available in wxWidgets. | |
| [**DocMDI**](DocMDI/) | The app shows how to work with DocMDI. | ![DocMDI](DocMDI/screenshot.png) |
| [**Drawing**](Drawing/) | A simple application which shows you how to draw with wxWidgets. | ![Drawing](Drawing/screenshot.png) |
| [**File system**](File%20system/) | What you can do with filesystem working with the wxWidgets library? You'll be surprised how much, just try it! | ![File system](File%20system/screenshot.png) |
| [**FTP**](FTP/) | A simple FTP protocol example. _For Windows please link the Ws2_32.lib library if required._| ![FTP](FTP/screenshot.png) |
| [**Hangman game**](Hangman%20game/) | A simple game made in wxWidgets in which you have to guess the words made up by your opponent within a certain time, otherwise... I don't want to be in your shoes! | ![Hangman](Hangman%20game/screenshot.png) |
| [**HTTP**](HTTP/) | A simple HTTP protocol example. | |
| [**Internationalization**](Internationalization/) | This program will teach you how to work with different languages in wxWidgets GUI apps. | ![Internationalization](Internationalization/screenshot.png) |
| [**Log**](Log/) | Find out how you can collect diagnostic logs of your app using the powerful wxWidgets mechanisms. And this is possible in several interesting ways... | ![Log](Log/screenshot.png) |
| [**MDI**](MDI/) | The app shows how to work with MDI. | ![MDI](MDI/screenshot.png) |
| [**Multithreading**](Multithreading/) | One thread is not enough! See how to use multiple threads in the wxWidgets app. | ![Multithreading](Multithreading/screenshot.png) |
| [**OpenGL**](OpenGL/) | A small introduction to the fascinating world of 3D. _**SOIL and GLEW are required!**_ | ![OpenGL](OpenGL/screenshot.png) |
| [**Socket**](Socket/) | A set of two applications demonstrating client/server communication. | |
| [**Text editor - wxRibbonBar example**](Text%20editor%20-%20wxRibbonBar%20example/) | Are standard toolbars boring you? The wxRibbonBar is an answer to your frustration! The program not only shows you how to place the Ribbon Bar in your program, but also provides information on how to make various advanced elements of the bar, such as galleries and others. | ![Text editor](Text%20editor%20-%20wxRibbonBar%20example/screenshot.png) |
| [**Text editor - wxRichTextCtrl example**](Text%20editor%20-%20wxRichTextCtrl%20example/) | WOW! My own 'Office Writer'! | |
| [**Time**](Time/) | A simple time stuff example. | ![Time](Time/screenshot.png) |
| [**Useful utilities**](Useful%20utilities/) | Different files useful with some projects. | |
| [**Validators**](Validators/) | How to use validators? This app will give you an answer... | ![Validators](Validators/screenshot.png) |
| [**wxAUI example**](wxAUI%20example/) | The program demonstrates how to use the advanced GUI mode of the wxAUI floating frame library. | ![wxAUI](wxAUI%20example/screenshot.png) |
| [**wxC++ (Your own IDE!)**](wxC++%20%28Your%20own%20IDE%21%29/) | A simple, but fully-functional integrated development environment (IDE) with support for creating and organising a development project. The program demonstrates working with wxScintilla and teaches how to implement not only code folding, but also how to perform syntax highlighting specific to selected keywords. | ![wxC++](wxC++%20(Your%20own%20IDE!)/screenshot.png) |

---
© 2018, 2025 Bartosz Warzocha (bartosz.warzocha@gmail.com)

