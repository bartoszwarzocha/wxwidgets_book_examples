/////////////////////////////////////////////////////////////////////////////
// Name:        main.cpp
// Purpose:     My wxWidgets app
// Author:      Imie Nazwisko <imie.nazwisko@email.org>
// Created:     2018-01-01
// Copyright:   (c) 2018 by Imie Nazwisko
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <wx/ribbon/bar.h> // wxRibbonBar
#include <wx/ribbon/art.h> // wxRibbonDefaultArtProvider
#include <wx/ribbon/buttonbar.h> // wxRibbonButtonBar
#include <wx/ribbon/toolbar.h> // wxRibbonToolBar
#include <wx/ribbon/gallery.h> // wxRibbonGallery

#include <wx/richtext/richtextctrl.h> // wxRichTextCtrl class
#include <wx/richtext/richtextprint.h> // Printing
#include <wx/richtext/richtextstyledlg.h> // Style management dialog
#include <wx/richtext/richtextformatdlg.h> // Formatting dialog
#include <wx/richtext/richtextxml.h> // XML support
#include <wx/richtext/richtexthtml.h> // HTML support
#include <wx/richtext/richtextsymboldlg.h> // Special characters dialog
#include <wx/richtext/richtextstyles.h> // Styles

#include <wx/sstream.h>
#include <wx/fontenum.h>
#include <wx/listctrl.h>
#include <wx/numdlg.h>
#include <wx/colordlg.h>

#include "utils.h"
#include "HFPrintEditor.h"
#include "main.h"

const wxString appName = wxT("wxRibbonBar Presentation");

// Making work easier
#define _PNG(x) wxBitmap(wxGetAppFile(wxT(x)), wxBITMAP_TYPE_PNG)

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
	ID_PRINT,
	ID_PRINT_PREVIEW,
	ID_PRINT_SETUP,
	//
	ID_BOLD,
	ID_ITALIC,
	ID_UNDERLINE,
	ID_LEFT,
	ID_RIGHT,
	ID_CENTER,
	ID_JUSTIFY,
	ID_SUPERSCRIPT,
	ID_SUBSCRIPT,
	ID_INDENT_PLUS,
	ID_INDENT_MINUS,
	ID_INTERLINE,
	ID_INTERLINE1_0,
	ID_INTERLINE1_5,
	ID_INTERLINE2_0,
	ID_FONT,
	ID_STYLE,
	//
	ID_LIST,
	ID_ORDER_LIST,
	ID_UNORDER_LIST,
	ID_LIST_LVL_UP,
	ID_LIST_LVL_DOWN,
	ID_LIST_REFRESH,
	ID_LIST_CLEAR,
	//
	ID_SYMBOL,
	ID_URL,
	ID_HEADER,
	ID_FOOTER,
	ID_IMAGE,
	ID_TABLE,
	//
	ID_SHOW_SOURCE,
	ID_SHOW_XML,
	ID_SHOW_HTML,
	//
	ID_STYLE_CTRL,
	//
	ID_SYMBOL_GALLERY,
	//
	ID_COLOUR1,
	ID_COLOUR2,
	ID_COLOUR3,
	ID_COLOURD,
};

//---------------------------------------------------------------------------
// Event table
//---------------------------------------------------------------------------
BEGIN_EVENT_TABLE(MyFrame, wxFrame)
	EVT_CLOSE(MyFrame::CloseApp)
	//
	EVT_MENU_RANGE(ID_INTERLINE1_0, ID_INTERLINE2_0, MyFrame::OnInterline)
	//
	EVT_MENU_RANGE(ID_LIST_LVL_UP, ID_LIST_LVL_DOWN, MyFrame::OnListLevel)
	EVT_MENU(ID_LIST_REFRESH, MyFrame::OnListRefresh)
	EVT_MENU(ID_LIST_CLEAR, MyFrame::OnListClear)
	//
	EVT_MENU(ID_SHOW_XML, MyFrame::OnShowXML)
	EVT_MENU(ID_SHOW_HTML, MyFrame::OnShowHTML)
	//
	EVT_UPDATE_UI_RANGE(ID_BOLD, ID_JUSTIFY, MyFrame::OnUpdate)
	//////////////////////////////////////////////////////////////////////////////////
	EVT_RIBBONBUTTONBAR_CLICKED(ID_CLOSE, MyFrame::OnClose)
	//
	EVT_RIBBONBUTTONBAR_CLICKED(wxID_OPEN, MyFrame::OnOpen)
	EVT_RIBBONBUTTONBAR_CLICKED(wxID_SAVE, MyFrame::OnSave)
	EVT_RIBBONBUTTONBAR_CLICKED(wxID_SAVEAS, MyFrame::OnSaveAs)
	EVT_RIBBONBUTTONBAR_CLICKED(ID_PRINT, MyFrame::OnPrint)
	EVT_RIBBONBUTTONBAR_CLICKED(ID_PRINT_PREVIEW, MyFrame::OnPrintPreview)
	EVT_RIBBONBUTTONBAR_CLICKED(ID_PRINT_SETUP, MyFrame::OnPrintSetup)
	EVT_RIBBONBUTTONBAR_DROPDOWN_CLICKED(ID_SHOW_SOURCE, MyFrame::OnShowSource)
	EVT_RIBBONBUTTONBAR_CLICKED(ID_ABOUT_PROGRAM, MyFrame::OnAboutProgram)
	//
	EVT_RIBBONBUTTONBAR_CLICKED(wxID_CUT, MyFrame::OnEdit)
	EVT_RIBBONBUTTONBAR_CLICKED(wxID_COPY, MyFrame::OnEdit)
	EVT_RIBBONBUTTONBAR_CLICKED(wxID_PASTE, MyFrame::OnEdit)
	EVT_RIBBONBUTTONBAR_CLICKED(wxID_SELECTALL, MyFrame::OnEdit)
	EVT_RIBBONBUTTONBAR_CLICKED(wxID_UNDO, MyFrame::OnEdit)
	EVT_RIBBONBUTTONBAR_CLICKED(wxID_REDO, MyFrame::OnEdit)
	//
	EVT_RIBBONTOOLBAR_CLICKED(ID_BOLD, MyFrame::OnFormat)
	EVT_RIBBONTOOLBAR_CLICKED(ID_ITALIC, MyFrame::OnFormat)
	EVT_RIBBONTOOLBAR_CLICKED(ID_UNDERLINE, MyFrame::OnFormat)
	EVT_RIBBONTOOLBAR_CLICKED(ID_LEFT, MyFrame::OnFormat)
	EVT_RIBBONTOOLBAR_CLICKED(ID_CENTER, MyFrame::OnFormat)
	EVT_RIBBONTOOLBAR_CLICKED(ID_RIGHT, MyFrame::OnFormat)
	EVT_RIBBONTOOLBAR_CLICKED(ID_JUSTIFY, MyFrame::OnFormat)
	EVT_RIBBONTOOLBAR_CLICKED(ID_INDENT_PLUS, MyFrame::OnIndent)
	EVT_RIBBONTOOLBAR_CLICKED(ID_INDENT_MINUS, MyFrame::OnIndent)
	EVT_RIBBONTOOLBAR_DROPDOWN_CLICKED(ID_INTERLINE, MyFrame::OnSetInterline)
	EVT_RIBBONTOOLBAR_CLICKED(ID_SUPERSCRIPT, MyFrame::OnSuperscript)
	EVT_RIBBONTOOLBAR_CLICKED(ID_SUBSCRIPT, MyFrame::OnSubscript)
	//
	EVT_RIBBONBUTTONBAR_CLICKED(ID_ORDER_LIST, MyFrame::OnList)
	EVT_RIBBONBUTTONBAR_CLICKED(ID_UNORDER_LIST, MyFrame::OnList)
	EVT_RIBBONBUTTONBAR_DROPDOWN_CLICKED(ID_LIST, MyFrame::OnList)
	//
	EVT_RIBBONBUTTONBAR_CLICKED(ID_FONT, MyFrame::OnFont)
	EVT_RIBBONBUTTONBAR_CLICKED(ID_STYLE, MyFrame::OnStyle)
	//
	EVT_RIBBONBUTTONBAR_CLICKED(ID_SYMBOL, MyFrame::OnSymbol)
	EVT_RIBBONBUTTONBAR_CLICKED(ID_URL, MyFrame::OnURL)
	EVT_RIBBONBUTTONBAR_CLICKED(ID_IMAGE, MyFrame::OnImage)
	EVT_RIBBONBUTTONBAR_CLICKED(ID_TABLE, MyFrame::OnTable)
	EVT_RIBBONBUTTONBAR_CLICKED(ID_HEADER, MyFrame::OnHeader)
	EVT_RIBBONBUTTONBAR_CLICKED(ID_FOOTER, MyFrame::OnFooter)
	//
	EVT_RIBBONBAR_HELP_CLICK(wxID_ANY, MyFrame::OnHelp)
	//
	EVT_RIBBONGALLERY_CLICKED(ID_SYMBOL_GALLERY, MyFrame::OnSymbolGallery)
	//
	EVT_RIBBONBUTTONBAR_CLICKED(ID_COLOUR1, MyFrame::OnColour)
	EVT_RIBBONBUTTONBAR_CLICKED(ID_COLOUR2, MyFrame::OnColour)
	EVT_RIBBONBUTTONBAR_CLICKED(ID_COLOUR3, MyFrame::OnColour)
	EVT_RIBBONBUTTONBAR_CLICKED(ID_COLOURD, MyFrame::OnColour)
