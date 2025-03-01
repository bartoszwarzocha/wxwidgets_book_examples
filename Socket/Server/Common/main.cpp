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

#include <wx/thread.h>
#include <wx/socket.h>
#include <wx/cmdline.h>

#include "main.h"

const wxString appName = wxT("Server test 1.0.0");

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
};

//---------------------------------------------------------------------------
// Event table
//---------------------------------------------------------------------------
BEGIN_EVENT_TABLE(MyFrame, wxFrame)
EVT_MENU(ID_CLOSE, MyFrame::OnClose)
EVT_MENU(ID_ABOUT_PROGRAM, MyFrame::OnAboutProgram)
EVT_CLOSE(MyFrame::CloseApp)
//
EVT_SOCKET(wxID_ANY, MyFrame::OnNewConnection)
END_EVENT_TABLE()

//---------------------------------------------------------------------------
// OnInit()
//---------------------------------------------------------------------------
bool MyApp::OnInit()
{
    wxInitAllImageHandlers();

    // Command line
    wxCmdLineParser parser(argc, argv);
    parser.AddOption(wxT("p"), wxT("port"), wxT("Port for connections"), wxCMD_LINE_VAL_NUMBER, wxCMD_LINE_PARAM_OPTIONAL);
    parser.SetSwitchChars(wxT("-/"));

    long port = 6000;
    if (parser.Parse() == 0)
    {
        parser.Found(wxT("port"), &port);
    }

    //
    MyFrame* frame = new MyFrame(NULL, wxID_ANY, appName, wxDefaultPosition, wxSize(600, 400), wxDEFAULT_FRAME_STYLE);
    frame->Show(true);

    frame->CreateServer(port);

    return true;
}

//---------------------------------------------------------------------------
// OnExit()
//---------------------------------------------------------------------------
int MyApp::OnExit()
{
    thread_pool.Clear();
    return wxApp::OnExit();
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

    log = new wxTextCtrl(panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY);

    wxLog::SetActiveTarget(new wxLogTextCtrl(log));

    ps->Add(log, 1, wxEXPAND | wxALL, 0);
    ms->Add(panel, 1, wxEXPAND | wxALL, 0);

    CenterOnScreen();
}

//---------------------------------------------------------------------------
// MyFrame destructor
//-------------------------------------------------------------------------
MyFrame::~MyFrame()
{
    wxDELETE(server);
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
// Function creates a server
//---------------------------------------------------------------------------
void MyFrame::CreateServer(unsigned short port)
{
    wxIPV4address ip;
    ip.LocalHost();
    ip.Service(port);

    server = new wxSocketServer(ip, wxSOCKET_NOWAIT);
    if (!server->IsOk())
    {
        wxLogWarning(wxT("Cannot listen on the specified port! Restart the server..."));
        return;
    }

    wxIPV4address used_ip;
    if (server->GetLocal(used_ip))
    {
        wxLogMessage(wxT("Listening on %s:%u"), used_ip.IPAddress(), used_ip.Service());
    }
    else
    {
        wxLogError(wxT("Cannot get address! Try restarting the server..."));
        return;
    }

    server->SetEventHandler(*this);
    server->SetNotify(wxSOCKET_CONNECTION_FLAG);
    server->Notify(true);
}

//---------------------------------------------------------------------------
// Function creates a new connection
//---------------------------------------------------------------------------
void MyFrame::OnNewConnection(wxSocketEvent& event)
{
    if (event.GetSocketEvent() != wxSOCKET_CONNECTION)
    {
        wxLogWarning(wxT("Unexpected event!"));
        return;
    }

    wxCriticalSectionLocker locker(wxGetApp().cs);

    wxSocketBase* socket = server->Accept(false);
    if (socket)
    {
        // Creating a connected thread...
        ServerThread* th = new ServerThread(socket);
        if (th->Create() == wxTHREAD_NO_ERROR)
        {
            wxGetApp().thread_pool.Add(th);
            th->Run();

            wxIPV4address ip;
            if (socket->GetPeer(ip))
            {
                wxLogMessage(wxString::Format(wxT("New connection from %s:%u"), ip.IPAddress(), ip.Service()));
            }
        }
        else
        {
            wxLogError(wxT("Cannot create thread."));
            return;
        }
    }
    else
    {
        wxLogWarning(wxT("Connection rejected..."));
    }
}

//---------------------------------------------------------------------------
// ServerThread class elements
//---------------------------------------------------------------------------
BEGIN_EVENT_TABLE(ServerThread, wxEvtHandler)
EVT_SOCKET(wxID_ANY, ServerThread::OnSocketEvent)
END_EVENT_TABLE()

void ServerThread::OnSocketEvent(wxSocketEvent& event)
{
    char buffer[4096];
    memset(buffer, 0x00, sizeof(buffer));
    wxString msg = wxEmptyString;

    switch ((int)event.GetSocketEvent())
    {
    case wxSOCKET_INPUT:
        socket->SetNotify(wxSOCKET_LOST_FLAG);

        wxLogMessage(wxT("Receiving data... (Thread ID: %lld)"), this->GetId());

        while (socket->IsData())
        {
            socket->Read(buffer, sizeof(buffer));
        }

        wxLogMessage(wxT("Read: '%s' (characters: %d)"), buffer, strlen(buffer));

        // Sending acknowledgment of message receipt
        msg.Clear();
        msg = wxString::Format(wxT("Read characters: %d"), strlen(buffer));
        socket->WriteMsg(msg, msg.Len());

        socket->wxSocketBase::SetEventHandler(*this);
        socket->SetNotify(wxSOCKET_INPUT_FLAG | wxSOCKET_LOST_FLAG);
        socket->Notify(true);
        break;

    case wxSOCKET_LOST:
        wxLogMessage(wxT("Connection ended... (Thread ID: %lld)"), this->GetId());

        // Cleanup
        socket->Destroy(); // Delete socket
        this->wxEvtHandler::~wxEvtHandler(); // Destroy wxEvtHandler
        wxGetApp().cs.Enter();
        wxGetApp().thread_pool.Remove(this); // Remove thread
        wxGetApp().cs.Leave();
        break;
    }
}
