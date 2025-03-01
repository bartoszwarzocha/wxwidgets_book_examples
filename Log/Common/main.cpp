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

#include <wx/stdpaths.h>
#include <wx/radiobox.h>

#include "main.h"

const wxString appName = wxT("wxWidgets Logging System Presentation 1.0.0");

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
	ID_DOLOGS,
	ID_INFO
};

//---------------------------------------------------------------------------
// Event table
//---------------------------------------------------------------------------
BEGIN_EVENT_TABLE(MyFrame, wxFrame)
EVT_MENU(ID_CLOSE, MyFrame::OnClose)
EVT_MENU(ID_ABOUT_PROGRAM, MyFrame::OnAboutProgram)
EVT_CLOSE(MyFrame::CloseApp)
EVT_BUTTON(ID_DOLOGS, MyFrame::OnDoLogs)
EVT_BUTTON(ID_INFO, MyFrame::OnInfo)
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
	: wxFrame(parent, id, title, pos, size, style), wnd(NULL)
{
	mb = new wxMenuBar();

	mFile = new wxMenu();
	mHelp = new wxMenu();


	mFile->AppendSeparator();
	mFile->Append(ID_CLOSE, wxT("&Close program\tCtrl-X"), wxT("Closes the program"));

	mHelp->Append(ID_ABOUT_PROGRAM, wxT("&About\tF1"), wxT("About the program"));

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

	//

	log = new wxTextCtrl(panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY);

	wxArrayString out_options;
	out_options.Add(wxT("wxLogBuffer"));
	out_options.Add(wxT("wxLogWindow"));
	out_options.Add(wxT("wxLogGui"));
	out_options.Add(wxT("wxLogTxtCtrl"));
	out = new wxRadioBox(panel, wxID_ANY, wxT(" Log output "), wxDefaultPosition, wxDefaultSize, out_options, 2, wxRA_SPECIFY_ROWS);

	wxButton* do_logs = new wxButton(panel, ID_DOLOGS, wxT("Generate logs"));
	wxButton* info = new wxButton(panel, ID_INFO, wxT("Information about selected class"));

	ps->Add(log, 1, wxEXPAND | wxALL, 4);
	ps->Add(out, 0, wxEXPAND | wxALL, 4);
	ps->Add(do_logs, 0, wxEXPAND | wxALL, 4);
	ps->Add(info, 0, wxEXPAND | wxLEFT | wxBOTTOM | wxRIGHT, 4);

	ms->Add(panel, 1, wxEXPAND | wxALL, 0);

	//

	CenterOnScreen();

	wxLog::SetLogLevel(wxLOG_Max);
	wxLog::SetVerbose(true);
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
// Function displays "About..." window
//---------------------------------------------------------------------------
void MyFrame::OnAboutProgram(wxCommandEvent& event)
{
	wxMessageBox(appName + wxT("\nCopyright (c) 2025 by ..."), wxT("About..."), wxOK | wxICON_INFORMATION);

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
// Function generates logs
//---------------------------------------------------------------------------
void MyFrame::OnDoLogs(wxCommandEvent& WXUNUSED(event))
{
	wxLog* prev_log = NULL;

	if (out->GetSelection() == 0)
	{
		prev_log = wxLog::SetActiveTarget(new wxLogBuffer());
	}
	else if (out->GetSelection() == 1)
	{
		if (!wnd) wnd = new wxLogWindow(this, wxT("Log"), true, true);
		else wnd->Show(true);
		prev_log = wxLog::SetActiveTarget(wnd);
	}
	else if (out->GetSelection() == 2)
	{
		prev_log = wxLog::SetActiveTarget(new wxLogGui());
	}
	else if (out->GetSelection() == 3)
	{
		prev_log = wxLog::SetActiveTarget(new wxLogTextCtrl(log));
	}

	wxLogMessage(wxT("Function wxLogMessage(): %d, %c, %f"), 15, 'c', 0.456);
	wxLogVerbose(wxT("Function wxLogVerbose(): %d, %c, %f"), 15, 'c', 0.456);
	wxLogWarning(wxT("Function wxLogWarning(): %d, %c, %f"), 15, 'c', 0.456);
	wxLogDebug(wxT("Function wxLogDebug(): %d, %c, %f"), 15, 'c', 0.456);
	wxLogStatus(wxT("Function wxLogStatus(): %d, %c, %f"), 15, 'c', 0.456);
	wxLogError(wxT("Function wxLogError(): %d, %c, %f"), 15, 'c', 0.456);

	if (prev_log != wnd && prev_log != NULL) wxDELETE(prev_log);
}

//---------------------------------------------------------------------------
// Function displays information about log types
//---------------------------------------------------------------------------
void MyFrame::OnInfo(wxCommandEvent& WXUNUSED(event))
{
	wxString info = wxEmptyString;

	switch (out->GetSelection())
	{
	case 0: // wxLogBuffer
		info = wxT("The wxLogBuffer class allows collecting logs into a special buffer, the contents of which can be read as a string. Try the buffer both when running the program normally and when running it in the console.");
		break;
	case 1: // wxLogWindow
		info = wxT("The wxLogWindow log window is one of the classes derived from wxLogChain. It allows displaying a parallel log in a separate window.");
		break;
	case 2: // wxLogGui
		info = wxT("The wxLogGui class is the standard log output for applications with a graphical user interface. The log is displayed in the form of a special dialog window.");
		break;
	case 3: // wxLogTextCtrl
		info = wxT("When using the wxLogTextCtrl class, the log output is redirected to the specified text control.");
		break;
	default: break;
	}

	wxMessageBox(info);
}