END_EVENT_TABLE()

//---------------------------------------------------------------------------
// OnInit()
//---------------------------------------------------------------------------
bool MyApp::OnInit()
{
	wxInitAllImageHandlers();

	// POLISH LANGUAGE FOR DIALOG WINDOWS
	// Setting the Polish language for dialog window buttons.
	// By default, windows have buttons with labels in English.
	// Polish language is loaded from the pl/pl.mo file.
	// You can find more about this in the fourth part of the book.
	// DISABLED FOR NOW (not needed in current version)
	//loc.Init(wxLANGUAGE_POLISH, wxLOCALE_LOAD_DEFAULT);
	//loc.AddCatalog("pl");

    MyFrame *frame = new MyFrame(NULL, wxID_ANY, appName, wxDefaultPosition, wxSize(800,600), wxDEFAULT_FRAME_STYLE);
    frame->Show(true);
    return true;
}

//---------------------------------------------------------------------------
// MyFrame constructor
//---------------------------------------------------------------------------
MyFrame::MyFrame(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &pos, const wxSize &size, long style)
: wxFrame(parent, id, title, pos, size, style)
{
	// Initializing variables
	file_type = wxRICHTEXT_TYPE_ANY;
	file_path = wxEmptyString;

	// Status Bar
    sb = new wxStatusBar(this);
    sb->SetFieldsCount(2);
    this->SetStatusBar(sb);
    SetStatusText(title, 0);

	// Main sizer
	wxBoxSizer* main_sizer = new wxBoxSizer(wxVERTICAL);
	this->SetSizer(main_sizer);

	// Panel
	panel = new wxPanel(this);
	wxBoxSizer* panel_sizer = new wxBoxSizer(wxVERTICAL);
	panel->SetSizer(panel_sizer);

	// RICH TEXT
	wxRichTextBuffer::AddHandler(new wxRichTextXMLHandler);
	wxRichTextBuffer::AddHandler(new wxRichTextHTMLHandler);

	richtext = new wxRichTextCtrl(panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxVSCROLL | wxHSCROLL | wxWANTS_CHARS | wxRICHTEXT_SETSTYLE_OPTIMIZE);

	// Style sheet
	style_sheet = new wxRichTextStyleSheet();
	InitStyle();
	richtext->SetStyleSheet(style_sheet);
	richtext->GetBuffer().SetStyleSheet(style_sheet);

	// Printing
	printing = new wxRichTextPrinting(wxEmptyString);
	printing->SetParentWindow(this);

	// Enabling editing
	richtext->SetEditable(true);
								
	//=================================================================================================
	// RIBBON BAR
	rb = new wxRibbonBar(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxRIBBON_BAR_DEFAULT_STYLE);
	rb->SetArtProvider(new wxRibbonDefaultArtProvider);
	
	// Remembering default colors
	rb->GetArtProvider()->GetColourScheme(&default_colour1, &default_colour2, NULL);
	default_text_colour = rb->GetArtProvider()->GetColour(wxRIBBON_ART_TAB_LABEL_COLOUR);

	wxRibbonPage* pMain = new wxRibbonPage(rb, wxID_ANY, wxT("Main"));
	wxRibbonPage* pEdit = new wxRibbonPage(rb, wxID_ANY, wxT("Edit"));
	wxRibbonPage* pInsert = new wxRibbonPage(rb, wxID_ANY, wxT("Insert"));
	wxRibbonPage* pSet = new wxRibbonPage(rb, wxID_ANY, wxT("Settings"));
	rb->SetActivePage(pMain);

	// Main
	wxRibbonPanel* pFile = new wxRibbonPanel(pMain, wxID_ANY, wxT("Program"), _PNG("gfx/32/exit.png"));
	wxRibbonPanel* pDoc = new wxRibbonPanel(pMain, wxID_ANY, wxT("Document"), _PNG("gfx/32/open.png"));
	wxRibbonPanel* pPrint = new wxRibbonPanel(pMain, wxID_ANY, wxT("Printing"), _PNG("gfx/32/print.png"));
	wxRibbonPanel* pHelp = new wxRibbonPanel(pMain, wxID_ANY, wxT("Help"), _PNG("gfx/32/about.png"));

	wxRibbonButtonBar* pFileSection = new wxRibbonButtonBar(pFile, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0);
	pFileSection->AddButton(ID_CLOSE, wxT("Close"), _PNG("gfx/32/exit.png"), wxT("Closes the program"));

	wxRibbonButtonBar* pDocSection = new wxRibbonButtonBar(pDoc, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0);
	pDocSection->AddButton(wxID_OPEN, wxT("Open"), _PNG("gfx/32/open.png"), wxT("Opening document"));
	pDocSection->AddButton(wxID_SAVE, wxT("Save"), _PNG("gfx/32/save.png"), wxT("Saving document"));
	pDocSection->AddButton(wxID_SAVEAS, wxT("Save as..."), _PNG("gfx/32/save_as.png"), wxT("Saving document with specified name"));

	wxRibbonButtonBar* pPrintSection = new wxRibbonButtonBar(pPrint, wxID_ANY);
	pPrintSection->AddButton(ID_PRINT, wxT("Print"), _PNG("gfx/32/print.png"), wxT("Printing document"));
	pPrintSection->AddButton(ID_PRINT_PREVIEW, wxT("Print preview"), _PNG("gfx/32/print_preview.png"), wxT("Print preview"));
	pPrintSection->AddButton(ID_PRINT_SETUP, wxT("Page setup"), _PNG("gfx/32/print_settings.png"), wxT("Page setup"));

	wxRibbonButtonBar* pHelpSection = new wxRibbonButtonBar(pHelp, wxID_ANY);
	pHelpSection->AddDropdownButton(ID_SHOW_SOURCE, wxT("View source"), _PNG("gfx/32/show_source.png"));
	pHelpSection->AddButton(ID_ABOUT_PROGRAM, wxT("About"), _PNG("gfx/32/about.png"), wxT("About the program"));

	// Edit
	wxRibbonPanel* pEditTools = new wxRibbonPanel(pEdit, wxID_ANY, wxT("Edit"), _PNG("gfx/32/copy.png"));
	wxRibbonPanel* pStory = new wxRibbonPanel(pEdit, wxID_ANY, wxT("History"), _PNG("gfx/32/undo.png"));
	wxRibbonPanel* pFormat = new wxRibbonPanel(pEdit, wxID_ANY, wxT("Format"), _PNG("gfx/22/space1_5.png"));
	wxRibbonPanel* pList = new wxRibbonPanel(pEdit, wxID_ANY, wxT("Lists"));
	wxRibbonPanel* pStyle = new wxRibbonPanel(pEdit, wxID_ANY, wxT("Styles"), _PNG("gfx/32/font.png"));
	wxRibbonPanel* pStyleChoice = new wxRibbonPanel(pEdit, wxID_ANY, wxT("Paragraph"));

	wxRibbonButtonBar* pEditToolSection = new wxRibbonButtonBar(pEditTools, wxID_ANY);
	pEditToolSection->AddButton(wxID_CUT, wxT("Cut"), _PNG("gfx/32/cut.png"), wxT("Cuts the selection"));
	pEditToolSection->AddButton(wxID_COPY, wxT("Copy"), _PNG("gfx/32/copy.png"), wxT("Copies the selection to the clipboard"));
	pEditToolSection->AddButton(wxID_PASTE, wxT("Paste"), _PNG("gfx/32/paste.png"), wxT("Pastes text from the clipboard"));
	pEditToolSection->AddButton(wxID_SELECTALL, wxT("Select all"), _PNG("gfx/32/select_all.png"), wxT("Selects all"));

	wxRibbonButtonBar* pStorySection = new wxRibbonButtonBar(pStory, wxID_ANY);
	pStorySection->AddButton(wxID_UNDO, wxT("Undo"), _PNG("gfx/32/undo.png"), wxT("Undoes the last operation"));
	pStorySection->AddButton(wxID_REDO, wxT("Redo"), _PNG("gfx/32/redo.png"), wxT("Redoes the last operation"));

	wxRibbonToolBar* pFormatToolBar = new wxRibbonToolBar(pFormat, wxID_ANY);
	pFormatToolBar->AddToggleTool(ID_BOLD, _PNG("gfx/22/format_bold.png"), wxT("Bold"));
	pFormatToolBar->AddToggleTool(ID_ITALIC, _PNG("gfx/22/format_italic.png"), wxT("Italic"));
	pFormatToolBar->AddToggleTool(ID_UNDERLINE, _PNG("gfx/22/format_underline.png"), wxT("Underline"));
	pFormatToolBar->AddSeparator();
	pFormatToolBar->AddToggleTool(ID_LEFT, _PNG("gfx/22/format_left.png"), wxT("Align left"));
	pFormatToolBar->AddToggleTool(ID_CENTER, _PNG("gfx/22/format_center.png"), wxT("Center"));
	pFormatToolBar->AddToggleTool(ID_RIGHT, _PNG("gfx/22/format_right.png"), wxT("Align right"));
	pFormatToolBar->AddToggleTool(ID_JUSTIFY, _PNG("gfx/22/format_justify.png"), wxT("Justify"));
	pFormatToolBar->AddSeparator();
	pFormatToolBar->AddTool(ID_INDENT_PLUS, _PNG("gfx/22/indent_plus.png"), wxT("Increase indent"));
	pFormatToolBar->AddTool(ID_INDENT_MINUS, _PNG("gfx/22/indent_minus.png"), wxT("Decrease indent"));
	pFormatToolBar->AddDropdownTool(ID_INTERLINE, _PNG("gfx/22/space1_5.png"));
	pFormatToolBar->AddSeparator();
	pFormatToolBar->AddTool(ID_SUPERSCRIPT, _PNG("gfx/22/superscript.png"), wxT("Superscript"));
	pFormatToolBar->AddTool(ID_SUBSCRIPT, _PNG("gfx/22/subscript.png"), wxT("Subscript"));
	pFormatToolBar->SetRows(2, 3);

	wxRibbonButtonBar* pListSection = new wxRibbonButtonBar(pList, wxID_ANY);
	pListSection->AddButton(ID_ORDER_LIST, wxT("Numbered list"), _PNG("gfx/32/ordered.png"), wxT("Numbered list"));
	pListSection->AddButton(ID_UNORDER_LIST, wxT("Bulleted list"), _PNG("gfx/32/unordered.png"), wxT("Bulleted list"));
	pListSection->AddDropdownButton(ID_LIST, wxT("Tools"), _PNG("gfx/32/renumber.png"));

	wxRibbonButtonBar* pStyleSection = new wxRibbonButtonBar(pStyle, wxID_ANY);
	pStyleSection->AddButton(ID_FONT, wxT("Font"), _PNG("gfx/32/font.png"), wxT("Font"));
	pStyleSection->AddButton(ID_STYLE, wxT("Styles"), _PNG("gfx/32/style.png"), wxT("Style management"));

	wxRichTextStyleListBox* style_ctrl = new wxRichTextStyleListBox(pStyleChoice, ID_STYLE_CTRL);
	style_ctrl->SetStyleSheet(style_sheet); // Attaching the style sheet
	style_ctrl->SetRichTextCtrl(richtext); // Attaching the editor
	style_ctrl->UpdateStyles(); // Updating styles
	style_ctrl->SetMaxSize(wxSize(100, -1));

	// Insert
	wxRibbonPanel* pInsertTools = new wxRibbonPanel(pInsert, wxID_ANY, wxT("Insert"));
	wxRibbonPanel* pSymbolGallery = new wxRibbonPanel(pInsert, wxID_ANY, wxT("Special character"));
	wxRibbonPanel* pPrintAddons = new wxRibbonPanel(pInsert, wxID_ANY, wxT("Print fields"));

	wxRibbonButtonBar* pInsertTSection = new wxRibbonButtonBar(pInsertTools, wxID_ANY);
	pInsertTSection->AddButton(ID_SYMBOL, wxT("Special character"), _PNG("gfx/32/char.png"), wxT("Special character"));
	pInsertTSection->AddButton(ID_URL, wxT("URL"), _PNG("gfx/32/url.png"), wxT("URL"));
	pInsertTSection->AddButton(ID_IMAGE, wxT("Image from file"), _PNG("gfx/32/image.png"), wxT("Image from file"));
	pInsertTSection->AddButton(ID_TABLE, wxT("Table"), _PNG("gfx/32/table.png"), wxT("Table"));

	wxRibbonGallery* symbolGallery = new wxRibbonGallery(pSymbolGallery, ID_SYMBOL_GALLERY);
	for (int i = 161; i <= 191; i++)
	{
		symbolGallery->Append(CreateSymbolBmp(i), wxID_ANY, new SymbolGalleryData(i));
	}
	this->Bind(wxEVT_BUTTON, [](wxCommandEvent&) { wxMessageBox(wxT("Additional gallery actions!")); }, ID_SYMBOL_GALLERY);

	wxRibbonButtonBar* pPrintASection = new wxRibbonButtonBar(pPrintAddons, wxID_ANY);
	pPrintASection->AddButton(ID_HEADER, wxT("Header"), _PNG("gfx/32/header.png"), wxT("Header"));
	pPrintASection->AddButton(ID_FOOTER, wxT("Footer"), _PNG("gfx/32/footer.png"), wxT("Footer"));

	// Settings
	wxRibbonPanel* pAppear = new wxRibbonPanel(pSet, wxID_ANY, wxT("Appearance"), _PNG("gfx/32/color1.png"));

	wxRibbonButtonBar* pAppearSection = new wxRibbonButtonBar(pAppear, wxID_ANY);
	pAppearSection->AddButton(ID_COLOUR1, wxT("Background color"), _PNG("gfx/32/color1.png"), wxT("Background color"));
	pAppearSection->AddButton(ID_COLOUR2, wxT("Active color"), _PNG("gfx/32/color2.png"), wxT("Active elements color"));
	pAppearSection->AddButton(ID_COLOUR3, wxT("Text color"), _PNG("gfx/32/color3.png"), wxT("Text color"));
	pAppearSection->AddButton(ID_COLOURD, wxT("Restore default"), _PNG("gfx/32/refresh.png"), wxT("Restores default colors"));

	rb->Realize();
	//=================================================================================================

	panel_sizer->Add(rb, 0, wxEXPAND | wxALL, 0);
	panel_sizer->Add(richtext, 1, wxEXPAND | wxALL, 0);
	main_sizer->Add(panel, 1, wxEXPAND | wxALL, 0);

    CenterOnScreen();

	//===========================================================
	// Loading test file
	if (wxFileExists(wxT("test.wxd")))
	{
		richtext->LoadFile(wxT("test.wxd"), wxRICHTEXT_TYPE_XML);
	}
	//===========================================================
}

