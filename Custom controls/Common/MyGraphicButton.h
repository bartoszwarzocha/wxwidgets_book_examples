/////////////////////////////////////////////////////////////////////////////
// Name:        MyGraphicButton.h
// Purpose:     My wxWidgets app
// Author:      Name Surname <name.surname@email.org>
// Created:     2025-01-01
// Copyright:   (c) 2025 by Name Surname
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _MYGRAPHICBTN_H__
#define _MYGRAPHICBTN_H__

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

// Definition of MyGraphicButtonEvent
class MyGraphicButtonEvent : public wxCommandEvent
{
public:
	MyGraphicButtonEvent(wxEventType EventType = wxEVT_NULL, int id = wxID_ANY)
		: wxCommandEvent(EventType, id) {
	}

	wxEvent* Clone() const { return new MyGraphicButtonEvent(*this); }

private:
	wxDECLARE_DYNAMIC_CLASS(MyGraphicButtonEvent);
};

wxDECLARE_EVENT(wxEVT_MYGBUTTON_ACTIVATED, MyGraphicButtonEvent);
wxDECLARE_EVENT(wxEVT_MYGBUTTON_HOVER, MyGraphicButtonEvent);
wxDECLARE_EVENT(wxEVT_MYGBUTTON_HOVER_END, MyGraphicButtonEvent);
wxDECLARE_EVENT(wxEVT_MYGBUTTON_CLICK, MyGraphicButtonEvent);

typedef void (wxEvtHandler::* MyGraphicButtonEventFunction)(MyGraphicButtonEvent&);
#define MyGraphicButtonEventHandler(func) wxEVENT_HANDLER_CAST(MyGraphicButtonEventFunction, func)                    

#define EVT_MYGBUTTON_ACTIVATED(id, func) \
	wx__DECLARE_EVT1(wxEVT_MYGBUTTON_ACTIVATED, id, MyGraphicButtonEventHandler(func))
#define EVT_MYGBUTTON_HOVER(id, func) \
	wx__DECLARE_EVT1(wxEVT_MYGBUTTON_HOVER, id, MyGraphicButtonEventHandler(func))
#define EVT_MYGBUTTON_HOVER_END(id, func) \
	wx__DECLARE_EVT1(wxEVT_MYGBUTTON_HOVER_END, id, MyGraphicButtonEventHandler(func))
#define EVT_MYGBUTTON_CLICK(id, func) \
	wx__DECLARE_EVT1(wxEVT_MYGBUTTON_CLICK, id, MyGraphicButtonEventHandler(func))

#define EVT_MYGBUTTONRANGE_ACTIVATED(id1, id2, func) \
	wx__DECLARE_EVT2(wxEVT_MYGBUTTON_ACTIVATED, id1, id2, MyGraphicButtonEventHandler(func))
#define EVT_MYGBUTTONRANGE_HOVER(id1, id2, func) \
	wx__DECLARE_EVT2(wxEVT_MYGBUTTON_HOVER, id1, id2, MyGraphicButtonEventHandler(func))
#define EVT_MYGBUTTONRANGE_HOVER_END(id1, id2, func) \
	wx__DECLARE_EVT2(wxEVT_MYGBUTTON_HOVER_END, id1, id2, MyGraphicButtonEventHandler(func))
#define EVT_MYGBUTTONRANGE_CLICK(id1, id2, func) \
	wx__DECLARE_EVT2(wxEVT_MYGBUTTON_CLICK, id1, id2, MyGraphicButtonEventHandler(func))

// MyGraphicButton class
class MyGraphicButton : public wxControl
{
public:
	MyGraphicButton() { Init(); }
	MyGraphicButton(wxWindow* parent, wxWindowID id, wxPoint pos, wxBitmap normal, wxBitmap hover = wxNullBitmap, wxBitmap activate = wxNullBitmap);
	virtual ~MyGraphicButton();

	bool Create(wxWindow* parent, wxWindowID id, wxPoint pos, wxBitmap normal, wxBitmap hover = wxNullBitmap, wxBitmap activate = wxNullBitmap);

	void SetBackground(const wxBitmap& bkg);

protected:
	virtual wxSize DoGetBestSize() const {
		if (n.IsOk()) return n.GetSize();
		else return wxSize(64, 64);
	}

	void OnHover(wxMouseEvent& event);
	void OnHoverEnd(wxMouseEvent& event);
	void OnClick(wxMouseEvent& event);
	void OnActivate(wxMouseEvent& event);

	void Init();

	void PaintNow();
	void OnPaint(wxPaintEvent& evt);
	void DoDraw(wxDC& dc);

private:
	enum image
	{
		IMG_NORMAL,
		IMG_HOVER,
		IMG_ACTIV
	};

	wxBitmap n, h, a;
	wxBitmap background;
	image type;

	wxDECLARE_DYNAMIC_CLASS(MyGraphicButton);
};

#endif

