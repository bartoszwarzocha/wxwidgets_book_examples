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

#define PROP_MAIN        wxT("General Settings")
#define PROP_PLAYER1     wxT("Player 1")
#define PROP_PLAYER2     wxT("Player 2")
#define PROP_GAME        wxT("Game")
#define PROP_PLAYER      wxT("Who starts?")
#define PROP_TIME        wxT("Game time")
#define PROP_TIME_INF    wxT("Infinite game")
#define PROP_BONUS_TIME  wxT("Delay?")
#define PROP_BONUS       wxT("Delay (s)")
#define PROP_WATCH       wxT("Clock")
#define PROP_WATCH_FONT  wxT("Font")
#define PROP_WATCH_START wxT("Start signal")
#define PROP_WATCH_STOP  wxT("End signal")
#define PROP_BONUS_COL   wxT("Delay color")
#define PROP_P1_SET      wxT("Player 1 settings")
#define PROP_P1_NAME     wxT("Player 1 name")
#define PROP_P1_IMG      wxT("Player 1 avatar")
#define PROP_P1_COL      wxT("Player 1 color")
#define PROP_P2_SET      wxT("Player 2 settings")
#define PROP_P2_NAME     wxT("Player 2 name")
#define PROP_P2_IMG      wxT("Player 2 avatar")
#define PROP_P2_COL      wxT("Player 2 color")

//---------------------------------------------------------------------------
// Player Class
//---------------------------------------------------------------------------
class Player
{
public:
	wxString name;
	wxString avatar;
	wxColour col;
	long time;
};

//---------------------------------------------------------------------------
// Property Class
//---------------------------------------------------------------------------
class Property
{
public:
	int who_first;
	long time;
	bool infinity;
	bool is_bonus;
	long bonus_time;
	long bonus_counter;

	wxFont font;
	wxString start_sound;
	wxString stop_sound;
	wxColour bonus_c;

	Player player1;
	Player player2;
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

	void OnTimer(wxTimerEvent& event);

	void OnStart(wxCommandEvent& event);
	void OnStartBtn(wxCommandEvent& event);
	void OnStop(wxCommandEvent& event);
	void OnChange(wxCommandEvent& event);
	void OnClear(wxCommandEvent& event);

	void OnProp(wxPropertyGridEvent& event);

private:
	void Draw();

	void DoStart();
	void DoStop();
	void DoClear();

	void CalculateTimes(int time);

	void GetProperties();

	void ConvertFromSec(long sec, int* h, int* m, int* s)
	{
		*h = floor(sec / 3600);
		*m = floor(sec / 60 % 60);
		*s = floor(sec % 60);
	}

	wxTimer timer;

	wxStatusBar* sb;
	wxMenuBar* mb;
	wxMenu* mFile;
	wxMenu* mGame;
	wxMenu* mHelp;

	wxPanel* panel, * left;

	wxPropertyGridManager* prop;

	wxBitmapButton* start_stop, * change;

	Player* cp;
	Property game_prop;

	wxStaticText* bonus_text;
	wxStaticText* p1_name_text;
	wxStaticText* p2_name_text;
	wxStaticText* p1_time_text;
	wxStaticText* p2_time_text;
	wxStaticBitmap* p1_image;
	wxStaticBitmap* p2_image;

	wxBoxSizer* main_sizer;
	wxBoxSizer* panel_sizer;
	wxBoxSizer* left_sizer;
	wxBoxSizer* info_sizer;
	wxFlexGridSizer* time_sizer;
	wxBoxSizer* button_sizer;

	bool game;

	DECLARE_EVENT_TABLE()
};

#endif
