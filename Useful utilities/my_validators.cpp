/////////////////////////////////////////////////////////////////////////////
// Name:        my_validators.cpp
// Purpose:     My wxWidgets app
// Author:      Imie Nazwisko <imie.nazwisko@email.org>
// Created:     2018-01-01
// Copyright:   (c) 2018 by Imie Nazwisko
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#include "my_validators.h"

// ----------------------------------------------------------------------------
// MyRegExValidator
// ----------------------------------------------------------------------------
wxBEGIN_EVENT_TABLE(MyRegExValidator, wxTextValidator)
	EVT_CHAR(MyRegExValidator::OnChar)
wxEND_EVENT_TABLE()

bool MyRegExValidator::TransferToWindow()
{
	if (m_stringValue == NULL) return true;
	else
	{
		wxTextEntry * const text = GetTextEntry();
		if (text)
		{
			text->SetValue(*m_stringValue);
			return true;
		}
		else return false;
	}
}

bool MyRegExValidator::TransferFromWindow(void)
{
	wxTextEntry *text = GetTextEntry();
	if (text && reg_ex.Matches(text->GetValue()))
	{
		if (m_stringValue != NULL) *m_stringValue = text->GetValue();
		return true;
	}
	else return false;
}

bool MyRegExValidator::IsValid(int key)
{
	bool result = false;
	
	wxTextEntry* text = GetTextEntry();
	if (!text) return result;

	wxString tmp = text->GetValue();
	long pos = text->GetInsertionPoint();

	tmp.insert(tmp.begin() + pos, (wxUniChar)key);
	result = reg_ex.Matches(tmp);
	
	return result;
}

void MyRegExValidator::OnChar(wxKeyEvent& event)
{
	event.Skip();

	int keyCode = event.GetUnicodeKey();
	if (keyCode == WXK_TAB || keyCode == WXK_RETURN ||
		keyCode == WXK_ESCAPE || keyCode == WXK_DELETE ||
		keyCode == WXK_BACK || keyCode == WXK_NONE) return;

	if (IsValid(keyCode)) return;

	if (!wxValidator::IsSilent()) wxBell();

	event.Skip(false);
}

// ----------------------------------------------------------------------------
// MyCustomValidator
// ----------------------------------------------------------------------------
wxBEGIN_EVENT_TABLE(MyCustomValidator, wxTextValidator)
	EVT_CHAR(MyCustomValidator::OnChar)
wxEND_EVENT_TABLE()

bool MyCustomValidator::TransferToWindow()
{
	if (m_stringValue == NULL) return true;
	else
	{
		wxTextEntry * const text = GetTextEntry();
		if (text)
		{
			text->SetValue(*m_stringValue);
			return true;
		}
		else return false;
	}
}

bool MyCustomValidator::TransferFromWindow()
{
	wxTextEntry *text = GetTextEntry();
	if (text && check_fn(text->GetValue()))
	{
		if (m_stringValue != NULL) *m_stringValue = text->GetValue();
		return true;
	}
	else return false;
}

bool MyCustomValidator::IsValid(int key)
{
	bool result = false;

	wxTextEntry* text = GetTextEntry();
	if (!text) return result;

	wxString tmp = text->GetValue();
	long pos = text->GetInsertionPoint();

	tmp.insert(tmp.begin() + pos, (wxUniChar)key);
	result = filter_fn(tmp);

	return result;
}

void MyCustomValidator::OnChar(wxKeyEvent& event)
{
	event.Skip();

	if (filter_fn == NULL) return;

	int keyCode = event.GetUnicodeKey();
	if (keyCode == WXK_TAB || keyCode == WXK_RETURN ||
		keyCode == WXK_ESCAPE || keyCode == WXK_DELETE ||
		keyCode == WXK_BACK || keyCode == WXK_NONE) return;

	if (IsValid(keyCode)) return;

	if (!wxValidator::IsSilent()) wxBell();

	event.Skip(false);
}

