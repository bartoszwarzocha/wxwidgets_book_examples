/////////////////////////////////////////////////////////////////////////////
// Name:        MyListCtrl.cpp
// Purpose:     My wxWidgets app
// Author:      Imie Nazwisko <imie.nazwisko@email.org>
// Created:     2018-01-01
// Copyright:   (c) 2018 by Imie Nazwisko
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#include "MyListCtrl.h"

BEGIN_EVENT_TABLE(MyListCtrl, wxListCtrl)
    EVT_LIST_COL_CLICK(wxID_ANY, MyListCtrl::OnSort)
END_EVENT_TABLE()

int wxCALLBACK DoSort(wxIntPtr item1, wxIntPtr item2, MyListCtrl::SortData* data)
{
    wxString str1 = data->list->GetItemText(item1, data->column);
    wxString str2 = data->list->GetItemText(item2, data->column);

    int result = data->list->string_compare_fn(str1, str2);
    if (data->direction == MyListCtrl::SORT_DESCENDING) result = -result;

    return result;
}

MyListCtrl::MyListCtrl(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, int style)
    : wxListCtrl(parent, id, pos, size, style)
{
    last_col = -1;
    direction = SORT_ASCENDING;
}

void MyListCtrl::OnSort(wxListEvent& event)
{
    // Ustawienie kierunku
    if (direction == SORT_ASCENDING) direction = SORT_DESCENDING;
    else direction = SORT_ASCENDING;

    // Ustawienie kolumn
    current_col = event.GetColumn();
    if (last_col != current_col) direction = SORT_ASCENDING;

    // Sortowanie
    Sort(current_col, direction);
}

bool MyListCtrl::Sort(int column, int dir)
{
    // Testy
    if (this->GetColumnCount() == 0) return true;
    if (this->GetItemCount() == 0) return true;

    if (dir == SORT_UNKNOWN) dir = SORT_ASCENDING;

    // ustawienie ikony kierunku sortowania
    if (this->GetImageList(wxIMAGE_LIST_SMALL))
    {
        wxListItem col;
        col.SetMask(wxLIST_MASK_IMAGE);

        if (column != last_col && last_col != -1)
        {
            col.SetImage(SORT_UNKNOWN);
            this->SetColumn(last_col, col);
        }

        col.SetImage(dir);
        this->SetColumn(column, col);
    }

    // Aktualizacja zmiennych
    last_col = column;

    // Sortowanie
    SortData *sd = new SortData(this, column, dir);
    bool result = this->SortItems((wxListCtrlCompare) DoSort, (wxIntPtr)sd);
    delete sd;

    // Przeindeksowanie
    if (result)
    {
        for (int i = 0; i < this->GetItemCount(); i++)
        {
            this->SetItemData(i, i);
        }
    }

    return result;
}

void MyListCtrl::GenerateSampleData(int cols, int rows, int default_icon, bool sort)
{
    if (cols == 0 || rows == 0) return;
    if (!this->InReportView()) return;

    this->ClearAll();

    // Ikona g³ównego elementu
    int icon = -1;
    if (this->GetImageList(wxIMAGE_LIST_SMALL)) icon = default_icon;

    // Kolumny
    for (int i = 0; i < cols; i++)
    {
        wxListItem tmp;
        tmp.SetText(wxString::Format(wxT("Kolumna %d"), i));
        this->InsertColumn(i, tmp);
        this->SetColumnWidth(i, 100);
    }

    // Wiersze
    for (int i = 0; i < rows; i++)
    {
        this->InsertItem(i, wxString::Format(wxT("Wiersz %d"), i), icon);
        for (int j = 1; j < cols; j++)
        {
            this->SetItem(i, j, wxString::Format(wxT("Dane %d,%d"), i, j), -1);
        }
    }

    if (sort)
    {
        // Indeksowanie
        for (int i = 0; i < this->GetItemCount(); i++)
        {
            this->SetItemData(i, i);
        }

        // Sortowanie
        this->Sort(0, SORT_ASCENDING);
    }
}

