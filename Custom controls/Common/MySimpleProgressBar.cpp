/////////////////////////////////////////////////////////////////////////////
// Name:        MySimpleProgressBar.cpp
// Purpose:     My wxWidgets app
// Author:      Name Surname <name.surname@email.org>
// Created:     2025-01-01
// Copyright:   (c) 2025 by Name Surname
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#include "MySimpleProgressBar.h"

wxIMPLEMENT_DYNAMIC_CLASS(MySimpleProgressBar, wxControl);

MySimpleProgressBar::MySimpleProgressBar(wxWindow* parent, wxWindowID id, wxPoint pos, wxSize size, int style)
{
	Create(parent, id, pos, size, style);
}

void MySimpleProgressBar::Init()
{
	value = 0;
	range = 100;

	col1 = *wxGREEN;
	col2 = *wxRED;

	is_back = false;

	Bind(wxEVT_PAINT, &MySimpleProgressBar::OnPaint, this, wxID_ANY);
	Bind(wxEVT_SIZE, &MySimpleProgressBar::OnSize, this, wxID_ANY);
}

bool MySimpleProgressBar::Create(wxWindow* parent, wxWindowID id, wxPoint pos, wxSize size, int style)
{
	if (!wxControl::Create(parent, id, pos, size, style))
	{
		return false;
	}

	Init();

	this->styles = style;
	this->SetMaxSize(size);

	return true;
}

MySimpleProgressBar::~MySimpleProgressBar()
{
	Unbind(wxEVT_PAINT, &MySimpleProgressBar::OnPaint, this, wxID_ANY);
	Unbind(wxEVT_SIZE, &MySimpleProgressBar::OnSize, this, wxID_ANY);
}

void MySimpleProgressBar::OnPaint(wxPaintEvent & WXUNUSED(event))
{
	wxPaintDC dc(this);
	DoDraw(dc);
}

void MySimpleProgressBar::OnSize(wxSizeEvent& event)
{
	is_back = false;
	PaintNow();

	event.Skip();
}

void MySimpleProgressBar::PaintNow()
{
	wxClientDC dc(this);
	DoDraw(dc);
}

void MySimpleProgressBar::DoDraw(wxDC&  dc)
{
	int w, h, c; // width, height, coordinate
	dc.GetSize(&w, &h);

	// Bar coordinates
	int x, y, fx, fy;
	if (styles & PBAR_WITH_MARGIN)
	{
		x = 3;
		y = 3;
		fx = w - (2 * 3);
		fy = h - (2 * 3);
	}
	else
	{
		x = 0;
		y = 0;
		fx = w;
		fy = h;
	}

	// Calculate the position of the value
	double W = static_cast<double>(this->value) / static_cast<double>(this->range);
	if (styles & PBAR_VERTICAL) // Horizontal
	{
		c = (fy + y) - (fy * W);
	}
	else // Vertical
	{
		c = (fx * W) + x;
	}

	// Background
	dc.SetBackground(wxBrush(this->GetBackgroundColour()));
	dc.Clear();

	// Margin
	if (styles & PBAR_WITH_MARGIN)
	{
		dc.SetPen(wxPen(this->GetForegroundColour(), 1));
		
		//dc.DrawRectangle(1, 1, w - 2, h - 2);

		dc.DrawLine(1, 1, 1, h - 2);
		dc.DrawLine(1, h - 2, w - 2, h - 2);
		dc.DrawLine(w - 2, h - 2, w - 2, 1);
		dc.DrawLine(w - 2, 1, 1, 1);
	}

	if (styles & PBAR_MONO_COLOR) // One color bar
	{
		dc.SetPen(wxPen(col1));
		dc.SetBrush(wxBrush(col1));
		if (styles & PBAR_VERTICAL)
		{
			dc.DrawRectangle(x, c, fx, (y + fy) - c);
		}
		else
		{
			dc.DrawRectangle(x, y, c, fy);
		}
	}
	else // Gradient bar
	{
		int R, G, B;

		// Color calculation
		R = (col2.Red() * W) + (col1.Red() * (1 - W));
		G = (col2.Green() * W) + (col1.Green() * (1 - W));
		B = (col2.Blue() * W) + (col1.Blue() * (1 - W));
		wxColour c_col(R, G, B);

		int pixel = 0;

		if (styles & PBAR_VERTICAL)
		{
			for (int i = (y + fy) - 1; i >= c; i--)
			{
				double Wi = ((double)pixel / ((fy + y) - c));
				R = (c_col.Red() * Wi) + (col1.Red() * (1 - Wi));
				G = (c_col.Green() * Wi) + (col1.Green() * (1 - Wi));
				B = (c_col.Blue() * Wi) + (col1.Blue() * (1 - Wi));

				dc.SetPen(wxPen(wxColour(R, G, B)));
				dc.DrawLine(x, i, x + fx, i);

				pixel++;
			}
		}
		else
		{
			for (int i = x; i < c; i++)
			{
				double Wi = ((double)pixel / (c - x));
				R = (c_col.Red() * Wi) + (col1.Red() * (1 - Wi));
				G = (c_col.Green() * Wi) + (col1.Green() * (1 - Wi));
				B = (c_col.Blue() * Wi) + (col1.Blue() * (1 - Wi));

				dc.SetPen(wxPen(wxColour(R, G, B)));
				dc.DrawLine(i, y, i, y + fy);

				pixel++;
			}
		}
	}
}
