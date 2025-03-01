/////////////////////////////////////////////////////////////////////////////
// Name:        MyResizableImgPanel.h
// Purpose:     My wxWidgets app
// Author:      Name Surname <name.surname@email.org>
// Created:     2025-01-01
// Copyright:   (c) 2025 by Name Surname
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _MYRESIZABLEIMGPANEL_H__
#define _MYRESIZABLEIMGPANEL_H__

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

// Definition of the MyGraphicButtonEvent
class MyResizableImgPanelEvent : public wxCommandEvent
{
public:
	MyResizableImgPanelEvent(wxEventType EventType = wxEVT_NULL, int id = wxID_ANY)
		:wxCommandEvent(EventType, id) { }

	wxEvent* Clone() const { return new MyResizableImgPanelEvent(*this); }

private:
	wxDECLARE_DYNAMIC_CLASS(MyResizableImgPanelEvent);
};

wxDECLARE_EVENT(wxEVT_MYPANEL_RESIZED, MyResizableImgPanelEvent);

typedef void (wxEvtHandler::*MyResizableImgPanelEventFunction)(MyResizableImgPanelEvent &);
#define MyResizableImgPanelEventHandler(func) wxEVENT_HANDLER_CAST(MyResizableImgPanelEventFunction, func)                    

#define EVT_MYPANEL_RESIZED(id, func) \
	wx__DECLARE_EVT1(wxEVT_MYPANEL_RESIZED, id, MyResizableImgPanelEventHandler(func))
#define EVT_MYPANELRANGE_RESIZED(id1, id2, func) \
	wx__DECLARE_EVT2(wxEVT_MYPANEL_RESIZED, id1, id2, MyResizableImgPanelEventHandler(func))

class MyResizableImgPanel : public wxPanel
{
public:
	MyResizableImgPanel() { Init(); }
	MyResizableImgPanel(wxWindow* parent, wxWindowID id, wxBitmap bmp);
	virtual ~MyResizableImgPanel();

	bool Create(wxWindow* parent, wxWindowID id, wxBitmap bmp);

	void ChangeImage(wxBitmap);
	wxBitmap GetBitmap() { return resized; }

protected:
	void Init();

	void PaintNow();
	void DoDraw(wxDC& dc);
	void OnPaint(wxPaintEvent & evt);
	void OnSize(wxSizeEvent& event);

private:
	wxImage image;
	wxBitmap resized;
	int w, h;

	wxDECLARE_DYNAMIC_CLASS(MyResizableImgPanel);
};

#endif