//---------------------------------------------------------------------------
// MyFrame destructor
//-------------------------------------------------------------------------
MyFrame::~MyFrame()
{
	wxDELETE(style_sheet);
	wxDELETE(printing);
}

//---------------------------------------------------------------------------
// Function initiates program closure
//---------------------------------------------------------------------------
void MyFrame::OnClose(wxRibbonButtonBarEvent& WXUNUSED(event))
{
    this->Close(true);
}

//---------------------------------------------------------------------------
// Function displays the "About" window
//---------------------------------------------------------------------------
void MyFrame::OnAboutProgram(wxRibbonButtonBarEvent& event)
{
    wxMessageBox(wxString::Format(wxT("%s Copyright (c) %d by..."), appName, wxDateTime::Now().GetYear()), wxT("About..."), wxOK|wxICON_INFORMATION);
	event.Skip();
}

//---------------------------------------------------------------------------
// Function displays a prompt to close the program
//---------------------------------------------------------------------------
void MyFrame::CloseApp(wxCloseEvent& WXUNUSED(event))
{
	if (richtext->IsModified() && !richtext->GetValue().IsEmpty())
	{
		int ret = wxMessageBox(wxT("The file has unsaved changes.\nDo you want to save them now?"), wxT("Question"), wxYES_NO | wxCANCEL | wxICON_QUESTION);

		if (ret == wxCANCEL)
		{
			return;
		}
		else  if (ret == wxYES)
		{
			if (file_path.IsEmpty())
			{
				SaveAs();
			}
			else
			{
				Save(file_path);
			}
		}
	}
	
    this->Destroy();
}

