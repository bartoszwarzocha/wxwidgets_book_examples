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

#include <wx/valtext.h> // Text validator
#include <wx/valnum.h> // Numeric validator
#include <wx/valgen.h> // Generic validator

#include "my_validators.h"

#include "main.h"

const wxString appName = wxT("Validators");

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
	ID_CHECK_GENERIC_VALIDATOR
};

//---------------------------------------------------------------------------
// Event table
//---------------------------------------------------------------------------
BEGIN_EVENT_TABLE(MyFrame, wxFrame)
EVT_MENU(ID_CLOSE, MyFrame::OnClose)
EVT_MENU(ID_ABOUT_PROGRAM, MyFrame::OnAboutProgram)
EVT_CLOSE(MyFrame::CloseApp)
EVT_BUTTON(ID_CHECK_GENERIC_VALIDATOR, MyFrame::OnCheckGenericValidator)
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

	wxBoxSizer* ms = new wxBoxSizer(wxVERTICAL);
	this->SetSizer(ms);

	panel = new wxPanel(this);
	MyBoxSizer* ps = new MyBoxSizer(wxVERTICAL);
	ms->Add(panel, 1, wxEXPAND | wxALL, 0);

	// Text validation - alphanumeric characters
	textValidAN = new wxTextCtrl(panel, wxID_ANY, wxEmptyString);
	wxTextValidator val1(wxFILTER_ALPHANUMERIC);
	textValidAN->SetValidator(val1);
	ps->AddWithLabel(wxT("wxTextValidator (AN)"), 140, textValidAN);

	// Text validation - numeric characters
	textValidN = new wxTextCtrl(panel, wxID_ANY, wxEmptyString);
	wxTextValidator val2(wxFILTER_NUMERIC);
	textValidN->SetValidator(val2);
	ps->AddWithLabel(wxT("wxTextValidator (N)"), 140, textValidN);

	// Text validation - according to the list of accepted characters
	textValidInc = new wxTextCtrl(panel, wxID_ANY, wxEmptyString);
	wxTextValidator val3(wxFILTER_INCLUDE_CHAR_LIST);
	val3.SetCharIncludes(wxT("abcdefg.-"));
	textValidInc->SetValidator(val3);
	ps->AddWithLabel(wxT("wxTextValidator (Inc)"), 140, textValidInc);

	// Text validation - according to the list of excluded characters
	textValidExt = new wxTextCtrl(panel, wxID_ANY, wxEmptyString);
	wxTextValidator val4(wxFILTER_EXCLUDE_CHAR_LIST);
	val4.SetCharExcludes(wxT("*()-+qwerty"));
	textValidExt->SetValidator(val4);
	ps->AddWithLabel(wxT("wxTextValidator (Ext)"), 140, textValidExt);

	// Integer validation
	intValid = new wxTextCtrl(panel, wxID_ANY, wxEmptyString);
	int test_int; // Helper variable
	wxIntegerValidator<int> val5(&test_int, wxNUM_VAL_THOUSANDS_SEPARATOR);
	val5.SetRange(0, 10000);
	intValid->SetValidator(val5);
	ps->AddWithLabel(wxT("wxIntegerValidator"), 140, intValid);

	// Floating-point validation
	floatValid = new wxTextCtrl(panel, wxID_ANY, wxEmptyString);
	float test_float;
	wxFloatingPointValidator<float> val6(&test_float, wxNUM_VAL_DEFAULT);
	val6.SetMin(-100);
	val6.SetMax(100);
	floatValid->SetValidator(val6);
	ps->AddWithLabel(wxT("wxFloatingPointValidator"), 140, floatValid);

	// Generic validator
	wxButton* check_gen = new wxButton(panel, ID_CHECK_GENERIC_VALIDATOR, wxT("wxGenericValidator"));
	ps->Add(check_gen, 0, wxALL, 4);

	// Regular expressions
	regexValid = new wxTextCtrl(panel, wxID_ANY, wxEmptyString);
	wxString test_regex;
	//wxRegEx r(wxT("^([0-9]([0-9]([-]([0-9]([0-9]([0-9])?)?)?)?)?)?$"));
	//if (r.IsValid()) wxMessageBox("reg ex is OK");
	MyRegExValidator val7(wxT("^([0-9]([0-9]([-]([0-9]([0-9]([0-9])?)?)?)?)?)?$"), &test_regex);
	regexValid->SetValidator(val7);
	ps->AddWithLabel(wxT("Regular expressions"), 140, regexValid);

	// INFORMATION
	wxButton* info = new wxButton(panel, wxID_HIGHEST + 1, wxT("Information"));
	info->SetForegroundColour(*wxBLUE);
	this->Bind(wxEVT_BUTTON, [](wxCommandEvent&)
		{
			wxMessageBox(wxT(
				"INFORMATION\n\n" \
				"wxTextValidator (AN) - test of text validator with alphanumeric characters\n\n" \
				"wxTextValidator (N) - test of text validator with numeric characters\n\n" \
				"wxTextValidator (Inc) - test of text validator with the list of accepted characters: abcdefg.-\n\n" \
				"wxTextValidator (Ext) - test of text validator with the list of excluded characters: *()-+qwerty\n\n" \
				"wxIntegerValidator - test of integer validator (range: from 0 to 10000)\n\n" \
				"wxFloatingPointValidator - test of floating-point validator (range: from -100 to 100)\n\n" \
				"wxGenericValidator - test of generic validator\n\n" \
				"Regular expressions - test of custom validator with regular expression support (checking PL postal code - format XX-XXX)\n\n"
			));
		}, wxID_HIGHEST + 1);
	ps->Add(info, 0, wxEXPAND | wxALL, 4);

	panel->SetSizer(ps);

	this->Fit();

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
// Function displays "About the program..." window
//---------------------------------------------------------------------------
void MyFrame::OnAboutProgram(wxCommandEvent& WXUNUSED(event))
{
	wxMessageBox(appName + wxT("\nCopyright (c) 2025 by ..."), wxT("About the program..."), wxOK | wxICON_INFORMATION);
}

//---------------------------------------------------------------------------
// Function displays a prompt to close the program
//---------------------------------------------------------------------------
void MyFrame::CloseApp(wxCloseEvent& WXUNUSED(event))
{
	this->Destroy();
}

//---------------------------------------------------------------------------
// Function demonstrating the operation of the generic validator
//---------------------------------------------------------------------------
void MyFrame::OnCheckGenericValidator(wxCommandEvent& WXUNUSED(event))
{
	wxString test_str;
	wxGenericValidator val(&test_str);

	MySimpleTextDialog dlg(this, wxT("Enter text"), val);
	if (dlg.ShowModal() == wxID_OK)
	{
		wxMessageBox(wxT("Variable test_str:\n") + test_str);
	}
	else
	{
		wxMessageBox("Operation was cancelled");
	}
}
