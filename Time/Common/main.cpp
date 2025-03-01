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
#include <wx/calctrl.h>          // wxCalendarCtrl
#include <wx/generic/calctrlg.h> // wxGenericCalendarCtrl
#include <wx/datectrl.h>         // wxDatePickerCtrl
#include <wx/timectrl.h>         // wxTimePickerCtrl

#include "main.h"

#include "my_dt_format.h"

const wxString appName = wxT("Time 1.0.0");

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
	ID_PROCESS_FORMAT,
	ID_CALENDAR,
	ID_DATEPICKER,
	ID_TIMEPICKER
};

//---------------------------------------------------------------------------
// Event table
//---------------------------------------------------------------------------
BEGIN_EVENT_TABLE(MyFrame, wxFrame)
EVT_MENU(ID_CLOSE, MyFrame::OnClose)
EVT_MENU(ID_ABOUT_PROGRAM, MyFrame::OnAboutProgram)
EVT_CLOSE(MyFrame::CloseApp)
//
EVT_BUTTON(ID_PROCESS_FORMAT, MyFrame::OnProcessFormat)
//
EVT_CALENDAR(ID_CALENDAR, MyFrame::OnCalendar)
EVT_DATE_CHANGED(ID_DATEPICKER, MyFrame::OnDatePicker)
EVT_TIME_CHANGED(ID_TIMEPICKER, MyFrame::OnTimePicker)
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
	mFile->Append(ID_CLOSE, wxT("&Close program\tCtrl-X"), wxT("Closes the program"));

	mHelp->Append(ID_ABOUT_PROGRAM, wxT("&About\tF1"), wxT("About the program"));

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
	main_sizer->Add(panel, 1, wxEXPAND | wxALL, 0);

	format = new wxTextCtrl(panel, -1, wxT("$D $MMM $YYYY, zodiac sign: $z, time $h:$m:$s"));
	process_format = new wxButton(panel, ID_PROCESS_FORMAT, wxT("Process"));
	panel_sizer->Add(format, 0, wxEXPAND | wxALL, 4);
	panel_sizer->Add(process_format, 0, wxALL, 4);

	// CALENDARS
	cal = new wxCalendarCtrl(panel, ID_CALENDAR, wxDateTime::Today(), wxDefaultPosition, wxDefaultSize, wxCAL_MONDAY_FIRST);
	cal->EnableHolidayDisplay();
	cal->Mark(21, true);

	gcal = new wxGenericCalendarCtrl(panel, ID_CALENDAR, wxDateTime::Today(), wxDefaultPosition, wxDefaultSize, wxCAL_MONDAY_FIRST);
	gcal->EnableHolidayDisplay();
	gcal->Mark(21, true);

	// Additional settings for the generic control
	wxCalendarDateAttr* bc = new wxCalendarDateAttr(wxCAL_BORDER_ROUND, *wxBLUE);
	wxCalendarDateAttr* rs = new wxCalendarDateAttr(wxColour(255, 0, 255), wxNullColour, *wxRED, wxNullFont, wxCAL_BORDER_SQUARE);
	gcal->SetAttr(15, bc); // For the 10th day of the month
	gcal->SetAttr(20, rs); // For the 20th day of the month

	// Adding calendars to the GUI
	wxFlexGridSizer* cs = new wxFlexGridSizer(2, 2, 4, 20);
	cs->Add(new wxStaticText(panel, wxID_ANY, wxT("wxCalendarCtrl")));
	cs->Add(new wxStaticText(panel, wxID_ANY, wxT("wxGenericCalendarCtrl")));
	cs->Add(cal);
	cs->Add(gcal);
	panel_sizer->Add(cs, 0, wxEXPAND | wxALL, 4);

	// DATE PICKER
	dpicker = new wxDatePickerCtrl(panel, ID_DATEPICKER, wxDefaultDateTime, wxDefaultPosition, wxDefaultSize, wxDP_DROPDOWN);
	wxFlexGridSizer* dps = new wxFlexGridSizer(2, 1, 4, 0);
	dps->Add(new wxStaticText(panel, wxID_ANY, wxT("wxDatePickerCtrl")));
	dps->Add(dpicker);
	panel_sizer->Add(dps, 0, wxEXPAND | wxALL, 4);

	// TIME PICKER
	tpicker = new wxTimePickerCtrl(panel, ID_TIMEPICKER);
	wxFlexGridSizer* tps = new wxFlexGridSizer(2, 1, 4, 0);
	tps->Add(new wxStaticText(panel, wxID_ANY, wxT("wxTimePickerCtrl")));
	tps->Add(tpicker);
	panel_sizer->Add(tps, 0, wxEXPAND | wxALL, 4);

	CenterOnScreen();
	Fit();
}

//---------------------------------------------------------------------------
// MyFrame destructor
//------------------------------------------------------------------------
MyFrame::~MyFrame()
{

}

//---------------------------------------------------------------------------
// Function to initiate program closure
//---------------------------------------------------------------------------
void MyFrame::OnClose(wxCommandEvent& WXUNUSED(event))
{
	this->Close(true);
}

//---------------------------------------------------------------------------
// Function to display the "About..." window
//---------------------------------------------------------------------------
void MyFrame::OnAboutProgram(wxCommandEvent& WXUNUSED(event))
{
	wxMessageBox(appName + wxT("\nCopyright (c) 2025 by ..."), wxT("About..."), wxOK | wxICON_INFORMATION);
}

//---------------------------------------------------------------------------
// Function to display a prompt for program closure
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
// Function to process the date according to the defined format
//---------------------------------------------------------------------------
void MyFrame::OnProcessFormat(wxCommandEvent& WXUNUSED(event))
{
	wxMessageBox(wxFormatDateTime(wxDateTime::Now(), format->GetValue()));
}

//---------------------------------------------------------------------------
// Function to display the clicked date in one of the calendars
//---------------------------------------------------------------------------
void MyFrame::OnCalendar(wxCalendarEvent& event)
{
	wxMessageBox(wxFormatDateTime(event.GetDate(), format->GetValue()));
}

//---------------------------------------------------------------------------
// Function to display the date from the wxDatePickerCtrl in the status bar
//---------------------------------------------------------------------------
void MyFrame::OnDatePicker(wxDateEvent& event)
{
	SetStatusText(wxFormatDateTime(event.GetDate(), wxT("DatePicker: $D $MMM $YYYY")), 1);
}

//---------------------------------------------------------------------------
// Function to display the time from the wxTimePickerCtrl in the title bar
//---------------------------------------------------------------------------
void MyFrame::OnTimePicker(wxDateEvent& event)
{
	SetTitle(event.GetDate().FormatISOTime());
}
