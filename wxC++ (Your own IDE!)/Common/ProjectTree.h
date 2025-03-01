/////////////////////////////////////////////////////////////////////////////
// Name:        ProjectTree.h
// Purpose:     My wxWidgets app
// Author:      Name Surname <name.surname@email.org>
// Created:     2025-01-01
// Copyright:   (c) 2025 by Name Surname
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _PROJECTTREE_H_
#define _PROJECTTREE_H_

#include <wx/treectrl.h>
#include <wx/filename.h>
#include <wx/stc/stc.h>

//---------------------------------------------------------------------------
// File structure
//---------------------------------------------------------------------------
struct ProjectFile
{
    // Constructor
    ProjectFile(const wxString& fp, const wxTreeItemId& fltr = NULL) : is_edit(false), filter(fltr)
    {
        SetFilePath(fp);
        stc = NULL;
    }

    void SetFilePath(const wxString& fp)
    {
        if (!fp.IsEmpty())
        {
            wxFileName file(fp);
            this->full_path = fp;
            this->name = file.GetFullName();
        }
    }

    void DeleteStc()
    {
        stc->Destroy();
        stc = NULL;
    }

    wxString full_path;     // Full file path
    wxString name;          // File name
    bool is_edit;           // Needs saving?
    wxTreeItemId filter;    // File filter
    wxStyledTextCtrl* stc;  // Editor
};

//---------------------------------------------------------------------------
// ProjectTreeData class
//---------------------------------------------------------------------------
class ProjectTreeData : public wxTreeItemData
{
public:
    enum // Tree item types
    {
        ITEM_TYPE_ROOT = 0, // Root
        ITEM_TYPE_FILTER,   // Filter
        ITEM_TYPE_FILE,     // File
        ITEM_TYPE_UNKNOWN   // Unknown
    };

    // Constructors
    ProjectTreeData() : type(ITEM_TYPE_UNKNOWN), file(NULL) {}
    ProjectTreeData(unsigned int t, ProjectFile* f)
        : type(t), file(f) {
    }
    // Destructor
    virtual ~ProjectTreeData()
    {
        if (this->file != NULL) delete this->file;
        this->file = NULL;
    }

    unsigned int type;
    ProjectFile* file;
};

//---------------------------------------------------------------------------
// ProjectTreeCtrl class
//---------------------------------------------------------------------------
class ProjectTreeCtrl : public wxTreeCtrl
{
public:
    // Constructors
    ProjectTreeCtrl() {} // For wxRTTI and sorting
    ProjectTreeCtrl(wxWindow* parent, wxWindowID id)
        : wxTreeCtrl(parent, id, wxDefaultPosition, wxDefaultSize) {
    }
    virtual ~ProjectTreeCtrl() {}

    wxTreeItemId FindByPath(const wxString& file_path);
    wxTreeItemId FindByPath(wxTreeItemId item, const wxString& file_path); // Recursive variant

    // Dangerous functions, as there may be at least two files with the same name in the tree...
    wxTreeItemId FindByName(const wxString& name);
    wxTreeItemId FindByName(wxTreeItemId item, const wxString& name); // Recursive variant

    void AppendItem(const wxString& name, unsigned int type, int image = -1, int sel_image = -1, ProjectFile* file = NULL);

    wxString GetRootName();

private:
    DECLARE_DYNAMIC_CLASS(ProjectTreeCtrl) // wxRTTI for Windows
};

#endif
