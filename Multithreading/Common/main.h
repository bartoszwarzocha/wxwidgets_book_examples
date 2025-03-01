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

#include <wx/thread.h>

// Declaration of the container for storing threads
WX_DEFINE_ARRAY_PTR(wxThread*, wxThreadPool);

// Declaration of thread events
wxDECLARE_EVENT(wxEVT_MYTHREAD_UPDATED, wxThreadEvent);
wxDECLARE_EVENT(wxEVT_MYTHREAD_CLOSED, wxThreadEvent);

//---------------------------------------------------------------------------
// MyApp class
//---------------------------------------------------------------------------
class MyApp : public wxApp
{
public:
    virtual bool OnInit() wxOVERRIDE;
    virtual int OnExit() wxOVERRIDE;

    wxString AddThread(wxThread* th)
    {
        if (semaphore)
        {
            mutex->Lock();
            wxSemaError serr = semaphore->TryWait();
            mutex->Unlock();

            if (serr != wxSEMA_NO_ERROR)
            {
                return wxT("Thread limit reached!");
            }
        }

        wxString err = GetThreadErrorDesc(th->Create());
        if (!err.IsEmpty()) return err;

        mutex->Lock();
        thread_pool.Add(th);
        mutex->Unlock();

        err = GetThreadErrorDesc(th->Run());
        return err;
    }

    void RemoveThread(wxThread* th)
    {
        wxMutexLocker lock(*mutex);
        thread_pool.Remove(th);
        if (semaphore) semaphore->Post();
    }

    unsigned int GetThreadCount() { return thread_pool.size(); }

    wxString GetThreadErrorDesc(const wxThreadError& err)
    {
        wxString desc = wxEmptyString;

        switch (err)
        {
        case wxTHREAD_NO_RESOURCE: desc = wxT("Insufficient resources."); break;
        case wxTHREAD_RUNNING: desc = wxT("Thread is already running."); break;
        case wxTHREAD_NOT_RUNNING: desc = wxT("Thread cannot be started."); break;
        case wxTHREAD_KILLED: desc = wxT("Thread was destroyed."); break;
        case wxTHREAD_MISC_ERROR: desc = wxT("Unspecified error."); break;
        default: break;
        }

        return desc;
    }

private:
    wxThreadPool thread_pool;
    wxMutex* mutex;
    wxSemaphore* semaphore;
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

    void OnClearLog(wxCommandEvent& WXUNUSED(event)) { log->Clear(); }

    void OnAddCount(wxCommandEvent& event);
    void OnAddCopy(wxCommandEvent& event);

    void OnThreadClose(wxThreadEvent& event);
    void OnThreadUpdate(wxThreadEvent& event);

    inline void Log(const wxString& s)
    {
        this->log->AppendText(s + wxT("\n"));
        this->SetStatusText(wxString::Format(wxT("Active threads: %d"), wxGetApp().GetThreadCount()), 1);
    }

private:
    wxStatusBar* sb;
    wxMenuBar* mb;
    wxMenu* mFile;
    wxMenu* mHelp;

    wxPanel* panel;

    wxTextCtrl* log;

    DECLARE_EVENT_TABLE()
};

//---------------------------------------------------------------------------
// MyCountThread class
//---------------------------------------------------------------------------
class MyCountThread : public wxThread
{
public:
    MyCountThread(MyFrame* hdlr)
        : wxThread(wxTHREAD_DETACHED), handler(hdlr) {
    }

    ~MyCountThread() {}

protected:
    virtual ExitCode Entry() wxOVERRIDE
    {
        // Countdown simulation
        for (int i = 10; i != 0; i--)
        {
            wxThreadEvent* evt = new wxThreadEvent(wxEVT_MYTHREAD_UPDATED);
            evt->SetExtraLong(i);
            evt->SetId(this->GetId());
            wxQueueEvent(handler, evt);
            Sleep(1000);
        }

        return (wxThread::ExitCode)0;
    }

    virtual void OnExit() wxOVERRIDE
    {
        wxGetApp().RemoveThread(this);

        wxThreadEvent* evt = new wxThreadEvent(wxEVT_MYTHREAD_CLOSED);
        evt->SetId(this->GetId());
        wxQueueEvent(handler, evt);
    }

    MyFrame* handler;
};

//---------------------------------------------------------------------------
// MyCopyThread class
//---------------------------------------------------------------------------
class MyCopyThread : public wxThread
{
public:
    MyCopyThread(MyFrame* hdlr)
        : wxThread(wxTHREAD_DETACHED), handler(hdlr) {
    }

    ~MyCopyThread() {}

protected:
    virtual ExitCode Entry() wxOVERRIDE
    {
        for (int i = 0; i <= 100; i += 5)
        {
            wxMutexGuiEnter();
            handler->Log(wxString::Format(wxT("[Thread ID: %lld] Copying, completed %02d %%."), this->GetId(), i));
            wxMutexGuiLeave();
            Sleep(500);
        }

        return (wxThread::ExitCode)0;
    }

    virtual void OnExit() wxOVERRIDE
    {
        wxGetApp().RemoveThread(this);

        // Thread event
        wxThreadEvent* evt = new wxThreadEvent(wxEVT_MYTHREAD_CLOSED);
        evt->SetId(this->GetId());
        wxQueueEvent(handler, evt);
    }

    MyFrame* handler;
};

#endif
