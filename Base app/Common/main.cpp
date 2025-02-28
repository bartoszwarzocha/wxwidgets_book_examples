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

#include "main.h"

const wxString appName = wxT("First wxWidgets program");

//---------------------------------------------------------------------------
// Implementation
//---------------------------------------------------------------------------
IMPLEMENT_APP(MyApp)

//---------------------------------------------------------------------------
// Event IDs
//---------------------------------------------------------------------------
enum
{
	ID_SHOW_DATE = wxID_HIGHEST + 1,
	ID_SHOW_SYS_VERSION,
	ID_CLOSE,
	ID_ABOUT_PROGRAM
};

//---------------------------------------------------------------------------
// Event table
//---------------------------------------------------------------------------
BEGIN_EVENT_TABLE(MyFrame, wxFrame)
	EVT_MENU_RANGE(ID_SHOW_DATE, ID_ABOUT_PROGRAM, MyFrame::OnMenuEvent)
	EVT_CLOSE(MyFrame::CloseApp)
    EVT_LEFT_DCLICK(MyFrame::OnMouseClick)
    EVT_RIGHT_UP(MyFrame::OnMouseClick)
	EVT_KEY_DOWN(MyFrame::OnKey)
	EVT_KEY_UP(MyFrame::OnKeyUp)
END_EVENT_TABLE()

//---------------------------------------------------------------------------
// OnInit()
//---------------------------------------------------------------------------
bool MyApp::OnInit()
{
    MyFrame *frame = new MyFrame(NULL, wxID_ANY, appName, wxDefaultPosition, wxSize(600,400), wxDEFAULT_FRAME_STYLE);
    frame->Show(true);

    return true;
}

//---------------------------------------------------------------------------
// MyFrame constructor
//---------------------------------------------------------------------------
MyFrame::MyFrame(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &pos, const wxSize &size, long style)
: wxFrame(parent, id, title, pos, size, style)
{
    mb = new wxMenuBar();

    mFile = new wxMenu();
    mHelp = new wxMenu();

    mFile->AppendCheckItem(ID_SHOW_DATE, wxT("Show &date\tCtrl-D"), wxT("Shows date"));
	mFile->Append(ID_SHOW_SYS_VERSION, wxT("Show OS &version\tCtrl-V"), 	wxT("Shows OS version"));
	mFile->AppendSeparator();
	mFile->Append(ID_CLOSE, wxT("&Close\tCtrl-X"), wxT("Closes program"));

	mHelp->Append(ID_ABOUT_PROGRAM, wxT("&About program\tF1"), wxT("About program"));

	mb->Append(mFile, wxT("&File"));
	mb->Append(mHelp, wxT("H&elp"));

	this->SetMenuBar(mb);

    sb = new wxStatusBar(this);
    sb->SetFieldsCount(2);
    this->SetStatusBar(sb);
    SetStatusText(title, 0);

    CenterOnScreen();

	Bind(wxEVT_MOTION, &MyFrame::OnMouseMove, this);
}

//---------------------------------------------------------------------------
// MyFrame destructor
//-------------------------------------------------------------------------
MyFrame::~MyFrame()
{
	
}

//---------------------------------------------------------------------------
// Function shows current date in the statusbar
//---------------------------------------------------------------------------
void MyFrame::OnShowDate(wxCommandEvent& event)
{
	bool status = event.IsChecked();

    if(status)
    {
        this->SetStatusText(wxDateTime::Now().FormatISODate(), 1);
    }
    else
    {
        this->SetStatusText(wxEmptyString, 1);
    }
}

