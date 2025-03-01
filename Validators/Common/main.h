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
// Class MyApp
//---------------------------------------------------------------------------
class MyApp : public wxApp
{
public:
	virtual bool OnInit();
};

//---------------------------------------------------------------------------
// Class MyFrame
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

	void OnCheckGenericValidator(wxCommandEvent& event);

private:
	wxStatusBar* sb;
	wxMenuBar* mb;
	wxMenu* mFile;
	wxMenu* mHelp;

	wxPanel* panel;

	wxTextCtrl* textValidAN;
	wxTextCtrl* textValidN;
	wxTextCtrl* textValidInc;
	wxTextCtrl* textValidExt;
	wxTextCtrl* intValid;
	wxTextCtrl* floatValid;
	wxTextCtrl* regexValid;

	DECLARE_EVENT_TABLE()
};

//---------------------------------------------------------------------------
// Class MyBoxSizer
//---------------------------------------------------------------------------
class MyBoxSizer : public wxBoxSizer
{
public:
	MyBoxSizer(int orientation) : wxBoxSizer(orientation) {}
	virtual ~MyBoxSizer() {}

	void AddWithLabel(const wxString& label, int label_width, wxWindow* wnd, int prop = 0, int flags = wxALL, int margin = 4, int spacer = 10)
	{
		wxBoxSizer* row_sizer = new wxBoxSizer(wxHORIZONTAL);
		wxStaticText* text = new wxStaticText(wnd->GetParent(), wxID_ANY, label, wxDefaultPosition, wxSize(label_width, -1));

		row_sizer->Add(text, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
		row_sizer->AddSpacer(spacer);
		row_sizer->Add(wnd, 1, wxEXPAND | wxALL, 0);

		this->Add(row_sizer, prop, flags, margin);
	}
};

//---------------------------------------------------------------------------
// Class MySimpleDialog
//---------------------------------------------------------------------------
class MySimpleTextDialog : public wxDialog
{
public:
	MySimpleTextDialog(wxWindow* parent, const wxString& msg, const wxValidator& val) : wxDialog(parent, wxID_ANY, wxT("Test"))
	{
		wxBoxSizer* ms = new wxBoxSizer(wxVERTICAL);
		this->SetSizer(ms);

		ms->Add(new wxStaticText(this, wxID_ANY, msg), 0, wxALL, 4);
		ms->Add(new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, val), 0, wxALL, 4);

		wxBoxSizer* btns = new wxBoxSizer(wxHORIZONTAL);
		btns->Add(new wxButton(this, wxID_OK, wxT("OK")), 0, wxALIGN_RIGHT | wxALL, 4);
		btns->Add(new wxButton(this, wxID_CANCEL, wxT("Cancel")), 0, wxALIGN_RIGHT | wxALL, 4);
		ms->Add(btns, 0, wxEXPAND | wxALL, 0);

		this->Fit();
	}

	virtual ~MySimpleTextDialog() {}
};

#endif
