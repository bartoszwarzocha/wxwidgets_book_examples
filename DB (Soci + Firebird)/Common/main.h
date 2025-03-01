/////////////////////////////////////////////////////////////////////////////
// Name:        main.h
// Purpose:     My wxWidgets app
// Author:      Name Surname <name.surname@email.org>
// Created:     2025-01-01
// Copyright:   (c) 2025 by Name Surname
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _MAIN_H_
#define _MAIN_H_

#include <wx/valtext.h>
#include <wx/spinctrl.h>
#include <wx/notebook.h>
#include <wx/listctrl.h>

//---------------------------------------------------------------------------
// Class MyApp
//---------------------------------------------------------------------------
class MyApp : public wxApp
{
public:
	virtual bool OnInit();
};

//---------------------------------------------------------------------------
// Class MyFrame
//---------------------------------------------------------------------------
class MyFrame : public wxFrame
{
public:
	MyFrame(wxWindow* parent, wxWindowID id, const wxString& title,
		const wxPoint& pos, const wxSize& size, long style);
	~MyFrame();

	void OnAboutProgram(wxCommandEvent& event);
	void OnClose(wxCommandEvent& event);
	void CloseApp(wxCloseEvent& event);

	void OnAddStuff(wxCommandEvent& event);
	void OnAddCustomer(wxCommandEvent& event);
	void OnAddOrder(wxCommandEvent& event);
	void OnCloseOrder(wxCommandEvent& event);
	void OnQuery(wxCommandEvent& event);

	void ReadCounters(bool show_msg = false);
	void LoadOrders(unsigned int count, bool closed = false);

	bool DBConnect();
	void DBDisconnect();

private:
	wxStatusBar* sb;

	wxPanel* panel;

	wxNotebook* notebook;

	wxListCtrl* new_orders;
	wxListCtrl* closed_orders;

	soci::session sql;

	//
	wxString db_file;
	wxString db_user;
	wxString db_password;
	wxString db_charset;

	DECLARE_EVENT_TABLE()
};

//---------------------------------------------------------------------------
// Class StuffDialog
//---------------------------------------------------------------------------
class StuffDialog : public wxDialog
{
public:
	StuffDialog(wxWindow* parent)
		: wxDialog(parent, wxID_ANY, wxT("New stuff"))
	{
		wxBoxSizer* ms = new wxBoxSizer(wxVERTICAL);
		this->SetSizer(ms);

		name = new wxTextCtrl(this, wxID_ANY);
		name->SetValidator(wxTextValidator(wxFILTER_EMPTY));

		price = new wxSpinCtrlDouble(this, wxID_ANY, wxT("100"));
		price->SetRange(0.01, 999.99);
		price->SetDigits(2);

		nr = new wxSpinCtrl(this, wxID_ANY, wxT("0"));
		nr->SetRange(1, 100000);

		//
		wxSizer* name_s = this->CreateTextSizer(wxT("Name"));
		name_s->Add(name, 1, wxEXPAND | wxLEFT | wxBOTTOM | wxRIGHT, 4);

		wxSizer* price_s = this->CreateTextSizer(wxT("Price"));
		price_s->Add(price, 0, wxEXPAND | wxLEFT | wxBOTTOM | wxRIGHT, 4);

		wxSizer* nr_s = this->CreateTextSizer(wxT("Catalog number"));
		nr_s->Add(nr, 0, wxEXPAND | wxLEFT | wxBOTTOM | wxRIGHT, 4);

		ms->Add(name_s, 0, wxEXPAND | wxALL, 4);
		ms->Add(price_s, 0, wxEXPAND | wxALL, 4);
		ms->Add(nr_s, 0, wxEXPAND | wxALL, 4);

		ms->Add(CreateSeparatedButtonSizer(wxOK | wxCANCEL), 0, wxEXPAND | wxALL, 8);
		this->FindWindowById(wxID_OK)->SetLabel(wxT("Save"));

		Fit();
		CenterOnParent();
	}

	virtual ~StuffDialog() {}

	wxTextCtrl* name; // Name
	wxSpinCtrlDouble* price; // Price
	wxSpinCtrl* nr; // Catalog number
};

