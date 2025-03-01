/////////////////////////////////////////////////////////////////////////////
// Name:        main.cpp
// Purpose:     My wxWidgets app
// Author:      Name Surname <name.surname@email.org>
// Created:     2025-01-01
// Copyright:   (c) 2025 by Name Surname
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Note: This file was originally written in Polish, hence the database
// queries are in Polish. Below are examples of how to understand the
// database requests contained in the strings.
//
// Example 1:
// "SELECT COUNT(*) FROM TOWARY"
// This query counts the number of records in the "TOWARY" (items) table.
//
// Example 2:
// "SELECT ID, CENA FROM TOWARY WHERE NAZWA='item_name'"
// This query selects the ID and price (CENA) of an item from the "TOWARY"
// table where the name (NAZWA) of the item matches 'item_name'.
/////////////////////////////////////////////////////////////////////////////


#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

// SOCI
#include <soci/soci.h>
#include <soci/firebird/soci-firebird.h>

#include <wx/fileconf.h>

#include "main.h"

const wxString appName = wxT("Shop 1.0.0 (SQL + Firebird + SOCI)");

//---------------------------------------------------------------------------
// Application implementation
//---------------------------------------------------------------------------
IMPLEMENT_APP(MyApp)

//---------------------------------------------------------------------------
// Identifiers
//---------------------------------------------------------------------------
enum
{
    ID_CLOSE = wxID_HIGHEST + 1,
    ID_ABOUT_PROGRAM,
    //
    ID_ADD_STUFF,
    ID_ADD_CUSTOMER,
    ID_ADD_ORDER,
    ID_CLOSE_ORDER,
    ID_QUERY
};

//---------------------------------------------------------------------------
// Event table
//---------------------------------------------------------------------------
BEGIN_EVENT_TABLE(MyFrame, wxFrame)
EVT_MENU(ID_CLOSE, MyFrame::OnClose)
EVT_MENU(ID_ABOUT_PROGRAM, MyFrame::OnAboutProgram)
EVT_CLOSE(MyFrame::CloseApp)
//
EVT_MENU(ID_ADD_STUFF, MyFrame::OnAddStuff)
EVT_MENU(ID_ADD_CUSTOMER, MyFrame::OnAddCustomer)
EVT_MENU(ID_QUERY, MyFrame::OnQuery)
EVT_MENU(ID_ADD_ORDER, MyFrame::OnAddOrder)
EVT_MENU(ID_CLOSE_ORDER, MyFrame::OnCloseOrder)
END_EVENT_TABLE()

//---------------------------------------------------------------------------
// OnInit()
//---------------------------------------------------------------------------
bool MyApp::OnInit()
{
    wxInitAllImageHandlers();

    MyFrame* frame = new MyFrame(NULL, wxID_ANY, appName, wxDefaultPosition, wxSize(600, 400), wxDEFAULT_FRAME_STYLE);
    frame->Show(true);
    return true;
}

