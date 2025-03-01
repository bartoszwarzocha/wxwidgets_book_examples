/////////////////////////////////////////////////////////////////////////////
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
// MyApp Class
//---------------------------------------------------------------------------
class MyApp : public wxApp
{
public:
	virtual bool OnInit();

	wxLocale loc;
};

//---------------------------------------------------------------------------
// MyFrame Class
//---------------------------------------------------------------------------
class MyFrame : public wxMDIParentFrame
{
public:
	MyFrame(wxWindow* parent, wxWindowID id, const wxString& title,
		const wxPoint& pos, const wxSize& size, long style);
	~MyFrame();

	void OnAboutProgram(wxCommandEvent& event);
	void OnClose(wxCommandEvent& event);
	void CloseApp(wxCloseEvent& event);
	void OnManage(wxCommandEvent& event);

private:
	unsigned int counter;

	DECLARE_EVENT_TABLE()
};

//---------------------------------------------------------------------------
// MyTextFrame Class
//---------------------------------------------------------------------------
class MyTextFrame : public wxMDIChildFrame
{
public:
	MyTextFrame(wxMDIParentFrame* parent, wxWindowID id, const wxString& title)
		: wxMDIChildFrame(parent, id, title)
	{
		wxBoxSizer* s = new wxBoxSizer(wxVERTICAL);
		this->SetSizer(s);

		text = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
		s->Add(text, 1, wxEXPAND | wxALL, 0);
	}

	virtual ~MyTextFrame() {}

	wxTextCtrl* GetTextCtrl() { return this->text; }

private:
	wxTextCtrl* text;
};

#endif
