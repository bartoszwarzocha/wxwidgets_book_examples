/////////////////////////////////////////////////////////////////////////////
// Name:        main.cpp
// Purpose:     My wxWidgets app
// Author:      Name Surname <name.surname@email.org>
// Created:     2025-01-01
// Copyright:   (c) 2025 by Name Surname
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "main.h"

const wxString appName = wxT("Client test 1.0.0");

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
    ID_CONNECT,
    ID_DISCONNECT,
    ID_SEND
};

//---------------------------------------------------------------------------
// Event table
//---------------------------------------------------------------------------
BEGIN_EVENT_TABLE(MyFrame, wxFrame)
EVT_MENU(ID_CLOSE, MyFrame::OnClose)
EVT_MENU(ID_ABOUT_PROGRAM, MyFrame::OnAboutProgram)
EVT_CLOSE(MyFrame::CloseApp)
//
EVT_BUTTON(ID_CONNECT, MyFrame::OnConnect)
EVT_BUTTON(ID_DISCONNECT, MyFrame::OnDisconnect)
EVT_SOCKET(wxID_ANY, MyFrame::OnSocketEvent)
EVT_BUTTON(ID_SEND, MyFrame::OnSendMessage)
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
    mb = new wxMenuBar();

    mFile = new wxMenu();
    mHelp = new wxMenu();

    mFile->AppendSeparator();
    mFile->Append(ID_CLOSE, wxT("&Close program\tCtrl-X"), wxT("Closes the program"));

    mHelp->Append(ID_ABOUT_PROGRAM, wxT("&About program\tF1"), wxT("About the program"));

    mb->Append(mFile, wxT("&File"));
    mb->Append(mHelp, wxT("&Help"));

    this->SetMenuBar(mb);

    sb = new wxStatusBar(this);
    sb->SetFieldsCount(2);
    this->SetStatusBar(sb);
    SetStatusText(title, 0);

    wxBoxSizer* ms = new wxBoxSizer(wxVERTICAL);
    this->SetSizer(ms);

    panel = new wxPanel(this);
    wxBoxSizer* ps = new wxBoxSizer(wxVERTICAL);
    panel->SetSizer(ps);

    wxStaticText* address_label = new wxStaticText(panel, wxID_ANY, wxT("Server IP address:"));
    wxStaticText* port_label = new wxStaticText(panel, wxID_ANY, wxT("Port:"));

    address = new wxTextCtrl(panel, wxID_ANY, wxT("127.0.0.1"));
    port = new wxTextCtrl(panel, wxID_ANY, wxT("6000"));
    msg = new wxTextCtrl(panel, wxID_ANY, wxT("ABCDEFG"));

    conn_btn = new wxButton(panel, ID_CONNECT, wxT("Connect"));
    send_btn = new wxButton(panel, ID_SEND, wxT("Send"));

    log = new wxTextCtrl(panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY);
    wxLog::SetActiveTarget(new wxLogTextCtrl(log));

    //
    wxBoxSizer* row = new wxBoxSizer(wxHORIZONTAL);
    row->Add(address_label, 0, wxALIGN_CENTER_VERTICAL | wxALL, 4);
    row->Add(address, 0, wxALL, 4);
    row->AddSpacer(8);
    row->Add(port_label, 0, wxALIGN_CENTER_VERTICAL | wxALL, 4);
    row->Add(port, 0, wxALL, 4);
    row->AddSpacer(8);
    row->Add(conn_btn, 0, wxALL, 4);

    wxBoxSizer* row2 = new wxBoxSizer(wxHORIZONTAL);
    row2->Add(msg, 1, wxEXPAND | wxALL, 4);
    row2->Add(send_btn, 0, wxALL, 4);

    ps->Add(row, 0, wxEXPAND | wxALL, 0);
    ps->Add(log, 1, wxEXPAND | wxALL, 0);
    ps->Add(row2, 0, wxEXPAND | wxALL, 0);

    ms->Add(panel, 1, wxEXPAND | wxALL, 0);

    CenterOnScreen();

    CreateClient();
}

