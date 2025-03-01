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

#include <wx/stdpaths.h>
#include <wx/statline.h>
#include <wx/spinctrl.h>

#include "utils.h"
#include "global_kbd_event.h"
#include "main.h"

const wxString appName = wxT("Hangman 1.0.0");

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
	ID_NEW_GAME,
	ID_TERMINATE_GAME,
	ID_WX_SITE,
	ID_WX_SITE_FORUM,
	ID_WX_SITE_WIKI,
	ID_LOG_COPY,
	ID_LOG_SAVE,
	ID_LEVEL,
	ID_TIMER,
};

//---------------------------------------------------------------------------
// Event table
//---------------------------------------------------------------------------
BEGIN_EVENT_TABLE(MyFrame, wxFrame)
	EVT_MENU(ID_CLOSE, MyFrame::OnClose)
	EVT_MENU(ID_ABOUT_PROGRAM, MyFrame::OnAboutProgram)
	EVT_MENU(ID_NEW_GAME, MyFrame::OnNewGame)
	EVT_MENU(ID_TERMINATE_GAME, MyFrame::OnTerminateGame)
	EVT_CLOSE(MyFrame::CloseApp)
	EVT_TOOL_RANGE(ID_WX_SITE, ID_WX_SITE_WIKI, MyFrame::OnWxSite)
	EVT_BUTTON(ID_LOG_COPY, MyFrame::OnLogCopySave)
	EVT_BUTTON(ID_LOG_SAVE, MyFrame::OnLogCopySave)
	EVT_TIMER(ID_TIMER, MyFrame::OnTimer)
END_EVENT_TABLE()

//---------------------------------------------------------------------------
// OnInit()
//---------------------------------------------------------------------------
bool MyApp::OnInit()
{
	wxInitAllImageHandlers();

	MyFrame *frame = new MyFrame(NULL, wxID_ANY, appName, wxDefaultPosition, wxSize(600,650), wxDEFAULT_FRAME_STYLE);
    frame->Show(true);

    return true;
}

