/////////////////////////////////////////////////////////////////////////////
// Name:        global_kbd_event.h
// Purpose:     My wxWidgets app
// Author:      Name Surname <name.surname@email.org>
// Created:     2025-01-01
// Copyright:   (c) 2025 by Name Surname
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _GLOBAL_KBD_EVENT_H_
#define _GLOBAL_KBD_EVENT_H_

#include <wx/wx.h>

class GlobalKeyEvtHandler : public wxEvtHandler
{
public:
    GlobalKeyEvtHandler();
    ~GlobalKeyEvtHandler();
    static void Register(wxWindow* w);
    static void Unregister(wxWindow* w);

private:
	void OnChar(wxKeyEvent& event);
    void OnKeyDown(wxKeyEvent& event);
    void OnKeyUp(wxKeyEvent& event);
};

GlobalKeyEvtHandler::GlobalKeyEvtHandler()
{
	Bind(wxEVT_CHAR,&GlobalKeyEvtHandler::OnChar,this);
	Bind(wxEVT_KEY_DOWN,&GlobalKeyEvtHandler::OnKeyDown,this);
	Bind(wxEVT_KEY_UP,&GlobalKeyEvtHandler::OnKeyUp,this);
}

GlobalKeyEvtHandler::~GlobalKeyEvtHandler()
{
	Unbind(wxEVT_CHAR,&GlobalKeyEvtHandler::OnChar,this);
	Unbind(wxEVT_KEY_DOWN,&GlobalKeyEvtHandler::OnKeyDown,this);
	Unbind(wxEVT_KEY_UP,&GlobalKeyEvtHandler::OnKeyUp,this);
}

void GlobalKeyEvtHandler::OnChar(wxKeyEvent& event)
{
    event.ResumePropagation(1);
    event.Skip();
}

void GlobalKeyEvtHandler::OnKeyDown(wxKeyEvent& event)
{
    event.ResumePropagation(1);
    event.Skip();
}

void GlobalKeyEvtHandler::OnKeyUp(wxKeyEvent& event)
{
    event.ResumePropagation(1);
    event.Skip();
}

void GlobalKeyEvtHandler::Register(wxWindow* w)
{
    for (wxWindow* child : w->GetChildren())
    {
        child->PushEventHandler(new GlobalKeyEvtHandler());
    }
}

void GlobalKeyEvtHandler::Unregister(wxWindow* w)
{
    for (wxWindow* child : w->GetChildren())
    {
        child->PopEventHandler(true);
    }
}

#endif
