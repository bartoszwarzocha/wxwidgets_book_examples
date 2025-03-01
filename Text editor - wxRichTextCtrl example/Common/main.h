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
#define STYLE_NAME_P1 wxT("Default paragraph")
#define STYLE_NAME_H1 wxT("Header 1")
#define STYLE_NAME_H2 wxT("Header 2")
#define STYLE_NAME_A1 wxT("URL")
#define STYLE_NAME_E1 wxT("Superscript")
#define STYLE_NAME_E2 wxT("Subscript")
#define STYLE_NAME_O1 wxT("Numbered list 1")
#define STYLE_NAME_O2 wxT("Numbered list 2")
#define STYLE_NAME_U1 wxT("Bulleted list")
#define STYLE_NAME_T1 wxT("Default table")
#define STYLE_NAME_C1 wxT("Default cell")

//---------------------------------------------------------------------------
// Class MyApp
//---------------------------------------------------------------------------
class MyApp: public wxApp
{
    public:
        virtual bool OnInit();

		wxLocale loc;
};

//---------------------------------------------------------------------------
// Class MyFrame
//---------------------------------------------------------------------------
class MyFrame: public wxFrame
{
    public:
        MyFrame(wxWindow *parent, wxWindowID id, const wxString &title,
            const wxPoint &pos, const wxSize &size, long style);
		~MyFrame();

		void OnAboutProgram(wxCommandEvent& event);
		void OnClose(wxCommandEvent& event);
		void CloseApp(wxCloseEvent& event);

		void OnOpen(wxCommandEvent& event);
		void OnSave(wxCommandEvent& event);
		void OnSaveAs(wxCommandEvent& event);
		void OnPrint(wxCommandEvent & event);
		void OnPrintPreview(wxCommandEvent& event);
		void OnPrintSetup(wxCommandEvent& event);

		void OnEdit(wxCommandEvent& event);

		void OnFormat(wxCommandEvent& event);
		void OnSuperscript(wxCommandEvent& event);
		void OnSubscript(wxCommandEvent& event);
		void OnIndent(wxCommandEvent& event);
		void OnInterline(wxCommandEvent& event);
		void OnUpdate(wxUpdateUIEvent& event);
		void OnFont(wxCommandEvent& event);
		void OnStyle(wxCommandEvent& event);

		void OnList(wxCommandEvent& event);
		void OnListLevel(wxCommandEvent& event);
		void OnListRefresh(wxCommandEvent& event);
		void OnListClear(wxCommandEvent& event);

		void OnSymbol(wxCommandEvent& event);
		void OnURL(wxCommandEvent& event);
		void OnHeader(wxCommandEvent& event);
		void OnFooter(wxCommandEvent& event);
		void OnImage(wxCommandEvent& event);
		void OnTable(wxCommandEvent& event);

		void OnShowXML(wxCommandEvent& event);
		void OnShowHTML(wxCommandEvent& event);

    private:
		void InitStyle();
		wxString GetXML();
		wxString GetHTML();

		void Save(const wxString& path);
		void SaveAs();
		wxRichTextFileType GetFileType(const wxString& path);

		wxToolBar *tb;
		wxStatusBar *sb;
        wxMenuBar *mb;
        wxMenu *mFile;
		wxMenu *mEdit;
		wxMenu *mFormat;
		wxMenu *mList;
		wxMenu *mInsert;
        wxMenu *mHelp;

		wxPanel *panel;

		wxRichTextStyleSheet *style_sheet;
		wxRichTextPrinting *printing;
		wxRichTextCtrl *richtext;

		wxRichTextFileType file_type;
		wxString file_path;

		DECLARE_EVENT_TABLE()
};

#endif