//---------------------------------------------------------------------------
// MyFrame constructor
//---------------------------------------------------------------------------
MyFrame::MyFrame(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &pos, const wxSize &size, long style)
: wxFrame(parent, id, title, pos, size, style)
{
	game = false; // The game has not started yet when the program is launched

	wxFont wordFont(36, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Courier New")); // Font for representing the word
	wxFont statFont(16, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Arial")); // Font for statistical data
	wxFont labelFont(7, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Arial")); // Font for labels

	wxColour myBlue(0, 180, 200);
	wxColour myGreen1(0, 140, 50);
	wxColour myGreen2(0, 160, 50);
	wxColour myGreen3(0, 180, 50);
	wxColour myGreen4(0, 200, 50);
	wxColour myRed(*wxRED);

	wxIcon icon(wxGetAppFile("gfx/icon.ico"), wxBITMAP_TYPE_ICO);
	SetIcon(icon);

	this->SetMinSize(this->GetSize());

	// MENU BAR
	mb = new wxMenuBar();

    mFile = new wxMenu();
    mHelp = new wxMenu();

	mFile->Append(ID_NEW_GAME, wxT("&New Game\tCtrl-N"), wxT("New Game"));
	mFile->Append(ID_TERMINATE_GAME, wxT("&Terminate Game"), wxT("Terminate Game"));
	mFile->AppendSeparator();
	mFile->Append(ID_CLOSE, wxT("&Close Program\tCtrl-X"), wxT("Close Program"));

	mHelp->Append(ID_ABOUT_PROGRAM, wxT("&About Program\tF1"), wxT("About Program"));

	mb->Append(mFile, wxT("&File"));
	mb->Append(mHelp, wxT("&Help"));

	this->SetMenuBar(mb);

	// TOOLBAR
	wxSiteMenu = new wxMenu();
    wxSiteMenu->Append(ID_WX_SITE, wxT("wxWidgets Site"), wxT("wxWidgets Site"));
    wxSiteMenu->Append(ID_WX_SITE_FORUM, wxT("wxWidgets Forum"), wxT("wxWidgets Forum"));
    wxSiteMenu->Append(ID_WX_SITE_WIKI, wxT("wxWiki"), wxT("wxWiki"));

	tb = new wxToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_FLAT|wxTB_HORIZONTAL|wxTB_TEXT);
    const wxSize toolBarBitmapSize(22,22);
    tb->SetToolBitmapSize(toolBarBitmapSize);

	tb->AddTool(ID_NEW_GAME, wxT("New Game"),
        wxBitmap(wxGetAppFile("gfx/new_game.png"), wxBITMAP_TYPE_PNG),
        wxNullBitmap, wxITEM_NORMAL, wxT("New Game"), wxT("Start a new game"));
    tb->AddTool(ID_TERMINATE_GAME, wxT("Terminate"),
        wxBitmap(wxGetAppFile("gfx/stop_game.png"), wxBITMAP_TYPE_PNG),
        wxNullBitmap, wxITEM_NORMAL, wxT("Terminate"), wxT("Terminate the game"));
    tb->AddSeparator();
    tb->AddTool(ID_WX_SITE, wxT("wxWidgets"),
        wxBitmap(wxGetAppFile("gfx/wx.png"), wxBITMAP_TYPE_PNG),
        wxNullBitmap, wxITEM_DROPDOWN, wxT("wxWidgets"), wxT("www.wxwidgets.org"));
    tb->AddStretchableSpace();
    tb->AddTool(ID_CLOSE, wxT("Close"),
        wxBitmap(wxGetAppFile("gfx/close.png"), wxBITMAP_TYPE_PNG),
        wxNullBitmap, wxITEM_NORMAL, wxT("Close"), wxT("Close the program"));

	tb->SetDropdownMenu(ID_WX_SITE, wxSiteMenu);

	tb->SetMargins(3,3);
	tb->Realize();
	SetToolBar(tb);

	// STATUS BAR
    sb = new wxStatusBar(this);
    sb->SetFieldsCount(2);
    this->SetStatusBar(sb);
    SetStatusText(title, 0);

	panel = new wxPanel(this);

	// Sizers and boxes
	leftSb = new wxStaticBox(panel, wxID_ANY, wxEmptyString);
	infoSb = new wxStaticBox(panel, wxID_ANY, wxT(" Information "));
	logSb = new wxStaticBox(panel, wxID_ANY, wxT(" Game Log "));
	propSb = new wxStaticBox(panel, wxID_ANY, wxT(" Settings "));

	mainSizer = new wxBoxSizer(wxHORIZONTAL);
	rightSizer = new wxBoxSizer(wxVERTICAL);
	leftSizer = new wxStaticBoxSizer(leftSb, wxVERTICAL);
	infoSizer = new wxStaticBoxSizer(infoSb, wxVERTICAL);
	logSizer = new wxStaticBoxSizer(logSb, wxVERTICAL);
	propSizer = new wxStaticBoxSizer(propSb, wxVERTICAL);

	panel->SetSizer(mainSizer);

	// Control definitions...
	gallows = new wxStaticBitmap(panel, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize,
        wxBORDER_SIMPLE);

	word = new wxStaticText(panel, wxID_ANY, wxT("Hangman"), wxDefaultPosition, wxDefaultSize,
		wxALIGN_CENTER_HORIZONTAL);
	round = new wxStaticText(panel, wxID_ANY, wxT("1 of 1"));
	points = new wxStaticText(panel, wxID_ANY, wxT("00:00"));
	player = new wxStaticText(panel, wxID_ANY, wxT("Player 1"));
    time = new wxStaticText(panel, wxID_ANY, wxT("000"));

	roundLabel = new wxStaticText(panel, wxID_ANY, wxT("Round"));
	pointsLabel = new wxStaticText(panel, wxID_ANY, wxT("Points"));
	playerLabel = new wxStaticText(panel, wxID_ANY, wxT("Guesser"));
    timeLabel = new wxStaticText(panel, wxID_ANY, wxT("Remaining Time"));
	levelLabel = new wxStaticText(panel, wxID_ANY, wxT("Difficulty Level"));
	roundsLabel = new wxStaticText(panel, wxID_ANY, wxT("Number of Rounds"));

    word->SetFont(wordFont);
    round->SetFont(statFont);
    points->SetFont(statFont);
    player->SetFont(statFont);
    time->SetFont(statFont);

    roundLabel->SetFont(labelFont);
    pointsLabel->SetFont(labelFont);
    playerLabel->SetFont(labelFont);
	timeLabel->SetFont(labelFont);
    levelLabel->SetFont(labelFont);
    roundsLabel->SetFont(labelFont);

	word->SetForegroundColour(myBlue);
	round->SetForegroundColour(myGreen1);
	points->SetForegroundColour(myGreen2);
	player->SetForegroundColour(myGreen3);
	time->SetForegroundColour(myGreen4);

	// Log
	log = new wxTextCtrl(panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_RICH|wxBORDER_NONE);
	log->SetEditable(false);
	log->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_FRAMEBK));
	log->SetFont(labelFont);

	// Log buttons
	wxBoxSizer *btns = new wxBoxSizer(wxHORIZONTAL);
	copyLog = new wxButton(panel, ID_LOG_COPY, wxT("Copy"));
	saveLog = new wxButton(panel, ID_LOG_SAVE, wxT("Save"));

	// Difficulty level
	wxArrayString arr;
	arr.Add(wxT("Easy (120 s, 9 errors)"));
	arr.Add(wxT("Medium (60 s, 6 errors)"));
	arr.Add(wxT("Hard (30 s, 3 errors)"));

	level = new wxComboBox(panel, wxID_ANY, arr.Item(0), wxDefaultPosition, wxDefaultSize, arr, wxCB_READONLY);

	// Number of rounds
	rounds = new wxSpinCtrl(panel, wxID_ANY, wxT("1"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 10);

    // Adding controls to sizers...
    leftSizer->Add(gallows, 1, wxEXPAND|wxLEFT|wxTOP|wxRIGHT, 4);
    leftSizer->Add(word, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 10);

	infoSizer->Add(roundLabel, 0, wxEXPAND|wxLEFT|wxTOP|wxRIGHT, 4);
    infoSizer->Add(round, 0, wxEXPAND|wxLEFT|wxBOTTOM|wxRIGHT, 4);
    infoSizer->Add(pointsLabel, 0, wxEXPAND|wxLEFT|wxTOP|wxRIGHT, 4);
    infoSizer->Add(points, 0, wxEXPAND|wxLEFT|wxBOTTOM|wxRIGHT, 4);
    infoSizer->Add(playerLabel, 0, wxEXPAND|wxLEFT|wxTOP|wxRIGHT, 4);
    infoSizer->Add(player, 0, wxEXPAND|wxLEFT|wxBOTTOM|wxRIGHT, 4);
    infoSizer->Add(timeLabel, 0, wxEXPAND|wxLEFT|wxTOP|wxRIGHT, 4);
    infoSizer->Add(time, 0, wxEXPAND|wxLEFT|wxBOTTOM|wxRIGHT, 4);

    propSizer->Add(levelLabel, 0, wxEXPAND|wxLEFT|wxTOP|wxRIGHT, 4);
	propSizer->Add(level, 0, wxEXPAND|wxLEFT|wxBOTTOM|wxRIGHT, 4);
    propSizer->Add(roundsLabel, 0, wxEXPAND|wxLEFT|wxTOP|wxRIGHT, 4);
	propSizer->Add(rounds, 0, wxEXPAND|wxLEFT|wxBOTTOM|wxRIGHT, 4);

	btns->Add(copyLog, 1, wxEXPAND|wxALL, 0);
	btns->AddSpacer(4);
	btns->Add(saveLog, 1, wxEXPAND|wxALL, 0);

	logSizer->Add(log, 1, wxEXPAND|wxALL, 4);
	logSizer->Add(btns,0,wxEXPAND|wxLEFT|wxBOTTOM|wxRIGHT,4);

	rightSizer->Add(infoSizer, 0, wxEXPAND|wxLEFT|wxTOP|wxRIGHT, 4);
	rightSizer->Add(logSizer, 1, wxEXPAND|wxLEFT|wxTOP|wxRIGHT, 4);
	rightSizer->Add(propSizer, 0, wxEXPAND|wxALL, 4);

	mainSizer->Add(leftSizer, 2, wxEXPAND|wxALL, 4);
	mainSizer->Add(rightSizer, 1, wxEXPAND|wxALL, 0);

    CenterOnScreen();

	DrawGallows(9);
	timer.SetOwner(this, ID_TIMER);

	GlobalKeyEvtHandler::Register(this);
}

//---------------------------------------------------------------------------
// MyFrame destructor
//-------------------------------------------------------------------------
MyFrame::~MyFrame()
{
    GlobalKeyEvtHandler::Unregister(this);
}

//---------------------------------------------------------------------------
// Function initiates program closure
//---------------------------------------------------------------------------
void MyFrame::OnClose(wxCommandEvent& WXUNUSED(event))
{
    this->Close(true);
}

//---------------------------------------------------------------------------
// Function displays the "About Program..." window
//---------------------------------------------------------------------------
void MyFrame::OnAboutProgram(wxCommandEvent& event)
{
    wxMessageBox(appName+wxT("\nCopyright (c) 2025 by ..."), wxT("About Program..."), wxOK|wxICON_INFORMATION);

	event.Skip();
}

//---------------------------------------------------------------------------
// Function displays a prompt to close the program
//---------------------------------------------------------------------------
void MyFrame::CloseApp(wxCloseEvent& WXUNUSED(event))
{
    if(game)
	{
		int action = wxMessageBox(wxT("Game in progress. Close the program?"), wxT("Question"), wxOK|wxCANCEL|wxICON_QUESTION);

		if(action == wxOK)
		{
			this->Destroy();
		}
		else
		{
			return;
		}
	}
	else
	{
		this->Destroy();
	}
}

//---------------------------------------------------------------------------
// Function handles starting a new game
//-------------------------------------------------------------------------
void MyFrame::OnNewGame(wxCommandEvent& WXUNUSED(event))
{
	wxString tmp;

	// Initialize variables and strings
	InitGame();

	// Information at the beginning of the game
	this->log->Clear();
	Log(wxT("NEW GAME"));
	Log(wxT("Level: ")+level->GetStringSelection());
	LogF(wxT("Number of rounds: %d"),rounds->GetValue());

	// Disable controls
	level->Disable();
	rounds->Disable();
	tb->EnableTool(ID_NEW_GAME,false);
	mb->Enable(ID_NEW_GAME,false);

	// Entering words
	for(int i = 1; i <= 2; i++)
	{
		wxMessageBox(wxString::Format(wxT("Player %d, enter the words to guess."),i),
			wxT("Enter words"),wxOK|wxICON_EXCLAMATION);

		for(int j = 1; j <= rounds->GetValue(); j++)
		{
			tmp = wxEmptyString;
			while(tmp.IsEmpty() || !tmp.IsWord() || tmp.Len() > 10)
			{
				tmp = wxGetTextFromUser(
					wxString::Format(wxT("Player %d\nWord %d of %d"),i,j,rounds->GetValue()),
					wxT("Enter word"),wxEmptyString,this);
			}
			tmp.MakeLower(); // Convert to lowercase
			words.Add(tmp);
		}

		LogF(wxT("Player %d entered the words..."),i);
	}

	Log(wxT("STARTING GAME"));
	log->Bind(wxEVT_KEY_UP,&MyFrame::OnKey,this);
	Game(INIT_PLAYER);

	log->SetFocus();
}


//---------------------------------------------------------------------------
// Function handles the course of the game
//-----------------------------------------------------------------------
void MyFrame::Game(int gameState)
{
	wxString tmp;
	wxString tmp_word;
	bool isOk = false;

	int easy[9] = {1,2,3,4,5,6,7,8,9};
	int medi[6] = {1,3,4,6,7,9};
	int hard[3] = {3,6,9};

	switch(gameState)
	{
		case INIT_PLAYER: // Start for player n
			// End the game when the number of rounds is reached
			if(roundNumber == rounds->GetValue()+1)
			{
				EndGame();
				return;
			}

			// Reset the gallows
			DrawGallows(0);

			// Display the initial time
			time->SetForegroundColour(wxColour(0, 180, 50));
			time->SetLabel(wxString::Format(wxT("%d"),gameTime));

			// Set the word to guess
			if(currPlayer == 1) currWord = words.Item((rounds->GetValue()-1)+roundNumber);
			else currWord = words.Item(roundNumber-1);

			letters = currWord.Len();

			// Message + log
			tmp = wxString::Format(wxT("PLAYER %d\nguesses word no. %d\n(letters: %d)"),
				currPlayer,roundNumber,letters);
			LogC(tmp,*wxBLUE);
			wxMessageBox(tmp,wxT("Attention!"),wxICON_EXCLAMATION);

			// Display dots in place of letters
			word->SetLabel(wxEmptyString);
			tmp = wxEmptyString;
			for(int j = 0; j < letters; j++)
			{
				tmp += ".";
			}
			SetWord(tmp);

			// Set variables
			error = 0;

			// Start the timer
			timer.Start(1000);
			break;

		case ANALYSIS: // Guessing the letter and drawing the gallows
			tmp = wxEmptyString;
			tmp_word = word->GetLabel();

			// Check letters
			for (unsigned int i = 0; i < currWord.Len(); i++)
			{
				if (letter.IsSameAs(currWord.GetChar(i)) && letter.IsSameAs(tmp_word.GetChar(i)))
				{
					tmp << letter;
					isOk = true;
				}
				else if (letter.IsSameAs(currWord.GetChar(i)))
				{
					tmp << letter;
					letters--;
					isOk = true;
				}
				else tmp << tmp_word.GetChar(i);
			}

			SetWord(tmp);

			if (!isOk)
			{
				Log(letter + wxT(" (Miss)"));
				switch (difficult)
				{
				case 0: DrawGallows(easy[error]); break;
				case 1: DrawGallows(medi[error]); break;
				case 2: DrawGallows(hard[error]); break;
				}

				error++;
				if ((error) == errorMax)
				{
					timer.Stop();
					wxMessageBox(wxT("YOU'RE HANGED!"), wxT("Sorry..."), wxICON_ERROR);
					Game(CHANGE_PLAYER);
				}
			}
			else
			{
				Log(letter + wxT(" (OK)"));
				// Add this line to update the gallows even if the letter is correct
				DrawGallows(error);
			}

			if (letters == 0) Game(CHANGE_PLAYER);
			break;

		case CHANGE_PLAYER: // Change player
			if(currPlayer == 1)
			{
				if(letters == 0) player1pts++;
				currPlayer = 2;
			}
			else
			{
				if(letters == 0) player2pts++;
				currPlayer = 1;
				roundNumber++;
				if(roundNumber <= rounds->GetValue())
				{
					round->SetLabel(wxString::Format(wxT("%d of %d"),
						roundNumber,rounds->GetValue()));
				}
			}

			timer.Stop();

			Log(wxT("Word: ")+currWord);
			points->SetLabel(wxString::Format(wxT("(P1) %d:%d (P2)"),player1pts,player2pts));
			player->SetLabel(wxString::Format(wxT("Player %d"),currPlayer));
			Game(INIT_PLAYER);
			break;

        default:
            break;
	}
}

//---------------------------------------------------------------------------
// Function handles terminating the current game
//-------------------------------------------------------------------------
void MyFrame::OnTerminateGame(wxCommandEvent& event)
{
	if(game)
	{
		if(wxMessageBox(wxT("Do you really want to terminate the game?"),
			wxT("Attention!"),wxYES_NO|wxICON_QUESTION) == wxYES)
		{
			LogC(wxT("Game terminated!"),*wxRED);
			EndGame();
		}
		else return;
	}

	event.Skip();
}

//---------------------------------------------------------------------------
// Function displays the specified wxWidgets website
//-------------------------------------------------------------------------
void MyFrame::OnWxSite(wxCommandEvent& event)
{
    wxString site = wxEmptyString;

    switch(event.GetId())
    {
        case ID_WX_SITE: site = wxT("https://www.wxwidgets.org"); break;
        case ID_WX_SITE_FORUM: site = wxT("https://forums.wxwidgets.org"); break;
        case ID_WX_SITE_WIKI: site = wxT("https://wiki.wxwidgets.org"); break;
    }

    if(!wxLaunchDefaultBrowser(site))
    {
        wxMessageBox("Cannot connect to the Internet.","Error",wxICON_ERROR|wxOK);
    }

    event.Skip();
}

//---------------------------------------------------------------------------
// Function handles copying/saving the log
//-----------------------------------------------------------------------
void MyFrame::OnLogCopySave(wxCommandEvent& event)
{
	switch(event.GetId())
	{
		case ID_LOG_COPY:
			log->SelectAll();
			log->Copy();
			log->SelectNone();
			wxMessageBox(wxT("Log content has been copied to the clipboard..."));
			break;

		case ID_LOG_SAVE:
			log->SaveFile(wxT("Hangman_LOG_")+wxDateTime::Now().FormatISODate()+wxT(".log"));
			wxMessageBox(wxT("Log saved to file..."));
			break;
	}
}

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Function initializes the program and sets all variables
//----------------------------------------------------------------------
void MyFrame::InitGame()
{
	game = true;
	player1pts = 0;
	player2pts = 0;
	gameTime = 0;
	words.Clear();
	roundNumber = 1;
	currPlayer = 1;

	difficult = level->GetSelection();
	switch(difficult)
	{
		case 0: gameTime = 120; errorMax = 9; break;
		case 1: gameTime = 60; errorMax = 6; break;
		case 2: gameTime = 30; errorMax = 3; break;
	}

	round->SetLabel(wxString::Format(wxT("%d of %d"),1,rounds->GetValue()));
	points->SetLabel(wxT("(P1) 0:0 (P2)"));
	player->SetLabel(wxT("Player 1"));
	time->SetLabel(wxT("000"));

	DrawGallows(0); // Clear the gallows
}

//---------------------------------------------------------------------------
// Function handles ending the game
//---------------------------------------------------------------------
void MyFrame::EndGame()
{
	log->Unbind(wxEVT_KEY_UP,&MyFrame::OnKey,this);

	game = false;
	ShowWinnerInfo();
	timer.Stop();
	SetWord(wxT("Hangman"));
	DrawGallows(9);

	// Enable controls
	level->Enable(true);
	rounds->Enable(true);
	tb->EnableTool(ID_NEW_GAME,true);
	mb->Enable(ID_NEW_GAME,true);
}

//---------------------------------------------------------------------------
// Function draws the gallows
//----------------------------------------------------------------------
void MyFrame::DrawGallows(int step)
{
	if(step < 0 && step > 9) return; // Protection

	// Create context...
	wxBitmap bmp(340,400);
	wxMemoryDC dc(bmp);
	dc.SetBackground(*wxWHITE_BRUSH);
	dc.Clear();

	// Set custom pens and brushes
	wxPen brownPen(wxColour(145,80,0),5);
	wxPen orangePen(wxColour(255,200,0),3);
	wxPen blackPen(*wxBLACK,5);
	wxBrush brownBrush(wxColour(145,80,0),wxBRUSHSTYLE_BDIAGONAL_HATCH);

	switch(step)
	{
		case 9: // Legs
			dc.SetPen(wxPen(*wxBLACK));
			dc.DrawSpline(170,240,163,260,165,280);
			dc.DrawSpline(170,240,177,260,175,280);
			dc.DrawEllipse(163,280,4,20);
			dc.DrawEllipse(173,280,4,20);

		case 8: // Arms
			dc.SetPen(wxPen(*wxBLACK));
			dc.DrawSpline(170,160,140,170,130,200);
			dc.DrawSpline(130,200,133,205,126,210);
			dc.DrawSpline(130,200,135,205,130,210);
			dc.DrawSpline(130,200,137,205,134,210);
			dc.DrawSpline(170,160,200,170,210,200);
			dc.DrawSpline(210,200,213,205,206,210);
			dc.DrawSpline(210,200,215,205,210,210);
			dc.DrawSpline(210,200,217,205,214,210);

		case 7: // Torso
			dc.SetPen(blackPen);
			dc.SetBrush(*wxLIGHT_GREY_BRUSH);
			dc.DrawEllipse(160,160,20,80);
			dc.SetPen(orangePen);
			dc.DrawSpline(160,170,170,173,180,170);

		case 6: // Head
			dc.SetPen(orangePen);
			dc.DrawLine(170,40,170,80);
			dc.SetPen(blackPen);
			dc.SetBrush(*wxWHITE_BRUSH);
			dc.DrawCircle(170,120,40);
			dc.SetPen(wxPen(*wxBLACK));
			dc.DrawEllipse(160,100,10,25);
			dc.DrawEllipse(180,100,10,25);
			dc.SetBrush(*wxBLUE_BRUSH);
			dc.DrawCircle(165,117,5);
			dc.DrawCircle(185,117,5);

		case 5: // Support 2
			dc.SetPen(brownPen);
			dc.DrawLine(60,80,100,40);

		case 4: // Crossbar
			dc.SetPen(brownPen);
			dc.DrawLine(60,40,280,40);

		case 3: // Support 1
			dc.SetPen(brownPen);
			dc.DrawLine(60,290,100,330);

		case 2: // Post
			dc.SetPen(brownPen);
			dc.DrawLine(60,40,60,330);

		case 1: // Ground
			dc.SetPen(brownPen);
			dc.SetBrush(brownBrush);
			dc.DrawRectangle(20,330,300,40);
			break;

		default:
			break;
	}

	// Set the gallows...
	gallows->SetBitmap(bmp);
	bmp = wxNullBitmap;
	leftSizer->RepositionChildren(wxSize(-1, -1));
}

//---------------------------------------------------------------------------
// Log functions
//---------------------------------------------------------------------
void MyFrame::Log(wxString s)
{
	this->log->AppendText(s+wxT("\n"));
}

void MyFrame::LogF(wxString s, int val)
{
	this->log->AppendText(wxString::Format(s,val)+wxT("\n"));
}

void MyFrame::LogC(wxString s, const wxColour& c)
{
	this->log->SetDefaultStyle(wxTextAttr(c));
	Log(s);
	this->log->SetDefaultStyle(wxTextAttr(*wxBLACK));
}

//---------------------------------------------------------------------------
// Function displays information about the winner
//--------------------------------------------------------------------

void MyFrame::ShowWinnerInfo()
{
	wxString win;

	if(player1pts > player2pts) win = wxT("PLAYER 1 WINS!");
	else if(player2pts > player1pts) win = wxT("PLAYER 2 WINS!");
	else win = wxT("DRAW!");

	LogC(win,*wxRED);
	wxMessageBox(wxT("Game result:\n")+win,wxT("Game result"),wxOK|wxICON_INFORMATION);
}

//---------------------------------------------------------------------------
// Function measures the time for a move
//-------------------------------------------------------------------
void MyFrame::OnTimer(wxTimerEvent& WXUNUSED(event))
{
	gameTime--;
	time->SetLabel(wxString::Format(wxT("%d"),gameTime));

	if(gameTime == 0)
	{
		Log(wxT("Time's up..."));
		timer.Stop();
		Game(CHANGE_PLAYER);
	}
	else if(gameTime == 10)
	{
		time->SetForegroundColour(*wxRED);
	}
}

//---------------------------------------------------------------------------
// Function receives the key code and maps it to a letter
//-----------------------------------------------------------------
void MyFrame::OnKey(wxKeyEvent& event)
{
	switch(event.GetKeyCode())
	{
		case 65: letter = "a"; break;
		case 66: letter = "b"; break;
		case 67: letter = "c"; break;
		case 68: letter = "d"; break;
		case 69: letter = "e"; break;
		case 70: letter = "f"; break;
		case 71: letter = "g"; break;
		case 72: letter = "h"; break;
		case 73: letter = "i"; break;
		case 74: letter = "j"; break;
		case 75: letter = "k"; break;
		case 76: letter = "l"; break;
		case 77: letter = "m"; break;
		case 78: letter = "n"; break;
		case 79: letter = "o"; break;
		case 80: letter = "p"; break;
		case 81: letter = "q"; break;
		case 82: letter = "r"; break;
		case 83: letter = "s"; break;
		case 84: letter = "t"; break;
		case 85: letter = "u"; break;
		case 86: letter = "v"; break;
		case 87: letter = "w"; break;
		case 88: letter = "x"; break;
		case 89: letter = "y"; break;
		case 90: letter = "z"; break;
		default: return;
	}

    event.Skip();
	Game(ANALYSIS);
}

//---------------------------------------------------------------------------
// Function displays the word and recalculates dimensions
//----------------------------------------------------------------
void MyFrame::SetWord(wxString s)
{
	word->SetLabel(s);
	leftSizer->FitInside(word);
	leftSizer->RepositionChildren(wxSize(-1, -1));
}

