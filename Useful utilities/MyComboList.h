/////////////////////////////////////////////////////////////////////////////
// Name:        MyComboList.h
// Purpose:     My wxWidgets app
// Author:      Imie Nazwisko <imie.nazwisko@email.org>
// Created:     2018-01-01
// Copyright:   (c) 2018 by Imie Nazwisko
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _MYCOMBOLIST_H_
#define _MYCOMBOLIST_H_

#include <wx/listctrl.h>
#include <wx/imaglist.h>
#include <wx/combo.h>

// #include "MyListCtrl.h" // Jeœli u¿ywamy naszej listy sortowanj...

class MyComboList : public wxListCtrl, public wxComboPopup
{
public:
	MyComboList(wxWindowID win_id): id(win_id), col(0) {}
	virtual ~MyComboList() {}

	// Funkcja obowi¹zkowa (wxComboPopup)
	virtual bool Create(wxWindow* parent)
	{
		return wxListCtrl::Create(parent, id, wxDefaultPosition, wxDefaultSize,
			wxLC_REPORT | wxLC_SINGLE_SEL | wxLC_HRULES | wxLC_VRULES);
	}

	// Funkcja obowi¹zkowa (wxComboPopup)
	virtual wxWindow *GetControl() { return this; }

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
	// Funkcja obowi¹zkowa (wxComboPopup)
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
