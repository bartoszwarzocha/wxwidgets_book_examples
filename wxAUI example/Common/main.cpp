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

#include <wx/aui/framemanager.h> // wxAuiManager
#include <wx/aui/auibook.h>      // wxAuiNotebook
#include <wx/aui/auibar.h>       // wxAuiToolBar
#include <wx/aui/dockart.h>      // wxAuiDockArt

#include <wx/treectrl.h>
#include <wx/listctrl.h>

#include "utils.h"
#include "main.h"

const wxString appName = wxT("wxAUI Presentation 1.0.0");

//---------------------------------------------------------------------------
// Application Implementation
//---------------------------------------------------------------------------
IMPLEMENT_APP(MyApp)

//---------------------------------------------------------------------------
// Identifiers
//---------------------------------------------------------------------------
enum
{
	ID_NEW_PROJECT = wxID_HIGHEST + 1,
	ID_OPEN_PROJECT,
	ID_ADD_EXIST_FILE,
	ID_SAVEALL,
	ID_RENAME,
	ID_CLOSE_FILE,
	ID_CLOSE,
	ID_FULLSCR,
	ID_WRAP,
	ID_NEW_CLASS,
	//
	ID_TREE,
	ID_CONSOLE,
	ID_NOTEBOOK
};

//---------------------------------------------------------------------------
// Event Table
//---------------------------------------------------------------------------
BEGIN_EVENT_TABLE(MyFrame, wxFrame)
	EVT_MENU(ID_NEW_PROJECT, MyFrame::OnAction)
	EVT_MENU(ID_OPEN_PROJECT, MyFrame::OnAction)
	EVT_MENU(wxID_OPEN, MyFrame::OnAction)
	EVT_MENU(wxID_NEW, MyFrame::OnAction)
	EVT_MENU(ID_ADD_EXIST_FILE, MyFrame::OnAction)
	EVT_MENU(wxID_SAVE, MyFrame::OnAction)
	EVT_MENU(wxID_SAVEAS, MyFrame::OnAction)
	EVT_MENU(ID_SAVEALL, MyFrame::OnAction)
	EVT_MENU(ID_RENAME, MyFrame::OnAction)
	EVT_MENU(wxID_REMOVE, MyFrame::OnAction)
	EVT_MENU(ID_CLOSE_FILE, MyFrame::OnAction)
	EVT_MENU(wxID_PRINT, MyFrame::OnAction)
	EVT_MENU(wxID_PREVIEW, MyFrame::OnAction)
	EVT_MENU(wxID_PAGE_SETUP, MyFrame::OnAction)
	EVT_MENU(ID_CLOSE, MyFrame::OnAction)
	EVT_MENU(wxID_COPY, MyFrame::OnAction)
	EVT_MENU(wxID_CUT, MyFrame::OnAction)
	EVT_MENU(wxID_PASTE, MyFrame::OnAction)
	EVT_MENU(wxID_SELECTALL, MyFrame::OnAction)
	EVT_MENU(wxID_UNDO, MyFrame::OnAction)
	EVT_MENU(wxID_REDO, MyFrame::OnAction)
	EVT_MENU(wxID_FIND, MyFrame::OnAction)
	EVT_MENU(wxID_REPLACE, MyFrame::OnAction)
	EVT_MENU(ID_FULLSCR, MyFrame::OnAction)
	EVT_MENU(ID_WRAP, MyFrame::OnAction)
	EVT_MENU(ID_NEW_CLASS, MyFrame::OnAction)
	EVT_MENU(wxID_ABOUT, MyFrame::OnAboutProgram)
	EVT_CLOSE(MyFrame::CloseApp)
	//
	EVT_AUINOTEBOOK_PAGE_CHANGED(ID_NOTEBOOK, MyFrame::OnPageChange)
	EVT_AUINOTEBOOK_PAGE_CLOSE(ID_NOTEBOOK, MyFrame::OnPageClose)
	//
	EVT_CHECKBOX(ID_WRAP, MyFrame::OnWrap)
END_EVENT_TABLE()

//---------------------------------------------------------------------------
// OnInit()
//---------------------------------------------------------------------------
bool MyApp::OnInit()
{
	wxInitAllImageHandlers();

    MyFrame *frame = new MyFrame(NULL, wxID_ANY, appName, wxDefaultPosition, wxSize(800,600), wxDEFAULT_FRAME_STYLE);
    frame->Show(true);
    return true;
}

