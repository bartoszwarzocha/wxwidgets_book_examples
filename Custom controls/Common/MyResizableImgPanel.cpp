/////////////////////////////////////////////////////////////////////////////
// Name:        MyResizableImgPanel.cpp
// Purpose:     My wxWidgets app
// Author:      Name Surname <name.surname@email.org>
// Created:     2025-01-01
// Copyright:   (c) 2025 by Name Surname
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#include "MyResizableImgPanel.h"

wxDEFINE_EVENT(wxEVT_MYPANEL_RESIZED, MyResizableImgPanelEvent);

// RTTI
wxIMPLEMENT_DYNAMIC_CLASS(MyResizableImgPanelEvent, wxCommandEvent);
wxIMPLEMENT_DYNAMIC_CLASS(MyResizableImgPanel, wxPanel);

MyResizableImgPanel::MyResizableImgPanel(wxWindow* parent, wxWindowID id, wxBitmap bmp)
{
	Create(parent, id, bmp);
}

void MyResizableImgPanel::Init()
{
	w = -1;
	h = -1;

	Bind(wxEVT_PAINT, &MyResizableImgPanel::OnPaint, this, wxID_ANY);
	Bind(wxEVT_SIZE, &MyResizableImgPanel::OnSize, this, wxID_ANY);
}

bool MyResizableImgPanel::Create(wxWindow* parent, wxWindowID id, wxBitmap bmp)
{
	if (!wxPanel::Create(parent, id))
	{
		return false;
	}

	Init();

	image = bmp.ConvertToImage();

	return true;
}

MyResizableImgPanel::~MyResizableImgPanel()
{
	Unbind(wxEVT_PAINT, &MyResizableImgPanel::OnPaint, this, wxID_ANY);
	Unbind(wxEVT_SIZE, &MyResizableImgPanel::OnSize, this, wxID_ANY);
}

void MyResizableImgPanel::OnPaint(wxPaintEvent & WXUNUSED(event))
{
	wxPaintDC dc(this);
	DoDraw(dc);
}

void MyResizableImgPanel::PaintNow()
{
	wxClientDC dc(this);
	DoDraw(dc);
}

void MyResizableImgPanel::DoDraw(wxDC&  dc)
{
	int neww, newh;
	dc.GetSize(&neww, &newh);

	if (neww != w || newh != h)
	{
		resized = wxBitmap(image.Scale(neww, newh, wxIMAGE_QUALITY_HIGH));
		w = neww;
		h = newh;
		dc.DrawBitmap(resized, 0, 0, false);
	}
	else
	{
		dc.DrawBitmap(resized, 0, 0, false);
	}
}

void MyResizableImgPanel::OnSize(wxSizeEvent& event)
{
	PaintNow();

	MyResizableImgPanelEvent evt(wxEVT_MYPANEL_RESIZED, this->GetId());
	evt.SetEventObject(this);
	ProcessWindowEvent(evt);

	event.Skip();
}

void MyResizableImgPanel::ChangeImage(wxBitmap bmp)
{
	image = bmp.ConvertToImage();
	this->PaintNow();
}