//---------------------------------------------------------------------------
// File utility functions
//---------------------------------------------------------------------------
wxRichTextFileType MyFrame::GetFileType(const wxString& path)
{
	wxFileName fn(path);
	wxString ext = fn.GetExt();
	wxRichTextFileType type = wxRICHTEXT_TYPE_ANY;

	ext.MakeLower(); // Convert to lowercase
	if (ext == wxT("wxd")) { type = wxRICHTEXT_TYPE_XML; }
	else if (ext == wxT("html")) { type = wxRICHTEXT_TYPE_HTML; }
	else if (ext == wxT("txt")) { type = wxRICHTEXT_TYPE_TEXT; }
	else if (ext == wxT("rtf")) { type = wxRICHTEXT_TYPE_RTF; }
	else if (ext == wxT("pdf")) { type = wxRICHTEXT_TYPE_PDF; }

	return type;
}

void MyFrame::Save(const wxString& path)
{
	if (!richtext->SaveFile(path, GetFileType(path)))
	{
		wxMessageBox(wxT("An error occurred while saving the file!"));
	}
}

void MyFrame::SaveAs()
{
	wxString filter = wxT("WX Document files (wxd)|*.wxd|HTML files (html)|*.html|Text files (txt)|*.txt");
	wxString file = wxFileSelector(wxT("Save file"), wxEmptyString, wxEmptyString, wxT("wxd"), filter, wxFD_SAVE);
	if (!file.IsEmpty())
	{
		Save(file);
		file_path = file;
	}
}

//---------------------------------------------------------------------------
// Open
//---------------------------------------------------------------------------
void MyFrame::OnOpen(wxRibbonButtonBarEvent& WXUNUSED(event))
{
	wxString filter = wxT("WX Document files (wxd)|*.wxd|HTML files (html)|*.html|Text files (txt)|*.txt");
	wxString file = wxFileSelector(wxT("Open file"), wxEmptyString, wxEmptyString, wxT("wxd"), filter, wxFD_OPEN);
	if (!file.IsEmpty())
	{
		if (richtext->LoadFile(file, GetFileType(file)))
		{
			file_path = file;
		}
		else
		{
			wxMessageBox(wxT("An error occurred while opening the file."));
		}
	}
}

//---------------------------------------------------------------------------
// Save
//---------------------------------------------------------------------------
void MyFrame::OnSave(wxRibbonButtonBarEvent& WXUNUSED(event))
{
	if (file_path.IsEmpty())
	{
		SaveAs();
	}
	else
	{
		Save(file_path);
	}
}

//---------------------------------------------------------------------------
// Save as
//---------------------------------------------------------------------------
void MyFrame::OnSaveAs(wxRibbonButtonBarEvent& WXUNUSED(event))
{
	SaveAs();
}

//---------------------------------------------------------------------------
// Print
//---------------------------------------------------------------------------
void MyFrame::OnPrint(wxRibbonButtonBarEvent& WXUNUSED(event))
{
	printing->PrintBuffer(richtext->GetBuffer());
}

//---------------------------------------------------------------------------
// Print preview
//---------------------------------------------------------------------------
void MyFrame::OnPrintPreview(wxRibbonButtonBarEvent& WXUNUSED(event))
{
	printing->PreviewBuffer(richtext->GetBuffer());
}

//---------------------------------------------------------------------------
// Page setup
//---------------------------------------------------------------------------
void MyFrame::OnPrintSetup(wxRibbonButtonBarEvent& WXUNUSED(event))
{
	printing->PageSetup();
}

//---------------------------------------------------------------------------
// Edit
//---------------------------------------------------------------------------
void MyFrame::OnEdit(wxRibbonButtonBarEvent& event)
{
	switch (event.GetId())
	{
		case wxID_CUT: richtext->Cut(); break;
		case wxID_COPY: richtext->Copy(); break;
		case wxID_PASTE: richtext->Paste(); break;
		case wxID_SELECTALL: richtext->SelectAll(); break;
		case wxID_UNDO: richtext->Undo(); break;
		case wxID_REDO: richtext->Redo(); break;
		default: return;
	}
}