//---------------------------------------------------------------------------
// MyFrame Constructor
//---------------------------------------------------------------------------
MyFrame::MyFrame(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &pos, const wxSize &size, long style)
: wxFrame(parent, id, title, pos, size, style)
{
	// MENU BAR
	mb = new wxMenuBar();

	mFile = new wxMenu();
	mAddFile = new wxMenu();
	mEdit = new wxMenu();
	mView = new wxMenu();
	mTools = new wxMenu();
	mHelp = new wxMenu();

	// File Menu -> Add file
	wxMenuItem *new_file = new wxMenuItem(mAddFile, wxID_NEW, wxT("New file\tCtrl-N"), wxT("Add a new file to the project"));
	new_file->SetBitmap(wxBitmap(wxGetAppFile("gfx/16/new_file.png"), wxBITMAP_TYPE_PNG));
	mAddFile->Append(new_file);
	mAddFile->Append(ID_ADD_EXIST_FILE, wxT("Existing file"), wxT("Add an existing file to the project"));

	// File Menu
	wxMenuItem *new_project = new wxMenuItem(mFile, ID_NEW_PROJECT, wxT("New project\tCtrl-Shift-N"), wxT("Create a new project"));
	new_project->SetBitmap(wxBitmap(wxGetAppFile("gfx/16/new_project.png"), wxBITMAP_TYPE_PNG));
	mFile->Append(new_project);
	wxMenuItem *open_project = new wxMenuItem(mFile, ID_OPEN_PROJECT, wxT("Open project\tCtrl-Shift-O"), wxT("Open a project"));
	open_project->SetBitmap(wxBitmap(wxGetAppFile("gfx/16/open_project.png"), wxBITMAP_TYPE_PNG));
	mFile->Append(open_project);
	mFile->AppendSeparator();
	wxMenuItem *open_file = new wxMenuItem(mFile, wxID_OPEN, wxT("Open file\tCtrl-O"), wxT("Open any file outside the project"));
	open_file->SetBitmap(wxBitmap(wxGetAppFile("gfx/16/open_project.png"), wxBITMAP_TYPE_PNG));
	mFile->Append(open_file);
	mFile->AppendSubMenu(mAddFile, wxT("Add file"));
	wxMenuItem *save_file = new wxMenuItem(mFile, wxID_SAVE, wxT("Save file\tCtrl-S"), wxT("Save the current file"));
	save_file->SetBitmap(wxBitmap(wxGetAppFile("gfx/16/save_file.png"), wxBITMAP_TYPE_PNG));
	mFile->Append(save_file);
	wxMenuItem *save_as = new wxMenuItem(mFile, wxID_SAVEAS, wxT("Save file as...\tCtrl-Shift-S"), wxT("Save the current file under a specified name"));
	save_as->SetBitmap(wxBitmap(wxGetAppFile("gfx/16/save_as.png"), wxBITMAP_TYPE_PNG));
	mFile->Append(save_as);
	wxMenuItem *save_all = new wxMenuItem(mFile, ID_SAVEALL, wxT("Save all"), wxT("Save all unsaved changes"));
	save_all->SetBitmap(wxBitmap(wxGetAppFile("gfx/16/save_all.png"), wxBITMAP_TYPE_PNG));
	mFile->Append(save_all);
	mFile->Append(ID_RENAME, wxT("Rename file"), wxT("Rename the file"));
	mFile->Append(wxID_REMOVE, wxT("Delete file"), wxT("Delete the file from the project or disk"));
	mFile->Append(ID_CLOSE_FILE, wxT("Close file\tCtrl-Alt-F"), wxT("Close the current file"));
	mFile->AppendSeparator();
	wxMenuItem *print = new wxMenuItem(mFile, wxID_PRINT, wxT("Print\tCtrl-P"), wxT("Print the current document"));
	print->SetBitmap(wxBitmap(wxGetAppFile("gfx/16/print.png"), wxBITMAP_TYPE_PNG));
	mFile->Append(print);
	wxMenuItem *preview = new wxMenuItem(mFile, wxID_PREVIEW, wxT("Print preview\tCtrl-Shift-P"), wxT("Print preview"));
	preview->SetBitmap(wxBitmap(wxGetAppFile("gfx/16/preview.png"), wxBITMAP_TYPE_PNG));
	mFile->Append(preview);
	mFile->Append(wxID_PAGE_SETUP, wxT("Page setup"), wxT("Page setup"));
	mFile->AppendSeparator();
	mFile->Append(ID_CLOSE, wxT("&Close program\tCtrl-Shift-X"), wxT("Close the program"));

	// Edit Menu
	wxMenuItem *copy = new wxMenuItem(mEdit, wxID_COPY, wxT("Copy\tCtrl-C"), wxT("Copy the selection to the clipboard"));
	copy->SetBitmap(wxBitmap(wxGetAppFile("gfx/16/copy.png"), wxBITMAP_TYPE_PNG));
	mEdit->Append(copy);
	wxMenuItem *cut = new wxMenuItem(mEdit, wxID_CUT, wxT("Cut\tCtrl-X"), wxT("Cut the selection and save it to the clipboard"));
	cut->SetBitmap(wxBitmap(wxGetAppFile("gfx/16/cut.png"), wxBITMAP_TYPE_PNG));
	mEdit->Append(cut);
	wxMenuItem *paste = new wxMenuItem(mEdit, wxID_PASTE, wxT("Paste\tCtrl-V"), wxT("Paste the contents of the clipboard"));
	paste->SetBitmap(wxBitmap(wxGetAppFile("gfx/16/paste.png"), wxBITMAP_TYPE_PNG));
	mEdit->Append(paste);
	mEdit->AppendSeparator();
	mEdit->Append(wxID_SELECTALL, wxT("Select all\tCtrl-A"), wxT("Select the contents of the current document"));
	mEdit->Append(wxID_REVERT, wxT("Invert selection\tCtrl-R"), wxT("Invert the selection"));
	mEdit->AppendSeparator();
	wxMenuItem *undo = new wxMenuItem(mEdit, wxID_UNDO, wxT("Undo\tCtrl-Z"), wxT("Undo the last operation"));
	undo->SetBitmap(wxBitmap(wxGetAppFile("gfx/16/undo.png"), wxBITMAP_TYPE_PNG));
	mEdit->Append(undo);
	wxMenuItem *redo = new wxMenuItem(mEdit, wxID_REDO, wxT("Redo\tCtrl-Y"), wxT("Redo the last operation"));
	redo->SetBitmap(wxBitmap(wxGetAppFile("gfx/16/redo.png"), wxBITMAP_TYPE_PNG));
	mEdit->Append(redo);
	mEdit->AppendSeparator();
	wxMenuItem *find = new wxMenuItem(mEdit, wxID_FIND, wxT("Find\tCtrl-F"), wxT("Find a string in the file"));
	find->SetBitmap(wxBitmap(wxGetAppFile("gfx/16/find.png"), wxBITMAP_TYPE_PNG));
	mEdit->Append(find);
	mEdit->Append(wxID_REPLACE, wxT("Find and replace\tCtrl-H"), wxT("Find and replace a string in the file"));

	// View Menu
	mView->AppendCheckItem(ID_FULLSCR, wxT("Full screen\tF11"), wxT("Toggle full screen mode"));
	mView->AppendCheckItem(ID_WRAP, wxT("Wrap lines\tCtrl-W"), wxT("Toggle line wrapping"));

	// Tools Menu
	mTools->Append(ID_NEW_CLASS, wxT("New C++ class"), wxT("Add a new C++ class to the project"));

	// Help Menu
	wxMenuItem *appinfo = new wxMenuItem(mHelp, wxID_ABOUT, wxT("&About\tF1"), wxT("About the program"));
	appinfo->SetBitmap(wxBitmap(wxGetAppFile("gfx/16/info.png"), wxBITMAP_TYPE_PNG));
	mHelp->Append(appinfo);

	mb->Append(mFile, wxT("&File"));
	mb->Append(mEdit, wxT("&Edit"));
	mb->Append(mView, wxT("&View"));
	mb->Append(mTools, wxT("&Tools"));
	mb->Append(mHelp, wxT("&Help"));

	this->SetMenuBar(mb);

	//
    sb = new wxStatusBar(this);
    sb->SetFieldsCount(2);
    this->SetStatusBar(sb);
    SetStatusText(title, 0);

	/*************************   wxAUI   ****************************/

	// Manager
	manager.SetManagedWindow(this);
	manager.SetFlags(wxAUI_MGR_DEFAULT);

	// MAIN WINDOW
	// Notebook
	notebook = new wxAuiNotebook(this, ID_NOTEBOOK, wxDefaultPosition, wxDefaultSize, wxAUI_NB_TAB_SPLIT | wxAUI_NB_DEFAULT_STYLE | wxAUI_NB_DEFAULT_STYLE);
	manager.AddPane(notebook, wxAuiPaneInfo().Left().Caption(wxT("Editor")).MaximizeButton(true).MinimizeButton(true).PinButton(true).PaneBorder(false).Dock().Resizable().FloatingSize(wxDefaultSize).CentrePane().DefaultPane());

	wxPanel *page1 = new wxPanel(notebook, wxID_ANY);
	wxPanel *page2 = new wxPanel(notebook, wxID_ANY);
	wxPanel *page3 = new wxPanel(notebook, wxID_ANY);

	notebook->AddPage(page1, wxT("Document 1"), false, -1);
	notebook->AddPage(page2, wxT("Document 2"), false, -1);
	notebook->AddPage(page3, wxT("Document 3"), false, -1);

	// Console
	console = new wxTextCtrl(this, ID_CONSOLE, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
	manager.AddPane(console, wxAuiPaneInfo().Bottom().Caption(wxT("Console")).MaximizeButton(true).MinimizeButton(true).PinButton(true).PaneBorder(true).Dock().Resizable().MinSize(wxSize(-1, 75)));

	// LEFT COLUMN
	// Project tree
	project_tree = new wxTreeCtrl(this, ID_TREE, wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE);
	manager.AddPane(project_tree, wxAuiPaneInfo().Left().Caption(wxT("Project tree")).PinButton(true).PaneBorder(true).Dock().Resizable().Row(0).MinSize(wxSize(150, -1)).Layer(1).Position(0));

	// Information
	list = new wxListCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT);
	manager.AddPane(list, wxAuiPaneInfo().Left().Caption(wxT("Information")).PinButton(true).PaneBorder(true).Dock().Resizable().Layer(1).Position(1));

	list->AppendColumn(wxEmptyString, wxLIST_FORMAT_LEFT, 70);
	list->AppendColumn(wxEmptyString, wxLIST_FORMAT_LEFT, 250);

	list->InsertItem(0, wxT("Folder"), -1);
	list->InsertItem(1, wxT("Lines"), -1);
	list->InsertItem(2, wxT("Characters"), -1);
	list->InsertItem(3, wxT("Size"), -1);

	// Settings
	settings = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
	manager.AddPane(settings, wxAuiPaneInfo().Left().Caption(wxT("Settings")).PinButton(true).PaneBorder(true).Dock().Resizable().Layer(1).Position(2));
	wxBoxSizer* settings_sizer = new wxBoxSizer(wxVERTICAL);
	settings->SetSizer(settings_sizer);

	wrap_lines = new wxCheckBox(settings, ID_WRAP, wxT(" Wrap lines"));
	settings_sizer->Add(wrap_lines, 0, wxEXPAND | wxALL, 4);

	// TOOLBARS
	// "File" Toolbar
	file_tb = new wxAuiToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxAUI_TB_HORZ_LAYOUT | wxAUI_TB_GRIPPER | wxAUI_TB_OVERFLOW);
	file_tb->AddTool(ID_NEW_PROJECT, wxT("New project"),
		wxBitmap(wxGetAppFile("gfx/22/new_project.png"), wxBITMAP_TYPE_PNG),
		wxNullBitmap, wxITEM_NORMAL, wxT("New project"), wxT("Create a new project"), NULL);
	file_tb->AddTool(ID_OPEN_PROJECT, wxT("Open project"),
		wxBitmap(wxGetAppFile("gfx/22/open_project.png"), wxBITMAP_TYPE_PNG),
		wxNullBitmap, wxITEM_NORMAL, wxT("Open project"), wxT("Open a project"), NULL);
	file_tb->AddSeparator();
	file_tb->AddTool(wxID_NEW, wxT("New file"),
		wxBitmap(wxGetAppFile("gfx/22/new_file.png"), wxBITMAP_TYPE_PNG),
		wxNullBitmap, wxITEM_NORMAL, wxT("New file"), wxT("Add a new file to the project"), NULL);
	file_tb->AddTool(wxID_SAVE, wxT("Save file"),
		wxBitmap(wxGetAppFile("gfx/22/save_file.png"), wxBITMAP_TYPE_PNG),
		wxNullBitmap, wxITEM_NORMAL, wxT("Save file"), wxT("Save the current file"), NULL);
	file_tb->AddTool(ID_SAVEALL, wxT("Save all"),
		wxBitmap(wxGetAppFile("gfx/22/save_all.png"), wxBITMAP_TYPE_PNG),
		wxNullBitmap, wxITEM_NORMAL, wxT("Save all"), wxT("Save all unsaved changes"), NULL);
	file_tb->AddSeparator();
	file_tb->AddTool(wxID_PRINT, wxT("Print"),
		wxBitmap(wxGetAppFile("gfx/22/print.png"), wxBITMAP_TYPE_PNG),
		wxNullBitmap, wxITEM_NORMAL, wxT("Print"), wxT("Print the current document"), NULL);
	file_tb->AddSeparator();
	file_tb->AddTool(wxID_FIND, wxT("Find"),
		wxBitmap(wxGetAppFile("gfx/22/find.png"), wxBITMAP_TYPE_PNG),
		wxNullBitmap, wxITEM_NORMAL, wxT("Find"), wxT("Find a string in the file"), NULL);
	file_tb->Realize();

	manager.AddPane(file_tb, wxAuiPaneInfo().ToolbarPane().Top().Row(1));

	// "Edit" Toolbar
	edit_tb = new wxAuiToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxAUI_TB_HORZ_LAYOUT | wxAUI_TB_GRIPPER);
	edit_tb->AddTool(wxID_COPY, wxT("Copy"),
		wxBitmap(wxGetAppFile("gfx/22/copy.png"), wxBITMAP_TYPE_PNG),
		wxNullBitmap, wxITEM_NORMAL, wxT("Copy"), wxT("Copy the selection to the clipboard"), NULL);
	edit_tb->AddTool(wxID_CUT, wxT("Cut"),
		wxBitmap(wxGetAppFile("gfx/22/cut.png"), wxBITMAP_TYPE_PNG),
		wxNullBitmap, wxITEM_NORMAL, wxT("Cut"), wxT("Cut the selection and save it to the clipboard"), NULL);
	edit_tb->AddTool(wxID_PASTE, wxT("Paste"),
		wxBitmap(wxGetAppFile("gfx/22/paste.png"), wxBITMAP_TYPE_PNG),
		wxNullBitmap, wxITEM_NORMAL, wxT("Paste"), wxT("Paste the contents of the clipboard"), NULL);
	edit_tb->AddSeparator();
	edit_tb->AddTool(wxID_UNDO, wxT("Undo"),
		wxBitmap(wxGetAppFile("gfx/22/undo.png"), wxBITMAP_TYPE_PNG),
		wxNullBitmap, wxITEM_NORMAL, wxT("Undo"), wxT("Undo the last operation"), NULL);
	edit_tb->AddTool(wxID_REDO, wxT("Redo"),
		wxBitmap(wxGetAppFile("gfx/22/redo.png"), wxBITMAP_TYPE_PNG),
		wxNullBitmap, wxITEM_NORMAL, wxT("Redo"), wxT("Redo the last operation"), NULL);
	edit_tb->Realize();

    manager.AddPane(edit_tb, wxAuiPaneInfo().ToolbarPane().Top().Row(1).Position(1));

    // Color settings
    manager.GetArtProvider()->SetColor(wxAUI_DOCKART_INACTIVE_CAPTION_COLOUR, wxColour(0, 111, 221));
    manager.GetArtProvider()->SetColor(wxAUI_DOCKART_INACTIVE_CAPTION_TEXT_COLOUR, *wxWHITE);
    manager.GetArtProvider()->SetColor(wxAUI_DOCKART_INACTIVE_CAPTION_GRADIENT_COLOUR, wxColour(0, 80, 150));

    // AUI realization
    manager.Update();

    CenterOnScreen();
    }

    //---------------------------------------------------------------------------
    // MyFrame Destructor
    //-------------------------------------------------------------------------
    MyFrame::~MyFrame()
    {
    manager.UnInit();
    }

    //---------------------------------------------------------------------------
    // Function initiates program closure
    //---------------------------------------------------------------------------
    void MyFrame::OnClose(wxCommandEvent& WXUNUSED(event))
    {
    this->Close(true);
    }

    //---------------------------------------------------------------------------
    // Function displays the "About..." window
    //---------------------------------------------------------------------------
    void MyFrame::OnAboutProgram(wxCommandEvent& event)
    {
    wxMessageBox(appName+wxT("\nCopyright (c) 2025 by ..."), wxT("About..."), wxOK|wxICON_INFORMATION);

    event.Skip();
    }

    //---------------------------------------------------------------------------
    // Function displays a prompt to close the program
    //---------------------------------------------------------------------------
    void MyFrame::CloseApp(wxCloseEvent& WXUNUSED(event))
    {
    int action = wxMessageBox(wxT("Do you want to close the program?"), wxT("Question"), wxOK|wxCANCEL|wxICON_QUESTION);

    if(action == wxOK)
    {
        this->Destroy();
    }
    else
    {
        return;
    }
    }