//---------------------------------------------------------------------------
// MyFrame destructor
//-------------------------------------------------------------------------
MyFrame::~MyFrame()
{
    wxDELETE(client);
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
void MyFrame::OnAboutProgram(wxCommandEvent& event)
{
    wxMessageBox(appName + wxT("\nCopyright (c) 2015 by ..."), wxT("About program..."), wxOK | wxICON_INFORMATION);
    event.Skip();
}

//---------------------------------------------------------------------------
// Function displays a question about closing the program
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
// Function creates a client object and assigns events to it
//---------------------------------------------------------------------------
void MyFrame::CreateClient()
{
    client = new wxSocketClient();

    client->SetEventHandler(*this);
    client->SetNotify(wxSOCKET_CONNECTION_FLAG | wxSOCKET_INPUT_FLAG | wxSOCKET_LOST_FLAG);
    client->Notify(true);
}

//---------------------------------------------------------------------------
// Function handles connection initiation
//---------------------------------------------------------------------------
void MyFrame::OnConnect(wxCommandEvent& WXUNUSED(event))
{
    if (address->GetValue().IsEmpty() || port->GetValue().IsEmpty())
    {
        wxLogWarning(wxT("Missing data to establish connection."));
    }

    wxIPV4address ip;
    ip.Hostname(address->GetValue());
    ip.Service(port->GetValue());

    client->Connect(ip, false);
}

//---------------------------------------------------------------------------
// Function closes the connection
//---------------------------------------------------------------------------
void MyFrame::OnDisconnect(wxCommandEvent& WXUNUSED(event))
{
    client->Close();
    conn_btn->SetId(ID_CONNECT);
    conn_btn->SetLabel(wxT("Connect"));
    wxLogWarning(wxT("Disconnected..."));
}

//---------------------------------------------------------------------------
// Function handles socket events
//---------------------------------------------------------------------------
void MyFrame::OnSocketEvent(wxSocketEvent& event)
{
    char buffer[4096];
    memset(buffer, 0x00, sizeof(buffer));
    wxString rcv_msg = wxEmptyString;

    wxIPV4address ip;
    client->GetPeer(ip);

    switch (event.GetSocketEvent())
    {
    case wxSOCKET_CONNECTION:
        wxLogMessage(wxT("Connected to %s:%d"), ip.IPAddress(), ip.Service());
        //
        conn_btn->SetId(ID_DISCONNECT);
        conn_btn->SetLabel(wxT("Disconnect"));
        break;

    case wxSOCKET_INPUT:
        wxLogMessage(wxT("Received data from server %s:%d"), ip.IPAddress(), ip.Service());
        if (client->IsData())
        {
            client->ReadMsg(buffer, sizeof(buffer));
        }
        wxLogMessage(wxString::Format(wxT("Server response: %s"), buffer));
        break;

    case wxSOCKET_LOST:
        conn_btn->SetId(ID_CONNECT);
        conn_btn->SetLabel(wxT("Connect"));
        wxLogWarning(wxT("Connection to %s:%d lost!"), ip.IPAddress(), ip.Service());
        break;

    default:
        wxLogMessage("Unknown event!");
        break;
    }
}

//---------------------------------------------------------------------------
// Function sends data to the server
//---------------------------------------------------------------------------
void MyFrame::OnSendMessage(wxCommandEvent& WXUNUSED(event))
{
    if (client->IsDisconnected())
    {
        wxLogWarning(wxT("Client not connected!"));
        return;
    }

    client->SetFlags(wxSOCKET_WAITALL);

    wxLogMessage(wxString::Format(wxT("Sending data (characters %d)"), msg->GetValue().Len()));

    client->Write(msg->GetValue(), msg->GetValue().Len());

    if (!client->WaitForRead(3))
    {
        wxLogWarning(wxT("Timeout!"));
    }
}
