/////////////////////////////////////////////////////////////////////////////
// Name:        utils.h
// Purpose:     Utils and tools file
// Author:      Imie Nazwisko <imie.nazwisko@email.org>
// Created:     2018-01-01
// Copyright:   (c) 2018 by Imie Nazwisko
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _UTILS_H_
#define _UTILS_H_

#include <wx/stdpaths.h>

// Ustawienia
#define _INSTALATOR_ 0 // Kompilacja pod instalator lub pakiet instalacyjny

//---------------------------------------------------------------------------
// Funkcja zwraca œcie¿kê do pliku programu w zale¿noœci od platformy
// i scenariusza kompilacji
//---------------------------------------------------------------------------
wxString wxGetAppFile(wxString file)
{
    wxString path = wxPathOnly(wxStandardPaths::Get().GetExecutablePath());
	
#if _INSTALATOR_ && defined (__WXGTK__)
    path = wxStandardPaths::Get().GetDataDir();
#endif

    path = path + wxT("/") + file;
    path.Replace("\\","/",true);
    path.Replace("//","/",true);

    return path;
}

#endif