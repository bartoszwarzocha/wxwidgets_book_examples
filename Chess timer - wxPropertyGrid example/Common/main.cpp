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

#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/manager.h>
#include <wx/propgrid/advprops.h>

#include <wx/splitter.h>
#include <wx/timer.h>
#include <wx/sound.h>

#include "main.h"

const wxString appName = wxT("Chess Clock 1.0.0");

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
	ID_PROPERTY,
	ID_START,
	ID_STOP,
	ID_CLEAR,
	ID_CHANGE_PLAYER,
	ID_TIMER,
};

//---------------------------------------------------------------------------
// Event table
//---------------------------------------------------------------------------
BEGIN_EVENT_TABLE(MyFrame, wxFrame)
EVT_MENU(ID_CLOSE, MyFrame::OnClose)
EVT_MENU(ID_ABOUT_PROGRAM, MyFrame::OnAboutProgram)
EVT_CLOSE(MyFrame::CloseApp)
//
EVT_MENU(ID_START, MyFrame::OnStart)
EVT_MENU(ID_STOP, MyFrame::OnStop)
EVT_MENU(ID_CHANGE_PLAYER, MyFrame::OnChange)
//
EVT_BUTTON(ID_START, MyFrame::OnStartBtn)
EVT_BUTTON(ID_STOP, MyFrame::OnStartBtn)
EVT_BUTTON(ID_CLEAR, MyFrame::OnStartBtn)
EVT_BUTTON(ID_CHANGE_PLAYER, MyFrame::OnChange)
//
EVT_TIMER(ID_TIMER, MyFrame::OnTimer)
//
EVT_PG_CHANGED(ID_PROPERTY, MyFrame::OnProp)
END_EVENT_TABLE()

//---------------------------------------------------------------------------
// OnInit()
//---------------------------------------------------------------------------
bool MyApp::OnInit()
{
	wxInitAllImageHandlers();

	MyFrame* frame = new MyFrame(NULL, wxID_ANY, appName, wxDefaultPosition, wxSize(800, 500), wxCAPTION | wxCLOSE_BOX | wxMINIMIZE_BOX);
	frame->Show(true);
	return true;
}

