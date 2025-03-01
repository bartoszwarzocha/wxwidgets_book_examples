/////////////////////////////////////////////////////////////////////////////
// Name:        MyResizableImgPanel.h
// Purpose:     My wxWidgets app
// Author:      Name Surname <name.surname@email.org>
// Created:     2025-01-01
// Copyright:   (c) 2025 by Name Surname
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _MYSIMPLEPROGRESSBAR_H__
#define _MYSIMPLEPROGRESSBAR_H__

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

enum MySimplePogressBarStyles
{
	PBAR_HORIZONTAL    = 0x00000001,
	PBAR_VERTICAL      = 0x00000002,
	PBAR_WITH_MARGIN   = 0x00000004,
	PBAR_MONO_COLOR    = 0x00000008,
	PBAR_DEFAULT_STYLE = (PBAR_HORIZONTAL | wxBORDER_SIMPLE)
};

class MySimpleProgressBar : public wxControl
{
public:
	MySimpleProgressBar() { Init(); }
	MySimpleProgressBar(wxWindow* parent, wxWindowID id, wxPoint pos = wxDefaultPosition, wxSize size = wxDefaultSize, int style = PBAR_DEFAULT_STYLE);
	virtual ~MySimpleProgressBar();

	bool Create(wxWindow* parent, wxWindowID id, wxPoint pos = wxDefaultPosition, wxSize size = wxDefaultSize, int style = PBAR_DEFAULT_STYLE);

	virtual void SetValue(unsigned int val)
	{ 
		if (val > range) val = range;
		this->value = val; 
		if(this->IsShown()) PaintNow();
	}
	
	virtual void SetRange(unsigned int max) 
	{ 
		this->range = max; 
		if(this->IsShown()) PaintNow(); 
	}

	virtual void SetColour1(const wxColour& c)
	{
		col1 = c;
		if(this->IsShown()) PaintNow();
	}

	virtual void SetColour2(const wxColour& c)
	{
		col2 = c;
		if(this->IsShown()) PaintNow();
	}

	virtual unsigned int GetValue() { return this->value; }
	virtual unsigned int GetRange() { return this->range; }
	virtual wxColour GetColour1() { return col1; }
	virtual wxColour GetColour2() { return col2; }
	
protected:
	virtual wxSize DoGetBestSize() const {
		if (styles & PBAR_VERTICAL) return wxSize(25, 150);
		else return wxSize(150, 25);
	}

	void Init();

	void PaintNow();
	void DoDraw(wxDC& dc);
	void OnPaint(wxPaintEvent & evt);
	void OnSize(wxSizeEvent& event);

private:
	int styles;
	unsigned int value;
	unsigned int range;
	wxColour col1;
	wxColour col2;

	bool is_back;

	wxDECLARE_DYNAMIC_CLASS(MySimpleProgressBar);
};

#endif