/////////////////////////////////////////////////////////////////////////////
// Name:        HFPrintEditor.cpp
// Purpose:     My wxWidgets app
// Author:      Name Surname <name.surname@email.org>
// Created:     2025-01-01
// Copyright:   (c) 2025 by Name Surname
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#include "HFPrintEditor.h"

enum
{
	ID_ADVANCED_PRINT_HF_LEFT = wxID_HIGHEST + 1001,
	ID_ADVANCED_PRINT_HF_CENTER,
	ID_ADVANCED_PRINT_HF_RIGHT,
};

BEGIN_EVENT_TABLE(HFPrintEditior, wxDialog)
	EVT_RADIOBOX(ID_ADVANCED_PRINT_HF_LEFT, HFPrintEditior::OnRadio)
	EVT_RADIOBOX(ID_ADVANCED_PRINT_HF_CENTER, HFPrintEditior::OnRadio)
	EVT_RADIOBOX(ID_ADVANCED_PRINT_HF_RIGHT, HFPrintEditior::OnRadio)
END_EVENT_TABLE()

HFPrintEditior::HFPrintEditior(wxWindow *parent, int type)
	: wxDialog(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxCAPTION | wxCLOSE_BOX)
{
	data_type = type;

	switch (type)
	{
		case HF_HEADER_TYPE: this->SetTitle(wxT("Header settings")); break;
		case HF_FOOTER_TYPE: this->SetTitle(wxT("Footer settings")); break;
	}

	wxArrayString choices;
	choices.Add(wxT("(None)"));						// 0
	choices.Add(wxT("Current date"));				// 1
	choices.Add(wxT("Current time"));				// 2
	choices.Add(wxT("Date and time"));				// 3
	choices.Add(wxT("Page number"));				// 4
	choices.Add(wxT("Number of pages"));			// 5
	choices.Add(wxT("Page number/number of pages"));// 6
	choices.Add(wxT("Title"));						// 7

	wxBoxSizer *MainSizer = new wxBoxSizer(wxVERTICAL);
	this->SetSizer(MainSizer);

	wxStaticBox *sb = new wxStaticBox(this, -1, wxT(" Settings "));
	wxStaticBoxSizer *sbs = new wxStaticBoxSizer(sb, wxVERTICAL);

	MainSizer->Add(sbs, 0, wxEXPAND | wxALL, 4);

	wxStaticBox *sb_title = new wxStaticBox(this, -1, wxT(" Title "));
	wxStaticBoxSizer * title_sbs = new wxStaticBoxSizer(sb_title, wxVERTICAL);

	left = new wxRadioBox(this, ID_ADVANCED_PRINT_HF_LEFT, wxT(" Left "),
		wxDefaultPosition, wxDefaultSize, choices, 8, wxRA_SPECIFY_ROWS);
	center = new wxRadioBox(this, ID_ADVANCED_PRINT_HF_CENTER, wxT(" Center "),
		wxDefaultPosition, wxDefaultSize, choices, 8, wxRA_SPECIFY_ROWS);
	right = new wxRadioBox(this, ID_ADVANCED_PRINT_HF_RIGHT, wxT(" Right "),
		wxDefaultPosition, wxDefaultSize, choices, 8, wxRA_SPECIFY_ROWS);

	doc_title = new wxTextCtrl(this, -1, wxEmptyString);
	doc_title->Disable();
	title_sbs->Add(doc_title, 1, wxEXPAND | wxALL, 4);

	wxBoxSizer *top = new wxBoxSizer(wxHORIZONTAL);
	top->Add(left, 0, wxEXPAND | wxALL, 4);
	top->Add(center, 0, wxEXPAND | wxALL, 4);
	top->Add(right, 0, wxEXPAND | wxALL, 4);

	sbs->Add(top, 0, wxEXPAND | wxALL, 4);
	sbs->Add(title_sbs, 0, wxEXPAND | wxALL, 4);

	// Buttons
	wxBoxSizer *btns = new wxBoxSizer(wxHORIZONTAL);
	MainSizer->Add(btns, 0, wxALIGN_RIGHT | wxALL, 4);

	wxButton *save = new wxButton(this, wxID_OK, wxT("Set"));
	wxButton *cancel = new wxButton(this, wxID_CANCEL, wxT("Cancel"));

	btns->Add(save, 0, wxALIGN_RIGHT | wxALL, 4);
	btns->Add(cancel, 0, wxEXPAND | wxALL, 4);

	this->Fit();
	this->CenterOnParent();
}

//--------------------------------------------------------------------------------------------------
// Handling radioBox clicks in header and footer settings dialogs
//--------------------------------------------------------------------------------------------------
void HFPrintEditior::OnRadio(wxCommandEvent& WXUNUSED(event))
{
	if (this->left->GetSelection() == 7 ||
		this->center->GetSelection() == 7 ||
		this->right->GetSelection() == 7)
	{
		doc_title->Enable(true);
	}
	else doc_title->Disable();
}

//--------------------------------------------------------------------------------------------------
// Load data into the printing engine
//--------------------------------------------------------------------------------------------------
void HFPrintEditior::SetPrintSettings(wxRichTextPrinting* prn)
{
	wxArrayString labels;
	labels.Add("");									// 0
	labels.Add("@DATE@");							// 1
	labels.Add("@TIME@");							// 2
	labels.Add("@DATE@ @TIME@");					// 3
	labels.Add("Page @PAGENUM@");					// 4
	labels.Add("Pages @PAGESCNT@");					// 5
	labels.Add("Page @PAGENUM@/@PAGESCNT@");		// 6
	labels.Add("@TITLE@");							// 7

	prn->SetTitle(doc_title->GetValue());

	if (data_type == HF_FOOTER_TYPE)
	{
		prn->SetFooterText(labels[left->GetSelection()], wxRICHTEXT_PAGE_ALL, wxRICHTEXT_PAGE_LEFT);
		prn->SetFooterText(labels[center->GetSelection()], wxRICHTEXT_PAGE_ALL, wxRICHTEXT_PAGE_CENTRE);
		prn->SetFooterText(labels[right->GetSelection()], wxRICHTEXT_PAGE_ALL, wxRICHTEXT_PAGE_RIGHT);
	}
	else
	{
		prn->SetHeaderText(labels[left->GetSelection()], wxRICHTEXT_PAGE_ALL, wxRICHTEXT_PAGE_LEFT);
		prn->SetHeaderText(labels[center->GetSelection()], wxRICHTEXT_PAGE_ALL, wxRICHTEXT_PAGE_CENTRE);
		prn->SetHeaderText(labels[right->GetSelection()], wxRICHTEXT_PAGE_ALL, wxRICHTEXT_PAGE_RIGHT);
	}
}