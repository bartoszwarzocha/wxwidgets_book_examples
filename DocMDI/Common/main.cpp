/////////////////////////////////////////////////////////////////////////////
// Name:        main.cpp
// Purpose:     My wxWidgets app
// Author:      Imie Nazwisko <imie.nazwisko@email.org>
// Created:     2018-01-01
// Copyright:   (c) 2018 by Imie Nazwisko
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <iostream>

#include <wx/mdi.h>
#include <wx/docmdi.h> // wxDocument
#include <wx/tokenzr.h>
#include <wx/stream.h>
#include <wx/sstream.h>
#include <wx/txtstrm.h>
#include <wx/app.h>

#include "main.h"

const wxString appName = wxT("DocMDI 1.0.0");

//---------------------------------------------------------------------------
// Application implementation
//---------------------------------------------------------------------------
IMPLEMENT_APP(MyApp)

wxIMPLEMENT_DYNAMIC_CLASS(MyDocument, wxDocument)
wxIMPLEMENT_DYNAMIC_CLASS(MyView, wxView)

//---------------------------------------------------------------------------
// Identifiers
//---------------------------------------------------------------------------
enum
{
    ID_CLOSE = wxID_HIGHEST + 1,
    ID_ABOUT_PROGRAM,
    ID_ADD_WND_TXT,
    ID_ADD_WND_DOC,
    ID_DEL_WND,
};

//---------------------------------------------------------------------------
// Event table
//---------------------------------------------------------------------------
BEGIN_EVENT_TABLE(MyFrame, wxMDIParentFrame)
EVT_MENU(ID_CLOSE, MyFrame::OnClose)
EVT_MENU(ID_ABOUT_PROGRAM, MyFrame::OnAboutProgram)
EVT_CLOSE(MyFrame::CloseApp)
EVT_MENU_RANGE(ID_ADD_WND_TXT, ID_DEL_WND, MyFrame::OnManage)
END_EVENT_TABLE()

//---------------------------------------------------------------------------
// OnInit()
//---------------------------------------------------------------------------
bool MyApp::OnInit()
{
    wxInitAllImageHandlers();

    // POLISH LANGUAGE FOR MDI WINDOW MENU
	// DISABLED FOR NOW
    //loc.Init(wxLANGUAGE_POLISH, wxLOCALE_LOAD_DEFAULT);
    //loc.AddCatalog("pl");

    // Document manager
    MyDocManager* man = new MyDocManager();

    // Relationships between document and view
    new wxDocTemplate(man, "GDO", "*.gdo", "", "gdo", "Graphics Document Object", "Graphics Document Object", CLASSINFO(MyDocument), CLASSINFO(MyView));

    MyFrame* frame = new MyFrame(man, NULL, wxID_ANY, appName, wxDefaultPosition, wxSize(600, 400), wxDEFAULT_FRAME_STYLE);
    frame->Show(true);
    return true;
}

int MyApp::OnExit()
{
    wxDocManager* man = wxDocManager::GetDocumentManager();
    delete man;
    return wxApp::OnExit();
}

//---------------------------------------------------------------------------
// MyFrame constructor
//---------------------------------------------------------------------------
MyFrame::MyFrame(MyDocManager* man, wxFrame* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
    : wxDocMDIParentFrame((wxDocManager*)man, parent, id, title, pos, size, style), counter(4), manager(man)
{
    manager->SetFrame(this);

    wxMenuBar* mb = new wxMenuBar();

    wxMenu* mFile = new wxMenu();
    wxMenu* mMan = new wxMenu();
    wxMenu* mHelp = new wxMenu();

    mFile->AppendSeparator();
    mFile->Append(ID_CLOSE, wxT("&Close program\tCtrl-X"), wxT("Closes the program"));

    mMan->Append(ID_ADD_WND_TXT, wxT("Add new text window\tCtrl-N"), wxT("Adds a new text window"));
    mMan->Append(ID_ADD_WND_DOC, wxT("Add new wxDocument window\tAlt-N"), wxT("Adds a new wxDocument window"));
    mMan->Append(ID_DEL_WND, wxT("Delete window\tCtrl-D"), wxT("Deletes the current window"));

    mHelp->Append(ID_ABOUT_PROGRAM, wxT("&About program\tF1"), wxT("About the program"));

    mb->Append(mFile, wxT("&File"));
    mb->Append(mMan, wxT("&Management"));
    mb->Append(mHelp, wxT("&Help"));

    this->SetMenuBar(mb);

    wxStatusBar* sb = new wxStatusBar(this);
    sb->SetFieldsCount(2);
    this->SetStatusBar(sb);
    SetStatusText(title, 0);

    //
    MyTextFrame* child1 = new MyTextFrame(this, wxID_ANY, wxT("Text 1"));
    MyTextFrame* child2 = new MyTextFrame(this, wxID_ANY, wxT("Text 2"));
    MyTextFrame* child3 = new MyTextFrame(this, wxID_ANY, wxT("Text 3"));

    child1->Show();
    child2->Show();
    child3->Show();

    // wxDocument
    manager->CreateDocument(wxT("document_test.gdo"), wxDOC_SILENT);

    this->Cascade();

    CenterOnScreen();
}

//---------------------------------------------------------------------------
// Function initiates program closure
//---------------------------------------------------------------------------
void MyFrame::OnClose(wxCommandEvent& WXUNUSED(event))
{
    this->Close(true);
}

//---------------------------------------------------------------------------
// Function displays "About program..." window
//---------------------------------------------------------------------------
void MyFrame::OnAboutProgram(wxCommandEvent& event)
{
    wxMessageBox(appName + wxT("\nCopyright (c) 2025 by ..."), wxT("About program..."), wxOK | wxICON_INFORMATION);
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

//---------------------------------------------------------------------------
// Function handles window management
//---------------------------------------------------------------------------
void MyFrame::OnManage(wxCommandEvent& event)
{
    MyTextFrame* tf = NULL;

    switch (event.GetId())
    {
    case ID_ADD_WND_TXT:
        tf = new MyTextFrame(this, wxID_ANY, wxString::Format(wxT("Text %d"), counter));
        tf->Show();
        tf->Activate();
        counter++;
        break;

    case ID_ADD_WND_DOC:
        manager->CreateNewDocument();
        counter++;
        break;

    case ID_DEL_WND:
        tf = (MyTextFrame*)GetActiveChild();
        wxDELETE(tf);
        break;
    }
}
