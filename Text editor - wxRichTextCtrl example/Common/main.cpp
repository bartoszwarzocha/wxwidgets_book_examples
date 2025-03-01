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

#include <wx/richtext/richtextctrl.h> // Class wxRichTextCtrl
#include <wx/richtext/richtextprint.h> // Printing
#include <wx/richtext/richtextstyledlg.h> // Style management dialog
#include <wx/richtext/richtextformatdlg.h> // Formatting dialog
#include <wx/richtext/richtextxml.h> // XML support
#include <wx/richtext/richtexthtml.h> // HTML support
#include <wx/richtext/richtextsymboldlg.h> // Special characters dialog

#include <wx/sstream.h>
#include <wx/fontenum.h>
#include <wx/listctrl.h>
#include <wx/numdlg.h>

#include "utils.h"
#include "HFPrintEditor.h"
#include "main.h"

const wxString appName = wxT("wxRichTextCtrl Presentation");

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
	ID_INTERLINE1_0,
	ID_INTERLINE1_5,
	ID_INTERLINE2_0,
	ID_FONT,
	ID_STYLE,
	//
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
	ID_SHOW_XML,
	ID_SHOW_HTML,
	//
	ID_STYLE_CTRL
};

//---------------------------------------------------------------------------
// Event table
//---------------------------------------------------------------------------
BEGIN_EVENT_TABLE(MyFrame, wxFrame)
EVT_MENU(ID_CLOSE, MyFrame::OnClose)
EVT_MENU(ID_ABOUT_PROGRAM, MyFrame::OnAboutProgram)
EVT_CLOSE(MyFrame::CloseApp)
//
EVT_MENU(wxID_OPEN, MyFrame::OnOpen)
EVT_MENU(wxID_SAVE, MyFrame::OnSave)
EVT_MENU(wxID_SAVEAS, MyFrame::OnSaveAs)
EVT_MENU(ID_PRINT, MyFrame::OnPrint)
EVT_MENU(ID_PRINT_PREVIEW, MyFrame::OnPrintPreview)
EVT_MENU(ID_PRINT_SETUP, MyFrame::OnPrintSetup)
//
EVT_MENU(wxID_CUT, MyFrame::OnEdit)
EVT_MENU(wxID_COPY, MyFrame::OnEdit)
EVT_MENU(wxID_PASTE, MyFrame::OnEdit)
EVT_MENU(wxID_SELECTALL, MyFrame::OnEdit)
EVT_MENU(wxID_UNDO, MyFrame::OnEdit)
EVT_MENU(wxID_REDO, MyFrame::OnEdit)
//
EVT_MENU_RANGE(ID_BOLD, ID_JUSTIFY, MyFrame::OnFormat)
EVT_MENU(ID_SUPERSCRIPT, MyFrame::OnSuperscript)
EVT_MENU(ID_SUBSCRIPT, MyFrame::OnSubscript)
EVT_MENU_RANGE(ID_INDENT_PLUS, ID_INDENT_MINUS, MyFrame::OnIndent)
EVT_MENU_RANGE(ID_INTERLINE1_0, ID_INTERLINE2_0, MyFrame::OnInterline)
EVT_MENU(ID_FONT, MyFrame::OnFont)
EVT_MENU(ID_STYLE, MyFrame::OnStyle)
//
EVT_MENU_RANGE(ID_ORDER_LIST, ID_UNORDER_LIST, MyFrame::OnList)
EVT_MENU_RANGE(ID_LIST_LVL_UP, ID_LIST_LVL_DOWN, MyFrame::OnListLevel)
EVT_MENU(ID_LIST_REFRESH, MyFrame::OnListRefresh)
EVT_MENU(ID_LIST_CLEAR, MyFrame::OnListClear)
//
EVT_MENU(ID_SYMBOL, MyFrame::OnSymbol)
EVT_MENU(ID_URL, MyFrame::OnURL)
EVT_MENU(ID_HEADER, MyFrame::OnHeader)
EVT_MENU(ID_FOOTER, MyFrame::OnFooter)
EVT_MENU(ID_IMAGE, MyFrame::OnImage)
EVT_MENU(ID_TABLE, MyFrame::OnTable)
//
EVT_MENU(ID_SHOW_XML, MyFrame::OnShowXML)
EVT_MENU(ID_SHOW_HTML, MyFrame::OnShowHTML)
//
EVT_UPDATE_UI_RANGE(ID_BOLD, ID_JUSTIFY, MyFrame::OnUpdate)
END_EVENT_TABLE()

