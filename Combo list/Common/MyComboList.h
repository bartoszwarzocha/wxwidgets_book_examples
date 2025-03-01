/////////////////////////////////////////////////////////////////////////////
// Name:        MyComboList.h
// Purpose:     My wxWidgets app
// Author:      Name Surname <name.surname@email.org>
// Created:     2025-01-01
// Copyright:   (c) 2025 by Name Surname
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _MYCOMBOLIST_H_
#define _MYCOMBOLIST_H_

#include <wx/listctrl.h>
#include <wx/imaglist.h>
#include <wx/combo.h>

// #include "MyListCtrl.h" // If we use our sorted list...

class MyComboList : public wxListCtrl, public wxComboPopup
{
public:
    MyComboList(wxWindowID win_id) : id(win_id), col(0) {}
    virtual ~MyComboList() {}

    // Required function (wxComboPopup)
    virtual bool Create(wxWindow* parent)
    {
        return wxListCtrl::Create(parent, id, wxDefaultPosition, wxDefaultSize,
            wxLC_REPORT | wxLC_SINGLE_SEL | wxLC_HRULES | wxLC_VRULES);
    }

    // Required function (wxComboPopup)
    virtual wxWindow* GetControl() { return this; }

    virtual wxString GetStringSelection(int column, bool dismiss = true)
    {
        if (column > this->GetColumnCount() || column < 0) return wxEmptyString;
        if (dismiss) this->Dismiss();
        this->col = column;
        return GetStringValue();
    }

    virtual int GetSelection(bool dismiss = true)
    {
        int index = this->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
        if (dismiss) this->Dismiss();
        return index;
    }

private:
    // Required function (wxComboPopup)
    virtual wxString GetStringValue() const
    {
        int index = this->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
        if (index >= 0) return this->GetItemText(index, col);
        else return wxEmptyString;
    }

    wxWindowID id;
    int col;
};

#endif
