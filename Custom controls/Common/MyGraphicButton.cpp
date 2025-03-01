/////////////////////////////////////////////////////////////////////////////
// Name:        MyGraphicButton.cpp
// Purpose:     My wxWidgets app
// Author:      Name Surname <name.surname@email.org>
// Created:     2025-01-01
// Copyright:   (c) 2025 by Name Surname
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#include "MyGraphicButton.h"

wxDEFINE_EVENT(wxEVT_MYGBUTTON_ACTIVATED, MyGraphicButtonEvent);
wxDEFINE_EVENT(wxEVT_MYGBUTTON_HOVER, MyGraphicButtonEvent);
wxDEFINE_EVENT(wxEVT_MYGBUTTON_HOVER_END, MyGraphicButtonEvent);
wxDEFINE_EVENT(wxEVT_MYGBUTTON_CLICK, MyGraphicButtonEvent);

// RTTI
wxIMPLEMENT_DYNAMIC_CLASS(MyGraphicButtonEvent, wxCommandEvent);
wxIMPLEMENT_DYNAMIC_CLASS(MyGraphicButton, wxControl);

MyGraphicButton::MyGraphicButton(wxWindow* parent, wxWindowID id, wxPoint pos, wxBitmap normal, wxBitmap hover, wxBitmap activate)
{
	Create(parent, id, pos, normal, hover, activate);
}

void MyGraphicButton::Init()
{
	this->n = wxNullBitmap;
	this->h = wxNullBitmap;
	this->a = wxNullBitmap;

	this->background = wxNullBitmap;

	this->type = IMG_NORMAL;

	Bind(wxEVT_ENTER_WINDOW, &MyGraphicButton::OnHover, this, wxID_ANY);
	Bind(wxEVT_LEAVE_WINDOW, &MyGraphicButton::OnHoverEnd, this, wxID_ANY);
	Bind(wxEVT_LEFT_DOWN, &MyGraphicButton::OnClick, this, wxID_ANY);
	Bind(wxEVT_LEFT_UP, &MyGraphicButton::OnActivate, this, wxID_ANY);
	Bind(wxEVT_PAINT, &MyGraphicButton::OnPaint, this, wxID_ANY);
}

bool MyGraphicButton::Create(wxWindow* parent, wxWindowID id, wxPoint pos, wxBitmap normal, wxBitmap hover, wxBitmap activate)
{
	if (!wxControl::Create(parent, id, pos, wxDefaultSize, wxBORDER_NONE))
	{
		return false;
	}

	Init();

	this->n = normal;
	this->h = hover;
	this->a = activate;

	this->SetSize(normal.GetSize());

	return true;
}

MyGraphicButton::~MyGraphicButton()
{
	Unbind(wxEVT_ENTER_WINDOW, &MyGraphicButton::OnHover, this, wxID_ANY);
	Unbind(wxEVT_LEAVE_WINDOW, &MyGraphicButton::OnHoverEnd, this, wxID_ANY);
	Unbind(wxEVT_LEFT_DOWN, &MyGraphicButton::OnClick, this, wxID_ANY);
	Unbind(wxEVT_LEFT_UP, &MyGraphicButton::OnActivate, this, wxID_ANY);
	Unbind(wxEVT_PAINT, &MyGraphicButton::OnPaint, this, wxID_ANY);
}

void MyGraphicButton::OnHover(wxMouseEvent& WXUNUSED(event))
{
	if (!wxGetMouseState().LeftIsDown() && !h.IsNull())
	{
		this->type = IMG_HOVER;
	}
	else if (wxGetMouseState().LeftIsDown() && !a.IsNull())
	{
		this->type = IMG_ACTIV;
	}

	PaintNow();

	MyGraphicButtonEvent event(wxEVT_MYGBUTTON_HOVER, this->GetId());
	event.SetEventObject(this);
	ProcessWindowEvent(event);
}

//! The cursor leaves the "button"
void MyGraphicButton::OnHoverEnd(wxMouseEvent& WXUNUSED(event))
{
	if (!n.IsNull()) this->type = IMG_NORMAL;

	PaintNow();

	MyGraphicButtonEvent event(wxEVT_MYGBUTTON_HOVER_END, this->GetId());
	event.SetEventObject(this);
	ProcessWindowEvent(event);
}

//! The "button" is clicked (LPM down);
void MyGraphicButton::OnClick(wxMouseEvent& WXUNUSED(event))
{
	if (!a.IsNull()) this->type = IMG_ACTIV;

	PaintNow();

	MyGraphicButtonEvent event(wxEVT_MYGBUTTON_CLICK, this->GetId());
	event.SetEventObject(this);
	ProcessWindowEvent(event);
}

//! The "button" is activated (LPM up);
void MyGraphicButton::OnActivate(wxMouseEvent& WXUNUSED(event))
{
	if (!h.IsNull()) this->type = IMG_HOVER;

	PaintNow();

	MyGraphicButtonEvent evt(wxEVT_MYGBUTTON_ACTIVATED, this->GetId());
	evt.SetEventObject(this);
	ProcessEvent(evt);
}

void MyGraphicButton::SetBackground(const wxBitmap& b)
{
	this->background = b.GetSubBitmap(this->GetRect());

	PaintNow();
}

void MyGraphicButton::OnPaint(wxPaintEvent & WXUNUSED(event))
{
	wxPaintDC dc(this);
	DoDraw(dc);
}

void MyGraphicButton::PaintNow()
{
	wxClientDC dc(this);
	DoDraw(dc);
}

void MyGraphicButton::DoDraw(wxDC& dc)
{
	dc.SetBackground(*wxTRANSPARENT_BRUSH);
	dc.Clear();

	if (!this->background.IsNull())
	{
		dc.DrawBitmap(this->background, 0, 0, false);
	}

	switch (type)
	{
	case IMG_NORMAL: dc.DrawBitmap(n, 0, 0, true); break;
	case IMG_HOVER: dc.DrawBitmap(h, 0, 0, false); break;
	case IMG_ACTIV: dc.DrawBitmap(a, 0, 0, false); break;
	default: dc.DrawBitmap(n, 0, 0, false); break;
	}
}