//---------------------------------------------------------------------------
// OnInit()
//---------------------------------------------------------------------------
bool MyApp::OnInit()
{
	wxInitAllImageHandlers();

	// POLISH LANGUAGE FOR DIALOGS
	// Setting Polish language for dialog buttons.
	// Dialogs by default have buttons with labels in English.
	// Polish language is loaded from the pl/pl.mo file.
	// More about this can be found in the fourth part of the book.
	// DISABLED - Please enable if you have installed your language pack
	//loc.Init(wxLANGUAGE_POLISH, wxLOCALE_LOAD_DEFAULT);
	//loc.AddCatalog("pl");

	MyFrame* frame = new MyFrame(NULL, wxID_ANY, appName, wxDefaultPosition, wxSize(800, 600), wxDEFAULT_FRAME_STYLE);
	frame->Show(true);
	return true;
}

//---------------------------------------------------------------------------
// MyFrame constructor
//---------------------------------------------------------------------------
MyFrame::MyFrame(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
	: wxFrame(parent, id, title, pos, size, style)
{
	// Initializing variables
	file_type = wxRICHTEXT_TYPE_ANY;
	file_path = wxEmptyString;

	// MENU
	mb = new wxMenuBar();

	mFile = new wxMenu();
	mEdit = new wxMenu();
	mFormat = new wxMenu();
	mList = new wxMenu();
	mInsert = new wxMenu();
	mHelp = new wxMenu();

	mFile->Append(wxID_OPEN, wxT("Open\tCtrl+O"), wxT("Open document"));
	mFile->Append(wxID_SAVE, wxT("Save\tCtrl+S"), wxT("Save document"));
	mFile->Append(wxID_SAVEAS, wxT("Save as..."), wxT("Save document with specified name"));
	mFile->AppendSeparator();
	mFile->Append(ID_PRINT, wxT("Print"), wxT("Print document"));
	mFile->Append(ID_PRINT_PREVIEW, wxT("Print preview"), wxT("Print preview"));
	mFile->Append(ID_PRINT_SETUP, wxT("Page setup"), wxT("Page setup"));
	mFile->AppendSeparator();
	mFile->Append(ID_CLOSE, wxT("&Close program\tCtrl+Q"), wxT("Close program"));

	mEdit->Append(wxID_CUT, wxT("Cut\tCtrl+X"), wxT("Cut selection"));
	mEdit->Append(wxID_COPY, wxT("Copy\tCtrl+C"), wxT("Copy selection to clipboard"));
	mEdit->Append(wxID_PASTE, wxT("Paste\tCtrl+V"), wxT("Paste clipboard content"));
	mEdit->AppendSeparator();
	mEdit->Append(wxID_SELECTALL, wxT("Select all\tCtrl+A"), wxT("Select all"));
	mEdit->AppendSeparator();
	mEdit->Append(wxID_UNDO, wxT("Undo\tCtrl+Z"), wxT("Undo last change"));
	mEdit->Append(wxID_REDO, wxT("Redo\tCtrl+Y"), wxT("Redo last change"));

	mFormat->AppendCheckItem(ID_BOLD, wxT("Bold\tCtrl+B"), wxT("Bold selection"));
	mFormat->AppendCheckItem(ID_ITALIC, wxT("Italic\tCtrl+I"), wxT("Italic selection"));
	mFormat->AppendCheckItem(ID_UNDERLINE, wxT("Underline\tCtrl+U"), wxT("Underline selection"));
	mFormat->AppendSeparator();
	mFormat->AppendCheckItem(ID_LEFT, wxT("Align left"), wxT("Align selection left"));
	mFormat->AppendCheckItem(ID_RIGHT, wxT("Align right"), wxT("Align selection right"));
	mFormat->AppendCheckItem(ID_CENTER, wxT("Center"), wxT("Center selection"));
	mFormat->AppendCheckItem(ID_JUSTIFY, wxT("Justify"), wxT("Justify selection"));
	mFormat->AppendSeparator();
	mFormat->Append(ID_SUPERSCRIPT, wxT("Superscript"), wxT("Superscript"));
	mFormat->Append(ID_SUBSCRIPT, wxT("Subscript"), wxT("Subscript"));
	mFormat->AppendSeparator();
	mFormat->Append(ID_INTERLINE1_0, wxT("Line spacing 1"), wxT("Line spacing 1"));
	mFormat->Append(ID_INTERLINE1_5, wxT("Line spacing 1.5"), wxT("Line spacing 1.5"));
	mFormat->Append(ID_INTERLINE2_0, wxT("Line spacing 2"), wxT("Line spacing 2"));
	mFormat->AppendSeparator();
	mFormat->Append(ID_INDENT_PLUS, wxT("Increase indent"), wxT("Increase indent"));
	mFormat->Append(ID_INDENT_MINUS, wxT("Decrease indent"), wxT("Decrease indent"));
	mFormat->AppendSeparator();
	mFormat->Append(ID_FONT, wxT("Font"), wxT("Set font for selection"));
	mFormat->AppendSeparator();
	mFormat->Append(ID_STYLE, wxT("Styles"), wxT("Text styles"));

	mList->Append(ID_ORDER_LIST, wxT("Numbered list"), wxT("Numbered list"));
	mList->Append(ID_UNORDER_LIST, wxT("Bulleted list"), wxT("Bulleted list"));
	mList->AppendSeparator();
	mList->Append(ID_LIST_LVL_UP, wxT("Increase list level"), wxT("Increase list level"));
	mList->Append(ID_LIST_LVL_DOWN, wxT("Decrease list level"), wxT("Decrease list level"));
	mList->AppendSeparator();
	mList->Append(ID_LIST_REFRESH, wxT("Renumber list"), wxT("Renumber list"));
	mList->AppendSeparator();
	mList->Append(ID_LIST_CLEAR, wxT("Disable list"), wxT("Disable list for paragraph"));

	mInsert->Append(ID_SYMBOL, wxT("Special character"), wxT("Insert special character"));
	mInsert->Append(ID_URL, wxT("URL"), wxT("Insert URL"));
	mInsert->Append(ID_IMAGE, wxT("Image from file"), wxT("Insert image from file"));
	mInsert->Append(ID_TABLE, wxT("Table"), wxT("Insert table"));
	mInsert->AppendSeparator();
	mInsert->Append(ID_HEADER, wxT("Header"), wxT("Edit header"));
	mInsert->Append(ID_FOOTER, wxT("Footer"), wxT("Edit footer"));


	mHelp->Append(ID_SHOW_XML, wxT("Show XML source"), wxT("Display XML source"));
	mHelp->Append(ID_SHOW_HTML, wxT("Show HTML source"), wxT("Display HTML source"));
	mHelp->AppendSeparator();
	mHelp->Append(ID_ABOUT_PROGRAM, wxT("&About\tF1"), wxT("About"));

	mb->Append(mFile, wxT("&File"));
	mb->Append(mEdit, wxT("&Edit"));
	mb->Append(mFormat, wxT("&Format"));
	mb->Append(mList, wxT("&Lists"));
	mb->Append(mInsert, wxT("&Insert"));
	mb->Append(mHelp, wxT("&Help"));

	this->SetMenuBar(mb);

	// Tool Bar
	tb = CreateToolBar();
	const wxSize ToolBarBitmapSize(22, 22);
	tb->SetToolBitmapSize(ToolBarBitmapSize);

	tb->AddTool(wxID_OPEN, wxEmptyString, wxBitmap(wxGetAppFile(wxT("gfx/22/doc_open.png")), wxBITMAP_TYPE_PNG), wxT("Open"));
	tb->AddTool(wxID_SAVE, wxEmptyString, wxBitmap(wxGetAppFile(wxT("gfx/22/doc_save.png")), wxBITMAP_TYPE_PNG), wxT("Save"));
	tb->AddTool(wxID_SAVEAS, wxEmptyString, wxBitmap(wxGetAppFile(wxT("gfx/22/doc_save_as.png")), wxBITMAP_TYPE_PNG), wxT("Save as..."));
	tb->AddSeparator();
	tb->AddTool(ID_PRINT, wxEmptyString, wxBitmap(wxGetAppFile(wxT("gfx/22/print.png")), wxBITMAP_TYPE_PNG), wxT("Print"));
	tb->AddTool(ID_PRINT_PREVIEW, wxEmptyString, wxBitmap(wxGetAppFile(wxT("gfx/22/print_preview.png")), wxBITMAP_TYPE_PNG), wxT("Print preview"));
	tb->AddSeparator();
	tb->AddTool(wxID_UNDO, wxEmptyString, wxBitmap(wxGetAppFile(wxT("gfx/22/undo.png")), wxBITMAP_TYPE_PNG), wxT("Undo"));
	tb->AddTool(wxID_REDO, wxEmptyString, wxBitmap(wxGetAppFile(wxT("gfx/22/redo.png")), wxBITMAP_TYPE_PNG), wxT("Redo"));
	tb->AddSeparator();
	tb->AddTool(ID_FONT, wxEmptyString, wxBitmap(wxGetAppFile(wxT("gfx/22/font.png")), wxBITMAP_TYPE_PNG), wxT("Change font"));
	tb->AddTool(ID_STYLE, wxEmptyString, wxBitmap(wxGetAppFile(wxT("gfx/22/style.png")), wxBITMAP_TYPE_PNG), wxT("Styles"));
	tb->AddSeparator();
	tb->AddCheckTool(ID_BOLD, wxEmptyString, wxBitmap(wxGetAppFile(wxT("gfx/22/format_bold.png")), wxBITMAP_TYPE_PNG), wxNullBitmap, wxT("Bold"));
	tb->AddCheckTool(ID_ITALIC, wxEmptyString, wxBitmap(wxGetAppFile(wxT("gfx/22/format_italic.png")), wxBITMAP_TYPE_PNG), wxNullBitmap, wxT("Italic"));
	tb->AddCheckTool(ID_UNDERLINE, wxEmptyString, wxBitmap(wxGetAppFile(wxT("gfx/22/format_underline.png")), wxBITMAP_TYPE_PNG), wxNullBitmap, wxT("Underline"));
	tb->AddSeparator();
	tb->AddCheckTool(ID_LEFT, wxEmptyString, wxBitmap(wxGetAppFile(wxT("gfx/22/format_left.png")), wxBITMAP_TYPE_PNG), wxNullBitmap, wxT("Align left"));
	tb->AddCheckTool(ID_CENTER, wxEmptyString, wxBitmap(wxGetAppFile(wxT("gfx/22/format_center.png")), wxBITMAP_TYPE_PNG), wxNullBitmap, wxT("Center"));
	tb->AddCheckTool(ID_RIGHT, wxEmptyString, wxBitmap(wxGetAppFile(wxT("gfx/22/format_right.png")), wxBITMAP_TYPE_PNG), wxNullBitmap, wxT("Align right"));
	tb->AddCheckTool(ID_JUSTIFY, wxEmptyString, wxBitmap(wxGetAppFile(wxT("gfx/22/format_justify.png")), wxBITMAP_TYPE_PNG), wxNullBitmap, wxT("Justify"));
	tb->AddSeparator();
	tb->AddTool(ID_ORDER_LIST, wxEmptyString, wxBitmap(wxGetAppFile(wxT("gfx/22/list_ordered.png")), wxBITMAP_TYPE_PNG), wxT("Numbered list"));
	tb->AddTool(ID_UNORDER_LIST, wxEmptyString, wxBitmap(wxGetAppFile(wxT("gfx/22/list_unordered.png")), wxBITMAP_TYPE_PNG), wxT("Bulleted list"));
	tb->AddSeparator();
	tb->AddTool(ID_SYMBOL, wxEmptyString, wxBitmap(wxGetAppFile(wxT("gfx/22/symbol.png")), wxBITMAP_TYPE_PNG), wxT("Special character"));
	tb->AddTool(ID_IMAGE, wxEmptyString, wxBitmap(wxGetAppFile(wxT("gfx/22/image.png")), wxBITMAP_TYPE_PNG), wxT("Image"));

	tb->Realize();

	// Status Bar
	sb = new wxStatusBar(this);
	sb->SetFieldsCount(2);
	this->SetStatusBar(sb);
	SetStatusText(title, 0);

	// Main sizer & panel
	wxBoxSizer* main_sizer = new wxBoxSizer(wxVERTICAL);
	this->SetSizer(main_sizer);

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

	// Enable editing
	richtext->SetEditable(true);

	// Style control
	wxRichTextStyleComboCtrl* style_ctrl = new wxRichTextStyleComboCtrl(panel, ID_STYLE_CTRL, wxDefaultPosition, wxSize(150, -1));
	style_ctrl->SetStyleSheet(style_sheet); // Attach style sheet
	style_ctrl->SetRichTextCtrl(richtext); // Attach editor
	style_ctrl->UpdateStyles(); // Update styles

	panel_sizer->Add(richtext, 1, wxEXPAND | wxALL, 0);
	panel_sizer->Add(style_ctrl, 0, wxALL, 4);
	main_sizer->Add(panel, 1, wxEXPAND | wxALL, 0);

	CenterOnScreen();

	//===========================================================
	// Load test file
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
void MyFrame::OnClose(wxCommandEvent& WXUNUSED(event))
{
	this->Close(true);
}

//---------------------------------------------------------------------------
// Function displays "About" window
//---------------------------------------------------------------------------
void MyFrame::OnAboutProgram(wxCommandEvent& event)
{
	wxMessageBox(wxString::Format(wxT("%s Copyright (c) %d by..."), appName, wxDateTime::Now().GetYear()), wxT("About"), wxOK | wxICON_INFORMATION);
	event.Skip();
}

//---------------------------------------------------------------------------
// Function displays prompt to close the program
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
void MyFrame::OnOpen(wxCommandEvent& WXUNUSED(event))
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
void MyFrame::OnSave(wxCommandEvent& WXUNUSED(event))
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
void MyFrame::OnSaveAs(wxCommandEvent& WXUNUSED(event))
{
	SaveAs();
}

//---------------------------------------------------------------------------
// Print
//---------------------------------------------------------------------------
void MyFrame::OnPrint(wxCommandEvent& WXUNUSED(event))
{
	printing->PrintBuffer(richtext->GetBuffer());
}

//---------------------------------------------------------------------------
// Print preview
//---------------------------------------------------------------------------
void MyFrame::OnPrintPreview(wxCommandEvent& WXUNUSED(event))
{
	printing->PreviewBuffer(richtext->GetBuffer());
}

//---------------------------------------------------------------------------
// Page setup
//---------------------------------------------------------------------------
void MyFrame::OnPrintSetup(wxCommandEvent& WXUNUSED(event))
{
	printing->PageSetup();
}

//---------------------------------------------------------------------------
// Edit
//---------------------------------------------------------------------------
void MyFrame::OnEdit(wxCommandEvent& event)
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
void MyFrame::OnFormat(wxCommandEvent& event)
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
void MyFrame::OnSuperscript(wxCommandEvent& WXUNUSED(event))
{
	wxRichTextRange range;
	long point = richtext->GetInsertionPoint();

	// Determine the range of data to change
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
void MyFrame::OnSubscript(wxCommandEvent& WXUNUSED(event))
{
	wxRichTextRange range;
	long point = richtext->GetInsertionPoint();

	// Determine the range of data to change
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
void MyFrame::OnIndent(wxCommandEvent& event)
{
	wxRichTextRange range;
	long point = richtext->GetInsertionPoint();
	int indent = 0;

	// Determine the range of data to change
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

		// Define the indentation
		switch (event.GetId())
		{
		case ID_INDENT_PLUS: indent += 50; break;
		case ID_INDENT_MINUS: indent -= 50; break;
		default: indent = 0;
		}

		// Set the new indentation for the range
		attr.SetFlags(wxTEXT_ATTR_LEFT_INDENT);
		attr.SetLeftIndent(wxMax(0, indent));
		richtext->SetStyle(range, attr);
	}
}

//---------------------------------------------------------------------------
// Line spacing
//---------------------------------------------------------------------------
void MyFrame::OnInterline(wxCommandEvent& event)
{
	wxRichTextRange range;
	long point = richtext->GetInsertionPoint();
	int interline = 0;

	// Determine the range of data to change
	if (richtext->HasSelection())
	{
		range = richtext->GetSelectionRange();
	}
	else
	{
		range = wxRichTextRange(point, point);
	}

	// Get the current attributes
	wxRichTextAttr attr;
	if (richtext->GetStyle(point, attr))
	{
		// Define the line spacing
		switch (event.GetId())
		{
		case ID_INTERLINE1_0: interline = 0; break;
		case ID_INTERLINE1_5: interline = 10; break;
		case ID_INTERLINE2_0: interline = 20; break;
		default: interline = 0;
		}

		// Set the new line spacing for the range
		attr.SetFlags(wxTEXT_ATTR_LINE_SPACING);
		attr.SetLineSpacing(interline);
		richtext->SetStyle(range, attr);
	}
}

//---------------------------------------------------------------------------
// Font
//---------------------------------------------------------------------------
void MyFrame::OnFont(wxCommandEvent& WXUNUSED(event))
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
void MyFrame::OnStyle(wxCommandEvent& WXUNUSED(event))
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
		wxRICHTEXT_ORGANISER_SHOW_ALL |     // Show all styles
		wxRICHTEXT_ORGANISER_OK_CANCEL,       // OK and Cancel buttons
		style_sheet, richtext, this);
	dlg->ShowModal();
}

//---------------------------------------------------------------------------
// Lists
//---------------------------------------------------------------------------
void MyFrame::OnList(wxCommandEvent& event)
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
// Clear list
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
void MyFrame::OnSymbol(wxCommandEvent& WXUNUSED(event))
{
	// Read style attributes at the insertion point
	long cursor = richtext->GetInsertionPoint();
	wxRichTextAttr attr;
	attr.SetFlags(wxTEXT_ATTR_FONT);
	richtext->GetStyle(cursor, attr);

	// Determine the current font
	wxString fname;
	if (attr.HasFont() && attr.GetFont().Ok())
	{
		fname = attr.GetFont().GetFaceName();
	}

	// Select special character
	wxSymbolPickerDialog dlg(wxT("*"), wxEmptyString, fname, this);
	if (dlg.ShowModal() == wxID_OK)
	{
		if (dlg.HasSelection())
		{
			// Insert special character
			richtext->WriteText(dlg.GetSymbol());

			// If a custom font is used...
			if (!dlg.UseNormalFont())
			{
				// Set the selected font for the special character
				attr.SetFont(wxFont(dlg.GetFontName()));
				richtext->SetStyle(cursor, cursor + 1, attr);
			}
		}
	}
}

//---------------------------------------------------------------------------
// URL
//---------------------------------------------------------------------------
void MyFrame::OnURL(wxCommandEvent& WXUNUSED(event))
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
void MyFrame::OnImage(wxCommandEvent& WXUNUSED(event))
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
void MyFrame::OnTable(wxCommandEvent& WXUNUSED(event))
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
void MyFrame::OnHeader(wxCommandEvent& WXUNUSED(event))
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
void MyFrame::OnFooter(wxCommandEvent& WXUNUSED(event))
{
	HFPrintEditior dlg(this, HF_FOOTER_TYPE);
	if (dlg.ShowModal() == wxID_OK)
	{
		dlg.SetPrintSettings(printing);
	}
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
	wxRichTextParagraphStyleDefinition* p1 = new wxRichTextParagraphStyleDefinition(STYLE_NAME_P1);
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
	wxRichTextParagraphStyleDefinition* h1 = new wxRichTextParagraphStyleDefinition(STYLE_NAME_H1);
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
	wxRichTextParagraphStyleDefinition* h2 = new wxRichTextParagraphStyleDefinition(STYLE_NAME_H2);
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
	wxRichTextCharacterStyleDefinition* a1 = new wxRichTextCharacterStyleDefinition(STYLE_NAME_A1);
	wxRichTextAttr a1_attr;
	a1_attr.SetFlags(wxTEXT_ATTR_FONT);
	a1_attr.SetFontFaceName(romanFont.GetFaceName());
	a1_attr.SetTextColour(*wxBLUE);
	a1_attr.SetFontUnderlined(true);
	a1->SetStyle(a1_attr);
	style_sheet->AddCharacterStyle(a1);

	// Superscript
	wxRichTextCharacterStyleDefinition* e1 = new wxRichTextCharacterStyleDefinition(STYLE_NAME_E1);
	wxRichTextAttr e1_attr;
	e1_attr.SetFlags(wxTEXT_ATTR_FONT | wxTEXT_ATTR_EFFECTS);
	e1_attr.SetFontFaceName(romanFont.GetFaceName());
	e1_attr.SetTextEffectFlags(wxTEXT_ATTR_EFFECT_SUPERSCRIPT);
	e1_attr.SetTextEffects(wxTEXT_ATTR_EFFECT_SUPERSCRIPT);
	e1->SetStyle(e1_attr);
	style_sheet->AddCharacterStyle(e1);

	// Subscript
	wxRichTextCharacterStyleDefinition* e2 = new wxRichTextCharacterStyleDefinition(STYLE_NAME_E2);
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

	// Set default style
	richtext->SetDefaultStyle(p1_attr);
}
