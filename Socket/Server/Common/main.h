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

WX_DEFINE_ARRAY_PTR(wxThread*, wxThreadPool);

//---------------------------------------------------------------------------
// MyApp class
//---------------------------------------------------------------------------
class MyApp : public wxApp
{
public:
    virtual bool OnInit();
    virtual int OnExit();

    wxThreadPool thread_pool;
    wxCriticalSection cs;
};

DECLARE_APP(MyApp);

//---------------------------------------------------------------------------
// MyFrame class
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

    void OnNewConnection(wxSocketEvent& event);

    void CreateServer(unsigned short port);

private:
    wxStatusBar* sb;
    wxMenuBar* mb;
    wxMenu* mFile;
    wxMenu* mHelp;

    wxPanel* panel;

    wxTextCtrl* log;

    wxSocketServer* server;

    DECLARE_EVENT_TABLE()
};

//---------------------------------------------------------------------------
// ServerThread class
//---------------------------------------------------------------------------
class ServerThread : public wxThread, public wxEvtHandler
{
public:
    ServerThread(wxSocketBase* skt)
        : wxThread(wxTHREAD_JOINABLE), wxEvtHandler(), socket(skt) {
    }

    virtual ~ServerThread() {}

    void OnSocketEvent(wxSocketEvent& event);

protected:
    virtual ExitCode Entry()
    {
        socket->wxSocketBase::SetEventHandler(*this);
        socket->SetNotify(wxSOCKET_INPUT_FLAG | wxSOCKET_LOST_FLAG);
        socket->Notify(true);

        wxLog::SetThreadActiveTarget(wxLog::GetActiveTarget());
        wxLogMessage(wxT("Thread created ID: %lld"), this->GetId());

        return (wxThread::ExitCode)0;
    }

    wxSocketBase* socket;

    DECLARE_EVENT_TABLE()
};

#endif