//---------------------------------------------------------------------------
// Class CustomerDialog
//---------------------------------------------------------------------------
class CustomerDialog : public wxDialog
{
public:
	CustomerDialog(wxWindow* parent)
		: wxDialog(parent, wxID_ANY, wxT("New customer"))
	{
		wxBoxSizer* ms = new wxBoxSizer(wxVERTICAL);
		this->SetSizer(ms);

		sname = new wxTextCtrl(this, wxID_ANY);
		sname->SetValidator(wxTextValidator(wxFILTER_EMPTY));

		name = new wxTextCtrl(this, wxID_ANY);
		name->SetValidator(wxTextValidator(wxFILTER_EMPTY));

		address = new wxTextCtrl(this, wxID_ANY);
		address->SetValidator(wxTextValidator(wxFILTER_EMPTY));

		email = new wxTextCtrl(this, wxID_ANY);
		email->SetValidator(wxTextValidator(wxFILTER_EMPTY));

		//
		wxSizer* sname_s = this->CreateTextSizer(wxT("Surname"));
		sname_s->Add(sname, 1, wxEXPAND | wxLEFT | wxBOTTOM | wxRIGHT, 4);

		wxSizer* name_s = this->CreateTextSizer(wxT("First name"));
		name_s->Add(name, 1, wxEXPAND | wxLEFT | wxBOTTOM | wxRIGHT, 4);

		wxSizer* address_s = this->CreateTextSizer(wxT("Address"));
		address_s->Add(address, 1, wxEXPAND | wxLEFT | wxBOTTOM | wxRIGHT, 4);

		wxSizer* email_s = this->CreateTextSizer(wxT("E-mail"));
		email_s->Add(email, 1, wxEXPAND | wxLEFT | wxBOTTOM | wxRIGHT, 4);

		ms->Add(sname_s, 0, wxEXPAND | wxALL, 4);
		ms->Add(name_s, 0, wxEXPAND | wxALL, 4);
		ms->Add(address_s, 0, wxEXPAND | wxALL, 4);
		ms->Add(email_s, 0, wxEXPAND | wxALL, 4);

		ms->Add(CreateSeparatedButtonSizer(wxOK | wxCANCEL), 0, wxEXPAND | wxALL, 8);
		this->FindWindowById(wxID_OK)->SetLabel(wxT("Save"));

		Fit();
		CenterOnParent();
	}

	virtual ~CustomerDialog() {}

	wxTextCtrl* sname; // Surname
	wxTextCtrl* name; // First name
	wxTextCtrl* address; // Address
	wxTextCtrl* email; // E-mail
};

//---------------------------------------------------------------------------
// Class OrderDialog
//---------------------------------------------------------------------------
class OrderDialog : public wxDialog
{
public:
	OrderDialog(wxWindow* parent)
		: wxDialog(parent, wxID_ANY, wxT("New order"))
	{
		wxBoxSizer* ms = new wxBoxSizer(wxVERTICAL);
		this->SetSizer(ms);

		stuff = new wxComboBox(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, 0, wxCB_READONLY);

		customer = new wxComboBox(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, 0, wxCB_READONLY);

		count = new wxSpinCtrl(this, wxID_ANY, wxT("1"));
		count->SetRange(1, 1000);

		//
		wxSizer* stuff_s = this->CreateTextSizer(wxT("Stuff"));
		stuff_s->Add(stuff, 1, wxEXPAND | wxLEFT | wxBOTTOM | wxRIGHT, 4);

		wxSizer* customer_s = this->CreateTextSizer(wxT("Customer"));
		customer_s->Add(customer, 0, wxEXPAND | wxLEFT | wxBOTTOM | wxRIGHT, 4);

		wxSizer* count_s = this->CreateTextSizer(wxT("Quantity"));
		count_s->Add(count, 0, wxEXPAND | wxLEFT | wxBOTTOM | wxRIGHT, 4);

		ms->Add(stuff_s, 0, wxEXPAND | wxALL, 4);
		ms->Add(count_s, 0, wxEXPAND | wxALL, 4);
		ms->Add(customer_s, 0, wxEXPAND | wxALL, 4);

		ms->Add(CreateSeparatedButtonSizer(wxOK | wxCANCEL), 0, wxEXPAND | wxALL, 8);
		this->FindWindowById(wxID_OK)->SetLabel(wxT("Save"));

		Fit();
		CenterOnParent();
	}

	virtual ~OrderDialog() {}

	wxComboBox* stuff; // Stuff
	wxComboBox* customer; // Customer
	wxSpinCtrl* count; // Quantity
};

#endif
