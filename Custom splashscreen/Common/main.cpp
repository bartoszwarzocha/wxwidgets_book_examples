////////////////////////////////////////////////////////////////////////////////
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

#include "MySplashScreen.h"
#include "main.h"

const wxString appName = wxT("Custom Splash Screen 1.0.0");

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
};

//---------------------------------------------------------------------------
// Event table
//---------------------------------------------------------------------------
BEGIN_EVENT_TABLE(MyFrame, wxFrame)
EVT_MENU(ID_CLOSE, MyFrame::OnClose)
EVT_MENU(ID_ABOUT_PROGRAM, MyFrame::OnAboutProgram)
EVT_CLOSE(MyFrame::CloseApp)
END_EVENT_TABLE()

//---------------------------------------------------------------------------
// OnInit()
//---------------------------------------------------------------------------
bool MyApp::OnInit()
{
    wxInitAllImageHandlers();

    wxBitmap splash_bmp(wxT("gfx/splash.png"), wxBITMAP_TYPE_PNG);
    MySplashScreen* sscreen = new MySplashScreen(splash_bmp, wxSPLASH_CENTRE_ON_SCREEN | wxSPLASH_NO_TIMEOUT, -1, NULL, wxID_ANY);
    sscreen->AddLabelField(wxPoint(20, 170), wxFont(20, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL), *wxWHITE);
    sscreen->AddLabelField(wxPoint(20, 250), wxFont(12, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL), *wxLIGHT_GREY);
    sscreen->Show();

    sscreen->SetLabel(0, wxT("Loading application modules,\nplease wait..."));

    // Emulate loading data into the program
    wxString dot = wxT(".");
    wxSleep(1);
    for (int i = 0; i <= 100; i++)
    {
        sscreen->SetLabel(1, wxString::Format("Progress: %02d%% %s", i, dot));
        wxMilliSleep(33);
        dot += wxT(".");
    }
    wxSleep(1);

    // Destroy the splash screen
    sscreen->Destroy();

    MyFrame* frame = new MyFrame(NULL, wxID_ANY, appName, wxDefaultPosition, wxSize(800, 600), wxDEFAULT_FRAME_STYLE);
    frame->Show(true);
    return true;
}

//---------------------------------------------------------------------------
// MyFrame constructor
//---------------------------------------------------------------------------
MyFrame::MyFrame(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
    : wxFrame(parent, id, title, pos, size, style)
{
    mb = new wxMenuBar();

    mFile = new wxMenu();
    mHelp = new wxMenu();


    mFile->AppendSeparator();
    mFile->Append(ID_CLOSE, wxT("&Close Program\tCtrl-X"), wxT("Closes the program"));

    mHelp->Append(ID_ABOUT_PROGRAM, wxT("&About Program\tF1"), wxT("About the program"));

    mb->Append(mFile, wxT("&File"));
    mb->Append(mHelp, wxT("&Help"));

    this->SetMenuBar(mb);

    sb = new wxStatusBar(this);
    sb->SetFieldsCount(2);
    this->SetStatusBar(sb);
    SetStatusText(title, 0);

    panel = new wxPanel(this);

    CenterOnScreen();
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
// Function displays "About Program..." window
//---------------------------------------------------------------------------
void MyFrame::OnAboutProgram(wxCommandEvent& event)
{
    wxMessageBox(appName + wxT("\nCopyright (c) 2025 by ..."), wxT("About Program..."), wxOK | wxICON_INFORMATION);

    event.Skip();
}

//---------------------------------------------------------------------------
// Function displays a question about closing the program
//---------------------------------------------------------------------------
void MyFrame::CloseApp(wxCloseEvent& WXUNUSED(event))
{
    int action = wxMessageBox(wxT("Do you want to close the program?"), wxT("Question"), wxOK | wxCANCEL | wxICON_QUESTION);

    if (action == wxOK)
    {
        this->Destroy();
    }
    else
    {
        return;
    }
}
