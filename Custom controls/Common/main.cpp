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

#include <wx/timer.h>

#include "MyResizableImgPanel.h"
#include "MyGraphicButton.h"
#include "MySimpleProgressBar.h"
#include "MyCalendar.h"

#include "main.h"

const wxString appName = wxT("Custom Controls 1.0.0");

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
	ID_TIMER,
	//
	ID_PANEL,
	ID_BUTTON,
	ID_CALENDAR
};

//---------------------------------------------------------------------------
// Event table
//---------------------------------------------------------------------------
BEGIN_EVENT_TABLE(MyFrame, wxFrame)
EVT_MENU(ID_CLOSE, MyFrame::OnClose)
EVT_MENU(ID_ABOUT_PROGRAM, MyFrame::OnAboutProgram)
EVT_CLOSE(MyFrame::CloseApp)
EVT_TIMER(ID_TIMER, MyFrame::OnTimer)
//
EVT_MYPANEL_RESIZED(ID_PANEL, MyFrame::OnPanelResize)
EVT_MYGBUTTON_ACTIVATED(ID_BUTTON, MyFrame::OnButton)
EVT_MYCAL_LCLICKED(ID_CALENDAR, MyFrame::OnCalendar)
END_EVENT_TABLE()

//---------------------------------------------------------------------------
// OnInit()
//---------------------------------------------------------------------------
bool MyApp::OnInit()
{
	wxInitAllImageHandlers();

	MyFrame* frame = new MyFrame(NULL, wxID_ANY, appName, wxDefaultPosition, wxSize(800, 600), wxDEFAULT_FRAME_STYLE);
	frame->Show(true);
	return true;
}

//---------------------------------------------------------------------------
// MyFrame constructor
//---------------------------------------------------------------------------
MyFrame::MyFrame(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
	: wxFrame(parent, id, title, pos, size, style)
{
	timer = new wxTimer(this, ID_TIMER);

	mb = new wxMenuBar();

	mFile = new wxMenu();
	mHelp = new wxMenu();


	mFile->AppendSeparator();
	mFile->Append(ID_CLOSE, wxT("&Close program\tCtrl-X"), wxT("Closes the program"));

	mHelp->Append(ID_ABOUT_PROGRAM, wxT("&About program\tF1"), wxT("About the program"));

	mb->Append(mFile, wxT("&File"));
	mb->Append(mHelp, wxT("&Help"));

	this->SetMenuBar(mb);

	sb = new wxStatusBar(this);
	sb->SetFieldsCount(2);
	this->SetStatusBar(sb);
	SetStatusText(title, 0);

	wxBoxSizer* ms = new wxBoxSizer(wxVERTICAL);
	this->SetSizer(ms);

	panel = new MyResizableImgPanel(this, ID_PANEL, wxBitmap("gfx/back.jpg", wxBITMAP_TYPE_JPEG));
	wxBoxSizer* ps = new wxBoxSizer(wxVERTICAL);
	panel->SetSizer(ps);

	wxBitmap normal("gfx/btn/n.png", wxBITMAP_TYPE_PNG);
	wxBitmap hover("gfx/btn/h.png", wxBITMAP_TYPE_PNG);
	wxBitmap activ("gfx/btn/a.png", wxBITMAP_TYPE_PNG);
	button = new MyGraphicButton(panel, ID_BUTTON, wxDefaultPosition, normal, hover, activ);
	button->SetBackground(wxBitmap("gfx/back.jpg", wxBITMAP_TYPE_JPEG));

	pbar = new MySimpleProgressBar(panel, wxID_ANY, wxDefaultPosition, wxSize(-1, 25), PBAR_DEFAULT_STYLE | PBAR_WITH_MARGIN);
	pbar->SetForegroundColour(*wxWHITE);
	pbar->SetBackgroundColour(*wxBLACK);
	pbar->SetColour1(wxColour(255, 0, 255));
	pbar->SetColour2(wxColour(255, 255, 0));
	pbar->SetRange(1000);

	calendar = new MyCalendar(panel, ID_CALENDAR, wxDefaultPosition, wxSize(650, 350));

	wxBoxSizer* row = new wxBoxSizer(wxHORIZONTAL);
	row->Add(button, 0, wxALL, 10);
	row->Add(pbar, 1, wxEXPAND | wxALIGN_CENTER_VERTICAL | wxTOP | wxRIGHT | wxBOTTOM, 10);

	ps->Add(row, 0, wxEXPAND | wxALL, 0);
	ps->Add(calendar, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 10);
	ms->Add(panel, 1, wxEXPAND | wxALL, 0);

	CenterOnScreen();
}

//---------------------------------------------------------------------------
// MyFrame destructor
//-------------------------------------------------------------------------
MyFrame::~MyFrame()
{
	delete timer;
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
	wxMessageBox(appName + wxT("\nCopyright (c) 2025 by ..."), wxT("About program..."), wxOK | wxICON_INFORMATION);
	event.Skip();
}

//---------------------------------------------------------------------------
// Function to display program closure confirmation
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

