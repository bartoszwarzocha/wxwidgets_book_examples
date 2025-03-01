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
#include <wx/tglbtn.h>
#include "wx/fs_zip.h"

#include <wx/textdlg.h>                    // Getting text
#include <wx/numdlg.h>                     // Getting numeric values
#include <wx/fontdlg.h>                    // Font selection
#include <wx/richtext/richtextsymboldlg.h> // Symbol selection
#include <wx/colordlg.h>                   // Color selection
#include <wx/choicdlg.h>                   // Option selection
#include <wx/rearrangectrl.h>              // Data arrangement
#include <wx/dirdlg.h>                     // Directory operations
#include <wx/filedlg.h>                    // File operations
#include <wx/msgdlg.h>                     // Informational dialogs
#include <wx/infobar.h>                    // Info bar
#include <wx/aboutdlg.h>                   // About dialogs
#include <wx/log.h>                        // Diagnostic window
#include <wx/progdlg.h>                    // Progress windows
#ifdef __WXMSW__
#include <wx/appprogress.h>                // Taskbar progress visualization
#endif
#include <wx/busyinfo.h>                   // Busy dialog
#include <wx/notifmsg.h>                   // System notifications
#include <wx/tipdlg.h>                     // "Tip of the day"
#include <wx/richtooltip.h>                // Enhanced tooltips
#include <wx/wxhtml.h>                     // wxHtmlHelpController class
#include <wx/propdlg.h>                    // Property sheets
#include <wx/fdrepdlg.h>                   // Find and replace dialog
#include <wx/printdlg.h>                   // Print dialog
#include <wx/wizard.h>                     // wxWizard

#include <wx/clrpicker.h>                  // wxColourPickerCtrl class
#include <wx/fontpicker.h>                 // wxFontPickerCtrl class
#include <wx/filepicker.h>                 // wxDirPickerCtrl and wxFilePickerCtrl classes

#include "main.h"

const wxString appName = wxT("Dialogs");

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
	ID_CLEAR,
	//
	ID_WXGETTEXTFROMUSER,
	ID_WXGETPASSWORDFROMUSER,
	ID_WXTEXTENTRYDIALOG,
	ID_WXPASSWORDENTRYDIALOG,
	ID_WXGETNUMBERFROMUSER,
	ID_WXGETFONTFROMUSER,
	ID_WXFONTDIALOG,
	ID_WXSYMBOLPICKERDIALOG,
	ID_WXGETCOLOURFROMUSER,
	ID_WXCOLOURDIALOG,
	ID_WXGETSINGLECHOICE1,
	ID_WXGETSINGLECHOICE2,
	ID_WXGETSINGLECHOICE3,
	ID_WXGETSINGLECHOICEDATA,
	ID_WXGETSINGLECHOICEINDEX,
	ID_WXSINGLECHOICEDIALOG,
	ID_WXGETSELECTEDCHOICES,
	ID_WXMULTICHOICEDIALOG,
	ID_WXREARRANGEDIALOG,
	ID_WXDIRSELECTOR,
	ID_WXDIRDIALOG,
	ID_WXFILESELECTOR,
	ID_WXFILESELECTOREX,
	ID_WXLOADFILESELECTOR,
	ID_WXSAVEFILESELECTOR,
	ID_WXFILEDIALOG,
	ID_WXFILEDIALOGEXTRACTRL,
	ID_WXMESSAGEDIALOG,
	ID_WXMESSAGEDIALOG2,
	ID_WXMESSAGEBOX,
	ID_WXINFOBAR,
	ID_WXABOUTBOX,
	ID_WXLOGMESSAGE,
	ID_WXPROGRESSDIALOG,
	ID_WXAPPPROGRESSINDICATOR,
	ID_WXBUSYCURSOR,
	ID_WXBUSYINFO1,
	ID_WXBUSYINFO2,
	ID_WXNOTIFICATIONMESSAGE,
	ID_REQUESTUSERATTENTION,
	ID_WXTIPPROVIDER,
	ID_WXRICHTOOLTIP,
	ID_WXHTMLHELPDIALOG,
	ID_WXPROPERTYSHEETDIALOG,
	ID_WXFINDREPLACEDIALOG1,
	ID_WXFINDREPLACEDIALOG2,
	ID_WXPRINTDIALOG,
	ID_WXWIZARD,
	//
	ID_MORE_INFO,
	//
	ID_WXCOLOURPICKERCTRL,
	ID_WXFONTPICKERCTRL,
	ID_WXDIRPICKERCTRL,
	ID_WXFILEPICKERCTRL,
};