//---------------------------------------------------------------------------
// Funktion shows OS info window 
//---------------------------------------------------------------------------
void MyFrame::OnShowSysVersion(wxCommandEvent& WXUNUSED(event))
{
	wxString system_vers = wxPlatformInfo::Get().GetOperatingSystemDescription();

   wxMessageBox(system_vers, wxT("Information"), wxOK|wxICON_INFORMATION);
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void MyFrame::OnClose(wxCommandEvent& WXUNUSED(event))
{
    this->Close(true);
}

//---------------------------------------------------------------------------
// Function shows info about program
//---------------------------------------------------------------------------
void MyFrame::OnAboutProgram(wxCommandEvent& event)
{
    wxMessageBox(wxT("Copyright (c) 2015 by ..."), wxT("About..."), wxOK|wxICON_INFORMATION);
	event.Skip();
}

//---------------------------------------------------------------------------
// Funktion shows a question about the program closing
//---------------------------------------------------------------------------
void MyFrame::CloseApp(wxCloseEvent& WXUNUSED(event))
{
    int action = wxMessageBox(wxT("Do you want close the program?"), wxT("Question"), wxOK|wxCANCEL|wxICON_QUESTION);

    if(action == wxOK)
    {
        this->Destroy();
    }
    else
    {
        return;
    }
}

//---------------------------------------------------------------------------
// Function shows the cursor position in the statusbar
//---------------------------------------------------------------------------
void MyFrame::OnMouseMove(wxMouseEvent& event)
{
	if(!mb->FindItem(ID_SHOW_DATE)->IsChecked())
	{
		this->SetStatusText(wxString::Format(wxT("Cursor: %d, %d"), event.GetX(), event.GetY()), 1);
	}
}

//---------------------------------------------------------------------------
// Function shows the mouse action position in the statusbar
//---------------------------------------------------------------------------
void MyFrame::OnMouseClick(wxMouseEvent& event)
{
	wxString key = wxEmptyString;
	wxString msg = wxEmptyString;

	if(event.LeftDClick())
	{
		key = wxT("LPM");
	}
	else if(event.RightUp()) 
	{
		key = wxT("PPM");
	}
	else 
	{
		key = wxT("Unknown");
	}

	msg = wxT("Button: ") + key + wxString::Format(wxT("\nCoordinates: %d, %d"), event.GetX(), event.GetY());

	wxMessageBox(msg, wxT("Information..."), wxOK|wxICON_INFORMATION);
}

//---------------------------------------------------------------------------
// Example function for range menu events
//---------------------------------------------------------------------------
void MyFrame::OnMenuEvent(wxCommandEvent& event)
{
	switch(event.GetId())
    {
        case ID_SHOW_DATE:
            // Show date...
            break;

        case ID_SHOW_SYS_VERSION:
            // Show OS info...
            break;

        case ID_CLOSE:
            // Close the program...
            break;

        case ID_ABOUT_PROGRAM:
            // About...
            break;
    }

	wxMessageBox(wxString::Format(wxT("ID: %d"),event.GetId()),wxEmptyString,wxOK);
}

//---------------------------------------------------------------------------
// The key has been pushed!
//---------------------------------------------------------------------------
void MyFrame::OnKey(wxKeyEvent& event)
{
	SetStatusText(wxString::Format(wxT("Key code: %d, Mod: %d"), event.GetKeyCode(), event.GetModifiers()), 0);
	
	if(!mb->FindItem(ID_SHOW_DATE)->IsChecked())
	{
	    if(event.GetModifiers() == wxMOD_ALT && event.GetKeyCode() == 71)
	    {
		    Unbind(wxEVT_MOTION, &MyFrame::OnMouseMove, this);
			SetStatusText(wxDateTime::Now().FormatISOTime(), 1);	
	    }
	}
}

//---------------------------------------------------------------------------
// The key has been released!
//---------------------------------------------------------------------------
void MyFrame::OnKeyUp(wxKeyEvent& WXUNUSED(event))
{
	wxPoint mc = ScreenToClient(wxGetMousePosition());
	SetStatusText(wxString::Format(wxT("Cursor: %d, %d"), mc.x, mc.y), 1);
	Bind(wxEVT_MOTION, &MyFrame::OnMouseMove, this);
}