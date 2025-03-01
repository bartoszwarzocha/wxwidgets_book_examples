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

#define STYLE_NAME_S1 wxT("Default")
#define STYLE_NAME_P1 wxT("Default Paragraph")
#define STYLE_NAME_H1 wxT("Header 1")
#define STYLE_NAME_H2 wxT("Header 2")
#define STYLE_NAME_A1 wxT("URL")
#define STYLE_NAME_E1 wxT("Superscript")
#define STYLE_NAME_E2 wxT("Subscript")
#define STYLE_NAME_O1 wxT("Numbered List 1")
#define STYLE_NAME_O2 wxT("Numbered List 2")
#define STYLE_NAME_U1 wxT("Bulleted List")
#define STYLE_NAME_T1 wxT("Default Table")
#define STYLE_NAME_C1 wxT("Default Cell")

//---------------------------------------------------------------------------
// Class MyApp
//---------------------------------------------------------------------------
class MyApp : public wxApp
{
public:
	virtual bool OnInit();

	wxLocale loc;
};

//---------------------------------------------------------------------------
// Class MyFrame
//---------------------------------------------------------------------------
class MyFrame : public wxFrame
{
public:
	MyFrame(wxWindow* parent, wxWindowID id, const wxString& title,
		const wxPoint& pos, const wxSize& size, long style);
	~MyFrame();

	void OnAboutProgram(wxRibbonButtonBarEvent& event);
	void OnClose(wxRibbonButtonBarEvent& event);
	void CloseApp(wxCloseEvent& event);

	void OnOpen(wxRibbonButtonBarEvent& event);
	void OnSave(wxRibbonButtonBarEvent& event);
	void OnSaveAs(wxRibbonButtonBarEvent& event);
	void OnPrint(wxRibbonButtonBarEvent& event);
	void OnPrintPreview(wxRibbonButtonBarEvent& event);
	void OnPrintSetup(wxRibbonButtonBarEvent& event);

	void OnEdit(wxRibbonButtonBarEvent& event);

	void OnFormat(wxRibbonToolBarEvent& event);
	void OnSuperscript(wxRibbonToolBarEvent& event);
	void OnSubscript(wxRibbonToolBarEvent& event);
	void OnIndent(wxRibbonToolBarEvent& event);
	void OnSetInterline(wxRibbonToolBarEvent& event);
	void OnInterline(wxCommandEvent& event);
	void OnUpdate(wxUpdateUIEvent& event);

	void OnFont(wxRibbonButtonBarEvent& event);
	void OnStyle(wxRibbonButtonBarEvent& event);

	void OnList(wxRibbonButtonBarEvent& event);
	void OnListLevel(wxCommandEvent& event);
	void OnListRefresh(wxCommandEvent& event);
	void OnListClear(wxCommandEvent& event);

	void OnSymbol(wxRibbonButtonBarEvent& event);
	void OnURL(wxRibbonButtonBarEvent& event);
	void OnHeader(wxRibbonButtonBarEvent& event);
	void OnFooter(wxRibbonButtonBarEvent& event);
	void OnImage(wxRibbonButtonBarEvent& event);
	void OnTable(wxRibbonButtonBarEvent& event);
	void OnSymbolGallery(wxRibbonGalleryEvent& event);

	void OnShowSource(wxRibbonButtonBarEvent& event);
	void OnShowXML(wxCommandEvent& event);
	void OnShowHTML(wxCommandEvent& event);

	void OnColour(wxRibbonButtonBarEvent& event);

	void OnHelp(wxRibbonBarEvent& WXUNUSED(event))
	{
		wxMessageBox(wxT("Clicked 'Help'..."));
	}

private:
	void InitStyle();
	wxString GetXML();
	wxString GetHTML();

	void Save(const wxString& path);
	void SaveAs();
	wxRichTextFileType GetFileType(const wxString& path);

	wxStatusBar* sb;
	wxRibbonBar* rb;

	wxPanel* panel;

	wxRichTextStyleSheet* style_sheet;
	wxRichTextPrinting* printing;
	wxRichTextCtrl* richtext;

	wxRichTextFileType file_type;
	wxString file_path;

	wxBitmap CreateSymbolBmp(int code, int size = 32, float scale = 0.5, int yoffset = 0);

	wxColour default_colour1, default_colour2, default_text_colour;
	wxColour colour1, colour2, text_colour;

	DECLARE_EVENT_TABLE()
};

//---------------------------------------------------------------------------
// Class SymbolGalleryData
//---------------------------------------------------------------------------
class SymbolGalleryData : public wxClientData
{
public:
	SymbolGalleryData(const int& item_code) : code(item_code), symbol(wxUniChar(item_code)) {}

	wxString GetSymbol() { return symbol; }
	wxColour GetCode() { return code; }

private:
	unsigned int code;
	wxString symbol;
};

#endif
