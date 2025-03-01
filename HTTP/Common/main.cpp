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

#include <wx/notebook.h>
#include <wx/webview.h>
#include <wx/protocol/http.h>
#include <wx/sstream.h>

#include "main.h"

const wxString appName = wxT("HTTP Handling Test 1.0.0");

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
	ID_LOAD,
};

//---------------------------------------------------------------------------
// Event table
//---------------------------------------------------------------------------
BEGIN_EVENT_TABLE(MyFrame, wxFrame)
EVT_MENU(ID_CLOSE, MyFrame::OnClose)
EVT_MENU(ID_ABOUT_PROGRAM, MyFrame::OnAboutProgram)
EVT_CLOSE(MyFrame::CloseApp)
//
EVT_BUTTON(ID_LOAD, MyFrame::OnLoad)
END_EVENT_TABLE()

//---------------------------------------------------------------------------
// OnInit()
//---------------------------------------------------------------------------
bool MyApp::OnInit()
{
	wxInitAllImageHandlers();

	MyFrame* frame = new MyFrame(NULL, wxID_ANY, appName, wxDefaultPosition, wxSize(600, 400), wxDEFAULT_FRAME_STYLE);
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


	mFile->AppendSeparator();
	mFile->Append(ID_CLOSE, wxT("&Close Program\tCtrl-X"), wxT("Closes the program"));

	mHelp->Append(ID_ABOUT_PROGRAM, wxT("&About Program\tF1"), wxT("About the program"));

	mb->Append(mFile, wxT("&File"));
	mb->Append(mHelp, wxT("&Help"));

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

	address = new wxTextCtrl(panel, wxID_ANY, wxT("www.wxwidgets.org"));
	wxButton* load_btn = new wxButton(panel, ID_LOAD, wxT("Load"));

	wxNotebook* notebook = new wxNotebook(panel, wxID_ANY);

	source = new wxTextCtrl(notebook, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY);

	wxPanel* view_page = new wxPanel(notebook, wxID_ANY);
	wxBoxSizer* vps = new wxBoxSizer(wxVERTICAL);
	view_page->SetSizer(vps);
	view = wxWebView::New(view_page, wxID_ANY);
	vps->Add(view, 1, wxEXPAND | wxALL, 0);

	wxPanel* view_src_page = new wxPanel(notebook, wxID_ANY);
	wxBoxSizer* vsps = new wxBoxSizer(wxVERTICAL);
	view_src_page->SetSizer(vsps);
	view_src = wxWebView::New(view_src_page, wxID_ANY);
	vsps->Add(view_src, 1, wxEXPAND | wxALL, 0);

	notebook->AddPage(source, wxT("Page Source"), true);
	notebook->AddPage(view_src_page, wxT("Preview from Fetched Source"), false);
	notebook->AddPage(view_page, wxT("Direct Preview"), false);

	//
	wxBoxSizer* row = new wxBoxSizer(wxHORIZONTAL);
	row->Add(address, 1, wxEXPAND | wxALL, 4);
	row->Add(load_btn, 0, wxALL, 4);

	ps->Add(row, 0, wxEXPAND | wxALL, 0);
	ps->Add(notebook, 1, wxEXPAND | wxALL, 0);

	ms->Add(panel, 1, wxEXPAND | wxALL, 0);

	CenterOnScreen();
}

//---------------------------------------------------------------------------
// MyFrame destructor
//-------------------------------------------------------------------------
MyFrame::~MyFrame()
{

}

//---------------------------------------------------------------------------
// Function initiates program closure
//---------------------------------------------------------------------------
void MyFrame::OnClose(wxCommandEvent& WXUNUSED(event))
{
	this->Close(true);
}

//---------------------------------------------------------------------------
// Function displays "About Program..." window
//---------------------------------------------------------------------------
void MyFrame::OnAboutProgram(wxCommandEvent& WXUNUSED(event))
{
	wxMessageBox(appName + wxT("\nCopyright (c) 2015 by ..."), wxT("About Program..."), wxOK | wxICON_INFORMATION);
}

//---------------------------------------------------------------------------
// Function displays a prompt to close the program
//---------------------------------------------------------------------------
void MyFrame::CloseApp(wxCloseEvent& WXUNUSED(event))
{
	int action = wxMessageBox(wxT("Do you want to close the program?"), wxT("Question"), wxOK | wxCANCEL | wxICON_QUESTION);

	if (action == wxOK)
	{
		this->Destroy();
	}
	else
	{
		return;
	}
}

//---------------------------------------------------------------------------
// Function performs HTTP connection and fetches the source of the specified page
//---------------------------------------------------------------------------
void MyFrame::OnLoad(wxCommandEvent& WXUNUSED(event))
{
	source->Clear();

	// Connecting via HTTP
	wxHTTP http;
	if (!http.Connect(address->GetValue()))
	{
		wxMessageBox(wxT("Cannot load the page!"), wxT("Error!"), wxOK | wxICON_ERROR);
		return;
	}

	// Preparing data streams
	wxString src;
	wxInputStream* is = http.GetInputStream(wxT("/"));
	wxStringOutputStream* os = new wxStringOutputStream(&src);

	int read = 0;
	char buffer[1024];

	// Fetching the page source...
	do
	{
		is->Read(&buffer, sizeof(buffer));
		os->Write(&buffer, is->LastRead());
		read = is->LastRead();
	} while (read > 0);

	// Displaying the source
	source->AppendText(src);
	view_src->SetPage(src, address->GetValue());

	// Displaying the HTTP response code
	SetStatusText(wxString::Format(wxT("HTTP: %d"), http.GetResponse()), 1);

	// Closing the connection
	http.Close();

	wxDELETE(is);
	wxDELETE(os);

	// Direct preview
	view->LoadURL(wxT("http://") + address->GetValue());
}
