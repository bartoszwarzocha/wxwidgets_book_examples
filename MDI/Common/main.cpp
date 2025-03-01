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

#include <wx/mdi.h>

#include "main.h"

const wxString appName = wxT("MDI 1.0.0");

//---------------------------------------------------------------------------
// Application Implementation
//---------------------------------------------------------------------------
IMPLEMENT_APP(MyApp)

//---------------------------------------------------------------------------
// Identifiers
//---------------------------------------------------------------------------
enum
{
	ID_CLOSE = wxID_HIGHEST + 1,
	ID_ABOUT_PROGRAM,
	ID_ADD_WND,
	ID_DEL_WND,
};

//---------------------------------------------------------------------------
// Event Table
//---------------------------------------------------------------------------
BEGIN_EVENT_TABLE(MyFrame, wxMDIParentFrame)
EVT_MENU(ID_CLOSE, MyFrame::OnClose)
EVT_MENU(ID_ABOUT_PROGRAM, MyFrame::OnAboutProgram)
EVT_CLOSE(MyFrame::CloseApp)
EVT_MENU_RANGE(ID_ADD_WND, ID_DEL_WND, MyFrame::OnManage)
END_EVENT_TABLE()

//---------------------------------------------------------------------------
// OnInit()
//---------------------------------------------------------------------------
bool MyApp::OnInit()
{
	wxInitAllImageHandlers();

	// POLISH LANGUAGE FOR MDI WINDOW MENU
	// DISABLED DUE TO MISSING PL TRANSLATION BUT YOU CAN SEE HOW TO SET YOUR OWN LANGUAGE
	//loc.Init(wxLANGUAGE_POLISH, wxLOCALE_LOAD_DEFAULT);
	//loc.AddCatalog("pl");

	MyFrame* frame = new MyFrame(NULL, wxID_ANY, appName, wxDefaultPosition, wxSize(600, 400), wxDEFAULT_FRAME_STYLE);
	frame->Show(true);
	return true;
}

//---------------------------------------------------------------------------
// MyFrame Constructor
//---------------------------------------------------------------------------
MyFrame::MyFrame(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
	: wxMDIParentFrame(parent, id, title, pos, size, style), counter(4)
{
	wxMenuBar* mb = new wxMenuBar();

	wxMenu* mFile = new wxMenu();
	wxMenu* mMan = new wxMenu();
	wxMenu* mHelp = new wxMenu();

	mFile->AppendSeparator();
	mFile->Append(ID_CLOSE, wxT("&Close Program\tCtrl-X"), wxT("Closes the program"));

	mMan->Append(ID_ADD_WND, wxT("Add New Window\tCtrl-N"), wxT("Adds a new window"));
	mMan->Append(ID_DEL_WND, wxT("Delete Window\tCtrl-D"), wxT("Deletes the current window"));

	mHelp->Append(ID_ABOUT_PROGRAM, wxT("&About Program\tF1"), wxT("About the program"));

	mb->Append(mFile, wxT("&File"));
	mb->Append(mMan, wxT("Manage"));
	mb->Append(mHelp, wxT("H&elp"));

	this->SetMenuBar(mb);

	wxStatusBar* sb = new wxStatusBar(this);
	sb->SetFieldsCount(2);
	this->SetStatusBar(sb);
	SetStatusText(title, 0);

	//
	MyTextFrame* child1 = new MyTextFrame(this, wxID_ANY, wxT("Window 1"));
	MyTextFrame* child2 = new MyTextFrame(this, wxID_ANY, wxT("Window 2"));
	MyTextFrame* child3 = new MyTextFrame(this, wxID_ANY, wxT("Window 3"));

	child1->Show();
	child2->Show();
	child3->Show();

	this->Cascade();
	//

	CenterOnScreen();
}

//---------------------------------------------------------------------------
// MyFrame Destructor
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
void MyFrame::OnAboutProgram(wxCommandEvent& event)
{
	wxMessageBox(appName + wxT("\nCopyright (c) 2025 by ..."), wxT("About Program..."), wxOK | wxICON_INFORMATION);
	event.Skip();
}

//---------------------------------------------------------------------------
// Function displays a question about closing the program
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
// Function handles window management
//---------------------------------------------------------------------------
void MyFrame::OnManage(wxCommandEvent& event)
{
	MyTextFrame* tf = NULL;

	switch (event.GetId())
	{
	case ID_ADD_WND:
		tf = new MyTextFrame(this, wxID_ANY, wxString::Format(wxT("Window %d"), counter));
		tf->Show();
		tf->Activate();
		counter++;
		break;

	case ID_DEL_WND:
		tf = (MyTextFrame*)GetActiveChild();
		wxDELETE(tf);
		break;
	}
}
