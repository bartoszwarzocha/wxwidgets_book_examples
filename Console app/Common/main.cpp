/////////////////////////////////////////////////////////////////////////////
// Name:        main.cpp
// Purpose:     My wxWidgets app
// Author:      Name Surname <name.surname@email.org>
// Created:     2025-01-01
// Copyright:   (c) 2025 by Name Surname
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <wx/app.h>

int main(int argc, char **argv)
{
    wxAppConsole::CheckBuildOptions(WX_BUILD_OPTIONS_SIGNATURE, "program");

    wxInitializer initializer;
    if (!initializer)
    {
        fprintf(stderr, "wxWidgets initialize error!");
        return -1;
    }

    wxPrintf(wxT("Aplikacja konsolowa wxWidgets\n"));
    wxPrintf(wxT("Data i czas: %s\n"), wxDateTime::Now().FormatISOCombined(' '));

    // Here you can do everything with fantastic wxWidgets...

    return 0;
}