//---------------------------------------------------------------------------
// Format
//---------------------------------------------------------------------------
void MyFrame::OnFormat(wxRibbonToolBarEvent& event)
{
	switch (event.GetId())
	{
		case ID_BOLD: richtext->ApplyBoldToSelection(); break; // Bold
		case ID_ITALIC: richtext->ApplyItalicToSelection(); break; // Italic
		case ID_UNDERLINE: richtext->ApplyUnderlineToSelection(); break; // Underline
		case ID_LEFT: richtext->ApplyAlignmentToSelection(wxTEXT_ALIGNMENT_LEFT); break; // Align left
		case ID_RIGHT: richtext->ApplyAlignmentToSelection(wxTEXT_ALIGNMENT_RIGHT); break; // Align right
		case ID_CENTER: richtext->ApplyAlignmentToSelection(wxTEXT_ALIGNMENT_CENTER); break; // Center
		case ID_JUSTIFY: richtext->ApplyAlignmentToSelection(wxTEXT_ALIGNMENT_JUSTIFIED); break; // Justify
		default: return;
	}
}

//---------------------------------------------------------------------------
// Function updates the GUI based on the attributes of the selected text
//---------------------------------------------------------------------------
void MyFrame::OnUpdate(wxUpdateUIEvent& event)
{
	switch (event.GetId())
	{
		case ID_BOLD: event.Check(richtext->IsSelectionBold()); break;
		case ID_ITALIC: event.Check(richtext->IsSelectionItalics()); break;
		case ID_UNDERLINE: event.Check(richtext->IsSelectionUnderlined()); break;
		case ID_LEFT: event.Check(richtext->IsSelectionAligned(wxTEXT_ALIGNMENT_LEFT)); break;
		case ID_RIGHT: event.Check(richtext->IsSelectionAligned(wxTEXT_ALIGNMENT_RIGHT)); break;
		case ID_CENTER: event.Check(richtext->IsSelectionAligned(wxTEXT_ALIGNMENT_CENTER)); break;
		case ID_JUSTIFY: event.Check(richtext->IsSelectionAligned(wxTEXT_ALIGNMENT_JUSTIFIED)); break;
		default: return;
	}
}

//---------------------------------------------------------------------------
// Superscript
//---------------------------------------------------------------------------
void MyFrame::OnSuperscript(wxRibbonToolBarEvent& WXUNUSED(event))
{
	wxRichTextRange range;
	long point = richtext->GetInsertionPoint();

	// Checking the range of data to change
	if (richtext->HasSelection())
	{
		range = richtext->GetSelectionRange();
	}
	else
	{
		range = wxRichTextRange(point, point);
	}

	richtext->SetStyle(range, style_sheet->FindCharacterStyle(STYLE_NAME_E1)->GetStyle());
}

//---------------------------------------------------------------------------
// Subscript
//---------------------------------------------------------------------------
void MyFrame::OnSubscript(wxRibbonToolBarEvent& WXUNUSED(event))
{
	wxRichTextRange range;
	long point = richtext->GetInsertionPoint();

	// Checking the range of data to change
	if (richtext->HasSelection())
	{
		range = richtext->GetSelectionRange();
	}
	else
	{
		range = wxRichTextRange(point, point);
	}

	richtext->SetStyle(range, style_sheet->FindCharacterStyle(STYLE_NAME_E2)->GetStyle());
}

//---------------------------------------------------------------------------
// Indent
//---------------------------------------------------------------------------
void MyFrame::OnIndent(wxRibbonToolBarEvent& event)
{
	wxRichTextRange range;
	long point = richtext->GetInsertionPoint();
	int indent = 0;

	// Checking the range of data to change
	if (richtext->HasSelection())
	{
		range = richtext->GetSelectionRange();
	}
	else
	{
		range = wxRichTextRange(point, point);
	}

	wxRichTextAttr attr;
	if (richtext->GetStyle(point, attr))
	{
		indent = attr.GetLeftIndent();

		// Defining the indent
		switch (event.GetId())
		{
			case ID_INDENT_PLUS: indent += 50; break;
			case ID_INDENT_MINUS: indent -= 50; break;
			default: indent = 0;
		}

		// Setting the new indent for the range
		attr.SetFlags(wxTEXT_ATTR_LEFT_INDENT);
		attr.SetLeftIndent(wxMax(0, indent));
		richtext->SetStyle(range, attr);
	}
}

//---------------------------------------------------------------------------
// Line spacing - menu
//---------------------------------------------------------------------------
void MyFrame::OnSetInterline(wxRibbonToolBarEvent& event)
{
	wxMenu menu;
	menu.Append(ID_INTERLINE1_0, wxT("Line spacing 1"), wxT("Line spacing 1"));
	menu.Append(ID_INTERLINE1_5, wxT("Line spacing 1.5"), wxT("Line spacing 1.5"));
	menu.Append(ID_INTERLINE2_0, wxT("Line spacing 2"), wxT("Line spacing 2"));

	event.PopupMenu(&menu);
}

//---------------------------------------------------------------------------
// Line spacing
//---------------------------------------------------------------------------
void MyFrame::OnInterline(wxCommandEvent& event)
{
	wxRichTextRange range;
	long point = richtext->GetInsertionPoint();
	int interline = 0;

	// Checking the range of data to change
	if (richtext->HasSelection())
	{
		range = richtext->GetSelectionRange();
	}
	else
	{	
		range = wxRichTextRange(point, point);
	}

	// Getting the current attributes
	wxRichTextAttr attr;
	if (richtext->GetStyle(point, attr))
	{
		// Defining the line spacing
		switch (event.GetId())
		{
			case ID_INTERLINE1_0: interline = 0; break;
			case ID_INTERLINE1_5: interline = 10; break;
			case ID_INTERLINE2_0: interline = 20; break;
			default: interline = 0;
		}

		// Setting the new line spacing for the range
		attr.SetFlags(wxTEXT_ATTR_LINE_SPACING);
		attr.SetLineSpacing(interline);
		richtext->SetStyle(range, attr);
	}
}

//---------------------------------------------------------------------------
// Font
//---------------------------------------------------------------------------
void MyFrame::OnFont(wxRibbonButtonBarEvent& WXUNUSED(event))
{
	wxRichTextRange range;
	if (!richtext->HasSelection()) return;

	int pages = wxRICHTEXT_FORMAT_FONT;
	range = richtext->GetSelectionRange();

	wxRichTextFormattingDialog dlg(pages, this);
	dlg.GetStyle(richtext, range);

	if (dlg.ShowModal() == wxID_OK)
	{
		dlg.ApplyStyle(richtext, range, wxRICHTEXT_SETSTYLE_WITH_UNDO | wxRICHTEXT_SETSTYLE_OPTIMIZE | wxRICHTEXT_SETSTYLE_CHARACTERS_ONLY);
	}
}

