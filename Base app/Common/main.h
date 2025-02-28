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

//---------------------------------------------------------------------------
// MyApp clss
//---------------------------------------------------------------------------
class MyApp: public wxApp
{
    public:
        virtual bool OnInit();
};

//---------------------------------------------------------------------------
// MyFrame class
//---------------------------------------------------------------------------
class MyFrame: public wxFrame
{
    public:
        MyFrame(wxWindow *parent, wxWindowID id, const wxString &title,
            const wxPoint &pos, const wxSize &size, long style);
		~MyFrame();

		// Menubar events
		void OnShowDate(wxCommandEvent& event);
        void OnShowSysVersion(wxCommandEvent& event);
        void OnClose(wxCommandEvent& event);
        void OnAboutProgram(wxCommandEvent& event);
		// Menubar range events
		void OnMenuEvent(wxCommandEvent& event);

		// wxEVT_CLOSE_WINDOW event
		void CloseApp(wxCloseEvent& event);

		// Mouse events
		void OnMouseMove(wxMouseEvent& event);
        void OnMouseClick(wxMouseEvent& event);

		// Keyboard events
		void OnKey(wxKeyEvent& event);
		void OnKeyUp(wxKeyEvent& event);

    private:
        wxStatusBar *sb;

        wxMenuBar *mb;

        wxMenu *mFile;
        wxMenu *mHelp;

		DECLARE_EVENT_TABLE()
};

#endif
