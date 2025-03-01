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

#include <wx/custombgwin.h>

class MyPanel : public wxCustomBackgroundWindow<wxPanel>
{
public:
	MyPanel(wxWindow* parent, wxBitmap bitmap) {
		Create(parent, wxID_ANY);
		SetBackgroundBitmap(bitmap);
	}
};

//---------------------------------------------------------------------------
// MyApp Class
//---------------------------------------------------------------------------
class MyApp : public wxApp
{
public:
	virtual bool OnInit();
};

//---------------------------------------------------------------------------
// MyFrame Class
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

	void OnTimer(wxTimerEvent& WXUNUSED(event))
	{
		unsigned int val = pbar->GetValue();

		if (val >= pbar->GetRange())
		{
			timer->Stop();
			wxMessageBox(wxT("The progress bar has reached its maximum value!"));
			pbar->SetValue(0);
			return;
		}

		val += 50;
		pbar->SetValue(val);
	}

	// Panel event
	void OnPanelResize(MyResizableImgPanelEvent& WXUNUSED(event))
	{
		button->SetBackground(panel->GetBitmap());
	}

	// Button event
	void OnButton(MyGraphicButtonEvent& WXUNUSED(event))
	{
		wxMessageBox(wxT("Button pressed!"));

		if (!timer->IsRunning())
		{
			timer->Start(500);
		}
	}

	// Calendar event
	void OnCalendar(MyCalendarEvent& event)
	{
		wxString msg = wxT("Calendar event!\n\n");
		msg += event.GetDate().FormatISODate() + wxT("\n");
		msg += event.GetNames();

		wxMessageBox(msg);
	}

private:
	wxStatusBar* sb;
	wxMenuBar* mb;
	wxMenu* mFile;
	wxMenu* mHelp;

	wxTimer* timer;

	MyResizableImgPanel* panel;
	MyGraphicButton* button;
	MySimpleProgressBar* pbar;
	MyCalendar* calendar;

	DECLARE_EVENT_TABLE()
};

#endif