//---------------------------------------------------------------------------
// Style management
//---------------------------------------------------------------------------
void MyFrame::OnStyle(wxRibbonButtonBarEvent& WXUNUSED(event))
{
	wxRichTextStyleOrganiserDialog* dlg = new wxRichTextStyleOrganiserDialog(
		wxRICHTEXT_ORGANISER_CREATE_STYLES |  // Create styles
		wxRICHTEXT_ORGANISER_DELETE_STYLES |  // Delete styles
		wxRICHTEXT_ORGANISER_EDIT_STYLES |    // Edit styles
		wxRICHTEXT_ORGANISER_RENAME_STYLES |  // Rename styles
		wxRICHTEXT_ORGANISER_APPLY_STYLES |   // Apply styles
		wxRICHTEXT_ORGANISER_RENUMBER |       // Renumber lists
		//wxRICHTEXT_ORGANISER_SHOW_PARAGRAPH | // Paragraph styles
		//wxRICHTEXT_ORGANISER_SHOW_LIST |      // List styles
		//wxRICHTEXT_ORGANISER_SHOW_BOX |       // Box styles
		//wxRICHTEXT_ORGANISER_SHOW_CHARACTER | // Character styles (e.g., URL)
		wxRICHTEXT_ORGANISER_SHOW_ALL |       // Show all styles
		wxRICHTEXT_ORGANISER_OK_CANCEL,       // OK and Cancel buttons
		style_sheet, richtext, this);
	dlg->ShowModal();
}

//---------------------------------------------------------------------------
// Lists
//---------------------------------------------------------------------------
void MyFrame::OnList(wxRibbonButtonBarEvent& event)
{
	if (event.GetId() == ID_LIST)
	{
		wxMenu mList;
		mList.Append(ID_LIST_LVL_UP, wxT("Increase list level"), wxT("Increases list level"));
		mList.Append(ID_LIST_LVL_DOWN, wxT("Decrease list level"), wxT("Decreases list level"));
		mList.AppendSeparator();
		mList.Append(ID_LIST_REFRESH, wxT("Renumber list"), wxT("Renumber list"));
		mList.AppendSeparator();
		mList.Append(ID_LIST_CLEAR, wxT("Disable list"), wxT("Disables list for the paragraph"));

		event.PopupMenu(&mList);
	}
	else
	{
		wxString style(STYLE_NAME_O1);
		if (event.GetId() == ID_UNORDER_LIST)
		{
			style = STYLE_NAME_U1;
		}

		if (richtext->HasSelection())
		{
			wxRichTextRange range = richtext->GetSelectionRange();
			richtext->SetListStyle(range, style, wxRICHTEXT_SETSTYLE_WITH_UNDO | wxRICHTEXT_SETSTYLE_RENUMBER);
		}
	}
}

//---------------------------------------------------------------------------
// List levels
//---------------------------------------------------------------------------
void MyFrame::OnListLevel(wxCommandEvent& event)
{
	if (richtext->HasSelection())
	{
		wxRichTextRange range = richtext->GetSelectionRange();
		richtext->PromoteList((event.GetId() == ID_LIST_LVL_UP) ? 1 : -1, range, NULL);
	}
}

//---------------------------------------------------------------------------
// Renumber list
//---------------------------------------------------------------------------
void MyFrame::OnListRefresh(wxCommandEvent& WXUNUSED(event))
{
	if (richtext->HasSelection())
	{
		wxRichTextRange range = richtext->GetSelectionRange();
		richtext->NumberList(range, NULL, wxRICHTEXT_SETSTYLE_WITH_UNDO | wxRICHTEXT_SETSTYLE_RENUMBER);
	}
}

//---------------------------------------------------------------------------
// Remove list
//---------------------------------------------------------------------------
void MyFrame::OnListClear(wxCommandEvent& WXUNUSED(event))
{
	if (richtext->HasSelection())
	{
		wxRichTextRange range = richtext->GetSelectionRange();
		richtext->ClearListStyle(range);
	}
}

//---------------------------------------------------------------------------
// Special character
//---------------------------------------------------------------------------
void MyFrame::OnSymbol(wxRibbonButtonBarEvent& WXUNUSED(event))
{
	// Reading style attributes at the insertion point
	long cursor = richtext->GetInsertionPoint();
	wxRichTextAttr attr;
	attr.SetFlags(wxTEXT_ATTR_FONT);
	richtext->GetStyle(cursor, attr);

	// Determining the current font
	wxString fname;
	if (attr.HasFont() && attr.GetFont().Ok())
	{
		fname = attr.GetFont().GetFaceName();
	}

	// Selecting a special character
	wxSymbolPickerDialog dlg(wxT("*"), wxEmptyString, fname, this);
	if (dlg.ShowModal() == wxID_OK)
	{
		if (dlg.HasSelection())
		{
			// Inserting the special character
			richtext->WriteText(dlg.GetSymbol());

			// If a custom font is used...
			if (!dlg.UseNormalFont())
			{
				// Setting the selected font for the special character
				attr.SetFont(wxFont(dlg.GetFontName()));
				richtext->SetStyle(cursor, cursor + 1, attr);
			}
		}
	}
}

//---------------------------------------------------------------------------
// URL
//---------------------------------------------------------------------------
void MyFrame::OnURL(wxRibbonButtonBarEvent& WXUNUSED(event))
{
	wxString desc = wxEmptyString;
	wxString url = wxGetTextFromUser(wxT("Enter address"), wxT("Address"));
	if (!url.IsEmpty())
	{
		desc = wxGetTextFromUser(wxT("Displayed text (empty if same as address)"), wxT("Text"));
		if (desc.IsEmpty())
		{
			desc = url;
		}

		richtext->BeginURL(url, STYLE_NAME_A1);
		richtext->WriteText(desc);
		richtext->EndURL();
	}
}

//---------------------------------------------------------------------------
// Insert image
//---------------------------------------------------------------------------
void MyFrame::OnImage(wxRibbonButtonBarEvent& WXUNUSED(event))
{
	wxString filter = wxT("PNG files (png)|*.png|JPG files (jpg)|*.jpg|BMP files|*.bmp");
	wxString file = wxFileSelector(wxT("Open file"), wxEmptyString, wxEmptyString, wxT("png"), filter, wxFD_OPEN);
	if (!file.IsEmpty())
	{
		richtext->WriteImage(file, wxBITMAP_TYPE_PNG);
	}
}

//---------------------------------------------------------------------------
// Insert table
//---------------------------------------------------------------------------
void MyFrame::OnTable(wxRibbonButtonBarEvent& WXUNUSED(event))
{
	int rows = wxGetNumberFromUser(wxT("Rows"), wxEmptyString, wxT("Rows"), 5);
	if (rows < 0) return;

	int cols = wxGetNumberFromUser(wxT("Columns"), wxEmptyString, wxT("Columns"), 4);
	if (cols < 0) return;

	wxRichTextAttr t_attr = style_sheet->FindBoxStyle(STYLE_NAME_T1)->GetStyle();
	wxRichTextAttr c_attr = style_sheet->FindBoxStyle(STYLE_NAME_C1)->GetStyle();
	richtext->WriteTable(rows, cols, t_attr, c_attr);
}

//---------------------------------------------------------------------------
// Header
//---------------------------------------------------------------------------
void MyFrame::OnHeader(wxRibbonButtonBarEvent& WXUNUSED(event))
{
	HFPrintEditior dlg(this, HF_HEADER_TYPE);
	if (dlg.ShowModal() == wxID_OK)
	{
		dlg.SetPrintSettings(printing);
	}
}

//---------------------------------------------------------------------------
// Footer
//---------------------------------------------------------------------------
void MyFrame::OnFooter(wxRibbonButtonBarEvent& WXUNUSED(event))
{
	HFPrintEditior dlg(this, HF_FOOTER_TYPE);
	if (dlg.ShowModal() == wxID_OK)
	{
		dlg.SetPrintSettings(printing);
	}
}

//---------------------------------------------------------------------------
// Show source
//---------------------------------------------------------------------------
void MyFrame::OnShowSource(wxRibbonButtonBarEvent& event)
{
	wxMenu menu;
	menu.Append(ID_SHOW_XML, wxT("Show XML"), wxT("Show XML"));
	menu.Append(ID_SHOW_HTML, wxT("Show HTML"), wxT("Show HTML"));

	event.PopupMenu(&menu);
}

