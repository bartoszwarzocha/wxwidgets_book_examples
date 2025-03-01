/////////////////////////////////////////////////////////////////////////////
// Name:        MyListCtrl.h
// Purpose:     My wxWidgets app
// Author:      Imie Nazwisko <imie.nazwisko@email.org>
// Created:     2018-01-01
// Copyright:   (c) 2018 by Imie Nazwisko
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _MYLISTCTRL_H_
#define _MYLISTCTRL_H_

#include <wx/listctrl.h>
#include <wx/imaglist.h>

class MyListCtrl : public wxListCtrl
{
public:
    MyListCtrl(wxWindow* parent, wxWindowID id, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, int style = wxLC_REPORT);
    virtual ~MyListCtrl() {}

    enum
    {
        SORT_UNKNOWN = -1,
        SORT_ASCENDING,
        SORT_DESCENDING
    };

    struct SortData
    {
        SortData(MyListCtrl* l, int c, int d): list(l), column(c), direction(d) {}

        MyListCtrl* list;
        int column;
        int direction;
    };

    void OnSort(wxListEvent& event);

    virtual void GenerateSampleData(int cols, int rows, int default_icon = 0, bool sort = true);
    virtual bool Sort(int column, int dir = SORT_ASCENDING);

    int(*string_compare_fn)(wxString item1, wxString item2);
    void SetStrCompareFn(int(*fn)(wxString item1, wxString item2)) { this->string_compare_fn = fn; }

private:
    int last_col;
    int current_col;
    int direction;

    DECLARE_EVENT_TABLE()
};


#endif