//---------------------------------------------------------------------------
// MyFrame constructor
//---------------------------------------------------------------------------
MyFrame::MyFrame(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
	: wxFrame(parent, id, title, pos, size, style)
{
	// Timer
	timer.SetOwner(this, ID_TIMER);
	game = false;

	//
	mb = new wxMenuBar();

	mFile = new wxMenu();
	mGame = new wxMenu();
	mHelp = new wxMenu();

	mFile->Append(ID_CLOSE, wxT("&Close program\tCtrl-X"), wxT("Closes the program"));

	mGame->Append(ID_START, wxT("Start\tCtrl+S"), wxT("Starts the game"));
	mGame->Append(ID_STOP, wxT("Stop\tCtrl+D"), wxT("Stops the game"));
	mGame->AppendSeparator();
	mGame->Append(ID_CHANGE_PLAYER, wxT("Change player\tSpace"));

	mHelp->Append(ID_ABOUT_PROGRAM, wxT("&About\tF1"), wxT("About the program"));

	mb->Append(mFile, wxT("&File"));
	mb->Append(mGame, wxT("&Game"));
	mb->Append(mHelp, wxT("&Help"));

	this->SetMenuBar(mb);

	sb = new wxStatusBar(this);
	sb->SetFieldsCount(2);
	this->SetStatusBar(sb);
	SetStatusText(title, 0);

	main_sizer = new wxBoxSizer(wxVERTICAL);
	panel_sizer = new wxBoxSizer(wxHORIZONTAL);
	left_sizer = new wxBoxSizer(wxVERTICAL);
	info_sizer = new wxBoxSizer(wxVERTICAL);
	time_sizer = new wxFlexGridSizer(3, 2, 8, 50);
	button_sizer = new wxBoxSizer(wxHORIZONTAL);

	// Main panel
	panel = new wxPanel(this);

	// Splitter
	wxSplitterWindow* splitter = new wxSplitterWindow(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_THIN_SASH);

	// Left column panel
	left = new wxPanel(splitter);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// PROPERTY GRID MANAGER
	prop = new wxPropertyGridManager(splitter, ID_PROPERTY, wxDefaultPosition, wxDefaultSize, wxPG_BOLD_MODIFIED | wxPG_SPLITTER_AUTO_CENTER | wxPG_TOOLBAR | wxPG_DESCRIPTION | wxPGMAN_DEFAULT_STYLE);
	prop->SetExtraStyle(wxPG_EX_MODE_BUTTONS);
	prop->SetMinSize(wxSize(200, -1));
	prop->SetDescBoxHeight(150);

	// PAGE - Main settings
	wxPropertyGridPage* page;
	page = prop->AddPage(PROP_MAIN);

	// CATEGORY
	page->Append(new wxPropertyCategory(PROP_GAME));
	page->SetPropertyHelpString(PROP_GAME, wxT("General game settings"));

	// Enumeration
	wxArrayString times;
	times.Add(wxT("1 minute"));
	times.Add(wxT("5 minutes"));
	times.Add(wxT("10 minutes"));
	times.Add(wxT("15 minutes"));
	times.Add(wxT("20 minutes"));
	times.Add(wxT("30 minutes"));
	times.Add(wxT("60 minutes"));
	wxEnumProperty* times_prop = new wxEnumProperty(PROP_TIME, wxPG_LABEL, times);
	page->SetPropertyHelpString(times_prop, wxT("Game time for one player"));
	page->Append(times_prop);

	// Enumeration
	wxArrayString players;
	players.Add(wxT("Player 1"));
	players.Add(wxT("Player 2"));
	page->Append(new wxEnumProperty(PROP_PLAYER, wxPG_LABEL, players));
	page->SetPropertyHelpString(PROP_PLAYER, wxT("Specifies which clock will count time first"));

	// Boolean value
	page->Append(new wxBoolProperty(PROP_TIME_INF, wxPG_LABEL, true));
	page->SetPropertyHelpString(PROP_TIME_INF, wxT("If TRUE, the game continues after time runs out"));

	// Boolean value
	page->Append(new wxBoolProperty(PROP_BONUS_TIME, wxPG_LABEL, true));
	page->SetPropertyHelpString(PROP_BONUS_TIME, wxT("Apply delay before starting the game?"));

	// Nested position - number
	page->Append(new wxIntProperty(PROP_BONUS, wxPG_LABEL, 30));
	page->SetPropertyHelpString(PROP_BONUS, wxT("Delay time before starting the game"));

	// CATEGORY
	page->Append(new wxPropertyCategory(PROP_WATCH));
	page->SetPropertyHelpString(PROP_WATCH, wxT("Clock settings"));

	// Font selection
	wxFont font(28, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
	page->Append(new wxFontProperty(PROP_WATCH_FONT, wxPG_LABEL, font));
	page->SetPropertyHelpString(PROP_WATCH_FONT, wxT("Clock font"));

	// File selection
	page->Append(new wxFileProperty(PROP_WATCH_START, wxPG_LABEL));
	page->SetPropertyAttribute(PROP_WATCH_START, wxPG_FILE_WILDCARD, wxT("WAV files (*.wav)|*.wav"));
	page->SetPropertyHelpString(PROP_WATCH_START, wxT("Sound played at the start of the game"));

	page->Append(new wxFileProperty(PROP_WATCH_STOP, wxPG_LABEL));
	page->SetPropertyAttribute(PROP_WATCH_STOP, wxPG_FILE_WILDCARD, wxT("WAV files (*.wav)|*.wav"));
	page->SetPropertyHelpString(PROP_WATCH_STOP, wxT("Sound played at the end of the game"));

	// Color selection
	page->Append(new wxColourProperty(PROP_BONUS_COL, wxPG_LABEL, wxColour(64, 128, 128)));

	// PAGE - Player 1
	page = prop->AddPage(PROP_PLAYER1);

	// CATEGORY
	page->Append(new wxPropertyCategory(PROP_P1_SET));
	page->SetPropertyHelpString(PROP_P1_SET, wxT("Player 1 settings"));

	// String
	page->Append(new wxStringProperty(PROP_P1_NAME, wxPG_LABEL, wxT("Player 1")));
	page->SetPropertyHelpString(PROP_P1_NAME, wxT("Player 1 name displayed in the information window"));

	// Image selection
	page->Append(new wxImageFileProperty(PROP_P1_IMG, wxPG_LABEL));
	page->SetPropertyHelpString(PROP_P1_IMG, wxT("Player 1 image"));

	// Color selection
	page->Append(new wxColourProperty(PROP_P1_COL, wxPG_LABEL, wxColour(0, 128, 255)));

	// PAGE - Player 2
	page = prop->AddPage(PROP_PLAYER2);

	// CATEGORY
	page->Append(new wxPropertyCategory(PROP_P2_SET));
	page->SetPropertyHelpString(PROP_P2_SET, wxT("Player 2 settings"));

	// String
	page->Append(new wxStringProperty(PROP_P2_NAME, wxPG_LABEL, wxT("Player 2")));
	page->SetPropertyHelpString(PROP_P2_NAME, wxT("Player 2 name displayed in the information window"));

	// Image selection
	page->Append(new wxImageFileProperty(PROP_P2_IMG, wxPG_LABEL));
	page->SetPropertyHelpString(PROP_P2_IMG, wxT("Player 2 image"));

	// Color selection
	page->Append(new wxColourProperty(PROP_P2_COL, wxPG_LABEL, wxColour(255, 0, 128)));

	/////////////////////////////////////////////////////////////////////////////////////////
	// Information window
	wxStaticText* bonus_label = new wxStaticText(left, wxID_ANY, wxT("Time to start the game:"));
	bonus_label->SetFont(wxFont(12, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
	bonus_text = new wxStaticText(left, wxID_ANY, wxEmptyString);
	bonus_text->SetFont(wxFont(14, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
	p1_name_text = new wxStaticText(left, wxID_ANY, wxEmptyString);
	p1_name_text->SetFont(wxFont(18, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
	p1_time_text = new wxStaticText(left, wxID_ANY, wxEmptyString);
	p2_name_text = new wxStaticText(left, wxID_ANY, wxEmptyString);
	p2_name_text->SetFont(wxFont(18, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
	p2_time_text = new wxStaticText(left, wxID_ANY, wxEmptyString);
	p1_image = new wxStaticBitmap(left, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxSize(48, 48));
	p2_image = new wxStaticBitmap(left, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxSize(48, 48));

	// Buttons
	start_stop = new wxBitmapButton(left, ID_START, wxBitmap(wxT("gfx/64/start.png"), wxBITMAP_TYPE_PNG));
	change = new wxBitmapButton(left, ID_CHANGE_PLAYER, wxBitmap(wxT("gfx/64/change.png"), wxBITMAP_TYPE_PNG));

	// Assembling the GUI
	this->SetSizer(main_sizer);
	main_sizer->Add(panel, 1, wxEXPAND | wxALL, 0);

	panel->SetSizer(panel_sizer);
	panel_sizer->Add(splitter, 1, wxEXPAND | wxALL, 0);

	time_sizer->Add(p1_name_text);
	time_sizer->Add(p2_name_text);
	time_sizer->Add(p1_image);
	time_sizer->Add(p2_image);
	time_sizer->Add(p1_time_text);
	time_sizer->Add(p2_time_text);

	info_sizer->AddSpacer(30);
	info_sizer->Add(bonus_label, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 4);
	info_sizer->Add(bonus_text, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 4);
	info_sizer->AddSpacer(30);
	info_sizer->Add(time_sizer, 1, wxALIGN_CENTER_HORIZONTAL | wxALL, 10);

	button_sizer->Add(start_stop, 1, wxEXPAND | wxALL, 4);
	button_sizer->Add(change, 1, wxEXPAND | wxALL, 4);

	left->SetSizer(left_sizer);
	left_sizer->Add(info_sizer, 1, wxEXPAND | wxALL, 0);
	left_sizer->Add(button_sizer, 0, wxEXPAND | wxALL, 0);

	/////////////////////////////////////////////////////////////////////////////////////////
	splitter->SplitVertically(left, prop, this->GetSize().x - 250);
	info_sizer->RecalcSizes();

	CenterOnScreen();

	//
	this->GetProperties();
}

//---------------------------------------------------------------------------
// MyFrame destructor
//------------------------------------------------------------------------
MyFrame::~MyFrame()
{

}

//---------------------------------------------------------------------------
// Function initiates program closure
//---------------------------------------------------------------------------
void MyFrame::OnClose(wxCommandEvent& WXUNUSED(event))
{
	this->Close(true);
}

//---------------------------------------------------------------------------
// Function displays the "About..." window
//---------------------------------------------------------------------------
void MyFrame::OnAboutProgram(wxCommandEvent& event)
{
	wxMessageBox(appName + wxT("\nCopyright (c) 2025 by ..."), wxT("About..."), wxOK | wxICON_INFORMATION);
	event.Skip();
}

//---------------------------------------------------------------------------
// Function displays a question about closing the program
//---------------------------------------------------------------------------
void MyFrame::CloseApp(wxCloseEvent& WXUNUSED(event))
{
	int action = wxMessageBox(wxT("Close the program?"), wxT("Question"), wxOK | wxCANCEL | wxICON_QUESTION);

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
// Function handles the passage of time
//---------------------------------------------------------------------------
void MyFrame::OnTimer(wxTimerEvent& WXUNUSED(event))
{
	if (game_prop.is_bonus && game_prop.bonus_counter > 0)
	{
		game_prop.bonus_counter--;

		Draw();
	}
	else
	{
		if (!game)
		{
			if (!game_prop.start_sound.IsEmpty())
			{
				wxSound::Play(game_prop.start_sound);
			}

			game = true;
		}

		cp->time--;

		Draw();

		if (cp->time == 0 && !game_prop.infinity)
		{
			if (!game_prop.stop_sound.IsEmpty())
			{
				wxSound::Play(game_prop.stop_sound);
			}

			DoStop();
		}
	}
}

//---------------------------------------------------------------------------
// Function initiates starting the game from the menu
//---------------------------------------------------------------------------
void MyFrame::OnStart(wxCommandEvent& WXUNUSED(event))
{
	DoStart();
}

//---------------------------------------------------------------------------
// Function handles the start button
//---------------------------------------------------------------------------
void MyFrame::OnStartBtn(wxCommandEvent& event)
{
	if (event.GetId() == ID_START)
	{
		DoStart();
		start_stop->SetId(ID_STOP);
		start_stop->SetBitmap(wxBitmap(wxT("gfx/64/stop.png"), wxBITMAP_TYPE_PNG));
	}
	else if (event.GetId() == ID_STOP)
	{
		DoStop();
	}
	else
	{
		DoClear();
		start_stop->SetId(ID_START);
		start_stop->SetBitmap(wxBitmap(wxT("gfx/64/start.png"), wxBITMAP_TYPE_PNG));
	}
}

//---------------------------------------------------------------------------
// Function stops the game from the menu
//---------------------------------------------------------------------------
void MyFrame::OnStop(wxCommandEvent& WXUNUSED(event))
{
	DoStop();
}

//---------------------------------------------------------------------------
// Function handles player change
//---------------------------------------------------------------------------
void MyFrame::OnChange(wxCommandEvent& WXUNUSED(event))
{
	if (!game) return;

	timer.Stop();

	if (cp->name.IsSameAs(game_prop.player1.name)) cp = &game_prop.player2;
	else cp = &game_prop.player1;

	timer.Start(1000);
}

//---------------------------------------------------------------------------
// Function initiates clearing data and screen
//---------------------------------------------------------------------------
void MyFrame::OnClear(wxCommandEvent& WXUNUSED(event))
{
	DoClear();
}

//---------------------------------------------------------------------------
// Function "draws" current values
//---------------------------------------------------------------------------
void MyFrame::Draw()
{
	bonus_text->SetForegroundColour(game_prop.bonus_c);

	p1_time_text->SetForegroundColour(game_prop.player1.col);
	p1_time_text->SetFont(game_prop.font);

	p2_time_text->SetForegroundColour(game_prop.player2.col);
	p2_time_text->SetFont(game_prop.font);

	p1_name_text->SetLabel(game_prop.player1.name);
	p2_name_text->SetLabel(game_prop.player2.name);

	int h, m, s;
	ConvertFromSec(game_prop.bonus_counter, &h, &m, &s);
	bonus_text->SetLabel(wxString::Format("%02d:%02d:%02d", h, m, s));
	ConvertFromSec(game_prop.player1.time, &h, &m, &s);
	if (game_prop.player1.time < 0)
	{
		p1_time_text->SetLabel(wxString::Format("-%02d:%02d:%02d", -h, -m, -s));
	}
	else
	{
		p1_time_text->SetLabel(wxString::Format("%02d:%02d:%02d", h, m, s));
	}
	ConvertFromSec(game_prop.player2.time, &h, &m, &s);
	if (game_prop.player2.time < 0)
	{
		p2_time_text->SetLabel(wxString::Format("-%02d:%02d:%02d", -h, -m, -s));
	}
	else
	{
		p2_time_text->SetLabel(wxString::Format("%02d:%02d:%02d", h, m, s));
	}

	left_sizer->RecalcSizes();
	info_sizer->RecalcSizes();
	time_sizer->RecalcSizes();
}

//---------------------------------------------------------------------------
// Function starts the game
//---------------------------------------------------------------------------
void MyFrame::DoStart()
{
	timer.Start(1000);

	if (game_prop.who_first == 0) cp = &game_prop.player1;
	else cp = &game_prop.player2;
}

//---------------------------------------------------------------------------
// Function ends the game
//---------------------------------------------------------------------------
void MyFrame::DoStop()
{
	timer.Stop();
	wxMessageBox(wxT("Pressing the START button again will reset the clocks."));
	start_stop->SetId(ID_CLEAR);
	game = false;
}

//---------------------------------------------------------------------------
// Function "resets" settings and variables
//---------------------------------------------------------------------------
void MyFrame::DoClear()
{
	if (game_prop.is_bonus)
	{
		game_prop.bonus_counter = game_prop.bonus_time;
	}
	else
	{
		game_prop.bonus_counter = 0;
	}
	game_prop.player1.time = game_prop.time;
	game_prop.player2.time = game_prop.time;

	Draw();
}

//---------------------------------------------------------------------------
// Function handles parameter change event in the property grid
//---------------------------------------------------------------------------
void MyFrame::OnProp(wxPropertyGridEvent& event)
{
	if (game)
	{
		wxMessageBox(wxT("Settings changes are not considered during the game!"));
		return;
	}

	wxString n = event.GetPropertyName();
	wxAny v = event.GetPropertyValue();

	if (n.IsSameAs(PROP_PLAYER)) { this->game_prop.who_first = v.As<int>(); }
	if (n.IsSameAs(PROP_TIME))
	{
		this->game_prop.time = v.As<int>();
		CalculateTimes(game_prop.time);
	}
	if (n.IsSameAs(PROP_TIME_INF)) { this->game_prop.infinity = v.As<bool>(); }
	if (n.IsSameAs(PROP_BONUS_TIME)) { this->game_prop.is_bonus = v.As<bool>(); }
	if (n.IsSameAs(PROP_BONUS)) { this->game_prop.bonus_time = v.As<long>(); }
	if (n.IsSameAs(PROP_WATCH_FONT)) { this->game_prop.font = v.As<wxFont>(); }
	if (n.IsSameAs(PROP_WATCH_START)) { this->game_prop.start_sound = v.As<wxString>(); }
	if (n.IsSameAs(PROP_WATCH_STOP)) { this->game_prop.stop_sound = v.As<wxString>(); }
	if (n.IsSameAs(PROP_BONUS_COL)) { this->game_prop.bonus_c = v.As<wxColour>(); }

	if (n.IsSameAs(PROP_P1_NAME)) { this->game_prop.player1.name = v.As<wxString>(); }
	if (n.IsSameAs(PROP_P1_IMG)) { this->game_prop.player1.avatar = v.As<wxString>(); }
	if (n.IsSameAs(PROP_P1_COL)) { this->game_prop.player1.col = v.As<wxColour>(); }

	if (n.IsSameAs(PROP_P2_NAME)) { this->game_prop.player2.name = v.As<wxString>(); }
	if (n.IsSameAs(PROP_P2_IMG)) { this->game_prop.player2.avatar = v.As<wxString>(); }
	if (n.IsSameAs(PROP_P2_COL)) { this->game_prop.player2.col = v.As<wxColour>(); }

	// Refresh
	DoClear();

	if (!game_prop.player1.avatar.IsEmpty())
	{
		wxImage img(game_prop.player1.avatar);
		img.Rescale(p1_image->GetSize().GetX(), p1_image->GetSize().GetY());
		p1_image->SetBitmap(wxBitmap(img));
	}

	if (!game_prop.player2.avatar.IsEmpty())
	{
		wxImage img(game_prop.player2.avatar);
		img.Rescale(p2_image->GetSize().GetX(), p2_image->GetSize().GetY());
		p2_image->SetBitmap(wxBitmap(img));
	}
}

//---------------------------------------------------------------------------
// Function reads current parameter values
//---------------------------------------------------------------------------
void MyFrame::GetProperties()
{
	game_prop.who_first = prop->GetProperty(PROP_PLAYER)->GetValue().GetAny().As<int>();

	int time = (int)prop->GetProperty(PROP_TIME)->GetValue().GetLong();
	CalculateTimes(time);

	game_prop.infinity = prop->GetProperty(PROP_TIME_INF)->GetValue().GetBool();

	game_prop.is_bonus = prop->GetProperty(PROP_BONUS_TIME)->GetValue().GetBool();
	game_prop.bonus_c = prop->GetProperty(PROP_BONUS_COL)->GetValue().GetAny().As<wxColour>();
	game_prop.bonus_time = prop->GetProperty(PROP_BONUS)->GetValue().GetLong();

	game_prop.font = prop->GetProperty(PROP_WATCH_FONT)->GetValue().GetAny().As<wxFont>();
	game_prop.start_sound = prop->GetProperty(PROP_WATCH_START)->GetValueAsString();
	game_prop.stop_sound = prop->GetProperty(PROP_WATCH_STOP)->GetValueAsString();

	game_prop.player1.name = prop->GetProperty(PROP_P1_NAME)->GetValueAsString();
	game_prop.player1.avatar = prop->GetProperty(PROP_P1_IMG)->GetValueAsString();
	game_prop.player1.col = prop->GetProperty(PROP_P1_COL)->GetValue().GetAny().As<wxColour>();

	game_prop.player2.name = prop->GetProperty(PROP_P2_NAME)->GetValueAsString();
	game_prop.player2.avatar = prop->GetProperty(PROP_P2_IMG)->GetValueAsString();
	game_prop.player2.col = prop->GetProperty(PROP_P2_COL)->GetValue().GetAny().As<wxColour>();

	//
	DoClear();
}

//---------------------------------------------------------------------------
// Function calculates game times depending on the selected option
//---------------------------------------------------------------------------
void MyFrame::CalculateTimes(int time)
{
	switch (time)
	{
	case 0: game_prop.time = 1 * 60; break;
	case 1: game_prop.time = 5 * 60; break;
	case 2: game_prop.time = 10 * 60; break;
	case 3: game_prop.time = 15 * 60; break;
	case 4: game_prop.time = 20 * 60; break;
	case 5: game_prop.time = 30 * 60; break;
	case 6: game_prop.time = 60 * 60; break;
	}
}
