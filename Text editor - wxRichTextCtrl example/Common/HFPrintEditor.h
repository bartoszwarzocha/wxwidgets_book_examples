////////////////////////////////////////////////////////////////////////////////////
// Name:        HFPrintEditor.h
// Purpose:     My wxWidgets app
// Author:      Name Surname <name.surname@email.org>
// Created:     2025-01-01
// Copyright:   (c) 2025 by Name Surname
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _HFPRINTEDITOR_H_
#define _HFPRINTEDITOR_H_

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

#include <wx/dialog.h>
#include <wx/richtext/richtextprint.h>

enum
{
	HF_HEADER_TYPE = 1,
	HF_FOOTER_TYPE = 2
};

//--------------------------------------------------------------------------------------------------
// Class HFPrintEditior
//--------------------------------------------------------------------------------------------------
class HFPrintEditior : public wxDialog
{
public:
	HFPrintEditior(wxWindow* parent, int type = HF_HEADER_TYPE);

	void OnRadio(wxCommandEvent& event);

	void SetPrintSettings(wxRichTextPrinting* prn);

private:
	wxRadioBox *left, *center, *right;
	wxTextCtrl *doc_title;
	int data_type;

private:
	DECLARE_EVENT_TABLE()
};

#endif