//---------------------------------------------------------------------------
// Event table
//---------------------------------------------------------------------------
BEGIN_EVENT_TABLE(MyFrame, wxFrame)
EVT_MENU(ID_CLOSE, MyFrame::OnClose)                 // Close application
EVT_MENU(ID_CLEAR, MyFrame::OnClear)                 // Clear log window
EVT_CLOSE(MyFrame::CloseApp)                         // 'System' close application
//
EVT_MENU(ID_WXGETTEXTFROMUSER, MyFrame::On_wxGetTextFromUser)
EVT_MENU(ID_WXGETPASSWORDFROMUSER, MyFrame::On_wxGetPasswordFromUser)
EVT_MENU(ID_WXTEXTENTRYDIALOG, MyFrame::On_wxTextEntryDialog)
EVT_MENU(ID_WXPASSWORDENTRYDIALOG, MyFrame::On_wxPasswordEntryDialog)
EVT_MENU(ID_WXGETNUMBERFROMUSER, MyFrame::On_wxGetNumberFromUser)
EVT_MENU(ID_WXGETFONTFROMUSER, MyFrame::On_wxGetFontFromUser)
EVT_MENU(ID_WXFONTDIALOG, MyFrame::On_wxFontDialog)
EVT_MENU(ID_WXSYMBOLPICKERDIALOG, MyFrame::On_wxSymbolPickerDialog)
EVT_MENU(ID_WXGETCOLOURFROMUSER, MyFrame::On_wxGetColourFromUser)
EVT_MENU(ID_WXCOLOURDIALOG, MyFrame::On_wxColourDialog)
EVT_MENU(ID_WXGETSINGLECHOICE1, MyFrame::On_wxGetSingleChoice1)
EVT_MENU(ID_WXGETSINGLECHOICE2, MyFrame::On_wxGetSingleChoice2)
EVT_MENU(ID_WXGETSINGLECHOICE3, MyFrame::On_wxGetSingleChoice3)
EVT_MENU(ID_WXGETSINGLECHOICEDATA, MyFrame::On_wxGetSingleChoiceData)
EVT_MENU(ID_WXGETSINGLECHOICEINDEX, MyFrame::On_wxGetSingleChoiceIndex)
EVT_MENU(ID_WXSINGLECHOICEDIALOG, MyFrame::On_wxSingleChoiceDialog)
EVT_MENU(ID_WXGETSELECTEDCHOICES, MyFrame::On_wxGetSelectedChoices)
EVT_MENU(ID_WXMULTICHOICEDIALOG, MyFrame::On_wxMultiChoiceDialog)
EVT_MENU(ID_WXREARRANGEDIALOG, MyFrame::On_wxRearrangeDialog)
EVT_MENU(ID_WXDIRSELECTOR, MyFrame::On_wxDirSelector)
EVT_MENU(ID_WXDIRDIALOG, MyFrame::On_wxDirDialog)
EVT_MENU(ID_WXFILESELECTOR, MyFrame::On_wxFileSelector)
EVT_MENU(ID_WXFILESELECTOREX, MyFrame::On_wxFileSelectorEx)
EVT_MENU(ID_WXLOADFILESELECTOR, MyFrame::On_wxLoadFileSelector)
EVT_MENU(ID_WXSAVEFILESELECTOR, MyFrame::On_wxSaveFileSelector)
EVT_MENU(ID_WXFILEDIALOG, MyFrame::On_wxFileDialog)
EVT_MENU(ID_WXFILEDIALOGEXTRACTRL, MyFrame::On_wxFileDialogExtraCtrl)
EVT_MENU(ID_WXMESSAGEDIALOG, MyFrame::On_wxMessageDialog)
EVT_MENU(ID_WXMESSAGEDIALOG2, MyFrame::On_wxMessageDialog2)
EVT_MENU(ID_WXMESSAGEBOX, MyFrame::On_wxMessageBox)
EVT_MENU(ID_WXINFOBAR, MyFrame::On_wxInfoBar)
EVT_MENU(ID_WXABOUTBOX, MyFrame::On_wxAboutBox)
EVT_MENU(ID_WXLOGMESSAGE, MyFrame::On_wxLogMessage)
EVT_MENU(ID_WXPROGRESSDIALOG, MyFrame::On_wxProgressDialog)
EVT_MENU(ID_WXAPPPROGRESSINDICATOR, MyFrame::On_wxAppProgressIndicator)
EVT_MENU(ID_WXBUSYCURSOR, MyFrame::On_wxBusyCursor)
EVT_MENU(ID_WXBUSYINFO1, MyFrame::On_wxBusyInfo1)
EVT_MENU(ID_WXBUSYINFO2, MyFrame::On_wxBusyInfo2)
EVT_MENU(ID_WXNOTIFICATIONMESSAGE, MyFrame::On_wxNotificationMessage)
EVT_MENU(ID_REQUESTUSERATTENTION, MyFrame::On_RequestUserAttention)
EVT_MENU(ID_WXTIPPROVIDER, MyFrame::On_wxTipProvider)
EVT_MENU(ID_WXRICHTOOLTIP, MyFrame::On_wxRichToolTip)
EVT_MENU(ID_WXHTMLHELPDIALOG, MyFrame::On_wxHtmlHelpDialog)
EVT_MENU(ID_WXPROPERTYSHEETDIALOG, MyFrame::On_wxPropertySheetDialog)
EVT_MENU(ID_WXFINDREPLACEDIALOG1, MyFrame::On_wxFindReplaceDialog1)
EVT_MENU(ID_WXFINDREPLACEDIALOG2, MyFrame::On_wxFindReplaceDialog2)
EVT_MENU(ID_WXPRINTDIALOG, MyFrame::On_wxPrintDialog)
EVT_MENU(ID_WXWIZARD, MyFrame::On_wxWizard)
//
EVT_COLOURPICKER_CHANGED(ID_WXCOLOURPICKERCTRL, MyFrame::On_wxColourPickerCtrl)
EVT_FONTPICKER_CHANGED(ID_WXFONTPICKERCTRL, MyFrame::On_wxFontPickerCtrl)
EVT_DIRPICKER_CHANGED(ID_WXDIRPICKERCTRL, MyFrame::On_wxDirPickerCtrl)
EVT_FILEPICKER_CHANGED(ID_WXFILEPICKERCTRL, MyFrame::On_wxFilePickerCtrl)
END_EVENT_TABLE()

//---------------------------------------------------------------------------
// OnInit()
//---------------------------------------------------------------------------
bool MyApp::OnInit()
{
	wxInitAllImageHandlers();
	wxFileSystem::AddHandler(new wxZipFSHandler);

	// POLISH LANGUAGE FOR DIALOG WINDOWS
	// Setting Polish language for dialog window buttons.
	// By default, windows have buttons with labels in English.
	// Polish language is loaded from the pl/pl.mo file.
	// More about this can be found in the fourth part of the book.
	
	// DISABLED FOR NOW
	//loc.Init(wxLANGUAGE_POLISH, wxLOCALE_LOAD_DEFAULT);
	//loc.AddCatalog("pl");

	MyFrame* frame = new MyFrame(NULL, wxID_ANY, appName, wxDefaultPosition, wxSize(800, 600), wxDEFAULT_FRAME_STYLE);
	frame->Show(true);
	return true;
}

