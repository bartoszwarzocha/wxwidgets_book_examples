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
#include <wx/graphics.h>

#include "main.h"

const wxString appName = wxT("Drawing 1.0.0");

//---------------------------------------------------------------------------
// Application Implementation
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
};

//---------------------------------------------------------------------------
// Event Table
//---------------------------------------------------------------------------
BEGIN_EVENT_TABLE(MyFrame, wxFrame)
EVT_MENU(ID_CLOSE, MyFrame::OnClose)
EVT_MENU(ID_ABOUT_PROGRAM, MyFrame::OnAboutProgram)
EVT_CLOSE(MyFrame::CloseApp)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(MyPanel, wxPanel)
EVT_PAINT(MyPanel::OnPaint)
END_EVENT_TABLE()

//---------------------------------------------------------------------------
// OnInit()
//---------------------------------------------------------------------------
bool MyApp::OnInit()
{
	wxInitAllImageHandlers();

	MyFrame* frame = new MyFrame(NULL, wxID_ANY, appName, wxDefaultPosition, wxSize(285, 445), wxDEFAULT_FRAME_STYLE);
	frame->Show(true);
	return true;
}

//---------------------------------------------------------------------------
// MyFrame Constructor
//---------------------------------------------------------------------------
MyFrame::MyFrame(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
	: wxFrame(parent, id, title, pos, size, style)
{
	mb = new wxMenuBar();

	mFile = new wxMenu();
	mHelp = new wxMenu();

	mFile->Append(ID_CLOSE, wxT("&Close Program\tCtrl-X"), wxT("Closes the program"));

	mHelp->Append(ID_ABOUT_PROGRAM, wxT("&About Program\tF1"), wxT("About the program"));

	mb->Append(mFile, wxT("&File"));
	mb->Append(mHelp, wxT("&Help"));

	this->SetMenuBar(mb);

	sb = new wxStatusBar(this);
	sb->SetFieldsCount(2);
	this->SetStatusBar(sb);
	SetStatusText(title, 0);

	panel = new MyPanel(this);

	CenterOnScreen();
}

//---------------------------------------------------------------------------
// MyFrame Destructor
//-------------------------------------------------------------------------
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
	//int action = wxMessageBox(wxT("Close the program?"), wxT("Question"), wxOK|wxCANCEL|wxICON_QUESTION);

	int action = wxOK;

	if (action == wxOK)
	{
		this->Destroy();
	}
	else
	{
		return;
	}
}

MyPanel::MyPanel(wxWindow* parent)
	: wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize)
{

}

void MyPanel::OnPaint(wxPaintEvent& WXUNUSED(event))
{
	Draw();
	Draw2();
}

void MyPanel::Draw()
{
    wxClientDC dc(this);
    wxGraphicsContext* gc = wxGraphicsContext::Create(dc);

    if (gc)
    {
        gc->SetPen(*wxBLACK_PEN);
        gc->SetBrush(*wxWHITE_BRUSH);

        // Drawing lines
        int size = 200;
        int offsetX = 50;
        int offsetY = offsetX + size;

        for (int i = 0; i < (size + 1); i += (int)(size / 10))
        {
            gc->StrokeLine(i + offsetX, offsetY, offsetX + size, offsetY - i);
        }

        // Drawing circle (using DrawEllipse)
        gc->SetPen(wxPen(*wxBLACK, 5));
        gc->DrawEllipse(100 - 50, 100 - 50, 100, 100);

        gc->SetPen(*wxRED_PEN);

        // Drawing spline using wxGraphicsPath
        wxGraphicsPath path = gc->CreatePath();
        path.MoveToPoint(10, 10);
        path.AddCurveToPoint(20, 10, 30, 20, 40, 10);
        path.AddLineToPoint(50, 30);
        gc->StrokePath(path);

        // Drawing polygons using wxGraphicsPath
        wxGraphicsPath polyPath = gc->CreatePath();
        polyPath.MoveToPoint(0, 0);
        polyPath.AddLineToPoint(10, 0);
        polyPath.AddLineToPoint(10, 30);
        polyPath.AddLineToPoint(0, 30);
        polyPath.AddLineToPoint(0, 20);
        polyPath.AddLineToPoint(30, 20);
        polyPath.AddLineToPoint(30, 30);
        polyPath.AddLineToPoint(20, 30);
        polyPath.AddLineToPoint(20, 0);
        polyPath.AddLineToPoint(30, 0);
        polyPath.AddLineToPoint(30, 10);
        polyPath.AddLineToPoint(0, 10);
        polyPath.CloseSubpath();

        gc->StrokePath(polyPath);

        gc->SetBrush(wxBrush(*wxBLUE, wxBRUSHSTYLE_CROSSDIAG_HATCH));
        gc->Translate(50, 50);
        gc->FillPath(polyPath);
        gc->Translate(-50, -50);

        gc->SetPen(wxPen(*wxBLACK));
        gc->SetBrush(*wxRED_BRUSH);
        gc->Translate(100, 100);
        gc->FillPath(polyPath);
        gc->Translate(-100, -100);

        // Drawing text
        gc->SetPen(wxPen(*wxLIGHT_GREY));
        gc->DrawText("wxWidgets", 20, 290);
        gc->SetPen(wxPen(*wxBLACK));

        wxFont font(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, "Arial");
        if (!font.IsOk()) {
            wxLogError("Font creation failed!");
        } else {
            gc->SetFont(font, *wxBLACK);
        }

        double angles[] = { 15, 30, 45, 60, 75, 90 };
        for (size_t i = 0; i < 6; ++i)
        {
            gc->PushState();
            gc->Translate(30 + i * 40, 330);
            gc->Rotate(wxDegToRad(-angles[i]));
            gc->DrawText("wxWidgets", 0, 0);
            gc->PopState();

        }

        // Resetting pen and brush
        gc->SetPen(wxNullPen);
        gc->SetBrush(wxNullBrush);

        delete gc;
    }
}

void MyPanel::Draw2()
{
    wxClientDC dc(this);

    dc.SetPen(*wxBLACK_PEN);
    dc.SetBrush(*wxWHITE_BRUSH);

    // PolyPolygon
    wxPoint v[6];
    // Triangle 1
    v[0] = wxPoint(100, 50);
    v[1] = wxPoint(150, 150);
    v[2] = wxPoint(50, 150);
    // Triangle 2
    v[3] = wxPoint(100, 60);
    v[4] = wxPoint(140, 140);
    v[5] = wxPoint(60, 140);

    int c[2] = { 3, 3 };

    dc.DrawPolyPolygon(2, c, v, 0, 0);

    // Resetting pen and brush
    dc.SetPen(wxNullPen);
    dc.SetBrush(wxNullBrush);
}
