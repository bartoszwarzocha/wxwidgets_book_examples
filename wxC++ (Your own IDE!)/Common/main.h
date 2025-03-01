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

#include "ProjectTree.h"
#include "MyXmlDocument.h"
#include <wx/notebook.h>
#include <wx/stc/stc.h>
#include <wx/fdrepdlg.h>
#include <wx/listctrl.h>

#define FILTER_HEADERS     wxT("Header files")
#define FILTER_SOURCES     wxT("Source files")
#define FILTER_RESOURCES   wxT("Resource files")
#define FILTER_OTHER_FILES wxT("Other files")

#define SUPPORTED_FILES_EXT wxT("Header files (*.h)|*.h|" \
        "Header files (*.hpp)|*.hpp|" \
        "Header files (*.hxx)|*.hxx|" \
        "Source files (*.c)|*.c|" \
        "Source files (*.cpp)|*.cpp|" \
        "Source files (*.cxx)|*.cxx|" \
        "Resource files (*.rc)|*.rc|" \
        "Resource files (*.res)|*.res|" \
        "All files|*.*")

#define STC_LINE_NUM_MARGIN 0  // Line number margin
#define STC_FOLD_MARGIN 1  // Fold margin

//---------------------------------------------------------------------------
// MyApp class
//---------------------------------------------------------------------------
class MyApp : public wxApp
{
public:
    virtual bool OnInit();
};

//---------------------------------------------------------------------------
// MyFrame class
//---------------------------------------------------------------------------
class MyFrame : public wxFrame
{
public:
    MyFrame(wxWindow* parent, wxWindowID id, const wxString& title,
        const wxPoint& pos, const wxSize& size, long style);
    ~MyFrame();

    void CloseApp(wxCloseEvent& event);

    void OnNewProject(wxCommandEvent& event);
    void OnOpenProject(wxCommandEvent& event);
    void OnOpenFile(wxCommandEvent& event);
    void OnAddNewFile(wxCommandEvent& event);
    void OnAddExistFile(wxCommandEvent& event);
    void OnSave(wxCommandEvent& event);
    void OnSaveAs(wxCommandEvent& event);
    void OnSaveAll(wxCommandEvent& event);
    void OnRename(wxCommandEvent& event);
    void OnRemove(wxCommandEvent& event);
    void OnCloseFile(wxCommandEvent& event);
    void OnPrint(wxCommandEvent& event);
    void OnPreview(wxCommandEvent& event);
    void OnPageSetup(wxCommandEvent& event);
    void OnClose(wxCommandEvent& event);
    void OnCopy(wxCommandEvent& event);
    void OnCut(wxCommandEvent& event);
    void OnPaste(wxCommandEvent& event);
    void OnSelectAll(wxCommandEvent& event);
    void OnUndo(wxCommandEvent& event);
    void OnRedo(wxCommandEvent& event);
    void OnFind(wxCommandEvent& event); // Common for all find options
    void OnFullScr(wxCommandEvent& event);
    void OnWrap(wxCommandEvent& event);
    void OnNewClass(wxCommandEvent& event);
    void OnAboutProgram(wxCommandEvent& event);
    void OnTreeContextMenu(wxTreeEvent& event);
    void OnTreeActivated(wxTreeEvent& event);
    void OnStcMarginClick(wxStyledTextEvent& event);
    void OnStcModified(wxStyledTextEvent& event);

    void OnDoFind(wxFindDialogEvent& event);
    void OnDoFindReplace(wxFindDialogEvent& event);
    void OnFindDialogClose(wxFindDialogEvent& event);

    void DoSave(wxStyledTextCtrl* stc);
    void DoSaveAll(bool save_no_project_files);

    void OnPageChange(wxBookCtrlEvent& event);

    enum
    {
        TREE_ICON_PROJECT = 0,   // Project
        TREE_ICON_FILTER,        // Filter
        TREE_ICON_H_FILE,        // H file
        TREE_ICON_H_FILE_EDIT,
        TREE_ICON_HPP_FILE,      // HPP file
        TREE_ICON_HPP_FILE_EDIT,
        TREE_ICON_C_FILE,        // C file
        TREE_ICON_C_FILE_EDIT,
        TREE_ICON_CPP_FILE,      // CPP file
        TREE_ICON_CPP_FILE_EDIT,
        TREE_ICON_RC_FILE,       // RC file
        TREE_ICON_RC_FILE_EDIT,
        TREE_ICON_FILE,          // Other file
        TREE_ICON_FILE_EDIT
    };

private:
    void CheckFilesToSave(bool check_no_project_files = false);
    bool CheckProject(bool new_project = true);
    bool TranslateTreeToXML();

    int GetFileIcon(const wxString& path, bool edit = false);
    void UpdateFileIcon(wxStyledTextCtrl* stc);

    void SortTreeFiles();
    bool TranslateXMLToTree();
    void ProcessXmlNode(wxXmlNode* node);
    void ProcessXmlStructure(wxXmlNode* node);

    bool AddFileToProject(const wxString& file_path);

    void SetEditorStyle(wxStyledTextCtrl* stc);
    wxStyledTextCtrl* OpenInEditor(const wxString& file_path);

    int DoFind(wxStyledTextCtrl* stc, const wxString& str, int flags);

    void SetDocInfo(wxStyledTextCtrl* stc);

    void CloseProjectFiles();

    static long GetNewEditorId()
    {
        static int id = 10000;
        return id++;
    }

    wxString PreparePrintString(const wxString& src);

    wxMenuBar* mb; // Menu bar
    wxMenu* mFile; // 'File' menu
    wxMenu* mAddFile; // 'File' → 'Add file' menu
    wxMenu* mEdit; // 'Edit' menu
    wxMenu* mView; // 'View' menu
    wxMenu* mTools; // 'Tools' menu
    wxMenu* mHelp; // 'Help' menu

    wxMenu* TreeContextMenu;

    wxStatusBar* sb;

    wxToolBar* tb;

    wxPanel* panel;
    wxPanel* left; // Left column of the GUI
    wxPanel* right; // Right column of the GUI
    wxPanel* rightTop; // Right column – top
    wxPanel* rightBottom; // Right column – bottom

    wxSplitterWindow* splitter;
    wxSplitterWindow* splitterRight;

    ProjectTreeCtrl* tree;
    wxTextCtrl* console;

    wxNotebook* container;

    MyXmlDocument* project;
    wxString project_path;

    wxFindReplaceData findData;
    wxFindReplaceDialog* findDialog;

    wxListCtrl* list;

    wxCheckBox* wrap_chkb;
    int wrap_mode;

    wxHtmlEasyPrinting* print_engine;

    DECLARE_EVENT_TABLE()
};

#endif
