/////////////////////////////////////////////////////////////////////////////
// Name:        MyCalendar.h
// Purpose:     My wxWidgets app
// Author:      Name Surname <name.surname@email.org>
// Created:     2025-01-01
// Copyright:   (c) 2025 by Name Surname
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _MYGORGANIZER_H__
#define _MYGORGANIZER_H__

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/datectrl.h>

// NameDays class
class NameDays
{
public:
	NameDays() { InitData(); }

	void  GetNamesByMonth(wxDateTime::Month month, wxArrayString* arr);
	wxString GetNamesByDay(wxDateTime::Month month, int day);

private:
	struct NameDay
	{
		NameDay(wxDateTime::Month m, int d, const wxString& n)
			: month(m), day(d), names(n) {
		}

		wxDateTime::Month month;
		int day;
		wxString names;
	};

protected:
	wxVector<NameDay> days;
	void InitData();
};

// MyCalendarEvent class
class MyCalendarEvent : public wxCommandEvent
{
public:
	MyCalendarEvent(wxEventType EventType = wxEVT_NULL, int id = wxID_ANY)
		:wxCommandEvent(EventType, id)
	{
		x_coord = -1;
		y_coord = -1;
		date = wxDefaultDateTime;
		names = wxEmptyString;
	}

	wxEvent* Clone() const { return new MyCalendarEvent(*this); }

	void SetX(int x) { x_coord = x; }
	void SetY(int y) { y_coord = y; }
	void SetPosition(const wxPoint& p) { SetX(p.x); SetY(p.y); }
	void SetDate(const wxDateTime& d) { date = d; }
	void SetNames(const wxString& n) { names = n; }

	int GetX() { return x_coord; }
	int GetY() { return y_coord; }
	wxPoint GetPosition() { return wxPoint(x_coord, y_coord); }
	wxDateTime GetDate() { return date; }
	wxString GetNames() { return names; }

private:
	int x_coord; // Horizontal coordinates
	int y_coord; // Vertical coordinates
	wxDateTime date; // Event date
	wxString names; // Name days

	wxDECLARE_DYNAMIC_CLASS(MyCalendarEvent);
};

wxDECLARE_EVENT(wxEVT_MYCAL_LCLICKED, MyCalendarEvent);
wxDECLARE_EVENT(wxEVT_MYCAL_RCLICKED, MyCalendarEvent);

typedef void (wxEvtHandler::* MyCalendarEventFunction)(MyCalendarEvent&);
#define MyGOrganizerEventHandler(func) wxEVENT_HANDLER_CAST(MyCalendarEventFunction, func)

#define EVT_MYCAL_LCLICKED(id, func) \
	wx__DECLARE_EVT1(wxEVT_MYCAL_LCLICKED, id, MyGOrganizerEventHandler(func))
#define EVT_MYCAL_RCLICKED(id, func) \
	wx__DECLARE_EVT1(wxEVT_MYCAL_RCLICKED, id, MyGOrganizerEventHandler(func))

// CalendarDay class
class CalendarDay
{
public:
	CalendarDay() { this->Reset(); }

	void Reset()
	{
		date = wxDefaultDateTime;
		year = -1;
		month = wxDateTime::Inv_Month;
		day = -1;

		day_num = wxEmptyString;
		week_day = -1;
		names = wxEmptyString;
		active = true;

		ResetCoords();
	}

	void ResetCoords() {
		x = 0;
		y = 0;
		fx = 0;
		fy = 0;
	}

	wxDateTime date;
	int year;
	wxDateTime::Month month;
	int day;

	wxString day_num;
	int week_day;
	wxString names;
	int x, y, fx, fy;
	bool active;
};

// MyCalendar class
class MyCalendar : public wxControl
{
public:
	MyCalendar() { Init(); }
	MyCalendar(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, int style = wxSIMPLE_BORDER);
	virtual ~MyCalendar();

	bool Create(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, int style = wxSIMPLE_BORDER);

	virtual void SetDate(const wxDateTime& date);

protected:
	virtual wxSize DoGetBestSize() const { return wxSize(650, 450); }

	void Init();

	void CalculateCalendarGrid();
	void CalculateMonthData();
	void PaintNow();
	void DoDraw(wxDC& dc);
	void DrawMonthData(wxDC& dc);
	void OnPaint(wxPaintEvent& evt);

	void OnLClick(wxMouseEvent& event);
	void OnRClick(wxMouseEvent& event);

	void OnChangeMonth(wxCommandEvent& event);
	void OnSetDate(wxCommandEvent& event);

	int GetIndex(const wxPoint& pos);

private:
	enum
	{
		ID_NEXT_MONTH,
		ID_PREV_MONTH,
		ID_CURR_MONTH,
		ID_SET_DATE,
		ID_SET_TODAY
	};

	wxDateTime today;
	wxDateTime view;

	wxVector<CalendarDay> days;
	NameDays names;
	wxArrayString names_in_month;

	// Calendar grid coordinates
	int coords[8][7][2]; // [columns][rows]([x][y])

	int week_col_width; // Width of the column with week numbers
	int day_row_height; // Height of the row with day names

	// Appearance settings (Add appropriate methods to modify...)
	// You can also add colors and fonts for other calendar elements.

	wxFont day_name_font; // Font for day names
	wxFont week_number_font; // Font for week numbers
	wxFont main_font; // Font for day numbers
	wxFont names_font; // Font for name days

	wxColour main_colour_active; // Color of the current month's days
	wxColour main_colour_inactive; // Color of the neighboring months' days
	wxColour today_colour; // Color highlighting the current day

	wxDECLARE_DYNAMIC_CLASS(MyCalendar);
};

// Dialog class for getting a date
class MyCalendarGetDateDlg : public wxDialog
{
public:
	MyCalendarGetDateDlg(wxWindow* parent, const wxDateTime& date)
		:wxDialog(parent, wxID_ANY, wxT("Enter date"), wxDefaultPosition, wxDefaultSize, wxCAPTION | wxCLOSE_BOX | wxMINIMIZE_BOX)
	{
		wxBoxSizer* ms = new wxBoxSizer(wxVERTICAL);
		this->SetSizer(ms);
		wxSizer* s = this->CreateTextSizer(wxT("New date:"));
		ms->Add(s, 1, wxEXPAND | wxALL, 4);
		dt = new wxDatePickerCtrl(this, wxID_ANY, date);
		s->Add(dt, 1, wxALIGN_CENTER_HORIZONTAL | wxALL, 8);
		ms->Add(CreateSeparatedButtonSizer(wxOK | wxCANCEL), 0, wxEXPAND | wxALL, 4);
		this->Fit();
	}

	wxDateTime GetDate() { return this->dt->GetValue(); }

private:
	wxDatePickerCtrl* dt;
};

# endif
