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
#include <wx/splitter.h>
#include <wx/imaglist.h>
#include <wx/html/htmprint.h>

#include "utils.h" // Access to wxGetAppFile() function
#include "main.h"

const wxString appName = wxT("wxC++ 1.0.0");

//---------------------------------------------------------------------------
// Application implementation
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
// Event table
//---------------------------------------------------------------------------
BEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU(ID_NEW_PROJECT, MyFrame::OnNewProject)      // New project
    EVT_MENU(ID_OPEN_PROJECT, MyFrame::OnOpenProject)    // Open project
    EVT_MENU(wxID_OPEN, MyFrame::OnOpenFile)             // Open file
    EVT_MENU(wxID_NEW, MyFrame::OnAddNewFile)            // Add new file
    EVT_MENU(ID_ADD_EXIST_FILE, MyFrame::OnAddExistFile) // Add existing file
    EVT_MENU(wxID_SAVE, MyFrame::OnSave)                 // Save file
    EVT_MENU(wxID_SAVEAS, MyFrame::OnSaveAs)             // Save file as...
    EVT_MENU(ID_SAVEALL, MyFrame::OnSaveAll)             // Save all unsaved changes
    EVT_MENU(ID_RENAME, MyFrame::OnRename)               // Rename current file
    EVT_MENU(wxID_REMOVE, MyFrame::OnRemove)             // Remove current file from project or disk
    EVT_MENU(ID_CLOSE_FILE, MyFrame::OnCloseFile)        // Close current file
    EVT_MENU(wxID_PRINT, MyFrame::OnPrint)               // Print
    EVT_MENU(wxID_PREVIEW, MyFrame::OnPreview)           // Print preview
    EVT_MENU(wxID_PAGE_SETUP, MyFrame::OnPageSetup)      // Page setup
    EVT_MENU(ID_CLOSE, MyFrame::OnClose)                 // Close program from menu
    EVT_MENU(wxID_COPY, MyFrame::OnCopy)                 // 'Copy'
    EVT_MENU(wxID_CUT, MyFrame::OnCut)                   // 'Cut'
    EVT_MENU(wxID_PASTE, MyFrame::OnPaste)               // 'Paste'
    EVT_MENU(wxID_SELECTALL, MyFrame::OnSelectAll)       // 'Select all'
    EVT_MENU(wxID_UNDO, MyFrame::OnUndo)                 // 'Undo'
    EVT_MENU(wxID_REDO, MyFrame::OnRedo)                 // 'Redo'
    EVT_MENU(wxID_FIND, MyFrame::OnFind)                 // 'Find'
    EVT_MENU(wxID_REPLACE, MyFrame::OnFind)              // 'Find and replace'
    EVT_MENU(ID_FULLSCR, MyFrame::OnFullScr)             // Fullscreen mode
    EVT_MENU(ID_WRAP, MyFrame::OnWrap)                   // Word wrap
	EVT_MENU(ID_NEW_CLASS, MyFrame::OnNewClass)          // C++ class wizard
    EVT_MENU(wxID_ABOUT, MyFrame::OnAboutProgram)        // About program
    EVT_CLOSE(MyFrame::CloseApp)                         // 'System' program close
    EVT_TREE_ITEM_RIGHT_CLICK(ID_TREE, MyFrame::OnTreeContextMenu) // Show context menu for files in tree
    EVT_TREE_ITEM_ACTIVATED(ID_TREE, MyFrame::OnTreeActivated) // Display clicked document in editor
    //
    EVT_FIND(wxID_ANY, MyFrame::OnDoFind)
    EVT_FIND_NEXT(wxID_ANY, MyFrame::OnDoFind)
    EVT_FIND_REPLACE(wxID_ANY, MyFrame::OnDoFindReplace)
    EVT_FIND_REPLACE_ALL(wxID_ANY, MyFrame::OnDoFindReplace)
    EVT_FIND_CLOSE(wxID_ANY, MyFrame::OnFindDialogClose)
    //
    EVT_NOTEBOOK_PAGE_CHANGED(ID_NOTEBOOK, MyFrame::OnPageChange)
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
// MyFrame constructor
//---------------------------------------------------------------------------
MyFrame::MyFrame(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &pos, const wxSize &size, long style)
: wxFrame(parent, id, title, pos, size, style), project(NULL), project_path(wxEmptyString)
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
    wxMenuItem *new_file = new wxMenuItem(mAddFile, wxID_NEW, wxT("New file\tCtrl-N"), wxT("Add new file to project"));
    new_file->SetBitmap(wxBitmap(wxGetAppFile("gfx/16/new_file.png"), wxBITMAP_TYPE_PNG));
    mAddFile->Append(new_file);
    mAddFile->Append(ID_ADD_EXIST_FILE, wxT("Existing file"), wxT("Add existing file to project"));

    // File Menu
    wxMenuItem *new_project = new wxMenuItem(mFile, ID_NEW_PROJECT, wxT("New project\tCtrl-Shift-N"), wxT("Create new project"));
    new_project->SetBitmap(wxBitmap(wxGetAppFile("gfx/16/new_project.png"), wxBITMAP_TYPE_PNG));
    mFile->Append(new_project);
    wxMenuItem *open_project = new wxMenuItem(mFile, ID_OPEN_PROJECT, wxT("Open project\tCtrl-Shift-O"), wxT("Open project"));
    open_project->SetBitmap(wxBitmap(wxGetAppFile("gfx/16/open_project.png"), wxBITMAP_TYPE_PNG));
    mFile->Append(open_project);
    mFile->AppendSeparator();
    wxMenuItem *open_file = new wxMenuItem(mFile, wxID_OPEN, wxT("Open file\tCtrl-O"), wxT("Open any file outside the project"));
    open_file->SetBitmap(wxBitmap(wxGetAppFile("gfx/16/open_project.png"), wxBITMAP_TYPE_PNG));
    mFile->Append(open_file);
    mFile->AppendSubMenu(mAddFile, wxT("Add file"));
    wxMenuItem *save_file = new wxMenuItem(mFile, wxID_SAVE, wxT("Save file\tCtrl-S"), wxT("Save current file"));
    save_file->SetBitmap(wxBitmap(wxGetAppFile("gfx/16/save_file.png"), wxBITMAP_TYPE_PNG));
    mFile->Append(save_file);
    wxMenuItem *save_as = new wxMenuItem(mFile, wxID_SAVEAS, wxT("Save file as...\tCtrl-Shift-S"), wxT("Save current file with specified name"));
    save_as->SetBitmap(wxBitmap(wxGetAppFile("gfx/16/save_as.png"), wxBITMAP_TYPE_PNG));
    mFile->Append(save_as);
    wxMenuItem *save_all = new wxMenuItem(mFile, ID_SAVEALL, wxT("Save all"), wxT("Save all unsaved changes"));
    save_all->SetBitmap(wxBitmap(wxGetAppFile("gfx/16/save_all.png"), wxBITMAP_TYPE_PNG));
    mFile->Append(save_all);
    mFile->Append(ID_RENAME, wxT("Rename file"), wxT("Rename file"));
    mFile->Append(wxID_REMOVE, wxT("Remove file"), wxT("Remove file from project or disk"));
    mFile->Append(ID_CLOSE_FILE, wxT("Close file\tCtrl-Alt-F"), wxT("Close current file"));
    mFile->AppendSeparator();
    wxMenuItem *print = new wxMenuItem(mFile, wxID_PRINT, wxT("Print\tCtrl-P"), wxT("Print current document"));
    print->SetBitmap(wxBitmap(wxGetAppFile("gfx/16/print.png"), wxBITMAP_TYPE_PNG));
    mFile->Append(print);
    wxMenuItem *preview = new wxMenuItem(mFile, wxID_PREVIEW, wxT("Print preview\tCtrl-Shift-P"), wxT("Print preview"));
    preview->SetBitmap(wxBitmap(wxGetAppFile("gfx/16/preview.png"), wxBITMAP_TYPE_PNG));
    mFile->Append(preview);
    mFile->Append(wxID_PAGE_SETUP, wxT("Page setup"), wxT("Page setup"));
    mFile->AppendSeparator();
    mFile->Append(ID_CLOSE, wxT("&Close program\tCtrl-Shift-X"), wxT("Close program"));

    // Edit Menu
    wxMenuItem *copy = new wxMenuItem(mEdit, wxID_COPY, wxT("Copy\tCtrl-C"), wxT("Copy selection to clipboard"));
    copy->SetBitmap(wxBitmap(wxGetAppFile("gfx/16/copy.png"), wxBITMAP_TYPE_PNG));
    mEdit->Append(copy);
    wxMenuItem *cut = new wxMenuItem(mEdit, wxID_CUT, wxT("Cut\tCtrl-X"), wxT("Cut selection and save to clipboard"));
    cut->SetBitmap(wxBitmap(wxGetAppFile("gfx/16/cut.png"), wxBITMAP_TYPE_PNG));
    mEdit->Append(cut);
    wxMenuItem *paste = new wxMenuItem(mEdit, wxID_PASTE, wxT("Paste\tCtrl-V"), wxT("Paste clipboard content"));
    paste->SetBitmap(wxBitmap(wxGetAppFile("gfx/16/paste.png"), wxBITMAP_TYPE_PNG));
    mEdit->Append(paste);
    mEdit->AppendSeparator();
    mEdit->Append(wxID_SELECTALL, wxT("Select all\tCtrl-A"), wxT("Select all content of current document"));
    mEdit->Append(wxID_REVERT, wxT("Invert selection\tCtrl-R"), wxT("Invert selection"));
    mEdit->AppendSeparator();
    wxMenuItem *undo = new wxMenuItem(mEdit, wxID_UNDO, wxT("Undo\tCtrl-Z"), wxT("Undo last operation"));
    undo->SetBitmap(wxBitmap(wxGetAppFile("gfx/16/undo.png"), wxBITMAP_TYPE_PNG));
    mEdit->Append(undo);
    wxMenuItem *redo = new wxMenuItem(mEdit, wxID_REDO, wxT("Redo\tCtrl-Y"), wxT("Redo last operation"));
    redo->SetBitmap(wxBitmap(wxGetAppFile("gfx/16/redo.png"), wxBITMAP_TYPE_PNG));
    mEdit->Append(redo);
    mEdit->AppendSeparator();
    wxMenuItem *find = new wxMenuItem(mEdit, wxID_FIND, wxT("Find\tCtrl-F"), wxT("Find string in file"));
    find->SetBitmap(wxBitmap(wxGetAppFile("gfx/16/find.png"), wxBITMAP_TYPE_PNG));
    mEdit->Append(find);
    mEdit->Append(wxID_REPLACE, wxT("Find and replace\tCtrl-H"), wxT("Find and replace string in file"));

    // View Menu
    mView->AppendCheckItem(ID_FULLSCR, wxT("Fullscreen\tF11"), wxT("Toggle fullscreen mode"));
    mView->AppendCheckItem(ID_WRAP, wxT("Word wrap\tCtrl-W"), wxT("Toggle word wrap"));

    // Tools Menu
    mTools->Append(ID_NEW_CLASS, wxT("New C++ class"), wxT("Add new C++ class to project"));

    // Help Menu
    wxMenuItem *info = new wxMenuItem(mHelp, wxID_ABOUT, wxT("&About\tF1"), wxT("About"));
    info->SetBitmap(wxBitmap(wxGetAppFile("gfx/16/info.png"), wxBITMAP_TYPE_PNG));
    mHelp->Append(info);

    mb->Append(mFile, wxT("&File"));
    mb->Append(mEdit, wxT("&Edit"));
    mb->Append(mView, wxT("&View"));
    mb->Append(mTools, wxT("&Tools"));
    mb->Append(mHelp, wxT("&Help"));

    this->SetMenuBar(mb);

    // CONTEXT MENU FOR PROJECT TREE
    TreeContextMenu = new wxMenu();
    TreeContextMenu->Append(ID_RENAME, wxT("Rename file"), wxT("Rename file"));
    TreeContextMenu->Append(wxID_REMOVE, wxT("Remove file"), wxT("Remove file from project or disk"));

    // TOOLBAR
    tb = new wxToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_FLAT | wxTB_HORIZONTAL);
    const wxSize toolBarBitmapSize(22, 22);
    tb->SetToolBitmapSize(toolBarBitmapSize);

    tb->AddTool(ID_NEW_PROJECT, wxT("New project"),
        wxBitmap(wxGetAppFile("gfx/22/new_project.png"), wxBITMAP_TYPE_PNG),
        wxNullBitmap, wxITEM_NORMAL, wxT("New project"), wxT("Create new project"));
    tb->AddTool(ID_OPEN_PROJECT, wxT("Open project"),
        wxBitmap(wxGetAppFile("gfx/22/open_project.png"), wxBITMAP_TYPE_PNG),
        wxNullBitmap, wxITEM_NORMAL, wxT("Open project"), wxT("Open project"));
    tb->AddSeparator();
    tb->AddTool(wxID_NEW, wxT("New file"),
        wxBitmap(wxGetAppFile("gfx/22/new_file.png"), wxBITMAP_TYPE_PNG),
        wxNullBitmap, wxITEM_NORMAL, wxT("New file"), wxT("Add new file to project"));
    tb->AddTool(wxID_SAVE, wxT("Save file"),
        wxBitmap(wxGetAppFile("gfx/22/save_file.png"), wxBITMAP_TYPE_PNG),
        wxNullBitmap, wxITEM_NORMAL, wxT("Save file"), wxT("Save current file"));
    tb->AddTool(ID_SAVEALL, wxT("Save all"),
        wxBitmap(wxGetAppFile("gfx/22/save_all.png"), wxBITMAP_TYPE_PNG),
        wxNullBitmap, wxITEM_NORMAL, wxT("Save all"), wxT("Save all unsaved changes"));
    tb->AddSeparator();
    tb->AddTool(wxID_PRINT, wxT("Print"),
        wxBitmap(wxGetAppFile("gfx/22/print.png"), wxBITMAP_TYPE_PNG),
        wxNullBitmap, wxITEM_NORMAL, wxT("Print"), wxT("Print current document"));
    tb->AddSeparator();
    tb->AddTool(wxID_FIND, wxT("Find"),
        wxBitmap(wxGetAppFile("gfx/22/find.png"), wxBITMAP_TYPE_PNG),
        wxNullBitmap, wxITEM_NORMAL, wxT("Find"), wxT("Find string in file"));
    tb->AddSeparator();
    tb->AddTool(wxID_COPY, wxT("Copy"),
        wxBitmap(wxGetAppFile("gfx/22/copy.png"), wxBITMAP_TYPE_PNG),
        wxNullBitmap, wxITEM_NORMAL, wxT("Copy"), wxT("Copy selection to clipboard"));
    tb->AddTool(wxID_CUT, wxT("Cut"),
        wxBitmap(wxGetAppFile("gfx/22/cut.png"), wxBITMAP_TYPE_PNG),
        wxNullBitmap, wxITEM_NORMAL, wxT("Cut"), wxT("Cut selection and save to clipboard"));
    tb->AddTool(wxID_PASTE, wxT("Paste"),
        wxBitmap(wxGetAppFile("gfx/22/paste.png"), wxBITMAP_TYPE_PNG),
        wxNullBitmap, wxITEM_NORMAL, wxT("Paste"), wxT("Paste clipboard content"));
    tb->AddSeparator();
    tb->AddTool(wxID_UNDO, wxT("Undo"),
        wxBitmap(wxGetAppFile("gfx/22/undo.png"), wxBITMAP_TYPE_PNG),
        wxNullBitmap, wxITEM_NORMAL, wxT("Undo"), wxT("Undo last operation"));
    tb->AddTool(wxID_REDO, wxT("Redo"),
        wxBitmap(wxGetAppFile("gfx/22/redo.png"), wxBITMAP_TYPE_PNG),
        wxNullBitmap, wxITEM_NORMAL, wxT("Redo"), wxT("Redo last operation"));

    tb->SetMargins(3, 3);
    tb->Realize();
    SetToolBar(tb);

    // STATUS BAR
    sb = new wxStatusBar(this);
    sb->SetFieldsCount(2);
    this->SetStatusBar(sb);
    SetStatusText(title, 0);

    // Main sizer for client area
    wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
    this->SetSizer(mainSizer);

    // Main panel
    panel = new wxPanel(this);
    wxBoxSizer *panelSizer = new wxBoxSizer(wxVERTICAL);
    panel->SetSizer(panelSizer);
    mainSizer->Add(panel, 1, wxEXPAND | wxALL, 0);

    // Container with two GUI columns
    splitter = new wxSplitterWindow(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_3DSASH | wxSP_NO_XP_THEME);
    panelSizer->Add(splitter, 1, wxEXPAND | wxALL, 0);

    left = new wxPanel(splitter, wxID_ANY);
    right = new wxPanel(splitter, wxID_ANY);

    left->SetMinSize(wxSize(200, -1));
    right->SetMinSize(wxSize(200, -1));

    splitter->SplitVertically(left, right, 250);
    //splitter->SetMinimumPaneSize(120); // Can be used instead of left/right->SetMinSize(wxSize(200, -1));

    wxBoxSizer *rightSizer = new wxBoxSizer(wxVERTICAL);
    right->SetSizer(rightSizer);

    // Right column container
    splitterRight = new wxSplitterWindow(right, wxID_ANY, wxDefaultPosition, wxDefaultSize,
        wxSP_3DSASH | wxSP_NO_XP_THEME);
    rightSizer->Add(splitterRight, 1, wxEXPAND | wxALL, 0);

    rightTop = new wxPanel(splitterRight, wxID_ANY);
    rightBottom = new wxPanel(splitterRight, wxID_ANY);

    rightTop->SetMinSize(wxSize(-1, 200));
    rightBottom->SetMinSize(wxSize(-1, 100));

    splitterRight->SplitHorizontally(rightTop, rightBottom, (this->GetSize().GetHeight() - 120));


    // Left column sizer
    wxBoxSizer* left_sizer = new wxBoxSizer(wxVERTICAL);
    left->SetSizer(left_sizer);

    // Editor area sizer
    wxBoxSizer *rightTop_sizer = new wxBoxSizer(wxVERTICAL);
    rightTop->SetSizer(rightTop_sizer);

    // Console area sizer
    wxBoxSizer *rightBottom_sizer = new wxBoxSizer(wxVERTICAL);
    rightBottom->SetSizer(rightBottom_sizer);

    // PROJECT FILE TREE
    tree = new ProjectTreeCtrl(left, ID_TREE);
    left_sizer->Add(tree, 2, wxEXPAND | wxALL, 4);

    wxImageList* img = new wxImageList(16, 16, false, 0);

    // TREE_ICON_PROJECT
    img->Add(wxBitmap(wxGetAppFile("gfx/16/tree/project.png"), wxBITMAP_TYPE_PNG));
    // TREE_ICON_FILTER
    img->Add(wxBitmap(wxGetAppFile("gfx/16/tree/filter.png"), wxBITMAP_TYPE_PNG));
    // TREE_ICON_H_FILE
    img->Add(wxBitmap(wxGetAppFile("gfx/16/tree/h_file.png"), wxBITMAP_TYPE_PNG));
    img->Add(wxBitmap(wxGetAppFile("gfx/16/tree/h_file_edit.png"), wxBITMAP_TYPE_PNG));
    // TREE_ICON_HPP_FILE
    img->Add(wxBitmap(wxGetAppFile("gfx/16/tree/hpp_file.png"), wxBITMAP_TYPE_PNG));
    img->Add(wxBitmap(wxGetAppFile("gfx/16/tree/hpp_file_edit.png"), wxBITMAP_TYPE_PNG));
    // TREE_ICON_C_FILE
    img->Add(wxBitmap(wxGetAppFile("gfx/16/tree/c_file.png"), wxBITMAP_TYPE_PNG));
    img->Add(wxBitmap(wxGetAppFile("gfx/16/tree/c_file_edit.png"), wxBITMAP_TYPE_PNG));
    // TREE_ICON_CPP_FILE
    img->Add(wxBitmap(wxGetAppFile("gfx/16/tree/cpp_file.png"), wxBITMAP_TYPE_PNG));
    img->Add(wxBitmap(wxGetAppFile("gfx/16/tree/cpp_file_edit.png"), wxBITMAP_TYPE_PNG));
    // TREE_ICON_RC_FILE
    img->Add(wxBitmap(wxGetAppFile("gfx/16/tree/rc_file.png"), wxBITMAP_TYPE_PNG));
    img->Add(wxBitmap(wxGetAppFile("gfx/16/tree/rc_file_edit.png"), wxBITMAP_TYPE_PNG));
    // TREE_ICON_FILE
    img->Add(wxBitmap(wxGetAppFile("gfx/16/tree/file.png"), wxBITMAP_TYPE_PNG));
    img->Add(wxBitmap(wxGetAppFile("gfx/16/tree/file_edit.png"), wxBITMAP_TYPE_PNG));

    tree->SetImageList(img);

    // NOTEBOOK
    container = new wxNotebook(rightTop, ID_NOTEBOOK);
    container->SetImageList(img);
    rightTop_sizer->Add(container, 1, wxEXPAND | wxALL, 0);

    wrap_mode = wxSTC_WRAP_WORD;

    // CONSOLE WINDOW AND LOG REDIRECTION
    console = new wxTextCtrl(rightBottom, ID_CONSOLE, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY | wxTE_MULTILINE);
    rightBottom_sizer->Add(console, 1, wxEXPAND | wxALL, 0);
    wxLog::SetActiveTarget(new wxLogTextCtrl(console));

    // STATISTICS LIST
    list = new wxListCtrl(left, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT | wxLC_HRULES | wxLC_VRULES | wxLC_NO_HEADER | wxBORDER_SIMPLE);
    left_sizer->Add(list, 1, wxEXPAND | wxALL, 4);

    list->AppendColumn(wxEmptyString, wxLIST_FORMAT_LEFT, 70);
    list->AppendColumn(wxEmptyString, wxLIST_FORMAT_LEFT, 250);

    list->InsertItem(0, wxT("Folder"), -1);
    list->InsertItem(1, wxT("Lines"), -1);
    list->InsertItem(2, wxT("Characters"), -1);
    list->InsertItem(3, wxT("Size"), -1);

    // SWITCHES
    wrap_chkb = new wxCheckBox(left, ID_WRAP, wxT(" Word wrap"));
    left_sizer->Add(wrap_chkb, 0, wxEXPAND | wxALL, 4);

    // Set initial values
    wrap_chkb->SetValue(true);
    mView->Check(ID_WRAP, true);

    // PRINT ENGINE
    print_engine = new wxHtmlEasyPrinting(appName, this);
    print_engine->SetFonts("Courier New", "Courier New");
    print_engine->SetFooter(wxT("<br />Page @PAGENUM@ of @PAGESCNT@"), wxPAGE_ALL);

    wxPageSetupDialogData* print_setup = print_engine->GetPageSetupData();
    print_setup->SetMarginBottomRight(wxPoint(15, 7)); // More space for footer
    print_setup->SetMarginTopLeft(wxPoint(15, 15));
    print_setup->SetMinMarginBottomRight(wxPoint(5, 5));
    print_setup->SetMinMarginTopLeft(wxPoint(5, 5));

    wxPrintData* print_data = print_engine->GetPrintData();
    print_data->SetOrientation(wxPORTRAIT);

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
    // Function displays "About program..." window
    //---------------------------------------------------------------------------
    void MyFrame::OnAboutProgram(wxCommandEvent& event)
    {
    wxMessageBox(appName+wxT("\nCopyright (c) 2025 by ..."), wxT("About program..."), wxOK|wxICON_INFORMATION);

    event.Skip();
    }

    //---------------------------------------------------------------------------
    // Function displays a question about closing the program
    //---------------------------------------------------------------------------
    void MyFrame::CloseApp(wxCloseEvent& WXUNUSED(event))
    {
    int action = wxMessageBox(wxT("Do you want to close the program?"), wxT("Question"), wxOK|wxCANCEL|wxICON_QUESTION);

    if(action == wxOK)
    {
        this->CheckFilesToSave(true);
        this->Destroy();
    }
    else
    {
        return;
    }
    }

    //---------------------------------------------------------------------------
    // Function handles creating a new project
    //---------------------------------------------------------------------------
    void MyFrame::OnNewProject(wxCommandEvent& event)
    {
    // Check if another project is currently open, and also
    // indirectly check if there are open files
    // that need to be closed
    if (!CheckProject(true))
    {
        return;
    }

    // Get the path for the new project file
    wxFileDialog saveDialog(this, wxT("Where to save the new project?"), "", "", "Project file (*.wxp)|*.wxp", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
    if (saveDialog.ShowModal() == wxID_CANCEL)
    {
        return;
    }

    this->project_path = saveDialog.GetPath();

    // Create the default project structure in the tree
    wxFileName file(this->project_path);
    wxString name = file.GetFullName();

    this->tree->AppendItem(name, ProjectTreeData::ITEM_TYPE_ROOT, TREE_ICON_PROJECT);

    this->tree->AppendItem(FILTER_HEADERS, ProjectTreeData::ITEM_TYPE_FILTER, TREE_ICON_FILTER);
    this->tree->AppendItem(FILTER_SOURCES, ProjectTreeData::ITEM_TYPE_FILTER, TREE_ICON_FILTER);
    this->tree->AppendItem(FILTER_RESOURCES, ProjectTreeData::ITEM_TYPE_FILTER, TREE_ICON_FILTER);
    this->tree->AppendItem(FILTER_OTHER_FILES, ProjectTreeData::ITEM_TYPE_FILTER, TREE_ICON_FILTER);

    this->tree->ExpandAll();

    // Create XML document
    if (TranslateTreeToXML())
    {
        wxLogMessage(wxT("New project file successfully created: ") + this->project_path);
    }
    else
    {
        // Cleanup
        this->tree->DeleteAllItems();
        if (this->project != NULL)
        {
            delete this->project;
            this->project = NULL;
        }
        this->project_path = wxEmptyString;

        wxLogError(wxT("Failed to create project file!"));
    }

    event.Skip();
    }

    //---------------------------------------------------------------------------
    // Function handles opening an existing project
    //---------------------------------------------------------------------------
    void MyFrame::OnOpenProject(wxCommandEvent& event)
    {
    // Check if another project is currently open, and also
    // indirectly check if there are open files
    // that need to be closed
    if (!CheckProject(false))
    {
        return;
    }

    // Open project file
    wxFileDialog openDialog(this, wxT("Open project"), "", "", "Project file (*.wxp)|*.wxp", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    if (openDialog.ShowModal() == wxID_CANCEL)
    {
        return;
    }

    this->project_path = openDialog.GetPath();

    // Read XML document and process it into the tree
    if (TranslateXMLToTree())
    {
        wxLogMessage(wxT("Project successfully opened: ") + this->project_path);
    }
    else
    {
        // Cleanup
        this->tree->DeleteAllItems();
        if (this->project != NULL)
        {
            delete this->project;
            this->project = NULL;
        }
        this->project_path = wxEmptyString;

        wxLogError(wxT("Failed to open project!"));
    }

    event.Skip();
    }

//---------------------------------------------------------------------------
// Function handles opening a file
//---------------------------------------------------------------------------
void MyFrame::OnOpenFile(wxCommandEvent& event)
{
    // Open file
    wxFileDialog openDialog(this, wxT("Open file"), "", "", SUPPORTED_FILES_EXT, wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    if (openDialog.ShowModal() == wxID_CANCEL)
    {
        return;
    }

    OpenInEditor(openDialog.GetPath());

    event.Skip();
}

//---------------------------------------------------------------------------
// Function handles adding a new file to the project
//---------------------------------------------------------------------------
void MyFrame::OnAddNewFile(wxCommandEvent& event)
{
    // Check if there is a project in the editor
    if (this->project == NULL)
    {
        wxLogWarning(wxT("No project detected!"));
        return;
    }

    // Get the path for the new file
    wxFileDialog saveDialog(this, wxT("Specify the location of the new file?"), "", "", SUPPORTED_FILES_EXT, wxFD_SAVE);
    if (saveDialog.ShowModal() == wxID_CANCEL)
    {
        return;
    }

    // Insert the file into the project
    if (!AddFileToProject(saveDialog.GetPath())) return;

    // Create a new file
    wxFile file;
    file.Create(saveDialog.GetPath());

    event.Skip();
}

//---------------------------------------------------------------------------
// Function handles adding an existing file to the project
//---------------------------------------------------------------------------
void MyFrame::OnAddExistFile(wxCommandEvent& event)
{
    // Check if there is a project in the editor
    if (this->project == NULL)
    {
        wxLogWarning(wxT("No project detected!"));
        return;
    }

    // Display the "Open file..." dialog
    wxFileDialog openDialog(this, wxT("Open file to add"), "", "", SUPPORTED_FILES_EXT, wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    if (openDialog.ShowModal() == wxID_CANCEL)
    {
        return;
    }

    // Insert the file into the project
    AddFileToProject(openDialog.GetPath());

    event.Skip();
}

//---------------------------------------------------------------------------
// Function handles saving the current document
//---------------------------------------------------------------------------
void MyFrame::OnSave(wxCommandEvent& event)
{
    if (container->GetPageCount() <= 0) return;

    wxStyledTextCtrl* stc = static_cast<wxStyledTextCtrl*>(container->GetPage(container->GetSelection()));
    DoSave(stc);

    event.Skip();
}

//---------------------------------------------------------------------------
// Function handles saving the current document with a specified name
//---------------------------------------------------------------------------
void MyFrame::OnSaveAs(wxCommandEvent& event)
{
    if (container->GetPageCount() <= 0) return;

    // Get the path for the new file
    wxFileDialog saveDialog(this, wxT("Where to save the file?"), "", "", SUPPORTED_FILES_EXT, wxFD_SAVE);
    if (saveDialog.ShowModal() == wxID_CANCEL)
    {
        return;
    }

    wxStyledTextCtrl* stc = static_cast<wxStyledTextCtrl*>(container->GetPage(container->GetSelection()));
    if (stc->wxStyledTextCtrl::SaveFile(saveDialog.GetPath()))
    {
        wxLogMessage(wxT("File " + saveDialog.GetPath() + " was successfully saved."));
    }
    else
    {
        wxLogWarning(wxT("There was a problem saving the file!"));
    }

    event.Skip();
}

//---------------------------------------------------------------------------
// Function handles saving all unsaved changes
//---------------------------------------------------------------------------
void MyFrame::OnSaveAll(wxCommandEvent& event)
{
    if (container->GetPageCount() <= 0) return;
    DoSaveAll(true);

    event.Skip();
}

//---------------------------------------------------------------------------
// Function handles renaming a file
//---------------------------------------------------------------------------
void MyFrame::OnRename(wxCommandEvent& event)
{
    // Check if the currently selected item is a file
    ProjectTreeData* data = static_cast<ProjectTreeData*>(tree->GetItemData(tree->GetSelection()));
    if (tree->GetCount() == 0 || data == NULL)
    {
        wxMessageBox(wxT("No file selected."));
        return;
    }

    // Get the new file name from the user
    wxString new_name = wxGetTextFromUser(wxT("Enter the new file name ") + data->file->name, wxT("Rename"));
    if (new_name.IsEmpty())
    {
        return;
    }

    // Rename the file
    wxFileName file_name(data->file->full_path);
    if (wxRenameFile(file_name.GetFullPath(), file_name.GetPath() + "\\" + new_name, false))
    {
        // Update data in the tree
        file_name.SetFullName(new_name);
        data->file->SetFilePath(file_name.GetFullPath());
        tree->SetItemText(tree->GetSelection(), new_name);

        // Update name in the container
        int index = container->FindPage(data->file->stc);
        if (index != wxNOT_FOUND)
        {
            container->SetPageText(index, new_name);
            data->file->stc->SetName(file_name.GetFullPath());
            wxLogMessage(data->file->stc->GetName());
        }

        // Sort files in the tree
        SortTreeFiles();

        // Save the tree to the XML file
        TranslateTreeToXML();

        wxLogMessage(wxT("File rename successful: %s"), new_name);
    }
    else
    {
        wxLogWarning(wxT("File rename failed."));
    }

    event.Skip();
}

//---------------------------------------------------------------------------
// Function handles removing a file from disk or project
//---------------------------------------------------------------------------
void MyFrame::OnRemove(wxCommandEvent& event)
{
    // Check if the currently selected item is a file
    ProjectTreeData* data = static_cast<ProjectTreeData*>(tree->GetItemData(tree->GetSelection()));
    if (tree->GetCount() == 0 || data == NULL)
    {
        wxMessageBox(wxT("No file selected."));
        return;
    }

    // Get the type of action from the user
    wxArrayString options;
    options.Add(wxT("Remove file only from project"));
    options.Add(wxT("Remove file from project and disk"));

    wxString action = wxGetSingleChoice(wxT("How do you want to remove the file?"), wxT("Choose an option"), options, 0, this);

    // Remove the file
    if (action == wxEmptyString) // Cancel
    {
        return;
    }
    else if (action.IsSameAs(options.Item(1))) // Remove file from project and disk
    {
        if (wxRemoveFile(data->file->full_path))
        {
            wxLogMessage(wxT("File successfully removed from disk: ") + data->file->name);
        }
        else
        {
            wxLogWarning(wxT("Failed to remove file from disk: ") + data->file->name);
        }
    }

    // Close the editor window if the file is displayed
    int index = container->FindPage(data->file->stc);
    if (index != wxNOT_FOUND)
    {
        container->DeletePage(container->FindPage(data->file->stc));
    }

    // Remove the file from the project tree
    tree->Delete(tree->GetSelection());

    // Sort files in the tree
    SortTreeFiles();

    // Save the tree to the XML file
    TranslateTreeToXML();

    // Update the statistics list
    if (container->GetPageCount() <= 0) SetDocInfo(NULL);

    event.Skip();
}

//---------------------------------------------------------------------------
// Function handles closing the current file
//---------------------------------------------------------------------------
void MyFrame::OnCloseFile(wxCommandEvent& event)
{
    int index = container->GetSelection();
    if (index == wxNOT_FOUND) return;

    wxStyledTextCtrl* stc = static_cast<wxStyledTextCtrl*>(container->GetPage(index));

    // Check if the document has unsaved changes
    if (stc->IsModified())
    {
        if (wxMessageBox(wxT("The file contains unsaved changes,\ndo you want to save them now?\nAll unsaved changes will be lost!"),
            wxT("Warning!"), wxYES_NO | wxICON_EXCLAMATION) == wxYES)
        {
            DoSave(stc);
        }
    }

    // Remove the container page (without destroying the editor)
    container->RemovePage(index);

    // Update data in the tree (destroying the editor)
    wxString file_path = stc->GetName();
    wxTreeItemId item = tree->FindByPath(file_path);
    ProjectTreeData* data = static_cast<ProjectTreeData*>(tree->GetItemData(item));
    if (data != NULL)
    {
        data->file->stc->Destroy();
        data->file->stc = NULL;
        tree->SetItemData(item, data);
        tree->SetItemImage(item, GetFileIcon(file_path));
    }

    // Update the statistics list
    if (container->GetPageCount() <= 0) SetDocInfo(NULL);

    event.Skip();
}

//---------------------------------------------------------------------------
// Function handles printing
//---------------------------------------------------------------------------
void MyFrame::OnPrint(wxCommandEvent& event)
{
    int index = container->GetSelection();
    
    if (index != wxNOT_FOUND)
    {
        wxStyledTextCtrl* stc = static_cast<wxStyledTextCtrl*>(container->GetPage(index));
		print_engine->SetHeader(stc->GetName(), wxPAGE_ALL);
        print_engine->PrintText(PreparePrintString(stc->GetText()));
    }

    event.Skip();
}

//---------------------------------------------------------------------------
// Function handles print preview
//---------------------------------------------------------------------------
void MyFrame::OnPreview(wxCommandEvent& event)
{
    int index = container->GetSelection();

    if (index != wxNOT_FOUND)
    {
        wxStyledTextCtrl* stc = static_cast<wxStyledTextCtrl*>(container->GetPage(index));
		print_engine->SetHeader(stc->GetName(), wxPAGE_ALL);
        print_engine->PreviewText(PreparePrintString(stc->GetText()));
    }

    event.Skip();
}

//---------------------------------------------------------------------------
// Function handles page setup
//---------------------------------------------------------------------------
void MyFrame::OnPageSetup(wxCommandEvent& event)
{
    print_engine->PageSetup();
    event.Skip();
}

//---------------------------------------------------------------------------
// Function handles copying selection to clipboard
//---------------------------------------------------------------------------
void MyFrame::OnCopy(wxCommandEvent& event)
{
    if (container->GetPageCount() > 0)
    {
        wxStyledTextCtrl* stc = static_cast<wxStyledTextCtrl*>(container->GetPage(container->GetSelection()));
        stc->Copy();
    }

    event.Skip();
}

//---------------------------------------------------------------------------
// Function handles cutting selection
//---------------------------------------------------------------------------
void MyFrame::OnCut(wxCommandEvent& event)
{
    if (container->GetPageCount() > 0)
    {
        wxStyledTextCtrl* stc = static_cast<wxStyledTextCtrl*>(container->GetPage(container->GetSelection()));
        stc->Cut();
    }

    event.Skip();
}

//---------------------------------------------------------------------------
// Function handles pasting clipboard content
//---------------------------------------------------------------------------
void MyFrame::OnPaste(wxCommandEvent& event)
{
    if (container->GetPageCount() > 0)
    {
        wxStyledTextCtrl* stc = static_cast<wxStyledTextCtrl*>(container->GetPage(container->GetSelection()));
        stc->Paste();
    }

    event.Skip();
}

//---------------------------------------------------------------------------
// Function handles "Select all" option
//---------------------------------------------------------------------------
void MyFrame::OnSelectAll(wxCommandEvent& event)
{
    if (container->GetPageCount() > 0)
    {
        wxStyledTextCtrl* stc = static_cast<wxStyledTextCtrl*>(container->GetPage(container->GetSelection()));
        stc->SelectAll();
    }

    event.Skip();
}

//---------------------------------------------------------------------------
// Function handles undoing the last operation
//---------------------------------------------------------------------------
void MyFrame::OnUndo(wxCommandEvent& event)
{
    if (container->GetPageCount() > 0)
    {
        wxStyledTextCtrl* stc = static_cast<wxStyledTextCtrl*>(container->GetPage(container->GetSelection()));
        stc->Undo();
    }

    event.Skip();
}

//---------------------------------------------------------------------------
// Function handles redoing the last operation
//---------------------------------------------------------------------------
void MyFrame::OnRedo(wxCommandEvent& event)
{
    if (container->GetPageCount() > 0)
    {
        wxStyledTextCtrl* stc = static_cast<wxStyledTextCtrl*>(container->GetPage(container->GetSelection()));
        stc->Redo();
    }

    event.Skip();
}

//---------------------------------------------------------------------------
// Function handles tasks related to string search
//---------------------------------------------------------------------------
void MyFrame::OnFind(wxCommandEvent& event)
{
    // Check if the container has open documents
    if (container->GetPageCount() == 0) return;

    // Check if the search window should be closed
    if (findDialog)
    {
        wxDELETE(findDialog);
        return;
    }

    // Search window
    wxString dialog_title = wxT("Find...");
    int dialog_style = 0;

    if(event.GetId() == wxID_REPLACE)
    {
        dialog_title = wxT("Find and replace...");
        dialog_style = wxFR_REPLACEDIALOG;
    }

    findDialog = new wxFindReplaceDialog(this, &findData, dialog_title, dialog_style);
    findDialog->Show(true);
}

//---------------------------------------------------------------------------
// Function handles displaying the program in fullscreen mode
//---------------------------------------------------------------------------
void MyFrame::OnFullScr(wxCommandEvent& WXUNUSED(event))
{
	ShowFullScreen(!IsFullScreen(), wxFULLSCREEN_NOBORDER | wxFULLSCREEN_NOCAPTION | wxFULLSCREEN_NOSTATUSBAR);
}

//---------------------------------------------------------------------------
// Function handles toggling between word wrap and no wrap
//---------------------------------------------------------------------------
void MyFrame::OnWrap(wxCommandEvent& event)
{
    wrap_mode = (event.IsChecked()) ? wxSTC_WRAP_WORD : wxSTC_WRAP_NONE;

    wxStyledTextCtrl* stc = NULL;

    int pages = container->GetPageCount();
    if (pages > 0)
    {
        for (int i = 0; i < pages; i++)
        {
            stc = static_cast<wxStyledTextCtrl*>(this->container->GetPage(i));
            stc->SetWrapMode(wrap_mode);
        }
    }

    // Synchronize controls
    if (event.GetEventType() == wxEVT_CHECKBOX)
    {
        mView->Check(ID_WRAP, event.IsChecked());
    }
    else
    {
        wrap_chkb->SetValue(event.IsChecked());
    }
}

//---------------------------------------------------------------------------
// Function generates code for a simple C++ class
//---------------------------------------------------------------------------
void MyFrame::OnNewClass(wxCommandEvent& WXUNUSED(event))
{
	int index = container->GetSelection();
	if (index != wxNOT_FOUND)
	{
		wxString class_name = wxGetTextFromUser(wxT("Enter class name..."), wxT("Class creation"));
		if (class_name.IsEmpty())
		{
			return;
		}

		wxString src = ("class " + class_name + "\n{\n");
		src += ("public:\n    " + class_name + "() {}\n");
		src += ("    virtual ~" + class_name + "() {}\n\n");
		src += ("private:\n    " + class_name + "(const " + class_name + "& other) {}\n");
		src += ("    " + class_name + "(" + class_name + "&& other) {}\n");
		src += ("    " + class_name + "& operator=(const " + class_name + "& other) {}\n");
		src += ("    " + class_name + "& operator=(" + class_name + "&& other) {}\n};");

		wxStyledTextCtrl* stc = static_cast<wxStyledTextCtrl*>(container->GetPage(index));
		if(stc != NULL) stc->AppendText(src);
	}
}

//---------------------------------------------------------------------------
// Function checks if there are files to save
//---------------------------------------------------------------------------
void MyFrame::CheckFilesToSave(bool check_no_project_files)
{
    bool files_to_save = false;
    for (unsigned int i = 0; i < container->GetPageCount(); i++)
    {
        wxStyledTextCtrl* stc = static_cast<wxStyledTextCtrl*>(container->GetPage(i));
        wxTreeItemId item = tree->FindByPath(stc->GetName());
        bool is_proj_file = item.IsOk();

        if (!check_no_project_files && !is_proj_file)
        {
            continue;
        }

        if (stc->IsModified())
        {
            files_to_save = true;
        }
    }

    if (files_to_save)
    {
        if (wxMessageBox(wxT("Detected files with unsaved changes,\ndo you want to save them all now?\nAll unsaved changes will be lost!"),
            wxT("Warning!"), wxYES_NO | wxICON_EXCLAMATION) == wxYES)
        {
            DoSaveAll(check_no_project_files);
        }
    }
}

//---------------------------------------------------------------------------
// Function checks if the editor is currently busy
//---------------------------------------------------------------------------
bool MyFrame::CheckProject(bool new_project)
{
    wxString msg;

    // Check if another project is currently open
    if (this->project != NULL)
    {
        // Set the question text for the appropriate situation – new project/open project
        if (new_project)
        {
            msg = wxT("Another project is currently open!\nDo you really want to close the current project and create a new one?");
        }
        else
        {
            msg = wxT("Another project is currently open!\nDo you really want to close it and open another one?");
        }

        // Display the question about closing the current project
        if (wxMessageBox(msg, "Question", wxYES_NO | wxICON_QUESTION) != wxYES)
        {
            // Negative answer – exit the function
            return false;
        }

        // Positive answer – check if there are files to save
        CheckFilesToSave();

        // Save the current project
        this->project->SaveXML(this->project_path);

        // Close project files in the editor
        this->CloseProjectFiles();

        // Clear the project tree
        this->tree->DeleteAllItems();

        // Destroy the current project
        delete this->project;
        this->project = NULL;
    }

    return true;
}

//---------------------------------------------------------------------------
// Function converts the project tree to an XML structure and saves it to a file
//---------------------------------------------------------------------------
bool MyFrame::TranslateTreeToXML()
{
    if (tree->GetCount() <= 0) return false; // If the tree is empty

    this->project = new MyXmlDocument();

    project->AddRoot(wxT("CppProject"));
    project->AddVersion("1", "0", "0", "0");
    project->AddSettings(wxT("author"), wxGetUserName());
    project->AddSettings(wxT("date"), wxDateTime::Now().FormatISOCombined(' '));

    wxTreeItemIdValue cookie;

    wxXmlNode* project_node = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("project"));
    project_node->AddAttribute(wxT("name"), tree->GetRootName().ToStdString());
    project->AddTopChild(project_node);

    wxTreeItemId item = tree->GetLastChild(tree->GetRootItem());
    while (item.IsOk())
    {
        // Setting filter data
        wxXmlNode* filter_node = new wxXmlNode(project_node, wxXML_ELEMENT_NODE, wxT("filter"));
        filter_node->AddAttribute(wxT("name"), tree->GetItemText(item).ToStdString());

        // Setting file data
        wxTreeItemId item_child = tree->GetFirstChild(item, cookie);
        while (item_child.IsOk())
        {
            wxXmlNode* file_node = project->SimpleNode(wxT("file"));

            // Adding file data
            ProjectTreeData* data = static_cast<ProjectTreeData*>(tree->GetItemData(item_child));
            wxXmlNode* file_path = project->TextValueNode(wxT("path"), data->file->full_path);
            file_node->AddChild(file_path);

            filter_node->AddChild(file_node);

            item_child = tree->GetNextChild(item, cookie);
        }

        item = tree->GetPrevSibling(item);
    }

    return project->SaveXML(this->project_path);
}

//---------------------------------------------------------------------------
// Function converts the XML structure to a project tree
//---------------------------------------------------------------------------
bool MyFrame::TranslateXMLToTree()
{
    this->project = new MyXmlDocument();

    if (!project->Load(this->project_path))
    {
        return false;
    }

    wxXmlNode *node = project->GetRoot()->GetChildren();
    ProcessXmlStructure(node);

    // Sorting files
    SortTreeFiles();

    if(tree->GetCount() > 0) tree->ExpandAll();
    return true;
}

//---------------------------------------------------------------------------
// Function processes XML tags
//---------------------------------------------------------------------------
void MyFrame::ProcessXmlNode(wxXmlNode* node)
{
    if (!node) return;

    wxString name = node->GetName();

    if (name.IsSameAs("version"))
    {
        wxString release = node->GetAttribute("release");
        wxString major = node->GetAttribute("major");
        wxString minor = node->GetAttribute("minor");
        wxString build = node->GetAttribute("build");

        // Here, version checking...

        wxLogMessage("Loading project file for version " + release + "." + major + "." + minor + "." + build);
    }
    else if (name.IsSameAs("settings"))
    {
        // Here, we do something with settings...
    }
    else if (name.IsSameAs("project"))
    {
        wxString proj = node->GetAttribute("name");
        tree->AppendItem(proj, ProjectTreeData::ITEM_TYPE_ROOT, TREE_ICON_PROJECT);
    }
    else if (name.IsSameAs("filter"))
    {
        wxString filt = node->GetAttribute("name");
        tree->AppendItem(filt, ProjectTreeData::ITEM_TYPE_FILTER, TREE_ICON_FILTER);
    }
    else if (name.IsSameAs("path"))
    {
        wxString path = node->GetChildren()->GetContent();

        if (!wxFileExists(path))
        {
            wxLogError("File " + path + " does not exist and was not loaded!");
            return;
        }

        int image = GetFileIcon(path);

        wxTreeItemId last_filt = tree->GetLastChild(tree->GetRootItem());

        ProjectFile *file = new ProjectFile(path, last_filt);
        tree->AppendItem(wxEmptyString, ProjectTreeData::ITEM_TYPE_FILE, image, -1, file);
    }
}

//---------------------------------------------------------------------------
// Function recursively processes all elements of the XML structure
//---------------------------------------------------------------------------
void MyFrame::ProcessXmlStructure(wxXmlNode* node)
{
    while (node)
    {
        if (node->GetType() == wxXML_ELEMENT_NODE)
        {
            ProcessXmlNode(node);
            ProcessXmlStructure(node->GetChildren());
        }

        node = node->GetNext();
    }
}

//---------------------------------------------------------------------------
// Function returns the icon index for a file based on its extension
//---------------------------------------------------------------------------
int  MyFrame::GetFileIcon(const wxString& path, bool edit)
{
    int image = -1;
    wxFileName file_name(path);
    wxString ext = file_name.GetExt();

    if (ext == "h" || ext == "H")
    {
        image = (edit) ? TREE_ICON_H_FILE_EDIT : TREE_ICON_H_FILE;
    }
    else if (ext == "hpp" || ext == "HPP" || ext == "hxx" || ext == "HXX")
    {
        image = (edit) ? TREE_ICON_HPP_FILE_EDIT : TREE_ICON_HPP_FILE;
    }
    else if (ext == "c" || ext == "C")
    {
        image = (edit) ? TREE_ICON_C_FILE_EDIT : TREE_ICON_C_FILE;
    }
    else if (ext == "cpp" || ext == "CPP" || ext == "cxx" || ext == "CXX")
    {
        image = (edit) ? TREE_ICON_CPP_FILE_EDIT : TREE_ICON_CPP_FILE;
    }
    else if (ext == "rc" || ext == "RC" || ext == "res" || ext == "RES")
    {
        image = (edit) ? TREE_ICON_RC_FILE_EDIT : TREE_ICON_RC_FILE;
    }
    else
    {
        image = (edit) ? TREE_ICON_FILE_EDIT : TREE_ICON_FILE;
    }

    return image;
}

//---------------------------------------------------------------------------
// Function sorts files in the tree
//---------------------------------------------------------------------------
void MyFrame::SortTreeFiles()
{
    wxTreeItemIdValue cookie;
    wxTreeItemId item = tree->GetFirstChild(tree->GetRootItem(), cookie);
    while(item)
    {
        tree->SortChildren(item);
        item = tree->GetNextChild(tree->GetRootItem(), cookie);
    }
}

//---------------------------------------------------------------------------
// Function adds a file to the project
//---------------------------------------------------------------------------
bool MyFrame::AddFileToProject(const wxString& file_path)
{
    // Check if a file with the given location already exists.
    // It is possible to load files with the same name but
    // different locations.
    wxTreeItemId tmp = tree->FindByPath(file_path);
    if (tmp.IsOk())
    {
        if (wxMessageBox(wxT("A file with the given name and location already exists!"), wxT("Warning"), wxOK | wxICON_EXCLAMATION))
        {
            return false;
        }
    }

    // Set the filter array
    wxArrayString filters;
    filters.Add(FILTER_HEADERS);
    filters.Add(FILTER_SOURCES);
    filters.Add(FILTER_RESOURCES);
    filters.Add(FILTER_OTHER_FILES);

    // Display the filter selection window for the file
    wxString filter = wxGetSingleChoice("Select a filter for the file", "Filter", filters, 0, this);
    if(filter.IsEmpty())
    {
        return false;
    }

    // Add the file to the project tree
    wxTreeItemId tree_filter = tree->FindByName(tree->GetRootItem(), filter);
    ProjectFile* file = new ProjectFile(file_path, tree_filter);
    tree->AppendItem(wxEmptyString, ProjectTreeData::ITEM_TYPE_FILE, GetFileIcon(file_path), -1, file);
    tree->Expand(tree_filter);

    // Sort files in the tree
    SortTreeFiles();

    // Update the project file
    if (TranslateTreeToXML())
    {
        file->stc = OpenInEditor(file_path);
        wxLogMessage(wxT("File ") + file_path + wxT(" was successfully added to the project."));
    }
    else
    {
        wxLogError(wxT("There was a problem updating the project file!"));
    }

    return true;
}

//---------------------------------------------------------------------------
// Function displays the context menu for files in the tree
//---------------------------------------------------------------------------
void MyFrame::OnTreeContextMenu(wxTreeEvent& event)
{
    // Check if the selected item is a file
    ProjectTreeData* data = static_cast<ProjectTreeData*>(tree->GetItemData(event.GetItem()));
    if (data == NULL)
    {
        return;
    }

    // Select the clicked tree item
    tree->SelectItem(event.GetItem());

    // Display the context menu at the right-click location
    PopupMenu(TreeContextMenu, event.GetPoint());

    event.Skip();
}

//---------------------------------------------------------------------------
// Function displays the file activated in the project tree in the editor
//---------------------------------------------------------------------------
void MyFrame::OnTreeActivated(wxTreeEvent& event)
{
    // Check if the selected item is a file
    ProjectTreeData* data = static_cast<ProjectTreeData*>(tree->GetItemData(event.GetItem()));
    if (data == NULL)
    {
        return;
    }

    // Check if the document is already open,
    // and if so, select it...
    if (data->file->stc != NULL)
    {
        container->SetSelection(container->FindPage(data->file->stc));
        return;
    }

    // Open the file in the editor and set the pointer to the editor window...
    data->file->stc = OpenInEditor(data->file->full_path);
}

//---------------------------------------------------------------------------
// wxStyledTextCtrl styles
//---------------------------------------------------------------------------
void MyFrame::SetEditorStyle(wxStyledTextCtrl* stc)
{
    // Cleaning
    stc->StyleClearAll();

    // LINE NUMBERS
    stc->SetMarginWidth(STC_LINE_NUM_MARGIN, 30);
    stc->SetMarginType(STC_LINE_NUM_MARGIN, wxSTC_MARGIN_NUMBER);
    stc->StyleSetForeground(wxSTC_STYLE_LINENUMBER, wxColour(155, 255, 255));
    stc->StyleSetBackground(wxSTC_STYLE_LINENUMBER, wxColour(0, 128, 255));

    stc->SetWrapMode(wrap_mode);

    stc->SetLexer(wxSTC_LEX_CPP);

    wxFont font(10, wxSWISS, wxNORMAL, wxNORMAL, false, "Courier New");
    wxFont fontb(10, wxSWISS, wxNORMAL, wxFONTWEIGHT_BOLD, false, "Courier New");

    stc->StyleSetFont(wxSTC_C_DEFAULT, font);
    stc->StyleSetFont(wxSTC_C_COMMENT, font);
    stc->StyleSetFont(wxSTC_C_COMMENTLINE, font);
    stc->StyleSetFont(wxSTC_C_COMMENTDOC, font);
    stc->StyleSetFont(wxSTC_C_NUMBER, font);
    stc->StyleSetFont(wxSTC_C_WORD, fontb);
    stc->StyleSetFont(wxSTC_C_STRING, font);
    stc->StyleSetFont(wxSTC_C_CHARACTER, font);
    stc->StyleSetFont(wxSTC_C_UUID, font);
    stc->StyleSetFont(wxSTC_C_PREPROCESSOR, font);
    stc->StyleSetFont(wxSTC_C_OPERATOR, font);
    stc->StyleSetFont(wxSTC_C_IDENTIFIER, font);
    stc->StyleSetFont(wxSTC_C_STRINGEOL, font);
    stc->StyleSetFont(wxSTC_C_VERBATIM, font);
    stc->StyleSetFont(wxSTC_C_REGEX, font);
    stc->StyleSetFont(wxSTC_C_COMMENTLINEDOC, font);
    stc->StyleSetFont(wxSTC_C_WORD2, fontb);
    stc->StyleSetFont(wxSTC_C_COMMENTDOCKEYWORD, font);
    stc->StyleSetFont(wxSTC_C_COMMENTDOCKEYWORDERROR, font);
    stc->StyleSetFont(wxSTC_C_GLOBALCLASS, font);
    stc->StyleSetFont(wxSTC_C_STRINGRAW, font);
    stc->StyleSetFont(wxSTC_C_TRIPLEVERBATIM, font);
    stc->StyleSetFont(wxSTC_C_HASHQUOTEDSTRING, font);
    stc->StyleSetFont(wxSTC_C_PREPROCESSORCOMMENT, font);

    stc->StyleSetForeground(wxSTC_C_STRING, wxColour(255, 0, 0));
    stc->StyleSetForeground(wxSTC_C_PREPROCESSOR, wxColour(0, 128, 0));
    stc->StyleSetForeground(wxSTC_C_IDENTIFIER, wxColour(0, 0, 0));
    stc->StyleSetForeground(wxSTC_C_NUMBER, wxColour(128, 0, 128));
    stc->StyleSetForeground(wxSTC_C_CHARACTER, wxColour(150, 0, 0));
    stc->StyleSetForeground(wxSTC_C_WORD, wxColour(0, 64, 128));
    stc->StyleSetForeground(wxSTC_C_WORD2, wxColour(0, 128, 128));
    stc->StyleSetForeground(wxSTC_C_COMMENT, wxColour(0, 128, 192));
    stc->StyleSetForeground(wxSTC_C_COMMENTLINE, wxColour(0, 128, 192));
    stc->StyleSetForeground(wxSTC_C_COMMENTDOC, wxColour(0, 128, 192));
    stc->StyleSetForeground(wxSTC_C_COMMENTDOCKEYWORD, wxColour(0, 128, 192));
    stc->StyleSetForeground(wxSTC_C_COMMENTDOCKEYWORDERROR, wxColour(0, 128, 192));

    stc->StyleSetBold(wxSTC_C_WORD, true);
    stc->StyleSetBold(wxSTC_C_WORD2, true);
    stc->StyleSetBold(wxSTC_C_COMMENTDOCKEYWORD, true);

    // CODE FOLDING
    stc->SetMarginWidth(STC_FOLD_MARGIN, 16);
    stc->SetMarginType(STC_FOLD_MARGIN, wxSTC_MARGIN_SYMBOL);
    stc->SetMarginMask(STC_FOLD_MARGIN, wxSTC_MASK_FOLDERS);
    stc->StyleSetBackground(STC_FOLD_MARGIN, wxColor(200, 200, 200));
    stc->SetMarginSensitive(STC_FOLD_MARGIN, true);

    stc->SetProperty(wxT("fold"), wxT("1"));

    wxColour custom(100, 100, 100);

    stc->MarkerDefine(wxSTC_MARKNUM_FOLDER, wxSTC_MARK_BOXPLUS);
    stc->MarkerSetForeground(wxSTC_MARKNUM_FOLDER, *wxWHITE);
    stc->MarkerSetBackground(wxSTC_MARKNUM_FOLDER, custom);

    stc->MarkerDefine(wxSTC_MARKNUM_FOLDEROPEN, wxSTC_MARK_BOXMINUS);
    stc->MarkerSetForeground(wxSTC_MARKNUM_FOLDEROPEN, *wxWHITE);
    stc->MarkerSetBackground(wxSTC_MARKNUM_FOLDEROPEN, custom);

    stc->MarkerDefine(wxSTC_MARKNUM_FOLDERTAIL, wxSTC_MARK_LCORNER);
    stc->MarkerSetBackground(wxSTC_MARKNUM_FOLDERTAIL, custom);

    stc->MarkerDefine(wxSTC_MARKNUM_FOLDEREND, wxSTC_MARK_BOXPLUSCONNECTED);
    stc->MarkerSetForeground(wxSTC_MARKNUM_FOLDEREND, *wxWHITE);
    stc->MarkerSetBackground(wxSTC_MARKNUM_FOLDEREND, custom);

    stc->MarkerDefine(wxSTC_MARKNUM_FOLDEROPENMID, wxSTC_MARK_BOXMINUSCONNECTED);
    stc->MarkerSetForeground(wxSTC_MARKNUM_FOLDEROPENMID, *wxWHITE);
    stc->MarkerSetBackground(wxSTC_MARKNUM_FOLDEROPENMID, custom);

    stc->MarkerDefine(wxSTC_MARKNUM_FOLDERMIDTAIL, wxSTC_MARK_TCORNER);
    stc->MarkerSetBackground(wxSTC_MARKNUM_FOLDERMIDTAIL, custom);

    stc->MarkerDefine(wxSTC_MARKNUM_FOLDERSUB, wxSTC_MARK_VLINE);
    stc->MarkerSetBackground(wxSTC_MARKNUM_FOLDERSUB, custom);
}

//---------------------------------------------------------------------------
// Function opens a file in the editor
//---------------------------------------------------------------------------
wxStyledTextCtrl* MyFrame::OpenInEditor(const wxString& file_name)
{
    // Creating a new wxStyledTextCtrl control
    wxWindowID new_id = GetNewEditorId();
    wxStyledTextCtrl* stc = new wxStyledTextCtrl(container, new_id, wxDefaultPosition, wxDefaultSize, 0L, file_name);
    this->SetEditorStyle(stc);

    // Setting keywords
    stc->SetKeyWords(0, wxGetWXKeyWords());
    stc->SetKeyWords(1, wxGetCPP11KeyWords());

    // Loading the file into the editor
    if (stc->LoadFile(file_name))
    {
        // Dynamically attaching events
        Bind(wxEVT_STC_MARGINCLICK, &MyFrame::OnStcMarginClick, this, new_id);
        Bind(wxEVT_STC_MODIFIED, &MyFrame::OnStcModified, this, new_id);

        // Adding a page to the container
        wxFileName file(file_name);
        container->AddPage(stc, file.GetFullName(), true, GetFileIcon(file_name));
    }
    else
    {
        wxLogWarning(wxT("Failed to load file ") + file_name + wxT("!"));
    }

    // Returning a pointer to the editor window
    return stc;
}

//---------------------------------------------------------------------------
// Function handles clicking the STC margin
//---------------------------------------------------------------------------
void MyFrame::OnStcMarginClick(wxStyledTextEvent& event)
{
    wxStyledTextCtrl* stc = static_cast<wxStyledTextCtrl*>(event.GetEventObject());

    switch (event.GetMargin())
    {
    case STC_LINE_NUM_MARGIN:
        break;

    // Handling the marker margin...
    case STC_FOLD_MARGIN:
        {
            int line_number = stc->LineFromPosition(event.GetPosition());
            if (stc->GetFoldLevel(line_number) & wxSTC_FOLDLEVELHEADERFLAG)
            {
                stc->ToggleFold(line_number);
            }
            break;
        }

    default:
        break;
    }
}

//---------------------------------------------------------------------------
// Function updates the icons of the selected item
//---------------------------------------------------------------------------
void MyFrame::UpdateFileIcon(wxStyledTextCtrl* stc)
{
    wxFileName file(stc->GetName());
    int icon_index = GetFileIcon(file.GetFullName(), stc->IsModified());
    wxTreeItemId tree_item = tree->FindByPath(file.GetFullPath());

    // Setting the edit icon
    tree->SetItemImage(tree_item, icon_index);
    container->SetPageImage(container->FindPage(stc), icon_index);
}

//---------------------------------------------------------------------------
// Function handles changes within the editor
//---------------------------------------------------------------------------
void MyFrame::OnStcModified(wxStyledTextEvent& event)
{
    wxStyledTextCtrl* stc = static_cast<wxStyledTextCtrl*>(event.GetEventObject());

    if (event.GetModificationType() & wxSTC_MOD_INSERTTEXT ||
        event.GetModificationType() & wxSTC_MOD_DELETETEXT)
    {
        UpdateFileIcon(stc);
        SetDocInfo(stc);
    }
}

//---------------------------------------------------------------------------
// Function searches for a string in the given editor control
//---------------------------------------------------------------------------
int MyFrame::DoFind(wxStyledTextCtrl* stc, const wxString& str, int flags)
{
    // Setting the position
    int pos = stc->GetCurrentPos();

    // Mapping search flags for wxStyledTextCtrl
    int find_flags = 0;
    if (flags & wxFR_MATCHCASE) find_flags |= wxSTC_FIND_MATCHCASE;
    if (flags & wxFR_WHOLEWORD) find_flags |= wxSTC_FIND_WHOLEWORD;

    // Helper variable for changing search directions
    static int last_direction = wxFR_DOWN;

    // SEARCHING
    int result = -1;
    if (flags & wxFR_DOWN)
    {
        // Searching downwards
        if (last_direction != wxFR_DOWN)
        {
            pos += str.Len();
            stc->GotoPos(pos);
        }
        stc->SetAnchor(pos);
        stc->SearchAnchor();
        result = stc->SearchNext(find_flags, str);
        last_direction = wxFR_DOWN;
    }
    else
    {
        // Searching upwards
        stc->SearchAnchor();
        result = stc->SearchPrev(find_flags, str);
        last_direction = -1;
    }

    if (result != -1)
    {
        if (flags & wxFR_DOWN)
        {
            stc->SetSelection(result, stc->GetAnchor());
        }
    }

    return result;
}

//---------------------------------------------------------------------------
// Function handles searching for a string from the search dialog
//---------------------------------------------------------------------------
void MyFrame::OnDoFind(wxFindDialogEvent& event)
{
    // Setting the active editor control
    wxStyledTextCtrl* stc = static_cast<wxStyledTextCtrl*>(container->GetPage(container->GetSelection()));

    // SEARCHING
    int result = DoFind(stc, event.GetFindString(), event.GetFlags());

    if(result == -1)
    {
        // File boundary...
        wxMessageBox(wxT("File boundary reached.\nSearch will be moved."));

        // ... moving the cursor to the other end
        if (event.GetFlags() & wxFR_DOWN)
        {
            stc->DocumentStart();
        }
        else
        {
            stc->DocumentEnd();
        }
    }
    else
    {
        // Ensuring the found element is displayed
        stc->EnsureCaretVisible();
    }
}

//---------------------------------------------------------------------------
// Function handles searching and replacing a string from the search dialog
//---------------------------------------------------------------------------
void MyFrame::OnDoFindReplace(wxFindDialogEvent& event)
{
    // Setting the active editor control
    wxStyledTextCtrl* stc = static_cast<wxStyledTextCtrl*>(container->GetPage(container->GetSelection()));

    if (event.GetEventType() == wxEVT_FIND_REPLACE)
    {
        if (!stc->GetSelections())
        {
            stc->ReplaceSelection(event.GetReplaceString());
        }
        else
        {
            wxLogMessage(wxT("No selection to replace. Search for strings."));
        }
    }
    else // wxEVT_FIND_REPLACE_ALL
    {
        int count = 0;
        int result = -1;

        stc->DocumentStart();

        do
        {
            result = DoFind(stc, event.GetFindString(), event.GetFlags());
            if (result != -1)
            {
                stc->ReplaceSelection(event.GetReplaceString());
                count++;
            }
        } while (result != -1);

        wxLogMessage(wxString::Format("Number of replaced strings: %d", count));
    }
}

//---------------------------------------------------------------------------
// Function handles closing the search dialog
//---------------------------------------------------------------------------
void MyFrame::OnFindDialogClose(wxFindDialogEvent& WXUNUSED(event))
{
    findDialog->Destroy();
    findDialog = NULL;
}

//---------------------------------------------------------------------------
// Function saves the document open in the editor page
//---------------------------------------------------------------------------
void MyFrame::DoSave(wxStyledTextCtrl* stc)
{
    wxString path = stc->GetName();
    if (stc->IsModified() && stc->wxStyledTextCtrl::SaveFile(path))
    {
        UpdateFileIcon(stc);
        wxLogMessage(wxT("File " + path + " was successfully saved."));
    }
    else
    {
        wxLogWarning(wxT("Failed to save file " + path + "!"));
    }
}

//---------------------------------------------------------------------------
// Function saves all unsaved documents
//---------------------------------------------------------------------------
void MyFrame::DoSaveAll(bool save_no_project_files)
{
    bool is_ok = true;
    int count = container->GetPageCount();
    for (int i = 0; i < count; i++)
    {
        wxStyledTextCtrl* stc = static_cast<wxStyledTextCtrl*>(container->GetPage(i));
        wxTreeItemId item = tree->FindByPath(stc->GetName());
        bool is_proj_file = item.IsOk();

        if (!save_no_project_files && !is_proj_file)
        {
            continue;
        }

        if (stc->IsModified() && stc->wxStyledTextCtrl::SaveFile(stc->GetName()))
        {
            UpdateFileIcon(stc);
        }
        else
        {
            is_ok = false;
        }
    }

    if (!is_ok)
    {
        wxLogWarning(wxT("There were problems saving some files!"));
    }
}

//---------------------------------------------------------------------------
// Function closes all project files
//---------------------------------------------------------------------------
void MyFrame::CloseProjectFiles()
{
    int count = container->GetPageCount();
    for (int i = count - 1; i >= 0; i--)
    {
        wxStyledTextCtrl* stc = static_cast<wxStyledTextCtrl*>(container->GetPage(i));
        wxTreeItemId item = tree->FindByPath(stc->GetName());
        if (item.IsOk())
        {
            container->DeletePage(i);
        }
    }

    SetDocInfo(NULL);
}

//---------------------------------------------------------------------------
// Function responds to notebook page change
//---------------------------------------------------------------------------
void MyFrame::OnPageChange(wxBookCtrlEvent& event)
{
    wxStyledTextCtrl* stc = static_cast<wxStyledTextCtrl*>(this->container->GetPage(event.GetSelection()));
    SetDocInfo(stc);
}

//---------------------------------------------------------------------------
// Function displays document information in the statistics list
//---------------------------------------------------------------------------
void MyFrame::SetDocInfo(wxStyledTextCtrl* stc)
{
    wxString file = wxEmptyString;
    wxString lines = wxEmptyString;
    wxString chars = wxEmptyString;
    wxString size = wxEmptyString;

    if (stc != NULL)
    {
        wxFileName src_home(stc->GetName());
        file = src_home.GetPathWithSep();
        lines = wxString::Format("%d", stc->GetLineCount());
        chars = wxString::Format("%d", stc->GetLength());
        size = src_home.GetHumanReadableSize(wxT("No data"));
    }

    list->SetItem(0, 1, file, -1);
    list->SetItem(1, 1, lines, -1);
    list->SetItem(2, 1, chars, -1);
    list->SetItem(3, 1, size, -1);
}

//---------------------------------------------------------------------------
// Function converts editor text to an HTML string
//---------------------------------------------------------------------------
wxString MyFrame::PreparePrintString(const wxString& src)
{
    wxString html = wxT("<html><body>");
    wxString tmp = src;

    // CHARACTER CONVERSION
    tmp.Replace("<", "&#60;"); // Decimal entities
    tmp.Replace(">", "&#62;");
    tmp.Replace("&", "\u0026"); // JS sequences
    tmp.Replace("\"", "\u0022");
    tmp.Replace("    ", "\t"); // 4 spaces to tab
    tmp.Replace("\t", "&nbsp;&nbsp;&nbsp;&nbsp;"); // Symbolic entities
    tmp.Replace("\n", "<br />"); // Line break to HTML tag

    html += tmp;
    html += wxT("</body></html>");

    // Returning the converted string
    return tmp;
}