//---------------------------------------------------------------------------
// MyFrame Constructor
//---------------------------------------------------------------------------
MyFrame::MyFrame(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
	: wxFrame(parent, id, title, pos, size, style)
{
	this->SetIcon(wxIcon(wxT("gfx/run.png"), wxBITMAP_TYPE_PNG));

	// MENU BAR
	mb = new wxMenuBar();

	mFile = new wxMenu();
	mDialogs = new wxMenu();
	mHelp = new wxMenu();

	// File Menu
	mFile->Append(ID_CLEAR, wxT("Clear log window"), wxT("Clear log"));
	mFile->Append(ID_CLOSE, wxT("&Close application\tCtrl-Shift-X"), wxT("Close application"));

	// Dialogs Menu
	mDialogs->Append(ID_WXGETTEXTFROMUSER, wxT("Function wxGetTextFromUser()"));
	mDialogs->Append(ID_WXGETPASSWORDFROMUSER, wxT("Function wxGetPasswordFromUser()"));
	mDialogs->Append(ID_WXTEXTENTRYDIALOG, wxT("Class wxTextEntryDialog"));
	mDialogs->Append(ID_WXPASSWORDENTRYDIALOG, wxT("Class wxPasswordEntryDialog"));
	mDialogs->Append(ID_WXGETNUMBERFROMUSER, wxT("Function wxGetNumberFromUser()"));
	mDialogs->AppendSeparator();
	mDialogs->Append(ID_WXGETFONTFROMUSER, wxT("Function wxGetFontFromUser()"));
	mDialogs->Append(ID_WXFONTDIALOG, wxT("Class wxFontDialog"));
	mDialogs->Append(ID_WXSYMBOLPICKERDIALOG, wxT("Class wxSymbolPickerDialog"));
	mDialogs->Append(ID_WXGETCOLOURFROMUSER, wxT("Function wxGetColourFromUser()"));
	mDialogs->Append(ID_WXCOLOURDIALOG, wxT("Class wxColourDialog"));
	mDialogs->Append(ID_WXGETSINGLECHOICE1, wxT("Function wxGetSingleChoice() - version 1"));
	mDialogs->Append(ID_WXGETSINGLECHOICE2, wxT("Function wxGetSingleChoice() - version 2"));
	mDialogs->Append(ID_WXGETSINGLECHOICE3, wxT("Function wxGetSingleChoice() - version 3"));
	mDialogs->Append(ID_WXGETSINGLECHOICEDATA, wxT("Function wxGetSingleChoiceData()"));
	mDialogs->Append(ID_WXGETSINGLECHOICEINDEX, wxT("Function wxGetSingleChoiceIndex()"));
	mDialogs->Append(ID_WXSINGLECHOICEDIALOG, wxT("Class wxSingleChoiceDialog"));
	mDialogs->Append(ID_WXGETSELECTEDCHOICES, wxT("Function wxGetSelectedChoices()"));
	mDialogs->Append(ID_WXMULTICHOICEDIALOG, wxT("Class wxMultiChoiceDialog"));
	mDialogs->Append(ID_WXREARRANGEDIALOG, wxT("Class wxRearrangeDialog"));
	mDialogs->AppendSeparator();
	mDialogs->Append(ID_WXDIRSELECTOR, wxT("Function wxDirSelector()"));
	mDialogs->Append(ID_WXDIRDIALOG, wxT("Class wxDirDialog"));
	mDialogs->Append(ID_WXFILESELECTOR, wxT("Function wxFileSelector()"));
	mDialogs->Append(ID_WXFILESELECTOREX, wxT("Function wxFileSelectorEx()"));
	mDialogs->Append(ID_WXLOADFILESELECTOR, wxT("Function wxLoadFileSelector()"));
	mDialogs->Append(ID_WXSAVEFILESELECTOR, wxT("Function wxSaveFileSelector()"));
	mDialogs->Append(ID_WXFILEDIALOG, wxT("Class wxFileDialog"));
	mDialogs->Append(ID_WXFILEDIALOGEXTRACTRL, wxT("Class wxFileDialog (extra controls)"));
	mDialogs->AppendSeparator();
	mDialogs->Append(ID_WXMESSAGEDIALOG, wxT("Class wxMessageDialog"));
	mDialogs->Append(ID_WXMESSAGEDIALOG2, wxT("Class wxMessageDialog - extended mode"));
	mDialogs->Append(ID_WXMESSAGEBOX, wxT("Function wxMessageBox()"));
	mDialogs->Append(ID_WXINFOBAR, wxT("Class wxInfoBar"));
	mDialogs->Append(ID_WXLOGMESSAGE, wxT("Diagnostic messages window"));
	mDialogs->Append(ID_WXPROGRESSDIALOG, wxT("Class wxProgressDialog"));
	mDialogs->Append(ID_WXAPPPROGRESSINDICATOR, wxT("Class wxAppProgressIndicator"));
	mDialogs->Append(ID_WXBUSYCURSOR, wxT("Busy state visualization by cursor"));
	mDialogs->Append(ID_WXBUSYINFO1, wxT("Class wxBusyInfo - basic mode"));
	mDialogs->Append(ID_WXBUSYINFO2, wxT("Class wxBusyInfo - extended mode"));
	mDialogs->Append(ID_WXNOTIFICATIONMESSAGE, wxT("Class wxNotificationMessage"));
	mDialogs->Append(ID_REQUESTUSERATTENTION, wxT("Function wxTopLevelWindow::RequestUserAttention()"));
	mDialogs->Append(ID_WXTIPPROVIDER, wxT("Class wxTipProvider"));
	mDialogs->Append(ID_WXRICHTOOLTIP, wxT("Class wxRichToolTip"));
	mDialogs->Append(ID_WXHTMLHELPDIALOG, wxT("Class wxHtmlHelpDialog"));
	mDialogs->AppendSeparator();
	mDialogs->Append(ID_WXPROPERTYSHEETDIALOG, wxT("Class wxPropertySheetDialog"));
	mDialogs->Append(ID_WXFINDREPLACEDIALOG1, wxT("Class wxFindReplaceDialog (find)"));
	mDialogs->Append(ID_WXFINDREPLACEDIALOG2, wxT("Class wxFindReplaceDialog (replace)"));
	mDialogs->Append(ID_WXPRINTDIALOG, wxT("Class wxPrintDialog"));
	mDialogs->Append(ID_WXWIZARD, wxT("Class wxWizard"));

	// Help Menu
	mHelp->Append(ID_WXABOUTBOX, wxT("Function wxAboutBox()\tF1"));

	mb->Append(mFile, wxT("&File"));
	mb->Append(mDialogs, wxT("&Power of Dialog"));
	mb->Append(mHelp, wxT("&Help"));

	this->SetMenuBar(mb);

	wxBoxSizer* ms = new wxBoxSizer(wxVERTICAL);
	this->SetSizer(ms);

	panel = new wxPanel(this);
	wxBoxSizer* ps = new wxBoxSizer(wxVERTICAL);
	panel->SetSizer(ps);
	ms->Add(panel, 1, wxEXPAND | wxALL, 0);

	log = new wxTextCtrl(panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY | wxTE_MULTILINE);
	wxLog::SetActiveTarget(new wxLogTextCtrl(log));
	ps->Add(log, 1, wxEXPAND | wxALL, 0);

	infobar = new wxInfoBar(panel);
	infobar->AddButton(ID_MORE_INFO, wxT("More information"));
	infobar->Connect(ID_MORE_INFO, wxEVT_BUTTON, wxCommandEventHandler(MyFrame::On_wxInfoBarMoreInfo), NULL, this);
	infobar->AddButton(wxID_CANCEL, wxT("Hide"));
	ps->Add(infobar, 0, wxEXPAND | wxALL, 0);

	// wxPickerBase controls *****
	wxBoxSizer* picker_s = new wxBoxSizer(wxHORIZONTAL);

	copicker = new wxColourPickerCtrl(panel, ID_WXCOLOURPICKERCTRL, log->GetForegroundColour());
	fopicker = new wxFontPickerCtrl(panel, ID_WXFONTPICKERCTRL, log->GetFont());
	dipicker = new wxDirPickerCtrl(panel, ID_WXDIRPICKERCTRL);
	fipicker = new wxFilePickerCtrl(panel, ID_WXFILEPICKERCTRL, wxT("tips.txt"));

	picker_s->Add(new wxStaticText(panel, wxID_ANY, wxT("wxPickerBase:")), 0, wxALL | wxALIGN_CENTER_VERTICAL, 4);
	picker_s->Add(copicker, 0, wxALL | wxALIGN_CENTER_VERTICAL, 4);
	picker_s->Add(fopicker, 0, wxALL | wxALIGN_CENTER_VERTICAL, 4);
	picker_s->Add(dipicker, 0, wxALL | wxALIGN_CENTER_VERTICAL, 4);
	picker_s->Add(fipicker, 0, wxALL | wxALIGN_CENTER_VERTICAL, 4);
	ps->Add(picker_s, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 0);

	//
	_LOG(wxT("Program ready to work..."));
	_LOG(wxT("Press Ctrl+Alt+Middle Mouse Button to display the dialog with wxWidgets version information..."));

	CenterOnScreen();

	this->find_dlg = NULL;
}

//---------------------------------------------------------------------------
// MyFrame Destructor
//-------------------------------------------------------------------------
MyFrame::~MyFrame()
{

}

//---------------------------------------------------------------------------
// Function initiates application closure
//---------------------------------------------------------------------------
void MyFrame::OnClose(wxCommandEvent& WXUNUSED(event))
{
	this->Close(true);
}

//---------------------------------------------------------------------------
// Function displays a prompt to close the application
//---------------------------------------------------------------------------
void MyFrame::CloseApp(wxCloseEvent& WXUNUSED(event))
{
	int action = wxMessageBox(wxT("Do you want to close the application?"), wxT("Question"), wxOK | wxCANCEL | wxICON_QUESTION);

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
// Function clears the log window
//---------------------------------------------------------------------------
void MyFrame::OnClear(wxCommandEvent& WXUNUSED(event))
{
	log->Clear();
}

//---------------------------------------------------------------------------
// Functions displaying subsequent dialog windows
//---------------------------------------------------------------------------
void MyFrame::On_wxGetTextFromUser(wxCommandEvent& WXUNUSED(event))
{
	wxString user_str;
	user_str = wxGetTextFromUser(wxT("Enter name"), wxT("Name"));
	if (!user_str.IsEmpty())
	{
		// Do something with user_str...
		_LOG(user_str);
	}
}

void MyFrame::On_wxGetPasswordFromUser(wxCommandEvent& WXUNUSED(event))
{
	wxString password;
	password = wxGetPasswordFromUser(wxT("Enter password"), wxT("Password protected access"));
	if (!password.IsEmpty())
	{
		// Do something with the password...
		_LOG(password); // Display in log for testing purposes only :)
	}
}

void MyFrame::On_wxTextEntryDialog(wxCommandEvent& WXUNUSED(event))
{
	wxTextEntryDialog dlg(this, wxT("Enter text..."), wxT("Text"), wxEmptyString, wxOK | wxCANCEL | wxTE_MULTILINE);
	dlg.SetMaxLength(250); // Length 250 characters
	dlg.SetTextValidator(wxFILTER_ALPHANUMERIC); // Only alphanumeric characters (no spaces!)
	if (dlg.ShowModal() == wxID_OK)
	{
		// You can add a check if the string is empty...
		_LOG(dlg.GetValue());
	}
}

void MyFrame::On_wxPasswordEntryDialog(wxCommandEvent& WXUNUSED(event))
{
	wxPasswordEntryDialog dlg(this, wxT("Enter password..."), wxT("Password protected access"));
	dlg.SetMaxLength(16); // Max password length is 16 characters
	if (dlg.ShowModal() == wxID_OK)
	{
		// Some operations with the password...
		_LOG(dlg.GetValue()); // Display in log for testing purposes only
	}
}

void MyFrame::On_wxGetNumberFromUser(wxCommandEvent& WXUNUSED(event))
{
	long a = wxGetNumberFromUser(wxT("Enter value..."), wxT("New value"), wxT("Change value"), 0, 0, 1000);
	if (a > -1) _LOG(wxString::Format("%ld", a));
}

void MyFrame::On_wxGetFontFromUser(wxCommandEvent& WXUNUSED(event))
{
	wxFont new_font = wxGetFontFromUser(this, log->GetFont(), wxT("Select new font..."));
	if (new_font.IsOk())
	{
		log->SetFont(new_font);

		// Update wxPickerBase controls
		fopicker->SetSelectedFont(new_font);

		_LOG(wxString::Format("New font: %s (%s, %d pt.)", new_font.GetFaceName(), new_font.GetStyleString(), new_font.GetPointSize()));
	}
}

void MyFrame::On_wxFontDialog(wxCommandEvent& WXUNUSED(event))
{
	wxFontData data;
	data.SetInitialFont(log->GetFont());
	data.SetColour(log->GetForegroundColour());

	wxFontDialog dlg(this, data);
	if (dlg.ShowModal() == wxID_OK)
	{
		data = dlg.GetFontData();
		wxFont new_font = data.GetChosenFont();
		if (new_font.IsOk())
		{
			log->SetFont(new_font);
			log->SetForegroundColour(data.GetColour());

			// Update wxPickerBase controls
			fopicker->SetSelectedFont(new_font);
			fopicker->SetSelectedColour(data.GetColour());
			copicker->SetColour(data.GetColour());

			_LOG(wxString::Format("New font: %s (%s, %d pt.)", new_font.GetFaceName(), new_font.GetStyleString(), new_font.GetPointSize()));
		}
	}
}

void MyFrame::On_wxSymbolPickerDialog(wxCommandEvent& WXUNUSED(event))
{
	wxSymbolPickerDialog dlg(wxEmptyString, log->GetFont().GetFaceName(), wxEmptyString, this);
	if (dlg.ShowModal() == wxID_OK)
	{
		_LOG(wxString::Format("Selected symbol: %s", dlg.GetSymbol()));
	}
}

void MyFrame::On_wxGetColourFromUser(wxCommandEvent& WXUNUSED(event))
{
	wxColourData data;
	data.SetCustomColour(0, wxColour(255, 50, 50)); // Custom color 1
	data.SetCustomColour(1, wxColour(0, 128, 128)); // Custom color 2
	data.SetCustomColour(2, wxColour(128, 0, 128)); // Custom color 3

#if __WXMSW__
	data.SetChooseFull(true);
#endif

	wxColour col = wxGetColourFromUser(this, log->GetForegroundColour(), wxT("Select color"), &data);

	log->SetForegroundColour(col);

	// Update wxPickerBase controls
	copicker->SetColour(col);
	fopicker->SetSelectedColour(col);

	wxString col_str = wxT("New color: ") + col.GetAsString();
	_LOG(col_str);
}

void MyFrame::On_wxColourDialog(wxCommandEvent& WXUNUSED(event))
{
	wxColourData data;
	data.SetColour(log->GetForegroundColour());     // Default color
	data.SetCustomColour(0, wxColour(150, 30, 55)); // Custom color 1
	data.SetCustomColour(1, wxColour(0, 255, 120)); // Custom color 2
	data.SetCustomColour(2, wxColour(128, 0, 128)); // Custom color 3

#if __WXMSW__
	data.SetChooseFull(true);
#endif

	wxColourDialog dlg(this, &data);
	if (dlg.ShowModal() == wxID_OK)
	{
		data = dlg.GetColourData();
		log->SetForegroundColour(data.GetColour());

		// Update wxPickerBase controls
		copicker->SetColour(data.GetColour());
		fopicker->SetSelectedColour(data.GetColour());

		wxString col_str = wxT("New color: ") + data.GetColour().GetAsString();
		_LOG(col_str);
	}
}

void MyFrame::On_wxGetSingleChoice1(wxCommandEvent& WXUNUSED(event))
{
	wxArrayString arr;
	arr.Add(wxT("Radish"));
	arr.Add(wxT("Cottage cheese"));
	arr.Add(wxT("Chives"));

	wxString choice = wxGetSingleChoice(wxT("What do you choose?"), wxT("Breakfast"), arr, 0, this);
	if (!choice.IsEmpty())
	{
		_LOG(choice);
	}
}

void MyFrame::On_wxGetSingleChoice2(wxCommandEvent& WXUNUSED(event))
{
	wxArrayString arr;
	arr.Add(wxT("Pork chop with fries"));
	arr.Add(wxT("Russian dumplings"));
	arr.Add(wxT("Rotisserie chicken"));

	wxString choice = wxGetSingleChoice(wxT("What do you choose?"), wxT("Lunch"), arr, this, 25, 25, false, 300, 200, 0);
	if (!choice.IsEmpty())
	{
		_LOG(choice);
	}
}

void MyFrame::On_wxGetSingleChoice3(wxCommandEvent& WXUNUSED(event))
{
	wxString arr[3] = { wxT("Toast"), wxT("Apple"), wxT("Kissel") };

	wxString choice = wxGetSingleChoice(wxT("What do you choose?"), wxT("Dinner"), 3, arr, 0, this);
	if (!choice.IsEmpty())
	{
		_LOG(choice);
	}
}

void MyFrame::On_wxGetSingleChoiceData(wxCommandEvent& WXUNUSED(event))
{
	const unsigned int PEOPLE_COUNT = 3;

	Person* people[PEOPLE_COUNT];
	people[0] = new Person(wxT("John"), wxT("Smith"), wxT("123456789"));
	people[1] = new Person(wxT("Anna"), wxT("Nowak"), wxT("987654321"));
	people[2] = new Person(wxT("Zygmunt"), wxT("Gil"), wxT("123123123"));

	// Selection options
	wxArrayString names;
	names.Add(people[0]->name);
	names.Add(people[1]->name);
	names.Add(people[2]->name);

	Person* choice = static_cast<Person*>(wxGetSingleChoiceData(wxT("Choose..."), wxT("People"), names, (void**)people, 0, this));
	if (choice)
	{
		_LOG(wxString::Format(wxT("%s %s, Tel: %s"), choice->name, choice->surname, choice->phone));
	}

	for (unsigned int i = 0; i < PEOPLE_COUNT; i++) { delete people[i]; }
}

void MyFrame::On_wxGetSingleChoiceIndex(wxCommandEvent& WXUNUSED(event))
{
	wxArrayString options;
	options.Add(wxT("Option 1"));
	options.Add(wxT("Option 2"));
	options.Add(wxT("Option 3"));

	int index = wxGetSingleChoiceIndex(wxT("Choose an option"), wxT("Options"), options, 0, this);
	if (index > -1)
	{
		_LOG(wxString::Format(wxT("Index: %d"), index));
	}
}

void MyFrame::On_wxSingleChoiceDialog(wxCommandEvent& WXUNUSED(event))
{
	const unsigned int PEOPLE_COUNT = 3;

	Person* people[PEOPLE_COUNT];
	people[0] = new Person(wxT("Wojtek"), wxT("Jankowski"), wxT("029265747"));
	people[1] = new Person(wxT("Sergiusz"), wxT("Nowak"), wxT("987984321"));
	people[2] = new Person(wxT("Ildefons"), wxT("Prus"), wxT("010101010"));

	// Selection options
	wxArrayString names;
	names.Add(people[0]->name);
	names.Add(people[1]->name);
	names.Add(people[2]->name);

	wxSingleChoiceDialog dlg(this, wxT("Choose a person..."), wxT("Candidates"), names, (void**)people);
	if (dlg.ShowModal() == wxID_OK)
	{
		wxString choice = dlg.GetStringSelection();
		int index = dlg.GetSelection();
		Person* p = static_cast<Person*>(dlg.GetSelectionData());

		_LOG(wxString::Format(wxT("\nSelected label: %s\nIndex: %d\nDetails: %s %s, Tel: %s"), choice, index, p->name, p->surname, p->phone));
	}

	for (unsigned int i = 0; i < PEOPLE_COUNT; i++) { delete people[i]; }
}

void MyFrame::On_wxGetSelectedChoices(wxCommandEvent& WXUNUSED(event))
{
	wxArrayString options;
	options.Add(wxT("Scrambled eggs"));
	options.Add(wxT("Orange juice"));
	options.Add(wxT("Roll"));
	options.Add(wxT("Cottage cheese"));
	options.Add(wxT("Banana"));

	wxArrayInt results;
	wxGetSelectedChoices(results, wxT("What will you have for breakfast?"), wxT("Menu"), options, this);

	if (results.size() > 0)
	{
		wxString msg = wxT("Selected options: ");
		for (int x : results) msg += wxString::Format("%d, ", x);
		_LOG(msg);
	}
}

void MyFrame::On_wxMultiChoiceDialog(wxCommandEvent& WXUNUSED(event))
{
	wxArrayString options;
	options.Add(wxT("Pork chop"));
	options.Add(wxT("Fries"));
	options.Add(wxT("Salad"));
	options.Add(wxT("Compote"));

	wxMultiChoiceDialog dlg(this, wxT("What will you have for lunch?"), wxT("Menu"), options);
	if (dlg.ShowModal() == wxID_OK)
	{
		wxArrayInt results = dlg.GetSelections();

		if (results.size() > 0)
		{
			wxString msg = wxT("Selected options: ");
			for (int x : results) msg += wxString::Format("%d, ", x);
			_LOG(msg);
		}
	}
}

void MyFrame::On_wxRearrangeDialog(wxCommandEvent& WXUNUSED(event))
{
	wxArrayString options;
	options.Add(wxT("Pork chop"));
	options.Add(wxT("Fries"));
	options.Add(wxT("Salad"));
	options.Add(wxT("Compote"));

	wxArrayInt order;
	order.Add(0);
	order.Add(1);
	order.Add(2);
	order.Add(3);

	wxRearrangeDialog dlg(this, wxT("Set menu..."), wxT("Menu"), order, options);

	wxCheckBox* cb = new wxCheckBox(&dlg, wxID_ANY, wxT(" Sample switch"));
	dlg.AddExtraControls(cb);

	if (dlg.ShowModal() == wxID_OK)
	{
		order = dlg.GetOrder();

		wxString msg = wxT("Order change: 0123 -> ");
		for (int x : order) msg += wxString::Format("%d", x);
		msg += wxString::Format(wxT(" - switch: %d"), cb->IsChecked());
		_LOG(msg);
	}
}

void MyFrame::On_wxDirSelector(wxCommandEvent& WXUNUSED(event))
{
	wxString dir = wxDirSelector(wxT("Select folder"));
	if (!dir.IsEmpty())
	{
		_LOG(dir);
	}
}

void MyFrame::On_wxDirDialog(wxCommandEvent& WXUNUSED(event))
{
	wxDirDialog dlg(this, wxT("Select folder"), wxEmptyString, wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);
	if (dlg.ShowModal() == wxID_OK)
	{
		_LOG(dlg.GetPath());
	}
}

void MyFrame::On_wxFileSelector(wxCommandEvent& WXUNUSED(event))
{
	wxString file = wxFileSelector(wxT("Save file"), wxEmptyString, wxEmptyString, wxT("txt"), wxT("All files|*.*|Text file (*.txt)|*.txt|"), wxFD_SAVE);
	if (!file.IsEmpty())
	{
		_LOG(file);
	}
}

void MyFrame::On_wxFileSelectorEx(wxCommandEvent& WXUNUSED(event))
{
	int type = -1;
	wxString file = wxFileSelectorEx(wxT("Open file"), wxEmptyString, wxEmptyString, &type, wxT("All files|*.*|Text file (*.txt)|*.txt|Image file (*.jpg)|*.jpg"), wxFD_OPEN);
	if (!file.IsEmpty())
	{
		wxString msg = file + wxString::Format(wxT(", Applied filter: %d"), type);
		_LOG(msg);
	}
}

void MyFrame::On_wxLoadFileSelector(wxCommandEvent& WXUNUSED(event))
{
	wxString file = wxLoadFileSelector(wxT("Open file"), wxT("txt"), wxEmptyString, this);
	if (!file.IsEmpty())
	{
		if (log->LoadFile(file))
		{
			_LOG(wxString::Format(wxT(">>> Successfully loaded: %s"), file));
		}
		else
		{
			_LOG(wxT("File open error!"));
		}
	}
}

void MyFrame::On_wxSaveFileSelector(wxCommandEvent& WXUNUSED(event))
{
	wxString file = wxSaveFileSelector(wxT("Save file"), wxT("txt"), wxEmptyString, this);
	if (!file.IsEmpty())
	{
		if (log->SaveFile(file))
		{
			_LOG(wxString::Format(wxT(">>> Successfully saved: %s"), file));
		}
		else
		{
			_LOG(wxT("File save error!"));
		}
	}
}

void MyFrame::On_wxFileDialog(wxCommandEvent& WXUNUSED(event))
{
	wxArrayString paths;

	wxFileDialog dlg(this, wxT("Select files"), wxEmptyString, wxEmptyString, wxEmptyString, wxFD_OPEN | wxFD_MULTIPLE);
	if (dlg.ShowModal() == wxID_OK)
	{
		dlg.GetPaths(paths);

		wxString msg = wxT("Selected files:\n");
		for (wxString x: paths) msg += (x + "\n");
		_LOG(msg);
	}
}

// Additional controls for wxFileDialog window
static wxWindow* CreateFileDialogExtraCtrls(wxWindow *parent)
{
	return new FileDialogExtraPanel(parent);
}

void MyFrame::On_wxFileDialogExtraCtrl(wxCommandEvent& WXUNUSED(event))
{
	wxFileDialog dlg(this, wxT("Save file..."), "", "", "", wxFD_SAVE);
	dlg.SetExtraControlCreator(&CreateFileDialogExtraCtrls); // Must be before ShowModal() !!!

	if (dlg.ShowModal() == wxID_OK)
	{
		wxString path = dlg.GetPath();
		if (!path.IsEmpty())
		{
			wxString msg = wxEmptyString;
			FileDialogExtraPanel* extra_panel = static_cast<FileDialogExtraPanel*>(dlg.GetExtraControl());

			// Set message
			if (log->SaveFile(path)) msg += (wxT("File successfully saved: ") + path);
			else msg += (wxT("Error occurred while saving the file: ") + path);

			if (extra_panel->IsChecked()) wxMessageBox(msg);
			_LOG(msg);
		}
	}
}

void MyFrame::On_wxMessageDialog(wxCommandEvent& WXUNUSED(event))
{
	wxMessageDialog dlg(this, wxT("Do you want to sing a song?"), wxT("Karaoke"), wxYES_NO | wxCANCEL | wxICON_QUESTION);
	dlg.SetExtendedMessage(wxT("Sing-along"));
	if (dlg.ShowModal() == wxID_YES)
	{
		_LOG(wxT("Great - with melody or professional?"));
	}
	else
	{
		_LOG(wxT("That's a pity..."));
	}
}

void MyFrame::On_wxMessageDialog2(wxCommandEvent& WXUNUSED(event))
{
	wxMessageDialog dlg(this, wxT("Choose an operation."), wxT("Operation choice"), wxYES_NO | wxCANCEL | wxHELP | wxICON_EXCLAMATION);
	dlg.SetYesNoLabels(wxID_SAVE, wxID_OPEN);
	dlg.SetHelpLabel(wxT("Tips"));
	dlg.SetExtendedMessage(wxT("Choose the operation to be performed\nnext."));

	switch (dlg.ShowModal())
	{
		case wxID_YES: _LOG(wxT("Your choice is SAVE")); break;
		case wxID_NO: _LOG(wxT("Your choice is OPEN")); break;
		case wxID_HELP: _LOG(wxT("You chose help.")); break;
		default: _LOG(wxT("You canceled...")); break;
	}
}

void MyFrame::On_wxMessageBox(wxCommandEvent& WXUNUSED(event))
{
	int answer = wxMessageBox("Do you want to display sample wxMessageBox() dialogs?", "Question", wxYES_NO | wxICON_NONE);

	if (answer == wxYES)
	{
		wxMessageBox("An error occurred, do you want to react?", "Error!", wxYES_NO | wxICON_ERROR);
		wxMessageBox("It will rain tomorrow, prepare your boots.", "Warning", wxOK | wxICON_WARNING);
		wxMessageBox("Are you hungry?", "Question", wxYES_NO | wxCANCEL | wxICON_QUESTION);
		wxMessageBox("This is the fourth dialog in a row. Only three more to go.", "Information", wxOK | wxICON_INFORMATION);
		wxMessageBox("Only two dialogs left!", "Attention!", wxOK | wxCANCEL | wxICON_EXCLAMATION);
		wxMessageBox("You should reconsider if you really want to click OK.", "Stop!", wxOK | wxICON_HAND);
		wxMessageBox("Administrator privileges are required to complete.", "Information", wxOK | wxICON_AUTH_NEEDED);
	}

	_LOG(wxT("Operation completed."));
}

void MyFrame::On_wxInfoBar(wxCommandEvent& WXUNUSED(event))
{
	infobar->ShowMessage(wxT("Message..."), wxICON_EXCLAMATION);
}

void MyFrame::On_wxInfoBarMoreInfo(wxCommandEvent& WXUNUSED(event))
{
	wxMessageBox(wxT("Additional information..."), wxT("Information"));
}

void MyFrame::On_wxAboutBox(wxCommandEvent& WXUNUSED(event))
{
	wxAboutDialogInfo info;
	info.SetName(wxT("Dialogs"));
	info.SetVersion(wxT("1.0"), wxT("1.0.0.0 PL"));
	info.SetDescription(wxT("Application demonstrating the operation of wxWidgets dialog windows"));
	info.SetWebSite(wxT("http://www.dialogiwx.com.pl"), wxT("Program website"));
	info.AddDeveloper(wxT("Marian Niewczas"));
	info.AddDeveloper(wxT("Marian's team and others..."));
	info.SetLicence(wxT("wxWindows Licence"));
	info.SetCopyright(wxT("Copyright \u00A9 2017 by Marian Niewczas"));

	wxAboutBox(info, this);

	_LOG(wxT("Operation completed."));
}

void MyFrame::On_wxLogMessage(wxCommandEvent& WXUNUSED(event))
{
	//wxLog* current = wxLog::SetActiveTarget(NULL);

	wxLogGui* log_gui = new wxLogGui();
	wxLog* current = wxLog::SetActiveTarget(log_gui);

	wxLogMessage(wxT("First diagnostic message..."));
	wxLogMessage(wxT("Second diagnostic message..."));
	wxLogMessage(wxT("Third diagnostic message..."));
	wxLogWarning(wxT("Something started to go wrong..."));
	wxLogWarning(wxT("Formatting hard drive!"));
	wxLogError(wxT("The program is unstable. File system error detected."));
	//wxLogFatalError(wxT("The program will be terminated."));

	wxLog::SetActiveTarget(current);
	_LOG(wxT("Returning to logging in the text control"));
}

void MyFrame::On_wxProgressDialog(wxCommandEvent& WXUNUSED(event))
{
	const int MAX = 100;
	wxProgressDialog dlg(wxT("Operation progress"), wxEmptyString, MAX, this, wxPD_APP_MODAL | wxPD_SMOOTH | wxPD_CAN_ABORT | wxPD_CAN_SKIP | wxPD_ELAPSED_TIME | wxPD_ESTIMATED_TIME | wxPD_REMAINING_TIME);

	// Was it skipped
	bool is_skipped = false;

	// Should the operation continue
	bool is_progress = true;

	// Test 1 - Connection simulation - waiting bar
	for (int i = 0; i <= MAX / 2; i++)
	{
		is_progress = dlg.Pulse(wxT("Connecting, please wait..."), &is_skipped);
		if (is_skipped) is_progress = false;
		if (!is_progress)  break;
		wxMilliSleep(100);
	}

	// Test 2 - Download simulation - progress bar
	if (is_progress)
	{
		wxString msg = wxT("Downloading");
		for (int i = 0; i <= MAX; i++)
		{
			if (i == MAX) msg += wxT(" completed");
			is_progress = dlg.Update(i, msg, &is_skipped);

			if (is_skipped) i = MAX - 1;
			if (!is_progress)
			{
				if (wxMessageBox(wxT("Do you really want to abort the current operation?"), wxT("Question"), wxYES_NO) == wxYES)
				{
					_LOG(wxT("Operation aborted."));
					break;
				}
				else
				{
					// Continue
					is_progress = true;
					dlg.Resume();
				}
			}

			wxMilliSleep(150);
		}
	}
}

void MyFrame::On_wxAppProgressIndicator(wxCommandEvent& WXUNUSED(event))
{
#ifdef __WXMSW__
	wxAppProgressIndicator progress;
	if (progress.IsAvailable())
	{
		for (int i = 0; i <= 100 / 2; i++)
		{
			progress.Pulse();
			wxMilliSleep(100);
		}

		for (int i = 0; i <= 100; i++)
		{
			progress.SetValue(i);
			wxMilliSleep(150);
		}
	}
#else
    _LOG(wxT("The wxAppProgressIndicator class is not supported on this platform."));
#endif
}

void MyFrame::On_wxBusyCursor(wxCommandEvent& WXUNUSED(event))
{
	wxMessageBox(wxT("The application will be busy for 10 seconds."));

	_LOG(wxString::Format("Is the application busy? (%d)", (int)wxIsBusy()));
	_LOG(wxT("Simulating application busy state (START)"));
	wxBeginBusyCursor(); // Change cursor
	for (int i = 0; i < 10; i++)
	{
		_LOG(wxString::Format("Is the application busy? (%d) (-%d sec.)", (int)wxIsBusy(), 10-i));
		wxMilliSleep(1000);
	}
	wxEndBusyCursor(); // Restore cursor
	_LOG(wxString::Format("Is the application busy? (%d)", (int)wxIsBusy()));
	_LOG(wxT("Simulating application busy state (STOP)"));
}

void MyFrame::On_wxBusyInfo1(wxCommandEvent& WXUNUSED(event))
{
	wxWindowDisabler disabler;

	wxBusyInfo dlg(wxT("Please wait, working..."));

	for (int i = 0; i < 5; i++)
	{
		wxSleep(1);
		wxApp::GetInstance()->Yield();
	}

	_LOG(wxT("Operation completed."));
}

void MyFrame::On_wxBusyInfo2(wxCommandEvent& WXUNUSED(event))
{
	wxWindowDisabler disabler;

	wxBusyInfoFlags flags;
	flags.Title(wxT("Operation in progress"));
	flags.Text(wxT("Please wait, working..."));
	flags.Label(wxT("Label"));
	flags.Foreground(*wxWHITE);
	flags.Background(wxColour(50, 50, 50));
	flags.Transparency(100);
    flags.Icon(wxIcon(wxT("gfx/run.png"), wxBITMAP_TYPE_PNG));
	flags.Parent(this);

	wxBusyInfo dlg(flags);

	for (int i = 0; i < 5; i++)
	{
		wxSleep(1);
		wxApp::GetInstance()->Yield();
	}

	_LOG(wxT("Operation completed."));
}

void MyFrame::On_wxNotificationMessage(wxCommandEvent& WXUNUSED(event))
{
	wxNotificationMessage* msg = new wxNotificationMessage(wxT("Test message"), wxT("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua..."), this, wxICON_ERROR);

	msg->Show(10);

	msg->Bind(wxEVT_NOTIFICATION_MESSAGE_CLICK, [](wxCommandEvent&) { wxMessageBox(wxT("Notification clicked.")); }, wxID_ANY);
	msg->Bind(wxEVT_NOTIFICATION_MESSAGE_DISMISSED, [](wxCommandEvent&) { wxMessageBox(wxT("Notification dismissed.")); }, wxID_ANY);
}

void MyFrame::On_RequestUserAttention(wxCommandEvent& WXUNUSED(event))
{
	wxMessageBox(wxT("After closing this window, within 5 seconds, cover the application window with another window.\nAfter this time, you should see the program icon highlighted in the taskbar."));
	wxSleep(5);
	this->RequestUserAttention();
}

void MyFrame::On_wxTipProvider(wxCommandEvent& WXUNUSED(event))
{
	wxTipProvider* tips = wxCreateFileTipProvider(wxT("tips.txt"), 0);
	bool show_on_startup = wxShowTip(this, tips);

	_LOG(wxString::Format(wxT("Last tip - Index: %d; Show on program start: %s"), tips->GetCurrentTip(), ((show_on_startup) ? wxT("YES") : wxT("NO"))));

	delete tips;
}

void MyFrame::On_wxRichToolTip(wxCommandEvent& WXUNUSED(event))
{
	wxRichToolTip tips(wxT("Information"), wxT("This is the log window.\nHere you can read current diagnostic messages."));
	tips.SetIcon(wxICON_EXCLAMATION);
	tips.SetTimeout(5000, 500);
	tips.SetTipKind(wxTipKind_Auto);
	tips.ShowFor(log);
}

void MyFrame::On_wxHtmlHelpDialog(wxCommandEvent& WXUNUSED(event))
{
	help = new wxHtmlHelpController();
	help->AddBook("help.zip");
	help->SetShouldPreventAppExit(true);
	help->DisplayContents();
}

void MyFrame::On_wxPropertySheetDialog(wxCommandEvent& WXUNUSED(event))
{
	wxPropertySheetDialog dlg(this, wxID_ANY, wxT("Settings"));
	dlg.CreateButtons(wxOK | wxCANCEL);

	// Page 1
	wxPanel *page1 = new wxPanel(dlg.GetBookCtrl());
	wxBoxSizer* p1s = new wxBoxSizer(wxVERTICAL);
	page1->SetSizer(p1s);

	wxArrayString choices;
	choices.Add(wxT("Yellow"));
	choices.Add(wxT("Green"));
	choices.Add(wxT("Blue"));
	wxRadioBox* rb = new wxRadioBox(page1, wxID_ANY, wxT("Color"), wxDefaultPosition, wxDefaultSize, choices);
	p1s->Add(rb, 0, wxEXPAND | wxALL, 4);

	wxCheckBox* cb = new wxCheckBox(page1, wxID_ANY, wxT(" RGB -> BGR conversion"));
	p1s->Add(cb, 0, wxEXPAND | wxALL, 4);

	// Page 2
	wxPanel *page2 = new wxPanel(dlg.GetBookCtrl());
	wxBoxSizer* p2s = new wxBoxSizer(wxVERTICAL);
	page2->SetSizer(p2s);

	wxToggleButton* btn = new wxToggleButton(page2, wxID_ANY, wxT("Button"));
	btn->SetValue(false);
	p2s->Add(btn, 0, wxALL | wxALIGN_CENTER, 4);

	// Add pages to dialog
	dlg.GetBookCtrl()->AddPage(page1, wxT("Page 1"), true, -1);
	dlg.GetBookCtrl()->AddPage(page2, wxT("Page 2"), false, -1);

	// Additional control in the dialog window
	dlg.GetInnerSizer()->Add(new wxCheckBox(&dlg, wxID_ANY, wxT(" Export settings to file")), 0, wxEXPAND|wxALL, 4);

	// Adjust size and center window
	dlg.LayoutDialog();

	// Display window and read data
	if (dlg.ShowModal() == wxID_OK)
	{
		_LOG(wxString::Format("wxRadioBox: %d; wxCheckBox: %d, wxToggleButton: %d", rb->GetSelection(), cb->IsChecked(), btn->GetValue()));
	}
	else
	{
		_LOG(wxT("Settings change canceled..."));
	}
}

void MyFrame::On_wxFindReplaceDialog1(wxCommandEvent& WXUNUSED(event))
{
	// DEMO
	// This window is only displayed for demonstration purposes and does not perform string searches.
	// Refer to chapter 7 of the book to learn how to implement
	// searching in your own applications.

	if (find_dlg) wxDELETE(find_dlg);

	find_dlg = new wxFindReplaceDialog(this, &find_data, wxT("Search (DEMO)"));
	find_dlg->Bind(wxEVT_FIND, [](wxFindDialogEvent& e) { wxMessageBox(wxT("Find: ") + e.GetFindString()); }, wxID_ANY, wxID_ANY);
	find_dlg->Bind(wxEVT_FIND_NEXT, [](wxFindDialogEvent& e) { wxMessageBox(wxT("Next: ") + e.GetFindString()); }, wxID_ANY, wxID_ANY);
	find_dlg->Show(true);

	_LOG(wxT("Operation completed."));
}

void MyFrame::On_wxFindReplaceDialog2(wxCommandEvent& WXUNUSED(event))
{
	// DEMO
	// This window is only displayed for demonstration purposes and does not perform string search and replace.
	// Refer to chapter 7 of the book to learn how to implement
	// search and replace in your own applications.

	if (find_dlg) wxDELETE(find_dlg);

	find_dlg = new wxFindReplaceDialog(this, &find_data, wxT("Find and Replace (DEMO)"), wxFR_REPLACEDIALOG);
	find_dlg->Bind(wxEVT_FIND, [](wxFindDialogEvent& e) { wxMessageBox(wxT("Find: ") + e.GetFindString()); }, wxID_ANY, wxID_ANY);
	find_dlg->Bind(wxEVT_FIND_NEXT, [](wxFindDialogEvent& e) { wxMessageBox(wxT("Next: ") + e.GetFindString()); }, wxID_ANY, wxID_ANY);
	find_dlg->Bind(wxEVT_FIND_REPLACE, [](wxFindDialogEvent& e) { wxMessageBox(wxString::Format(wxT("Replace %s with %s"), e.GetFindString(), e.GetReplaceString())); }, wxID_ANY, wxID_ANY);
	find_dlg->Bind(wxEVT_FIND_REPLACE_ALL, [](wxFindDialogEvent& e) { wxMessageBox(wxString::Format(wxT("Replace all %s with %s"), e.GetFindString(), e.GetReplaceString())); }, wxID_ANY, wxID_ANY);
	find_dlg->Show(true);

	_LOG(wxT("Operation completed."));
}

void MyFrame::On_wxPrintDialog(wxCommandEvent& WXUNUSED(event))
{
	wxPrintData data;
	data.SetOrientation(wxPORTRAIT);

	wxPrintDialogData dlg_data;
	dlg_data.SetNoCopies(3); // Number of copies
	dlg_data.SetPrintData(data);

	wxPrintDialog dlg(this, &dlg_data);
	if (dlg.ShowModal() == wxID_OK) // Show window
	{
		// Here, print the document...
		_LOG("Printing...");
	}
}

void MyFrame::On_wxWizard(wxCommandEvent& WXUNUSED(event))
{
	wxBitmap wiz_img(wxT("gfx/wizard.png"), wxBITMAP_TYPE_PNG);
	wxWizard* wizard = new wxWizard(this, wxID_ANY, wxT("DEMO Wizard"), wiz_img);

	// Page 1
	wxWizardPageSimple* page1 = new wxWizardPageSimple(wizard);
	wxBoxSizer* p1s = new wxBoxSizer(wxVERTICAL);
	wxCheckBox* cb1A = new wxCheckBox(page1, wxID_ANY, wxT(" Option A on page 1"));
	wxCheckBox* cb1B = new wxCheckBox(page1, wxID_ANY, wxT(" Option B on page 1"));
	p1s->Add(cb1A, 1, wxEXPAND | wxALL, 4);
	p1s->Add(cb1B, 1, wxEXPAND | wxALL, 4);
	page1->SetSizerAndFit(p1s);

	// Page 2
	wxWizardPageSimple* page2 = new wxWizardPageSimple(wizard);
	wxBoxSizer* p2s = new wxBoxSizer(wxVERTICAL);
	wxTextCtrl* text2 = new wxTextCtrl(page2, wxID_ANY, wxT("Page 2"), wxDefaultPosition, wxSize(-1, 500), wxTE_MULTILINE);
	p2s->Add(text2, 1, wxEXPAND | wxALL, 4);
	page2->SetSizerAndFit(p2s);

	// Page 3
	wxWizardPageSimple* page3 = new wxWizardPageSimple(wizard);
	wxBoxSizer* p3s = new wxBoxSizer(wxVERTICAL);
	wxArrayString choices;
	choices.Add(wxT("Yellow"));
	choices.Add(wxT("Green"));
	choices.Add(wxT("Blue"));
	wxRadioBox* rb = new wxRadioBox(page3, wxID_ANY, wxT("Color"), wxDefaultPosition, wxDefaultSize, choices);
	p3s->Add(rb, 0, wxEXPAND | wxALL, 4);
	page3->SetSizerAndFit(p3s);

	// Page 4
	wxWizardPageSimple* page4 = new wxWizardPageSimple(wizard);
	page4->SetSizer(new wxBoxSizer(wxVERTICAL));
	page4->GetSizer()->Add(new wxStaticText(page4, wxID_ANY, wxT("Last page...")), 1, wxALIGN_CENTER | wxALL, 20);

	// DEFINING THE ORDER OF WIZARD PAGES

	// Method 1
	(*page1).Chain(page2).Chain(page3).Chain(page4);

	// Method 2
	//page1->SetNext(page2);
	//page2->SetNext(page3);
	//page3->SetNext(page4);

	//page2->SetPrev(page1);
	//page3->SetPrev(page2);
	//page4->SetPrev(page3);

	// DISPLAYING THE WIZARD
	if (wizard->RunWizard(page1))
	{
		_LOG(wxString::Format(wxT("Option A: %d, Option B: %d, Text: %s, Color: %s"), cb1A->IsChecked(), cb1B->IsChecked(), text2->GetValue(), rb->GetStringSelection()));
	}

	wizard->Destroy();
}

void MyFrame::On_wxColourPickerCtrl(wxColourPickerEvent& event)
{
	log->SetForegroundColour(event.GetColour());
	fopicker->SetSelectedColour(event.GetColour());

	wxString col_str = wxT("New color: ") + event.GetColour().GetAsString();
	_LOG(col_str);
}

void MyFrame::On_wxFontPickerCtrl(wxFontPickerEvent& event)
{
	wxFont new_font = event.GetFont();
	if (new_font.IsOk())
	{
		log->SetFont(new_font);
		log->SetForegroundColour(fopicker->GetSelectedColour());
		copicker->SetColour(fopicker->GetSelectedColour());
		_LOG(wxString::Format("New font: %s (%s, %d pt.)", new_font.GetFaceName(), new_font.GetStyleString(), new_font.GetPointSize()));
	}
}

void MyFrame::On_wxDirPickerCtrl(wxFileDirPickerEvent& event)
{
	_LOG(wxT("Selected path: ") + event.GetPath());
}

void MyFrame::On_wxFilePickerCtrl(wxFileDirPickerEvent& event)
{
	_LOG(wxT("Selected file: ") + event.GetPath());
}

