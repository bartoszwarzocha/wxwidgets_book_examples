/////////////////////////////////////////////////////////////////////////////
// Name:        ProjectTree.cpp
// Purpose:     My wxWidgets app
// Author:      Name Surname <name.surname@email.org>
// Created:     2025-01-01
// Copyright:   (c) 2025 by Name Surname
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#include "ProjectTree.h"

IMPLEMENT_DYNAMIC_CLASS(ProjectTreeCtrl, wxTreeCtrl) // wxRTTI for Windows

//---------------------------------------------------------------------------
// Function to search for a file in the tree based on the path
//---------------------------------------------------------------------------
wxTreeItemId ProjectTreeCtrl::FindByPath(const wxString& file_path)
{
    wxTreeItemIdValue cookie1; // For root level
    wxTreeItemIdValue cookie2; // For filter level

    // ROOT level
    wxTreeItemId item = this->GetFirstChild(this->GetRootItem(), cookie1);
    while (item.IsOk())
    {
        // Filter level
        wxTreeItemId item_child = this->GetFirstChild(item, cookie2);
        while (item_child.IsOk())
        {
            // Searching at the file level
            ProjectTreeData* data = static_cast<ProjectTreeData*>(this->GetItemData(item_child));
            if (file_path.IsSameAs(data->file->full_path))
            {
                return item_child;
            }

            item_child = this->GetNextChild(item, cookie2);
        }

        item = this->GetNextChild(this->GetRootItem(), cookie1);
    }

    // No item found
    wxTreeItemId invalid;
    return invalid;
}

//---------------------------------------------------------------------------
// Function to recursively search the tree by the given path
//---------------------------------------------------------------------------
wxTreeItemId ProjectTreeCtrl::FindByPath(wxTreeItemId item, const wxString& file_path)
{
    wxTreeItemIdValue cookie;

    if (item.IsOk() && this->ItemHasChildren(item))
    {
        wxTreeItemId tmp_item = this->GetFirstChild(item, cookie);
        while (tmp_item.IsOk())
        {
            // Return the current item if the path passed as a parameter
            // is identical to the path stored in the item data
            ProjectTreeData* data = static_cast<ProjectTreeData*>(this->GetItemData(tmp_item));
            if (data && data->file)
            {
                if (file_path.IsSameAs(data->file->full_path)) return tmp_item;
            }

            // If it was not the correct item, check if it has children...
            if (this->ItemHasChildren(tmp_item))
            {
                // ... and then search recursively if the test is positive...
                wxTreeItemId tmp_item_child = this->FindByPath(tmp_item, file_path);
                if (tmp_item_child.IsOk()) return tmp_item_child;
            }

            // If none of the tests on the current item caused the function to end,
            // move to the next tree item...
            tmp_item = this->GetNextChild(item, cookie);
        }
    }

    // No item found
    wxTreeItemId invalid;
    return invalid;
}

//---------------------------------------------------------------------------
// Function to search for a file in the tree by name
//---------------------------------------------------------------------------
wxTreeItemId ProjectTreeCtrl::FindByName(const wxString& name)
{
    wxTreeItemIdValue cookie1; // For root level
    wxTreeItemIdValue cookie2; // For filter level

    // ROOT level
    wxTreeItemId item = this->GetFirstChild(this->GetRootItem(), cookie1);
    while (item.IsOk())
    {
        // Filter level
        wxTreeItemId item_child = this->GetFirstChild(item, cookie2);
        while (item_child.IsOk())
        {
            // Searching at the file level
            if (name.IsSameAs(this->GetItemText(item_child)))
            {
                return item_child;
            }

            item_child = this->GetNextChild(item, cookie2);
        }

        item = this->GetNextChild(this->GetRootItem(), cookie1);
    }

    // No item found
    wxTreeItemId invalid;
    return invalid;
}

//---------------------------------------------------------------------------
// Function to recursively search the tree by the given name
//---------------------------------------------------------------------------
wxTreeItemId ProjectTreeCtrl::FindByName(wxTreeItemId item, const wxString& name)
{
    wxTreeItemIdValue cookie;

    if (item.IsOk() && this->ItemHasChildren(item))
    {
        wxTreeItemId tmp_item = this->GetFirstChild(item, cookie);
        while (tmp_item.IsOk())
        {
            // Return the current item if the name passed as a parameter
            // is identical to the string displayed in the tree...
            if (name.IsSameAs(this->GetItemText(tmp_item))) return tmp_item;

            // If it was not the correct item, check if it has children...
            if (this->ItemHasChildren(tmp_item))
            {
                // ... and then search recursively if the test is positive...
                wxTreeItemId tmp_item_child = this->FindByName(tmp_item, name);
                if (tmp_item_child.IsOk()) return tmp_item_child;
            }

            // If none of the tests on the current item caused the function to end,
            // move to the next tree item...
            tmp_item = this->GetNextChild(item, cookie);
        }
    }

    // No item found
    wxTreeItemId invalid;
    return invalid;
}

//---------------------------------------------------------------------------
// Function to insert data into the tree
//---------------------------------------------------------------------------
void ProjectTreeCtrl::AppendItem(const wxString& name, unsigned int type, int image, int sel_image, ProjectFile* file)
{
    if (type == ProjectTreeData::ITEM_TYPE_ROOT)
    {
        if (!this->GetRootItem().IsOk())
        {
            this->AddRoot(name, image, sel_image, NULL);
        }
    }
    else if (type == ProjectTreeData::ITEM_TYPE_FILTER)
    {
        if (this->GetRootItem().IsOk())
        {
            this->wxTreeCtrl::AppendItem(this->GetRootItem(), name, image, sel_image, NULL);
        }
    }
    else if (type == ProjectTreeData::ITEM_TYPE_FILE)
    {
        if (file && file->filter.IsOk())
        {
            this->wxTreeCtrl::AppendItem(file->filter, file->name, image, sel_image, new ProjectTreeData(type, file));
        }
    }
}

//---------------------------------------------------------------------------
// Function returns the name of the root element of the tree
//---------------------------------------------------------------------------
wxString ProjectTreeCtrl::GetRootName()
{
    if (this->GetRootItem().IsOk())
    {
        return this->GetItemText(this->GetRootItem());
    }
    else
    {
        return wxEmptyString;
    }
}


