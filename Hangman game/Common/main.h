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
	void OnNewGame(wxCommandEvent& event);
	void OnTerminateGame(wxCommandEvent& event);
	void OnWxSite(wxCommandEvent& event);
	void OnShowWxSite(wxCommandEvent& event);
	void OnLogCopySave(wxCommandEvent& event);

	void OnTimer(wxTimerEvent& event);        // Function handling the passage of time
	void OnKey(wxKeyEvent& event);            // Keyboard event function

	// Game functions
	void InitGame();          // Initialize variables and controls
	void Game(int gameState); // Control the course of the game
	void EndGame();           // End the game
	void ShowWinnerInfo();    // Display winning information
	void SetWord(wxString s); // Display the word in the game window

	// Log functions
	void Log(wxString s);                     // Display plain text
	void LogF(wxString s, int val);           // Display formatted text
	void LogC(wxString s, const wxColour& c); // Display colored text

	void DrawGallows(int step);

	enum
	{
		INIT_PLAYER = 1,
		ANALYSIS,
		CHANGE_PLAYER
	};

private:
	wxStatusBar* sb;
	wxMenuBar* mb;
	wxMenu* mFile;
	wxMenu* mHelp;
	wxToolBar* tb;
	wxMenu* wxSiteMenu;

	wxPanel* panel;

	wxBoxSizer* mainSizer; // Main sizer
	wxBoxSizer* rightSizer; // Right column
	wxStaticBoxSizer* leftSizer; // Left column
	wxStaticBoxSizer* infoSizer; // Information
	wxStaticBoxSizer* logSizer; // Log
	wxStaticBoxSizer* propSizer; // Tools

	wxStaticBox* leftSb;
	wxStaticBox* infoSb;
	wxStaticBox* logSb;
	wxStaticBox* propSb;

	wxStaticBitmap* gallows;   // Gallows

	wxStaticText* word;        // Guessed word
	wxStaticText* round;       // Round
	wxStaticText* roundLabel;  // Round - label
	wxStaticText* points;      // Points
	wxStaticText* pointsLabel; // Points - label
	wxStaticText* time;        // Stopwatch
	wxStaticText* timeLabel;   // Stopwatch - label
	wxStaticText* player;      // Current player
	wxStaticText* playerLabel; // Current player - label
	wxStaticText* levelLabel;  // Difficulty level - label
	wxStaticText* roundsLabel; // Number of rounds - label

	wxTextCtrl* log;
	wxButton* copyLog;
	wxButton* saveLog;

	wxComboBox* level;
	wxSpinCtrl* rounds;

	wxTimer timer;       // Stopwatch

	int currPlayer;      // Current player
	int player1pts;      // Player 1 points
	int player2pts;      // Player 2 points
	int error;           // Next error number
	int errorMax;        // Maximum number of errors per move
	int roundNumber;     // Current round number
	int difficult;       // Difficulty level
	int gameTime;        // Time per move
	wxString currWord;   // Word to guess (for comparisons)
	int letters;		 // Number of letters to guess
	wxString letter;     // Current entered character
	wxArrayString words; // Array of words to guess
	bool game;			 // Is the game in progress? (to control some actions)

	DECLARE_EVENT_TABLE()
};

#endif
