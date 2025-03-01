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

#include <wx/socket.h>

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

    void OnConnect(wxCommandEvent& event);
    void OnDisconnect(wxCommandEvent& event);

    void OnSendMessage(wxCommandEvent& event);

    void OnSocketEvent(wxSocketEvent& event);

private:
    void CreateClient();

    wxStatusBar* sb;
    wxMenuBar* mb;
    wxMenu* mFile;
    wxMenu* mHelp;

    wxPanel* panel;

    wxTextCtrl* address;
    wxTextCtrl* port;
    wxTextCtrl* log;
    wxTextCtrl* msg;

    wxButton* conn_btn;
    wxButton* send_btn;

    wxSocketClient* client;

    DECLARE_EVENT_TABLE()
};

#endif
