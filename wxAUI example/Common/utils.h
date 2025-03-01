/////////////////////////////////////////////////////////////////////////////
// Name:        utils.h
// Purpose:     My wxWidgets app
// Author:      Name Surname <name.surname@email.org>
// Created:     2025-01-01
// Copyright:   (c) 2025 by Name Surname
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _UTILS_H_
#define _UTILS_H_

#include <wx/stdpaths.h>

// Settings
#define _INSTALLER_ 0 // Compilation for installer or installation package

//---------------------------------------------------------------------------
// Function returns the path to the program file depending on the platform
// and compilation scenario
//---------------------------------------------------------------------------
wxString wxGetAppFile(wxString file)
{
    wxString path = wxPathOnly(wxStandardPaths::Get().GetExecutablePath());

#if _INSTALLER_ && defined (__WXGTK__)
    path = wxStandardPaths::Get().GetDataDir();
#endif

    path = path + wxT("/") + file;
    path.Replace("\\", "/", true);
    path.Replace("//", "/", true);

    return path;
}

#endif