//---------------------------------------------------------------------------
// Show XML source
//---------------------------------------------------------------------------
void MyFrame::OnShowXML(wxCommandEvent& WXUNUSED(event))
{
	wxMessageBox(GetXML(), wxT("XML Source"));
}

//---------------------------------------------------------------------------
// Show HTML source
//---------------------------------------------------------------------------
void MyFrame::OnShowHTML(wxCommandEvent& WXUNUSED(event))
{
	wxMessageBox(GetHTML(), wxT("HTML Source"));
}

//---------------------------------------------------------------------------
// Get XML source as wxString
//---------------------------------------------------------------------------
wxString MyFrame::GetXML()
{
	wxString xml;
	wxStringOutputStream sstream(&xml);
	wxRichTextXMLHandler handler;

	if (handler.SaveFile(&richtext->GetBuffer(), sstream))
	{
		return xml;
	}
	else
	{
		wxMessageBox(wxT("An error occurred while reading XML data!"));
		return wxEmptyString;
	}
}

//---------------------------------------------------------------------------
// Get HTML source as wxString
//---------------------------------------------------------------------------
wxString MyFrame::GetHTML()
{
	wxString html;
	wxStringOutputStream sstream(&html);
	wxRichTextHTMLHandler handler;

	if (handler.SaveFile(&richtext->GetBuffer(), sstream))
	{
		return html;
	}
	else
	{
		wxMessageBox(wxT("An error occurred while reading HTML data!"));
		return wxEmptyString;
	}
}

//---------------------------------------------------------------------------
// Function creates initial styles
//---------------------------------------------------------------------------
void MyFrame::InitStyle()
{
	//
	wxArrayInt tabs;
	tabs.Add(50);
	tabs.Add(100);
	tabs.Add(150);
	tabs.Add(200);
	tabs.Add(250);

	wxFont romanFont(12, wxFONTFAMILY_ROMAN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);

	// Paragraph (normal)
	wxRichTextParagraphStyleDefinition *p1 = new wxRichTextParagraphStyleDefinition(STYLE_NAME_P1);
	wxRichTextAttr p1_attr;
	p1_attr.SetFlags(wxTEXT_ATTR_FONT |
		wxTEXT_ATTR_TABS |
		wxTEXT_ATTR_PARA_SPACING_BEFORE |
		wxTEXT_ATTR_PARA_SPACING_AFTER);
	p1_attr.SetFontFaceName(romanFont.GetFaceName());
	p1_attr.SetFontSize(12);
	p1_attr.SetParagraphSpacingAfter(0);
	p1_attr.SetParagraphSpacingBefore(0);
	p1_attr.SetTabs(tabs);
	p1->SetStyle(p1_attr);
	style_sheet->AddParagraphStyle(p1);

	// Header 1
	wxRichTextParagraphStyleDefinition *h1 = new wxRichTextParagraphStyleDefinition(STYLE_NAME_H1);
	wxRichTextAttr h1_attr;
	h1_attr.SetFlags(wxTEXT_ATTR_FONT |
		wxTEXT_ATTR_TEXT_COLOUR |
		wxTEXT_ATTR_PARA_SPACING_AFTER);
	h1_attr.SetFontFaceName(romanFont.GetFaceName());
	h1_attr.SetFontSize(20);
	h1_attr.SetTextColour(wxColour(128, 128, 128));
	h1_attr.SetFontWeight(wxFONTWEIGHT_BOLD);
	h1_attr.SetParagraphSpacingAfter(10);
	h1->SetStyle(h1_attr);
	style_sheet->AddParagraphStyle(h1);

	// Header 2
	wxRichTextParagraphStyleDefinition *h2 = new wxRichTextParagraphStyleDefinition(STYLE_NAME_H2);
	wxRichTextAttr h2_attr;
	h2_attr.SetFlags(wxTEXT_ATTR_FONT |
		wxTEXT_ATTR_PARA_SPACING_BEFORE |
		wxTEXT_ATTR_PARA_SPACING_AFTER);
	h2_attr.SetFontFaceName(romanFont.GetFaceName());
	h2_attr.SetFontSize(16);
	h2_attr.SetFontWeight(wxFONTWEIGHT_BOLD);
	h2_attr.SetParagraphSpacingAfter(5);
	h2_attr.SetParagraphSpacingBefore(0);
	h2->SetStyle(h2_attr);
	style_sheet->AddParagraphStyle(h2);

	// URL
	wxRichTextCharacterStyleDefinition *a1 = new wxRichTextCharacterStyleDefinition(STYLE_NAME_A1);
	wxRichTextAttr a1_attr;
	a1_attr.SetFlags(wxTEXT_ATTR_FONT);
	a1_attr.SetFontFaceName(romanFont.GetFaceName());
	a1_attr.SetTextColour(*wxBLUE);
	a1_attr.SetFontUnderlined(true);
	a1->SetStyle(a1_attr);
	style_sheet->AddCharacterStyle(a1);

	// Superscript
	wxRichTextCharacterStyleDefinition *e1 = new wxRichTextCharacterStyleDefinition(STYLE_NAME_E1);
	wxRichTextAttr e1_attr;
	e1_attr.SetFlags(wxTEXT_ATTR_FONT | wxTEXT_ATTR_EFFECTS);
	e1_attr.SetFontFaceName(romanFont.GetFaceName());
	e1_attr.SetTextEffectFlags(wxTEXT_ATTR_EFFECT_SUPERSCRIPT);
	e1_attr.SetTextEffects(wxTEXT_ATTR_EFFECT_SUPERSCRIPT);
	e1->SetStyle(e1_attr);
	style_sheet->AddCharacterStyle(e1);

	// Subscript
	wxRichTextCharacterStyleDefinition *e2 = new wxRichTextCharacterStyleDefinition(STYLE_NAME_E2);
	wxRichTextAttr e2_attr;
	e2_attr.SetFlags(wxTEXT_ATTR_FONT | wxTEXT_ATTR_EFFECTS);
	e2_attr.SetFontFaceName(romanFont.GetFaceName());
	e2_attr.SetTextEffectFlags(wxTEXT_ATTR_EFFECT_SUBSCRIPT);
	e2_attr.SetTextEffects(wxTEXT_ATTR_EFFECT_SUBSCRIPT);
	e2->SetStyle(e2_attr);
	style_sheet->AddCharacterStyle(e2);

	// Numbered list
	wxRichTextListStyleDefinition* ol = new wxRichTextListStyleDefinition(STYLE_NAME_O1);
	ol->SetAttributes(0, 30, 45, wxTEXT_ATTR_BULLET_STYLE_ARABIC | wxTEXT_ATTR_BULLET_STYLE_PERIOD);
	ol->SetAttributes(1, 60, 45, wxTEXT_ATTR_BULLET_STYLE_LETTERS_UPPER | wxTEXT_ATTR_BULLET_STYLE_PARENTHESES);
	ol->SetAttributes(2, 90, 45, wxTEXT_ATTR_BULLET_STYLE_LETTERS_LOWER | wxTEXT_ATTR_BULLET_STYLE_PARENTHESES);
	ol->SetAttributes(3, 120, 45, wxTEXT_ATTR_BULLET_STYLE_ROMAN_LOWER | wxTEXT_ATTR_BULLET_STYLE_PARENTHESES);
	ol->SetAttributes(4, 150, 45, wxTEXT_ATTR_BULLET_STYLE_ROMAN_UPPER | wxTEXT_ATTR_BULLET_STYLE_PARENTHESES);
	ol->SetAttributes(5, 180, 45, wxTEXT_ATTR_BULLET_STYLE_ARABIC | wxTEXT_ATTR_BULLET_STYLE_PARENTHESES);
	ol->SetAttributes(6, 210, 45, wxTEXT_ATTR_BULLET_STYLE_LETTERS_UPPER | wxTEXT_ATTR_BULLET_STYLE_PARENTHESES);
	ol->SetAttributes(7, 240, 45, wxTEXT_ATTR_BULLET_STYLE_LETTERS_LOWER | wxTEXT_ATTR_BULLET_STYLE_PARENTHESES);
	ol->SetAttributes(8, 270, 45, wxTEXT_ATTR_BULLET_STYLE_ROMAN_LOWER | wxTEXT_ATTR_BULLET_STYLE_PARENTHESES);
	ol->SetAttributes(9, 300, 45, wxTEXT_ATTR_BULLET_STYLE_ROMAN_UPPER | wxTEXT_ATTR_BULLET_STYLE_PARENTHESES);
	style_sheet->AddListStyle(ol);

	// Numbered list 2
	wxRichTextListStyleDefinition* ol2 = new wxRichTextListStyleDefinition(STYLE_NAME_O2);
	ol2->SetAttributes(0, 30, 45, wxTEXT_ATTR_BULLET_STYLE_ARABIC | wxTEXT_ATTR_BULLET_STYLE_PERIOD);
	for (int i = 1; i < 10; i++)
	{
		ol2->SetAttributes(i, ((i + 1) * 30), 45, wxTEXT_ATTR_BULLET_STYLE_ARABIC | wxTEXT_ATTR_BULLET_STYLE_PARENTHESES);
	}
	style_sheet->AddListStyle(ol2);

	// Bulleted list
	wxRichTextListStyleDefinition* ul = new wxRichTextListStyleDefinition(STYLE_NAME_U1);
	for (int i = 0; i < 10; i++)
	{
		ul->SetAttributes(i, ((i + 1) * 30), 45, wxTEXT_ATTR_BULLET_STYLE_STANDARD, ((i % 2 == 0) ? wxT("standard/circle") : wxT("standard/square")));
	}
	style_sheet->AddListStyle(ul);

	// Default table
	wxRichTextBoxStyleDefinition* t1 = new wxRichTextBoxStyleDefinition(STYLE_NAME_T1);
	wxRichTextAttr t1_attr;
	t1_attr.SetFlags(wxTEXT_BOX_ATTR_BORDER_COLOUR);
	t1_attr.GetTextBoxAttr().GetBorder().SetColour(wxColor(100, 100, 100));
	t1_attr.GetTextBoxAttr().GetBorder().SetStyle(wxTEXT_BOX_ATTR_BORDER_SOLID);
	t1_attr.GetTextBoxAttr().GetBorder().SetWidth(1, wxTEXT_ATTR_UNITS_PIXELS);
	t1->SetStyle(t1_attr);
	style_sheet->AddBoxStyle(t1);

	// Default table cell
	wxRichTextBoxStyleDefinition* c1 = new wxRichTextBoxStyleDefinition(STYLE_NAME_C1);
	wxRichTextAttr c1_attr;
	c1_attr.SetFlags(wxTEXT_ATTR_FONT | 
		wxTEXT_ATTR_LEFT_INDENT |
		wxTEXT_ATTR_RIGHT_INDENT |
		wxTEXT_ATTR_PARA_SPACING_BEFORE |
		wxTEXT_ATTR_PARA_SPACING_AFTER |
		wxTEXT_BOX_ATTR_BORDER_COLOUR);
	c1_attr.SetFontFaceName(romanFont.GetFaceName());
	c1_attr.SetFontSize(10);
	c1_attr.SetLeftIndent(15);
	c1_attr.SetRightIndent(15);
	c1_attr.SetParagraphSpacingBefore(15);
	c1_attr.SetParagraphSpacingAfter(15);
	c1_attr.GetTextBoxAttr().GetBorder().SetColour(wxColour(128, 128, 128));
	c1_attr.GetTextBoxAttr().GetBorder().SetStyle(wxTEXT_BOX_ATTR_BORDER_SOLID);
	c1_attr.GetTextBoxAttr().GetBorder().GetLeft().SetWidth(1, wxTEXT_ATTR_UNITS_PIXELS);
	c1_attr.GetTextBoxAttr().GetBorder().GetBottom().SetWidth(1, wxTEXT_ATTR_UNITS_PIXELS);
	c1->SetStyle(c1_attr);
	style_sheet->AddBoxStyle(c1);
	
	// Setting the default style
	richtext->SetDefaultStyle(p1_attr);
}

