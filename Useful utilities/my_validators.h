/////////////////////////////////////////////////////////////////////////////
// Name:        my_validators.h
// Purpose:     My wxWidgets app
// Author:      Imie Nazwisko <imie.nazwisko@email.org>
// Created:     2018-01-01
// Copyright:   (c) 2018 by Imie Nazwisko
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _MY_VALIDATORS_H_
#define _MY_VALIDATORS_H_

#include <wx/valtext.h>
#include <wx/regex.h>
#include <wx/textctrl.h>

// ----------------------------------------------------------------------------
// MyRegExValidator
// ----------------------------------------------------------------------------
class MyRegExValidator : public wxTextValidator
{
public:
	MyRegExValidator(wxString format, wxString* ptr = NULL) : reg_ex_format(format), reg_ex(format, wxRE_EXTENDED | wxRE_ADVANCED)
	{
		m_stringValue = ptr;
	}

	MyRegExValidator(const MyRegExValidator& val): reg_ex(val.reg_ex_format)
	{
		wxValidator::Copy(val);
		m_stringValue = val.m_stringValue;
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
// MyCustomValidator
// ----------------------------------------------------------------------------
class MyCustomValidator : public wxTextValidator
{
public:
	MyCustomValidator(bool(*cfn)(const wxString& str), bool(*ffn)(const wxString& str) = NULL, wxString* ptr = NULL): check_fn(cfn), filter_fn(ffn)
	{
		m_stringValue = ptr;
	}

	MyCustomValidator(const MyCustomValidator& val)
	{
		wxValidator::Copy(val);
		m_stringValue = val.m_stringValue;
		check_fn = val.check_fn;
		filter_fn = val.filter_fn;
	}

	virtual ~MyCustomValidator() {}

	virtual wxObject* Clone() const wxOVERRIDE { return new MyCustomValidator(*this); }

	virtual bool TransferToWindow() wxOVERRIDE;
	virtual bool TransferFromWindow() wxOVERRIDE;

	void OnChar(wxKeyEvent& event);

private:
	bool IsValid(int key);

	bool(*check_fn)(const wxString& str);
	bool(*filter_fn)(const wxString& str);

	DECLARE_EVENT_TABLE()
};



#endif