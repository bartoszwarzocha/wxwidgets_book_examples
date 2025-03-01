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

#include <wx/progdlg.h>

#include "main.h"

wxString appName = wxEmptyString;

//---------------------------------------------------------------------------
// Application implementation
//---------------------------------------------------------------------------
IMPLEMENT_APP(MyApp)

//---------------------------------------------------------------------------
// Identifiers
//---------------------------------------------------------------------------
enum
{
	ID_CLOSE = wxID_HIGHEST + 1,
	ID_ABOUT_PROGRAM,
	ID_SHOW_OWN,
	ID_SHOW_WX,
};

//---------------------------------------------------------------------------
// Event table
//---------------------------------------------------------------------------
BEGIN_EVENT_TABLE(MyFrame, wxFrame)
EVT_MENU(ID_CLOSE, MyFrame::OnClose)
EVT_MENU(ID_ABOUT_PROGRAM, MyFrame::OnAboutProgram)
EVT_CLOSE(MyFrame::CloseApp)
//
EVT_BUTTON(ID_SHOW_OWN, MyFrame::OnShowOwnMsg)
END_EVENT_TABLE()

//---------------------------------------------------------------------------
// OnInit()
//---------------------------------------------------------------------------
#define SET_LANG_SYSDEF "System default"
#define SET_LANG_ENGLISH "English"
#define SET_LANG_POLISH "Polish"
#define SET_LANG_GERMAN "German"

WX_DECLARE_STRING_HASH_MAP(wxLanguage, LangMap);

bool MyApp::OnInit()
{
	wxInitAllImageHandlers();

	//
	LangMap lang_map;
	lang_map[SET_LANG_SYSDEF] = wxLANGUAGE_DEFAULT;
	lang_map[SET_LANG_ENGLISH] = wxLANGUAGE_ENGLISH;
	lang_map[SET_LANG_POLISH] = wxLANGUAGE_POLISH;
	lang_map[SET_LANG_GERMAN] = wxLANGUAGE_GERMAN;

	wxArrayString langs;
	langs.Add(SET_LANG_SYSDEF);
	langs.Add(SET_LANG_ENGLISH);
	langs.Add(SET_LANG_POLISH);
	langs.Add(SET_LANG_GERMAN);

	wxString choice = wxGetSingleChoice(wxT("Choose your language:"), wxT("Language"), langs, 0);

	wxLanguage lang = wxLANGUAGE_UNKNOWN;
	if (choice.IsEmpty())
	{
		// If no language is selected, try to set the system language...
		lang = static_cast<wxLanguage>(wxLocale::GetSystemLanguage());

		// If the system language is unknown, set the default application language
		if (lang == wxLANGUAGE_UNKNOWN) lang = wxLANGUAGE_ENGLISH;
	}
	else
	{
		lang = lang_map[choice];
	}

	// Initialize language
	if (locale.Init(lang, wxLOCALE_LOAD_DEFAULT))
	{
		// Set translation
		if (lang != wxLANGUAGE_ENGLISH)
		{
			const wxLanguageInfo* lang_info = wxLocale::GetLanguageInfo(lang);

			locale.AddCatalogLookupPathPrefix(wxGetCwd() + "/locale");
			locale.AddCatalog(lang_info->CanonicalName.SubString(0, 1));
		}
	}
	else
	{
		wxLogWarning("This language is not supported by the system.");
		return false;
	}

	appName = _("Multilanguage app 1.0.0");
	const wxString linfo = wxString::Format(" (%s)", locale.GetCanonicalName());
	MyFrame* frame = new MyFrame(NULL, wxID_ANY, appName + linfo, wxDefaultPosition, wxSize(350, 250), wxDEFAULT_FRAME_STYLE);
	frame->Show(true);
	return true;
}

//---------------------------------------------------------------------------
// MyFrame constructor
//---------------------------------------------------------------------------
MyFrame::MyFrame(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
	: wxFrame(parent, id, title, pos, size, style)
{
	mb = new wxMenuBar();

	mFile = new wxMenu();
	mHelp = new wxMenu();

	mFile->Append(ID_CLOSE, _("&Close\tCtrl-X"), _("Close application"));

	mHelp->Append(ID_ABOUT_PROGRAM, _("&About program\tF1"), _("About program"));

	mb->Append(mFile, _("&File"));
	mb->Append(mHelp, _("&Help"));

	this->SetMenuBar(mb);

	sb = new wxStatusBar(this);
	sb->SetFieldsCount(2);
	this->SetStatusBar(sb);
	SetStatusText(title, 0);

	wxBoxSizer* ms = new wxBoxSizer(wxVERTICAL);
	this->SetSizer(ms);

	panel = new wxPanel(this);
	wxBoxSizer* ps = new wxBoxSizer(wxVERTICAL);
	panel->SetSizer(ps);

	wxButton* btn1 = new wxButton(panel, ID_SHOW_OWN, _("Show your own message with the translation"));

	// Info
	wxStaticText* info1 = new wxStaticText(panel, wxID_ANY, _("Language: ") + wxGetApp().locale.GetLocale());
	wxStaticText* info2 = new wxStaticText(panel, wxID_ANY, _("Sys name: ") + wxGetApp().locale.GetSysName());
	wxStaticText* info3 = new wxStaticText(panel, wxID_ANY, _("Canonical name: ") + wxGetApp().locale.GetCanonicalName());

	ps->Add(btn1, 0, wxEXPAND | wxALL, 0);
	ps->AddSpacer(30);
	ps->Add(info1, 0, wxEXPAND | wxLEFT, 10);
	ps->Add(info2, 0, wxEXPAND | wxLEFT, 10);
	ps->Add(info3, 0, wxEXPAND | wxLEFT, 10);
	ms->Add(panel, 1, wxEXPAND | wxALL, 0);

	CenterOnScreen();
}

//---------------------------------------------------------------------------
// MyFrame destructor
//---------------------------------------------------------------------------
MyFrame::~MyFrame()
{

}

//---------------------------------------------------------------------------
// Function to initiate program closure
//---------------------------------------------------------------------------
void MyFrame::OnClose(wxCommandEvent& WXUNUSED(event))
{
	this->Close(true);
}

//---------------------------------------------------------------------------
// Function to display "About program..." window
//---------------------------------------------------------------------------
void MyFrame::OnAboutProgram(wxCommandEvent& event)
{
	wxMessageBox(appName + _("\nCopyright (c) 2025 by ..."), _("About program"), wxOK | wxICON_INFORMATION);
	event.Skip();
}

//---------------------------------------------------------------------------
// Function to display a question about closing the program
//---------------------------------------------------------------------------
void MyFrame::CloseApp(wxCloseEvent& WXUNUSED(event))
{
	int action = wxMessageBox(_("Do you really want to quit?"), _("Question"), wxOK | wxCANCEL | wxICON_QUESTION);

	if (action == wxOK)
	{
		this->Destroy();
	}
	else
	{
		return;
	}
}
