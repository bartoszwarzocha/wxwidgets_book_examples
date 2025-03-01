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

#define _IS_PRODUCTION_ 0
#if _IS_PRODUCTION_
#define _LOG(s) wxLogMessage(wxT(s))
#else
#define _LOG(s) wxLogMessage(wxString::Format (wxT("[%s][%d][%s] %s"), wxString(__FILE__), (int)__LINE__, wxString(__WXFUNCTION__), s))
#endif

//---------------------------------------------------------------------------
// MyApp Class
//---------------------------------------------------------------------------
class MyApp : public wxApp
{
public:
	virtual bool OnInit();

	wxLocale loc;
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

	void CloseApp(wxCloseEvent& event);
	void OnClose(wxCommandEvent& event);

	void OnClear(wxCommandEvent& event);

	void On_wxGetTextFromUser(wxCommandEvent& event);
	void On_wxGetPasswordFromUser(wxCommandEvent& event);
	void On_wxTextEntryDialog(wxCommandEvent& event);
	void On_wxPasswordEntryDialog(wxCommandEvent& event);
	void On_wxGetNumberFromUser(wxCommandEvent& event);
	void On_wxGetFontFromUser(wxCommandEvent& event);
	void On_wxFontDialog(wxCommandEvent& event);
	void On_wxSymbolPickerDialog(wxCommandEvent& event);
	void On_wxGetColourFromUser(wxCommandEvent& event);
	void On_wxColourDialog(wxCommandEvent& event);
	void On_wxGetSingleChoice1(wxCommandEvent& event);
	void On_wxGetSingleChoice2(wxCommandEvent& event);
	void On_wxGetSingleChoice3(wxCommandEvent& event);
	void On_wxGetSingleChoiceData(wxCommandEvent& event);
	void On_wxGetSingleChoiceIndex(wxCommandEvent& event);
	void On_wxSingleChoiceDialog(wxCommandEvent& event);
	void On_wxGetSelectedChoices(wxCommandEvent& event);
	void On_wxMultiChoiceDialog(wxCommandEvent& event);
	void On_wxRearrangeDialog(wxCommandEvent& event);
	void On_wxDirSelector(wxCommandEvent& event);
	void On_wxDirDialog(wxCommandEvent& event);
	void On_wxFileSelector(wxCommandEvent& event);
	void On_wxFileSelectorEx(wxCommandEvent& event);
	void On_wxLoadFileSelector(wxCommandEvent& event);
	void On_wxSaveFileSelector(wxCommandEvent& event);
	void On_wxFileDialog(wxCommandEvent& event);
	void On_wxFileDialogExtraCtrl(wxCommandEvent& event);
	void On_wxMessageDialog(wxCommandEvent& event);
	void On_wxMessageDialog2(wxCommandEvent& event);
	void On_wxMessageBox(wxCommandEvent& event);
	void On_wxInfoBar(wxCommandEvent& event);
	void On_wxAboutBox(wxCommandEvent& event);
	void On_wxLogMessage(wxCommandEvent& event);
	void On_wxProgressDialog(wxCommandEvent& event);
	void On_wxAppProgressIndicator(wxCommandEvent& event);
	void On_wxBusyCursor(wxCommandEvent& event);
	void On_wxBusyInfo1(wxCommandEvent& event);
	void On_wxBusyInfo2(wxCommandEvent& event);
	void On_wxNotificationMessage(wxCommandEvent& event);
	void On_RequestUserAttention(wxCommandEvent& event);
	void On_wxTipProvider(wxCommandEvent& event);
	void On_wxRichToolTip(wxCommandEvent& event);
	void On_wxHtmlHelpDialog(wxCommandEvent& event);
	void On_wxPropertySheetDialog(wxCommandEvent& event);
	void On_wxFindReplaceDialog1(wxCommandEvent& event);
	void On_wxFindReplaceDialog2(wxCommandEvent& event);
	void On_wxPrintDialog(wxCommandEvent& event);
	void On_wxWizard(wxCommandEvent& event);

	void On_wxInfoBarMoreInfo(wxCommandEvent& event);

	void On_wxColourPickerCtrl(wxColourPickerEvent& event);
	void On_wxFontPickerCtrl(wxFontPickerEvent& event);
	void On_wxDirPickerCtrl(wxFileDirPickerEvent& event);
	void On_wxFilePickerCtrl(wxFileDirPickerEvent& event);

private:
	wxMenuBar* mb; // Menu bar
	wxMenu* mFile; // 'File' menu
	wxMenu* mDialogs; // 'Dialogs' menu
	wxMenu* mHelp; // 'Help' menu

	wxPanel* panel;

	wxTextCtrl* log;

	wxInfoBar* infobar;

	wxHtmlHelpController* help;

	wxFindReplaceData find_data;
	wxFindReplaceDialog* find_dlg;

	wxColourPickerCtrl* copicker;
	wxFontPickerCtrl* fopicker;
	wxDirPickerCtrl* dipicker;
	wxFilePickerCtrl* fipicker;

	DECLARE_EVENT_TABLE()
};

//---------------------------------------------------------------------------
// Person Structure
//---------------------------------------------------------------------------
struct Person
{
	Person(const wxString& n, const wxString& s, const wxString& p) : name(n), surname(s), phone(p) {}

	wxString name;
	wxString surname;
	wxString phone;
};

//---------------------------------------------------------------------------
// FileDialogExtraPanel Class
//---------------------------------------------------------------------------
class FileDialogExtraPanel : public wxPanel
{
public:
	FileDialogExtraPanel(wxWindow* parent) : wxPanel(parent, wxID_ANY)
	{
		wxBoxSizer* ps = new wxBoxSizer(wxHORIZONTAL);

		cb = new wxCheckBox(this, wxID_ANY, wxT(" Show operation confirmation"));
		ps->Add(cb, 0, wxEXPAND | wxALL, 4);

		this->SetSizerAndFit(ps); // SetSizer() + Fit()
	}

	virtual ~FileDialogExtraPanel() {}

	bool IsChecked() { return this->cb->IsChecked(); }

private:
	wxCheckBox* cb;
};

#endif
