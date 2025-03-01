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
#include "MyComboList.h"

#include "main.h"

const wxString appName = wxT("Combo list");

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
    ID_COMBO_LIST
};

//---------------------------------------------------------------------------
// Event table
//---------------------------------------------------------------------------
BEGIN_EVENT_TABLE(MyFrame, wxFrame)
EVT_MENU(ID_CLOSE, MyFrame::OnClose)
EVT_MENU(ID_ABOUT_PROGRAM, MyFrame::OnAboutProgram)
EVT_CLOSE(MyFrame::CloseApp)
//
EVT_LIST_ITEM_SELECTED(ID_COMBO_LIST, MyFrame::OnComboList)
END_EVENT_TABLE()

//---------------------------------------------------------------------------
// OnInit()
//---------------------------------------------------------------------------
bool MyApp::OnInit()
{
    wxInitAllImageHandlers();

    MyFrame* frame = new MyFrame(NULL, wxID_ANY, appName, wxDefaultPosition, wxSize(600, 400), wxDEFAULT_FRAME_STYLE);
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
    mFile->Append(ID_CLOSE, wxT("&Close program\tCtrl-X"), wxT("Closes the program"));

    mHelp->Append(ID_ABOUT_PROGRAM, wxT("&About\tF1"), wxT("About the program"));

    mb->Append(mFile, wxT("&File"));
    mb->Append(mHelp, wxT("H&elp"));

    this->SetMenuBar(mb);

    sb = new wxStatusBar(this);
    sb->SetFieldsCount(2);
    this->SetStatusBar(sb);
    SetStatusText(title, 0);

    panel = new wxPanel(this);
    wxBoxSizer* ps = new wxBoxSizer(wxVERTICAL);
    panel->SetSizer(ps);

    // Step 1. LIST FOR COMBO CONTROL
    list = new MyComboList(ID_COMBO_LIST);

    // Step 2. COMBO CONTROL
    combo = new wxComboCtrl(panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    combo->SetPopupMaxHeight(250);
    combo->SetPopupMinWidth(450);
    combo->SetPopupControl(list);
    ps->Add(combo, 0, wxALL, 4);

    // Step 3. FILL THE LIST WITH DATA
    wxListItem list_item;

    // Columns
    list_item.SetText(_T("Surname")); // 0
    list->InsertColumn(0, list_item);
    list->SetColumnWidth(0, 120);

    list_item.SetText(_T("Name")); // 1
    list->InsertColumn(1, list_item);
    list->SetColumnWidth(1, 100);

    list_item.SetText(_T("Phone")); // 2
    list->InsertColumn(2, list_item);
    list->SetColumnWidth(2, 100);

    // Add sample data
    for (int i = 0; i < 25; i++)
    {
        list->InsertItem(i, wxString::Format(wxT("Surname_%d"), i + 1), -1);
        list->SetItem(i, 1, wxString::Format(wxT("Name_%d"), i + 1), -1);
        list->SetItem(i, 2, wxString::Format(wxT("800 100 %d"), i + 199), -1);
    }

    CenterOnScreen();
}

//---------------------------------------------------------------------------
// MyFrame destructor
//---------------------------------------------------------------------------
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
// Function displays "About the program..." window
//---------------------------------------------------------------------------
void MyFrame::OnAboutProgram(wxCommandEvent& event)
{
    wxMessageBox(appName + wxT("\nCopyright (c) 2025 by ..."), wxT("About the program..."), wxOK | wxICON_INFORMATION);

    wxMessageBox(
        wxT("GetAppDocumentsDir(): " + wxStandardPaths::Get().GetAppDocumentsDir() + "\n\n")
        + wxT("GetConfigDir(): " + wxStandardPaths::Get().GetConfigDir() + "\n\n")
        + wxT("GetDataDir(): " + wxStandardPaths::Get().GetDataDir() + "\n\n")
        + wxT("GetDocumentsDir(): " + wxStandardPaths::Get().GetDocumentsDir() + "\n\n")
        + wxT("GetExecutablePath(): " + wxStandardPaths::Get().GetExecutablePath() + "\n\n")
#if defined(__WXGTK__)
        +wxT("GetInstallPrefix(): " + wxStandardPaths::Get().GetInstallPrefix() + "\n\n")
#endif
        + wxT("GetLocalDataDir(): " + wxStandardPaths::Get().GetLocalDataDir() + "\n\n")
        + wxT("GetPluginsDir(): " + wxStandardPaths::Get().GetPluginsDir() + "\n\n")
        + wxT("GetResourcesDir(): " + wxStandardPaths::Get().GetResourcesDir() + "\n\n")
        + wxT("GetTempDir(): " + wxStandardPaths::Get().GetTempDir() + "\n\n")
        + wxT("GetUserConfigDir(): " + wxStandardPaths::Get().GetUserConfigDir() + "\n\n")
        + wxT("GetUserDataDir(): " + wxStandardPaths::Get().GetUserDataDir() + "\n\n")
        + wxT("GetUserLocalDataDir(): " + wxStandardPaths::Get().GetUserLocalDataDir() + "\n\n"),
        wxEmptyString,
        wxOK
    );

    event.Skip();
}

//---------------------------------------------------------------------------
// Function displays a prompt to close the program
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

//---------------------------------------------------------------------------
// Function responds to combo list selection
//---------------------------------------------------------------------------
void MyFrame::OnComboList(wxListEvent& WXUNUSED(event))
{
    // Compose message
    wxString msg;
    wxString msg0 = list->GetStringSelection(0);
    wxString msg1 = list->GetStringSelection(1);
    wxString msg2 = list->GetStringSelection(2);

    msg = (msg0 + " " + msg1 + ", Tel: " + msg2);

    // Set combo control text
    combo->SetText(msg0);

    // Display message
    wxMessageBox(msg);
}