//---------------------------------------------------------------------------
// Function creates a bitmap representing a character with the given code
//---------------------------------------------------------------------------
wxBitmap MyFrame::CreateSymbolBmp(int code, int size, float scale, int yoffset)
{
	int x = 0, y = 0, sw = 0, sh = 0;

	wxString s = wxUniChar(code);
	wxBitmap bmp(size, size);
	wxMemoryDC dc(bmp);

	dc.SetPen(wxPen(wxColour(200, 200, 200)));
	dc.SetBrush(*wxWHITE);
	dc.DrawRectangle(0, 0, size, size);
	dc.SetTextForeground(*wxBLACK);
	dc.SetFont(wxFont(bmp.GetHeight() * scale, wxFONTFAMILY_ROMAN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
	
	dc.GetTextExtent(s, &sw, &sh);
	x = (bmp.GetWidth() / 2) - (sw / 2);
	y = ((bmp.GetHeight() / 2) - (sh / 2)) - yoffset;
	
	dc.DrawText(s, x, y);

	dc.SelectObject(wxNullBitmap);

	return bmp;
}

//---------------------------------------------------------------------------
// Function handles the special character gallery and inserts the symbol into the text
//---------------------------------------------------------------------------
void MyFrame::OnSymbolGallery(wxRibbonGalleryEvent& event)
{
	SymbolGalleryData* data = static_cast<SymbolGalleryData*>(event.GetGallery()->GetItemClientObject(event.GetGalleryItem()));
	richtext->WriteText(data->GetSymbol());
}

//---------------------------------------------------------------------------
// Function handles color changes
//---------------------------------------------------------------------------
void MyFrame::OnColour(wxRibbonButtonBarEvent& event)
{
	wxRibbonArtProvider *art = rb->GetArtProvider();
	art->GetColourScheme(&colour1, &colour2, NULL);

	switch (event.GetId())
	{
	case ID_COLOUR1: colour1 = wxGetColourFromUser(this); break;
	case ID_COLOUR2: colour2 = wxGetColourFromUser(this); break;
	case ID_COLOUR3: text_colour = wxGetColourFromUser(this); break;
	default: 
		colour1 = default_colour1;
		colour2 = default_colour2;
		text_colour = default_text_colour;
		break;
	}

	art->SetColourScheme(colour1, colour2, wxNullColour);
	art->SetColor(wxRIBBON_ART_BUTTON_BAR_LABEL_COLOUR, text_colour);
	art->SetColor(wxRIBBON_ART_TAB_LABEL_COLOUR, text_colour);
	art->SetColor(wxRIBBON_ART_PANEL_LABEL_COLOUR, text_colour);
	art->SetColor(wxRIBBON_ART_PANEL_HOVER_LABEL_COLOUR, text_colour);

	rb->Refresh();
}