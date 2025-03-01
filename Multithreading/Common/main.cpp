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
#include <wx/fileconf.h>

#include "main.h"

const wxString appName = wxT("MT App 1.0.0");

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
	//
	ID_ADD_COUNT_THREAD,
	ID_ADD_COPY_THREAD,
	//
	ID_CLEAR_LOG
};

//---------------------------------------------------------------------------
// Event table
//---------------------------------------------------------------------------
BEGIN_EVENT_TABLE(MyFrame, wxFrame)
EVT_MENU(ID_CLOSE, MyFrame::OnClose)
EVT_MENU(ID_ABOUT_PROGRAM, MyFrame::OnAboutProgram)
EVT_CLOSE(MyFrame::CloseApp)
//
EVT_BUTTON(ID_ADD_COUNT_THREAD, MyFrame::OnAddCount)
EVT_BUTTON(ID_ADD_COPY_THREAD, MyFrame::OnAddCopy)
//
EVT_MENU(ID_CLEAR_LOG, MyFrame::OnClearLog)
END_EVENT_TABLE()

// Thread event definition
wxDEFINE_EVENT(wxEVT_MYTHREAD_UPDATED, wxThreadEvent);
wxDEFINE_EVENT(wxEVT_MYTHREAD_CLOSED, wxThreadEvent);

//---------------------------------------------------------------------------
// OnInit()
//---------------------------------------------------------------------------
bool MyApp::OnInit()
{
	wxInitAllImageHandlers();

	// Read the maximum number of supported threads from the configuration
	int max_threads = 0;
	wxFileConfig config(wxEmptyString, wxEmptyString, wxT("app.conf"), wxEmptyString, wxCONFIG_USE_RELATIVE_PATH);
	config.Read(wxT("App/MaxThreads"), &max_threads, 5);

	// Multithreading
	if (max_threads)
	{
		semaphore = new wxSemaphore(max_threads, max_threads);
	}
	else
	{
		semaphore = NULL;
	}

	mutex = new wxMutex();
	//

	MyFrame* frame = new MyFrame(NULL, wxID_ANY, appName, wxDefaultPosition, wxSize(600, 400), wxDEFAULT_FRAME_STYLE);
	frame->Show(true);
	return true;
}

int MyApp::OnExit()
{
	wxDELETE(semaphore);
	wxDELETE(mutex);
	return wxApp::OnExit();
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

	mFile->Append(ID_CLEAR_LOG, wxT("Clear log"));
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

	wxBoxSizer* main_sizer = new wxBoxSizer(wxVERTICAL);
	this->SetSizer(main_sizer);

	panel = new wxPanel(this);
	wxBoxSizer* panel_sizer = new wxBoxSizer(wxVERTICAL);
	panel->SetSizer(panel_sizer);

	//

	log = new wxTextCtrl(panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY);

	wxButton* count_btn = new wxButton(panel, ID_ADD_COUNT_THREAD, wxT("Countdown simulation (operation on the thread side + event)"));
	wxButton* copy_btn = new wxButton(panel, ID_ADD_COPY_THREAD, wxT("File copy simulation (calling MyFrame class method - risk of instability!)"));

	panel_sizer->Add(log, 1, wxEXPAND | wxALL, 0);
	panel_sizer->Add(count_btn, 0, wxEXPAND | wxALL, 0);
	panel_sizer->Add(copy_btn, 0, wxEXPAND | wxALL, 0);

	main_sizer->Add(panel, 1, wxEXPAND | wxALL, 0);

	CenterOnScreen();

	Log(wxT("Application ready to work..."));

	// Example of attaching thread event handlers
	Bind(wxEVT_MYTHREAD_UPDATED, &MyFrame::OnThreadUpdate, this, wxID_ANY);
	Bind(wxEVT_MYTHREAD_CLOSED, &MyFrame::OnThreadClose, this, wxID_ANY);
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
// Function displays the "About program..." window
//---------------------------------------------------------------------------
void MyFrame::OnAboutProgram(wxCommandEvent& event)
{
	wxMessageBox(appName + wxT("\nCopyright (c) 2015 by ..."), wxT("About the program..."), wxOK | wxICON_INFORMATION);

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
// Function adds a thread simulating some operation
//---------------------------------------------------------------------------
void MyFrame::OnAddCount(wxCommandEvent& WXUNUSED(event))
{
	MyCountThread* th = new MyCountThread(this);
	wxString err = wxGetApp().AddThread(th);
	if (!err.IsEmpty())
	{
		wxLogError(wxString::Format(wxT("An error occurred while creating the thread!\n\n%s"), err));
	}
	else
	{
		Log(wxString::Format(wxT("New thread (Countdown simulation) ID: %lld"), th->GetId()));
	}
}

//---------------------------------------------------------------------------
// Function adds a thread simulating file copying
//---------------------------------------------------------------------------
void MyFrame::OnAddCopy(wxCommandEvent& WXUNUSED(event))
{
	MyCopyThread* th = new MyCopyThread(this);
	wxString err = wxGetApp().AddThread(th);
	if (!err.IsEmpty())
	{
		wxLogError(wxString::Format(wxT("An error occurred while creating the thread!\n\n%s"), err));
	}
	else
	{
		Log(wxString::Format(wxT("New thread (File copy simulation) ID: %lld"), th->GetId()));
	}
}

//---------------------------------------------------------------------------
// Function handles thread update event
//---------------------------------------------------------------------------
void MyFrame::OnThreadUpdate(wxThreadEvent& event)
{
	Log(wxString::Format(wxT("[Thread ID: %lld] Thread event - thread updated: %d"), event.GetId(), (int)event.GetExtraLong()));
}

//---------------------------------------------------------------------------
// Function handles thread close event
//---------------------------------------------------------------------------
void MyFrame::OnThreadClose(wxThreadEvent& event)
{
	Log(wxString::Format(wxT("[Thread ID: %lld] Thread event - thread closed!"), event.GetId()));
}
