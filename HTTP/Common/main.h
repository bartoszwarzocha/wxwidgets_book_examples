﻿/////////////////////////////////////////////////////////////////////////////
// Name:        main.h
// Purpose:     My wxWidgets app
// Author:      Name Surname <name.surname@email.org>
// Created:     2025-01-01
// Copyright:   (c) 2025 by Name Surname
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _MAIN_H_
#define _MAIN_H_

//---------------------------------------------------------------------------
// MyApp class
//---------------------------------------------------------------------------
class MyApp : public wxApp
{
public:
	virtual bool OnInit();
};

//---------------------------------------------------------------------------
// MyFrame class
//---------------------------------------------------------------------------
class MyFrame : public wxFrame
{
public:
	MyFrame(wxWindow* parent, wxWindowID id, const wxString& title,
		const wxPoint& pos, const wxSize& size, long style);
	~MyFrame();

	void OnAboutProgram(wxCommandEvent& event);
	void OnClose(wxCommandEvent& event);
	void CloseApp(wxCloseEvent& event);

	void OnLoad(wxCommandEvent& event);

private:
	wxStatusBar* sb;
	wxMenuBar* mb;
	wxMenu* mFile;
	wxMenu* mHelp;

	wxPanel* panel;

	wxTextCtrl* address;
	wxTextCtrl* source;

	wxWebView* view;
	wxWebView* view_src;

	DECLARE_EVENT_TABLE()
};

#endif