//---------------------------------------------------------------------------
// MyFrame constructor
//---------------------------------------------------------------------------
MyFrame::MyFrame(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
    : wxFrame(parent, id, title, pos, size, style)
{
    // Load configuration
    wxFileConfig config(wxEmptyString, wxEmptyString, wxT("db.conf"), wxEmptyString, wxCONFIG_USE_RELATIVE_PATH);

    config.Read("DB/File", &this->db_file, "SHOP.FDB");
    config.Read("DB/User", &this->db_user, "SYSDBA");
    config.Read("DB/Pass", &this->db_password, "masterkey");
    config.Read("DB/Char", &this->db_charset, "WIN1250");
    //

    wxMenuBar* mb = new wxMenuBar();

    wxMenu* mFile = new wxMenu();
    wxMenu* mBase = new wxMenu();
    wxMenu* mOrder = new wxMenu();
    wxMenu* mHelp = new wxMenu();


    mFile->AppendSeparator();
    mFile->Append(ID_CLOSE, wxT("&Close program\tCtrl-X"), wxT("Closes the program"));

    mBase->Append(ID_ADD_STUFF, wxT("Add stuff"), wxT("Adds stuff to the database"));
    mBase->Append(ID_ADD_CUSTOMER, wxT("Add customer"), wxT("Adds customer to the database"));
    mBase->AppendSeparator();
    mBase->Append(ID_QUERY, wxT("Custom query"), wxT("Custom query"));

    mOrder->Append(ID_ADD_ORDER, wxT("New order"), wxT("New order"));
    mOrder->Append(ID_CLOSE_ORDER, wxT("Send and close"), wxT("Closes the order"));

    mHelp->Append(ID_ABOUT_PROGRAM, wxT("&About program\tF1"), wxT("About the program"));

    mb->Append(mFile, wxT("&File"));
    mb->Append(mBase, wxT("&Database"));
    mb->Append(mOrder, wxT("&Orders"));
    mb->Append(mHelp, wxT("&Help"));

    this->SetMenuBar(mb);

    sb = new wxStatusBar(this);
    sb->SetFieldsCount(2);
    this->SetStatusBar(sb);
    SetStatusText(title, 0);

    //
    wxBoxSizer* ms = new wxBoxSizer(wxVERTICAL);
    this->SetSizer(ms);

    panel = new wxPanel(this);
    wxBoxSizer* ps = new wxBoxSizer(wxVERTICAL);
    panel->SetSizer(ps);

    //
    notebook = new wxNotebook(panel, wxID_ANY, wxDefaultPosition, wxSize(600, 400));

    new_orders = new wxListCtrl(notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT);
    new_orders->AppendColumn(wxT("ID"));
    new_orders->AppendColumn(wxT("Date"));
    new_orders->AppendColumn(wxT("Value"));
    new_orders->AppendColumn(wxT("Customer"), wxLIST_FORMAT_LEFT, 250);

    closed_orders = new wxListCtrl(notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT);
    closed_orders->AppendColumn(wxT("ID"));
    closed_orders->AppendColumn(wxT("Date"));
    closed_orders->AppendColumn(wxT("Value"));
    closed_orders->AppendColumn(wxT("Customer"), wxLIST_FORMAT_LEFT, 250);

    notebook->AddPage(new_orders, wxT("New orders"), true);
    notebook->AddPage(closed_orders, wxT("Closed orders"), false);

    ps->Add(notebook, 1, wxEXPAND | wxALL, 0);
    ms->Add(panel, 1, wxEXPAND | wxALL, 0);

    Fit();
    CenterOnScreen();

    // DATABASE
    if (!DBConnect()) return;

    // Display information about the database content
    ReadCounters(true);

    // Fetch information about recent orders
    // Max 250 new and last 100 closed
    LoadOrders(250);
    LoadOrders(100, true);

    DBDisconnect();
}

//---------------------------------------------------------------------------
// MyFrame destructor
//---------------------------------------------------------------------------
MyFrame::~MyFrame()
{

}

//---------------------------------------------------------------------------
// Function initiates program closure
//---------------------------------------------------------------------------
void MyFrame::OnClose(wxCommandEvent& WXUNUSED(event))
{
    this->Close(true);
}

//---------------------------------------------------------------------------
// Function displays "About program..." window
//---------------------------------------------------------------------------
void MyFrame::OnAboutProgram(wxCommandEvent& WXUNUSED(event))
{
    wxMessageBox(appName + wxT("\nCopyright (c) 2025 by ..."), wxT("About program..."), wxOK | wxICON_INFORMATION);
}

//---------------------------------------------------------------------------
// Function displays a prompt to close the program
//---------------------------------------------------------------------------
void MyFrame::CloseApp(wxCloseEvent& WXUNUSED(event))
{
    int action = wxMessageBox(wxT("Do you want to close the program?"), wxT("Question"), wxOK | wxCANCEL | wxICON_QUESTION);

    if (action == wxOK)
    {
        this->Destroy();
    }
    else
    {
        return;
    }
}

//---------------------------------------------------------------------------
// Function connects to the database
//---------------------------------------------------------------------------
bool MyFrame::DBConnect()
{
    bool is_ok = true;

    try
    {
        wxString db_conf = wxString::Format("service=%s user=%s password=%s charset=%s", db_file, db_user, db_password, db_charset);
        sql.open(soci::firebird, db_conf.ToStdString());
    }
    catch (std::exception const& e)
    {
        wxString s;
        wxMessageBox(s << "Error: " << e.what() << '\n');
        is_ok = false;
    }

    return is_ok;
}

//---------------------------------------------------------------------------
// Function disconnects from the database
//---------------------------------------------------------------------------
void MyFrame::DBDisconnect()
{
    try
    {
        sql.close();
    }
    catch (std::exception const& e)
    {
        wxString s;
        wxMessageBox(s << "Error: " << e.what() << '\n');
    }
}

//---------------------------------------------------------------------------
// Function retrieves information about the number of records in the database
//---------------------------------------------------------------------------
void MyFrame::ReadCounters(bool show_msg)
{
    try
    {
        int stuff_counter;
        int customer_counter;
        int new_order_counter;
        int closed_order_counter;

        sql << "SELECT COUNT(*) FROM STUFF", soci::into(stuff_counter);
        sql << "SELECT COUNT(*) FROM CUSTOMERS", soci::into(customer_counter);
        sql << "SELECT COUNT(*) FROM ORDERS WHERE STATUS=0", soci::into(new_order_counter);
        sql << "SELECT COUNT(*) FROM ORDERS WHERE STATUS=1", soci::into(closed_order_counter);

        wxString msg = wxString::Format(wxT("Stuff: %d, Customers: %d, Orders: %d/%d"), stuff_counter, customer_counter, new_order_counter, closed_order_counter);

        SetStatusText(msg, 1);

        if (show_msg)
        {
            wxMessageBox(msg);
        }
    }
    catch (std::exception const& e)
    {
        wxString s;
        wxMessageBox(s << "Error: " << e.what() << '\n');
    }
}

//---------------------------------------------------------------------------
// Function loads orders into the order lists
//---------------------------------------------------------------------------
void MyFrame::LoadOrders(unsigned int count, bool closed)
{
    wxListCtrl* list = (closed) ? closed_orders : new_orders;

    list->DeleteAllItems();

    std::vector<int> id(count);
    std::vector<std::tm> date(count);
    std::vector<double> amount(count);
    std::vector<std::string> sname(count);
    std::vector<std::string> name(count);

    std::string query = "SELECT a.ID, a.DATE, a.PRICE, b.NAME, b.SURNAME FROM ORDERS AS a, CUSTOMERS AS b WHERE b.ID=a.CUSTOMER_ID AND a.STATUS=";
    query += (closed) ? "1" : "0";

    try
    {
        sql << query, soci::into(id), soci::into(date), soci::into(amount), soci::into(name), soci::into(sname);
    }
    catch (std::exception const& e)
    {
        wxString s;
        wxMessageBox(s << "Error: " << e.what() << '\n');
    }

    for (unsigned int i = 0; i < id.size(); i++)
    {
        list->InsertItem(i, wxString::Format("%d", id[i]));
        list->SetItem(i, 1, wxDateTime(date[i]).FormatISODate());
        list->SetItem(i, 2, wxString::Format("%4.2f", amount[i]));
        list->SetItem(i, 3, wxString::Format("%s %s", name[i], sname[i]));
    }
}

//---------------------------------------------------------------------------
// Function handles adding stuff
//---------------------------------------------------------------------------
void MyFrame::OnAddStuff(wxCommandEvent& WXUNUSED(event))
{
    if (!DBConnect()) return;

    try
    {
        StuffDialog dlg(this);
        if (dlg.ShowModal() == wxID_OK)
        {
            // Check if the item with the given catalog number already exists
            int id = -1;
            sql << "SELECT ID FROM STUFF WHERE CAT_NUMBER=:nr", soci::into(id), soci::use(dlg.nr->GetValue());
            if (id > 0)
            {
                wxMessageBox(wxT("An item with the given catalog number already exists! Try again."));
                return;
            }

            // Save the new item
            sql << "INSERT INTO STUFF (NAME, PRICE, CAT_NUMBER) VALUES (:name, :price, :nr)",
                soci::use(dlg.name->GetValue().ToStdString()),
                soci::use(dlg.price->GetValue()),
                soci::use(dlg.nr->GetValue());

            ReadCounters();
        }
    }
    catch (std::exception const& e)
    {
        wxString s;
        wxMessageBox(s << "Error: " << e.what() << '\n');
    }

    DBDisconnect();
}

//---------------------------------------------------------------------------
// Function handles adding a customer
//---------------------------------------------------------------------------
void MyFrame::OnAddCustomer(wxCommandEvent& WXUNUSED(event))
{
    if (!DBConnect()) return;

    try
    {
        CustomerDialog dlg(this);
        if (dlg.ShowModal() == wxID_OK)
        {
            // Save the new customer
            sql << "INSERT INTO CUSTOMERS (SURNAME, NAME, ADDRESS, EMAIL) VALUES (:sname, :name, :address, :email)",
                soci::use(dlg.sname->GetValue().ToStdString()),
                soci::use(dlg.name->GetValue().ToStdString()),
                soci::use(dlg.address->GetValue().ToStdString()),
                soci::use(dlg.email->GetValue().ToStdString());

            ReadCounters();
        }
    }
    catch (std::exception const& e)
    {
        wxString s;
        wxMessageBox(s << "Error: " << e.what() << '\n');
    }

    DBDisconnect();
}

//---------------------------------------------------------------------------
// Function handles adding an order
//---------------------------------------------------------------------------
void MyFrame::OnAddOrder(wxCommandEvent& WXUNUSED(event))
{
    if (!DBConnect()) return;

    // Dialog window
    OrderDialog dlg(this);

    std::string tmp, tmp2;

    try
    {
        // Read the list of items
        soci::statement st = (sql.prepare << "SELECT NAME FROM STUFF ORDER BY NAME", soci::into(tmp));
        st.execute();
        while (st.fetch())
        {
            dlg.stuff->Append(wxString(tmp));
        }
        dlg.stuff->SetSelection(0);

        // Read the list of customers
        st = (sql.prepare << "SELECT NAME, SURNAME FROM CUSTOMERS ORDER BY SURNAME", soci::into(tmp), soci::into(tmp2));
        st.execute();
        while (st.fetch())
        {
            dlg.customer->Append(wxString(tmp) + " " + wxString(tmp2));
        }
        dlg.customer->SetSelection(0);

        // Display the window
        if (dlg.ShowModal() == wxID_OK)
        {
            // Find the item ID and price
            int stuff_id = 0;
            double price = 0;
            wxString query = wxString::Format(wxT("SELECT ID, PRICE FROM STUFF WHERE NAME='%s'"), dlg.stuff->GetStringSelection());
            st = (sql.prepare << query.ToStdString(), soci::into(stuff_id), soci::into(price));
            st.execute();
            st.fetch();

            // Find the customer ID
            int customer_id = 0;
            wxString customer = dlg.customer->GetStringSelection();
            wxString name = customer.SubString(0, customer.Find(" "));
            wxString sname = customer.SubString(customer.Find(" ") + 1, customer.Len());
            query = wxString::Format(wxT("SELECT ID FROM CUSTOMERS WHERE NAME='%s' AND SURNAME ='%s'"), name, sname);
            st = (sql.prepare << query.ToStdString(), soci::into(customer_id));
            st.execute();
            st.fetch();


            // Save the order
            query = wxString::Format(wxT("INSERT INTO STUFF (DATE, CUSTOMER_ID, STUFF_ID, QUANTITY, VALUE, STATUS) VALUES ('%s', %d, %d, %d, %f, 0)"),
                wxDateTime::Now().FormatISODate(), customer_id, stuff_id, dlg.count->GetValue(),
                dlg.count->GetValue() * price);

            st = (sql.prepare << query.ToStdString());
            st.execute();

            LoadOrders(250);
            ReadCounters();
        }

    }
    catch (std::exception const& e)
    {
        wxString s;
        wxMessageBox(s << "Error: " << e.what() << '\n');
    }

    DBDisconnect();
}

//---------------------------------------------------------------------------
// Function handles closing an order
//---------------------------------------------------------------------------
void MyFrame::OnCloseOrder(wxCommandEvent& WXUNUSED(event))
{
    // Get the index of the selected item
    int item_id = new_orders->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
    if (item_id == -1) return; // Check if the item was found

    if (!DBConnect()) return;

    try
    {
        wxString id = new_orders->GetItemText(item_id, 0);

        sql << "UPDATE ORDERS SET STATUS=1 WHERE ID=:id", soci::use(id.ToStdString());

        LoadOrders(250);
        LoadOrders(100, true);
        ReadCounters();
    }
    catch (std::exception const& e)
    {
        wxString s;
        wxMessageBox(s << "Error: " << e.what() << '\n');
    }

    DBDisconnect();
}

//---------------------------------------------------------------------------
// Function handles sending a custom query
//---------------------------------------------------------------------------
void MyFrame::OnQuery(wxCommandEvent& WXUNUSED(event))
{
    wxString query = wxGetTextFromUser(wxT("Enter query..."), wxT("Custom query"));
    if (query.IsEmpty()) return;

    if (!DBConnect()) return;

    try
    {
        sql << query.ToStdString();

        // In case the query affects current data...
        LoadOrders(250);
        LoadOrders(100, true);
        ReadCounters();
    }
    catch (std::exception const& e)
    {
        wxString s;
        wxMessageBox(s << "Error: " << e.what() << '\n');
    }

    DBDisconnect();
}
