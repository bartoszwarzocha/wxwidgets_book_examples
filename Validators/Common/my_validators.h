/////////////////////////////////////////////////////////////////////////////
// Name:        my_validators.h
// Purpose:     My wxWidgets app
// Author:      Name Surname <name.surname@email.org>
// Created:     2025-01-01
// Copyright:   (c) 2025 by Name Surname
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _MY_VALIDATORS_H_
#define _MY_VALIDATORS_H_

#include <wx/valtext.h>
#include <wx/regex.h>
#include <wx/textctrl.h>
#include <wx/defs.h>

// ----------------------------------------------------------------------------
// MyRegExValidator
// ----------------------------------------------------------------------------
class MyRegExValidator : public wxTextValidator
{
public:
	MyRegExValidator(wxString format, wxString* ptr = NULL) : reg_ex(format, wxRE_EXTENDED | wxRE_ADVANCED)
	{
		reg_ex_format = format;
		m_stringValue = ptr;
	}

	MyRegExValidator(const MyRegExValidator& val): reg_ex(val.reg_ex_format)
	{
		wxValidator::Copy(val);
		m_stringValue = val.m_stringValue;
		reg_ex_format = val.reg_ex_format;
	}

	virtual ~MyRegExValidator() {}

	virtual wxObject* Clone() const wxOVERRIDE { return new MyRegExValidator(*this); }

	virtual bool TransferToWindow() wxOVERRIDE;
	virtual bool TransferFromWindow() wxOVERRIDE;

	void OnChar(wxKeyEvent& event);

private:
	bool IsValid(int key);

	wxRegEx reg_ex;
	wxString reg_ex_format;

	DECLARE_EVENT_TABLE()
};

// ----------------------------------------------------------------------------
// MyUniversalValidator
// ----------------------------------------------------------------------------
template<typename T> class MyUniversalValidator : public wxTextValidator
{
public:
	MyUniversalValidator(T(*cfn)(const wxString& str), bool(*ffn)(const wxString& str) = NULL, wxString* ptr = NULL) : check_fn(cfn), filter_fn(ffn)
	{
		m_stringValue = ptr;
	}

	MyUniversalValidator(const MyUniversalValidator& val)
	{
		wxValidator::Copy(val);
		m_stringValue = val.m_stringValue;
		check_fn = val.check_fn;
		filter_fn = val.filter_fn;
	}

	virtual ~MyUniversalValidator() {}

	virtual wxObject* Clone() const wxOVERRIDE { return new MyUniversalValidator(*this); }

	virtual bool TransferToWindow() wxOVERRIDE
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

	virtual bool TransferFromWindow() wxOVERRIDE
	{
		wxTextEntry *text = GetTextEntry();
		if (text && check_fn(text->GetValue()))
		{
			if (m_stringValue != NULL) *m_stringValue = text->GetValue();
			return true;
		}
		else return false;
	}

	virtual void OnChar(wxKeyEvent& event)
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

private:
	virtual bool IsValid(int key)
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

	T(*check_fn)(const wxString& str);
	bool(*filter_fn)(const wxString& str);

	DECLARE_EVENT_TABLE()
};

wxBEGIN_EVENT_TABLE_TEMPLATE1(MyUniversalValidator, wxTextValidator, T)
	EVT_CHAR(MyUniversalValidator::OnChar)
wxEND_EVENT_TABLE()



#endif
