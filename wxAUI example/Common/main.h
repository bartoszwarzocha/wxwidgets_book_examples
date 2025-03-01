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
};

//---------------------------------------------------------------------------
// MyFrame Class
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

	void OnAction(wxCommandEvent& event)
	{
		SetStatusText(wxString::Format(wxT("Selected action ID %d"), event.GetId()), 1);
	}

	void OnPageChange(wxAuiNotebookEvent& event)
	{
		SetStatusText(wxString::Format(wxT("Selected page index %d"), event.GetSelection()), 1);
	}

	void OnPageClose(wxAuiNotebookEvent& event)
	{
		SetStatusText(wxString::Format(wxT("Closed page index %d"), event.GetOldSelection()), 1);
	}

	void OnWrap(wxCommandEvent& event)
	{
		SetStatusText(wxString::Format(wxT("Line wrapping: %s"), (event.IsChecked() ? wxT("YES") : wxT("NO"))), 1);
	}

private:
	wxStatusBar* sb;
	wxMenuBar* mb; // Menu bar
	wxMenu* mFile; // 'File' menu
	wxMenu* mAddFile; // 'File' → 'Add file' menu
	wxMenu* mEdit; // 'Edit' menu
	wxMenu* mView; // 'View' menu
	wxMenu* mTools; // 'Tools' menu
	wxMenu* mHelp; // 'Help' menu

	wxAuiManager manager;
	wxAuiNotebook* notebook;
	wxAuiToolBar* file_tb;
	wxAuiToolBar* edit_tb;

	wxTextCtrl* console;
	wxTreeCtrl* project_tree;
	wxListCtrl* list;
	wxPanel* settings;
	wxCheckBox* wrap_lines;

	DECLARE_EVENT_TABLE()
};

#endif
