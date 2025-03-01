/////////////////////////////////////////////////////////////////////////////
// Name:        MyCalendar.cpp
// Purpose:     My wxWidgets app
// Author:      Name Surname <name.surname@email.org>
// Created:     2025-01-01
// Copyright:   (c) 2025 by Name Surname
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#include "MyCalendar.h"

wxDEFINE_EVENT(wxEVT_MYCAL_LCLICKED, MyCalendarEvent);
wxDEFINE_EVENT(wxEVT_MYCAL_RCLICKED, MyCalendarEvent);

wxIMPLEMENT_DYNAMIC_CLASS(MyCalendarEvent, wxCommandEvent);
wxIMPLEMENT_DYNAMIC_CLASS(MyCalendar, wxControl);

// MyCalendar class functions
MyCalendar::MyCalendar(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, int style)
{
	Create(parent, id, pos, size, style);
}

void MyCalendar::Init()
{
	today = wxDateTime::Now();
	view = today;

	this->week_col_width = 50;
	this->day_row_height = 25;

	names = NameDays();

	for (int i = 0; i < (7 * 6); i++)
	{
		days.push_back(CalendarDay());
	}

	day_name_font = wxFont(10, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
	week_number_font = wxFont(8, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
	main_font = wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Impact"));
	names_font = wxFont(5, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Arial"));

	this->SetForegroundColour(*wxWHITE);
	this->SetBackgroundColour(*wxBLACK);
	main_colour_active = wxColour(220, 220, 220);
	main_colour_inactive = wxColour(120, 120, 120);
	today_colour = wxColour(75, 75, 75);

	Bind(wxEVT_LEFT_UP, &MyCalendar::OnLClick, this, wxID_ANY);
	Bind(wxEVT_RIGHT_UP, &MyCalendar::OnRClick, this, wxID_ANY);
	Bind(wxEVT_PAINT, &MyCalendar::OnPaint, this, wxID_ANY);

	Bind(wxEVT_MENU, &MyCalendar::OnChangeMonth, this, ID_NEXT_MONTH);
	Bind(wxEVT_MENU, &MyCalendar::OnChangeMonth, this, ID_PREV_MONTH);
	Bind(wxEVT_MENU, &MyCalendar::OnChangeMonth, this, ID_CURR_MONTH);
	Bind(wxEVT_MENU, &MyCalendar::OnSetDate, this, ID_SET_DATE);
	Bind(wxEVT_MENU, &MyCalendar::OnSetDate, this, ID_SET_TODAY);

	CalculateMonthData();
	CalculateCalendarGrid();
}

bool MyCalendar::Create(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, int style)
{
	if (!wxControl::Create(parent, id, pos, size, style))
	{
		return false;
	}

	Init();

	return true;
}

MyCalendar::~MyCalendar()
{
	Unbind(wxEVT_LEFT_UP, &MyCalendar::OnLClick, this, wxID_ANY);
	Unbind(wxEVT_RIGHT_UP, &MyCalendar::OnRClick, this, wxID_ANY);
	Unbind(wxEVT_PAINT, &MyCalendar::OnPaint, this, wxID_ANY);

	Unbind(wxEVT_MENU, &MyCalendar::OnChangeMonth, this, ID_NEXT_MONTH);
	Unbind(wxEVT_MENU, &MyCalendar::OnChangeMonth, this, ID_PREV_MONTH);
	Unbind(wxEVT_MENU, &MyCalendar::OnChangeMonth, this, ID_CURR_MONTH);
	Unbind(wxEVT_MENU, &MyCalendar::OnSetDate, this, ID_SET_DATE);
	Unbind(wxEVT_MENU, &MyCalendar::OnSetDate, this, ID_SET_TODAY);
}


void MyCalendar::SetDate(const wxDateTime& date)
{
	today = date;
	view = today;
	CalculateMonthData();

	PaintNow();
}

int MyCalendar::GetIndex(const wxPoint& pos)
{
	int index = -1;

	for (int i = 0; i < 42; i++)
	{
		if ((pos.x >= days[i].x && pos.x <= days[i].fx) &&
			(pos.y >= days[i].y && pos.y <= days[i].fy))
		{
			index = i;
			break;
		}
	}

	return index;
}

void MyCalendar::OnLClick(wxMouseEvent& event)
{
	int index = GetIndex(event.GetPosition());

	if (index != -1 && days[index].active)
	{
		MyCalendarEvent evt(wxEVT_MYCAL_LCLICKED, this->GetId());
		evt.SetEventObject(this); // Set the event object
		evt.SetPosition(event.GetPosition()); // Set the cursor position
		evt.SetDate(days[index].date); // Set the date in the event object
		evt.SetNames(days[index].names); // Set the name days in the event object
		ProcessWindowEvent(evt);
	}
}

void MyCalendar::OnRClick(wxMouseEvent& event)
{
	int index = GetIndex(event.GetPosition());

	// Context menu for days...
	wxMenu m;
	if (index != -1 && days[index].active)
	{
		m.SetTitle(days[index].date.FormatISODate());
	}
	// Here operations on the date, for example:
	// Add note, Add alarm, Add event, Add holiday, etc.
	m.Append(ID_NEXT_MONTH, wxT("Next month"));
	m.Append(ID_PREV_MONTH, wxT("Previous month"));
	m.AppendSeparator();
	m.Append(ID_CURR_MONTH, wxT("Current month"));
	m.AppendSeparator();
	m.Append(ID_SET_DATE, wxT("Set any date"));
	m.Append(ID_SET_TODAY, wxT("Set current date"));
	this->PopupMenu(&m, event.GetPosition());

	if (index != -1 && days[index].active)
	{
		MyCalendarEvent evt(wxEVT_MYCAL_RCLICKED, this->GetId());
		evt.SetEventObject(this); // Set the event object
		evt.SetPosition(event.GetPosition()); // Set the cursor position
		evt.SetDate(days[index].date); // Set the date in the event object
		evt.SetNames(days[index].names); // Set the name days in the event object
		ProcessWindowEvent(evt);
	}
}

void MyCalendar::OnChangeMonth(wxCommandEvent& event)
{
	wxDateTime::Month tmp_month = view.GetMonth();
	int tmp_year = view.GetYear();

	switch (event.GetId())
	{
	case ID_NEXT_MONTH:
		if (view.GetMonth() == wxDateTime::Dec)
		{
			tmp_month = wxDateTime::Jan;
			tmp_year++;
		}
		else
		{
			tmp_month = (wxDateTime::Month)(view.GetMonth() + 1);
		}
		break;

	case ID_PREV_MONTH:
		if (view.GetMonth() == wxDateTime::Jan)
		{
			tmp_month = wxDateTime::Dec;
			tmp_year--;
		}
		else
		{
			tmp_month = (wxDateTime::Month)(view.GetMonth() - 1);
		}
		break;

	case ID_CURR_MONTH:
		tmp_month = today.GetMonth();
		tmp_year = today.GetYear();
		break;
	}

	view.Set(1, tmp_month, tmp_year); // View update
	CalculateMonthData();
	PaintNow(); // Pain calendar again...
}

void MyCalendar::OnSetDate(wxCommandEvent& event)
{
	if (event.GetId() == ID_SET_TODAY)
	{
		this->SetDate(wxDateTime::Today());
	}
	else
	{
		MyCalendarGetDateDlg dlg(this, today);
		if (dlg.ShowModal() == wxID_OK)
		{
			this->SetDate(dlg.GetDate());
		}
	}
}

void MyCalendar::CalculateCalendarGrid()
{
	int x = week_col_width;
	int y = day_row_height;
	int width = (GetSize().GetWidth() - 1) - x;
	int height = (GetSize().GetHeight() - 1) - y;
	int mod_x = (int)(width / 7);
	int	mod_y = (int)(height / 6);
	int coord_x = x;
	int coord_y = y;
	int counter = 0;

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			if (j == 0) { coord_y = y; }
			else if (j == 6) { coord_y = (GetSize().GetHeight() - 1); }
			if (i == 7) { coord_x = (GetSize().GetWidth() - 1); }

			coords[i][j][0] = (i == 0) ? (x) : (coord_x);
			coords[i][j][1] = (j == 0) ? (y) : (coord_y);

			coord_y += mod_y;
		}

		coord_x += mod_x;
	}

	// Copy coordinates to the days array...
	for (int i = 0; i < 6; i++)	// Rows
	{
		for (int j = 0; j < 7; j++) // Columns
		{
			days[counter].x = coords[j][i][0];
			days[counter].y = coords[j][i][1];
			days[counter].fx = coords[j + 1][i + 1][0] - ((i == 5) ? 0 : 1);
			days[counter].fy = coords[j + 1][i + 1][1] - ((j == 6) ? 0 : 1);
			counter++;
		}
	}
}

void MyCalendar::CalculateMonthData()
{
	wxDateTime::Month month = view.GetMonth();
	int year = view.GetYear();

	names.GetNamesByMonth(month, &names_in_month);
	wxDateTime tmp(1, month, year);

	// Check how many days in the month
	int days_in_month = wxDateTime::GetNumberOfDays(month, year);

	// Check the first day of the month
	wxDateTime::WeekDay first_day_of_month = tmp.GetWeekDay();

	// Sunday conversion because wxDateTime::Sun = 0
	if (first_day_of_month == 0)  first_day_of_month = (wxDateTime::WeekDay)(7);

	// Days of the month
	int tmp_day = 0;
	for (int i = 1; i < days_in_month; i++)
	{
		tmp_day = (i + first_day_of_month) - 2;
		days[tmp_day].day_num = wxString::Format(wxT("%d"), i);
		days[tmp_day].names = names_in_month[i - 1];
		days[tmp_day].day = i;
		days[tmp_day].month = month;
		days[tmp_day].year = year;
		days[tmp_day].date.Set(i, month, year);
		days[tmp_day].week_day = days[tmp_day].date.GetWeekDay();
		days[tmp_day].active = true;
		tmp_day++;
	}

	// Days of previous and next month
	wxDateTime::Month tmp_month = month;

	// Previous month:
	if (month == wxDateTime::Jan) tmp_month = wxDateTime::Inv_Month;
	int earlier = wxDateTime::GetNumberOfDays((wxDateTime::Month)(tmp_month - 1), year);
	for (int i = (first_day_of_month - 2); i >= 0; i--)
	{
		days[i].day_num = wxString::Format(wxT("%d"), earlier);
		days[i].active = false;
		earlier--;
	}

	// Next month:
	int later = 1;
	for (tmp_day; tmp_day < 42; tmp_day++)
	{
		days[tmp_day].day_num = wxString::Format(wxT("%d"), later);
		days[tmp_day].active = false;
		later++;
	}
}

void MyCalendar::OnPaint(wxPaintEvent & WXUNUSED(event))
{
	wxPaintDC dc(this);
	DoDraw(dc);
}

void MyCalendar::PaintNow()
{
	wxClientDC dc(this);
	DoDraw(dc);
}

void MyCalendar::DoDraw(wxDC& dc)
{
	dc.SetBackground(wxBrush(this->GetBackgroundColour()));
	dc.Clear();

	int w, h; // width, height
	dc.GetSize(&w, &h);

	// Background of week numbers
	dc.SetPen(wxColour(100, 100, 100));
	dc.SetBrush(wxColour(100, 100, 100));
	dc.DrawRectangle(0, 0, this->week_col_width, h);

	// Background of day names
	dc.SetPen(wxColour(50, 50, 50));
	dc.SetBrush(wxColour(50, 50, 50));
	dc.DrawRectangle(0, 0, w, this->day_row_height);

	// Other lines...
	dc.SetPen(wxPen(this->GetForegroundColour()));
	for (int i = 0; i < 7; i++) { dc.DrawLine(coords[i][0][0], 0, coords[i][0][0], h); }
	for (int j = 0; j < 6; j++) { dc.DrawLine(0, coords[0][j][1], w, coords[0][j][1]); }

	// Week days
	int day_y = (int)day_row_height * 0.8;
	dc.SetTextForeground(wxColour(200, 200, 200));
	day_name_font.SetPointSize(day_row_height * 0.4);
	dc.SetFont(day_name_font);
	dc.DrawText(wxT("Pn"), coords[0][0][0] + 10, coords[0][0][1] - day_y);
	dc.DrawText(wxT("Wt"), coords[1][0][0] + 10, coords[1][0][1] - day_y);
	dc.DrawText(wxT("Śr"), coords[2][0][0] + 10, coords[2][0][1] - day_y);
	dc.DrawText(wxT("Cz"), coords[3][0][0] + 10, coords[3][0][1] - day_y);
	dc.DrawText(wxT("Pt"), coords[4][0][0] + 10, coords[4][0][1] - day_y);
	dc.SetTextForeground(*wxWHITE);
	dc.DrawText(wxT("Sb"), coords[5][0][0] + 10, coords[5][0][1] - day_y);
	dc.DrawText(wxT("Nd"), coords[6][0][0] + 10, coords[6][0][1] - day_y);

	DrawMonthData(dc);
}

void MyCalendar::DrawMonthData(wxDC& dc)
{
	wxDateTime tmp(1, view.GetMonth(), view.GetYear());

	// Week numbers
	int week_number = tmp.GetWeekOfYear();

	dc.SetTextForeground(this->GetForegroundColour());
	dc.SetFont(week_number_font);
	if (week_number == 0) week_number = 53;
	dc.DrawText(wxString::Format(wxT("%d"), week_number++), 10, coords[0][0][1] + 10);
	if (week_number == 54) week_number = 1;
	dc.DrawText(wxString::Format(wxT("%d"), week_number++), 10, coords[0][1][1] + 10);
	dc.DrawText(wxString::Format(wxT("%d"), week_number++), 10, coords[0][2][1] + 10);
	dc.DrawText(wxString::Format(wxT("%d"), week_number++), 10, coords[0][3][1] + 10);
	dc.DrawText(wxString::Format(wxT("%d"), week_number++), 10, coords[0][4][1] + 10);
	if (week_number == 0) week_number = 53;
	if (week_number == 54) week_number = 1;
	dc.DrawText(wxString::Format(wxT("%d"), week_number++), 10, coords[0][5][1] + 10);

	wxSize day_num_size(-1, -1);
	wxSize names_size(-1, -1);
	for (int i = 0; i < 42; i++)
	{
		// Current day highlighting
		if (days[i].date.IsSameDate(today))
		{
			dc.SetPen(today_colour);
			dc.SetBrush(today_colour);
			wxPoint points[3] = { wxPoint(days[i].x + 1,days[i].y + 1), wxPoint(days[i].x + 1, days[i].fy), wxPoint(days[i].fx - 1, days[i].fy) };
			dc.DrawPolygon(3, points);
		}

		if (days[i].active) // Active days in specific month
		{
			dc.SetFont(main_font);
			day_num_size = dc.GetTextExtent(days[i].day_num);
			dc.SetTextForeground(main_colour_active);
			dc.DrawText(days[i].day_num, (days[i].fx - 10 - day_num_size.GetWidth()), days[i].y + 10);

			// Naming days
			dc.SetFont(names_font);
			dc.SetTextForeground(wxColour(200, 200, 200));
			wxString name = days[i].names;
			// Format
			names_size = dc.GetTextExtent(name);
			if (names_size.GetWidth() > ((days[i].fx - days[i].x) - 20))
			{
				int where = name.Find(',', true);
				wxString sub = name.SubString(where + 1, name.Len());
				sub.Replace(" ", "");
				name = name.SubString(0, where);
				name += "\n" + sub;
				dc.DrawText(name, days[i].x + 10, days[i].fy - (names_size.GetHeight() * 3));
			}
			else dc.DrawText(name, days[i].x + 10, days[i].fy - (names_size.GetHeight() * 2));
		}
		else // Unactive days
		{
			dc.SetFont(main_font);
			day_num_size = dc.GetTextExtent(days[i].day_num);
			dc.SetTextForeground(main_colour_inactive);
			dc.DrawText(days[i].day_num, (days[i].fx - 10 - day_num_size.GetWidth()), days[i].y + 10);
		}
	}
}

// NameDays functions
void NameDays::GetNamesByMonth(wxDateTime::Month month, wxArrayString* arr)
{
	arr->Clear();

	for (NameDay d : days)
	{
		if (d.month == month)
		{
			arr->Add(d.names);
		}
	}
}

wxString NameDays::GetNamesByDay(wxDateTime::Month month, int day)
{
	wxString tmp = wxEmptyString;

	for (NameDay d : days)
	{
		if (d.day == day && d.month == month)
		{
			tmp = d.names;
			break;
		}
	}

	return tmp;
}


// Note: In original version of this function Polish names were used,
// because book was written and available in Polish only.
// In Poland people celebrate name days as well as birthdays.
// Here is the version of calendar of name days for France generated by AI.
void NameDays::InitData()
{
	days.push_back(NameDay(wxDateTime::Jan, 1, wxT("Marie, Joseph")));
	days.push_back(NameDay(wxDateTime::Jan, 2, wxT("Basile, Grégoire")));
	days.push_back(NameDay(wxDateTime::Jan, 3, wxT("Geneviève, Daniel")));
	days.push_back(NameDay(wxDateTime::Jan, 4, wxT("Élisabeth, Angèle")));
	days.push_back(NameDay(wxDateTime::Jan, 5, wxT("Édouard, Simon")));
	days.push_back(NameDay(wxDateTime::Jan, 6, wxT("Gaspard, Melchior, Balthazar")));
	days.push_back(NameDay(wxDateTime::Jan, 7, wxT("Julien, Lucien")));
	days.push_back(NameDay(wxDateTime::Jan, 8, wxT("Erhard, Laurent")));
	days.push_back(NameDay(wxDateTime::Jan, 9, wxT("Adrien, Pierre")));
	days.push_back(NameDay(wxDateTime::Jan, 10, wxT("Guillaume, Agathe")));
	days.push_back(NameDay(wxDateTime::Jan, 11, wxT("Honorata, Théodose")));
	days.push_back(NameDay(wxDateTime::Jan, 12, wxT("Benoît, Tatiana")));
	days.push_back(NameDay(wxDateTime::Jan, 13, wxT("Hilaire, Kentigern")));
	days.push_back(NameDay(wxDateTime::Jan, 14, wxT("Félix, Nina")));
	days.push_back(NameDay(wxDateTime::Jan, 15, wxT("Arnaud, Maur")));
	days.push_back(NameDay(wxDateTime::Jan, 16, wxT("Marcel, Honorat")));
	days.push_back(NameDay(wxDateTime::Jan, 17, wxT("Antoine, Mildred")));
	days.push_back(NameDay(wxDateTime::Jan, 18, wxT("Prisca, Volusien")));
	days.push_back(NameDay(wxDateTime::Jan, 19, wxT("Wulstan, Henri")));
	days.push_back(NameDay(wxDateTime::Jan, 20, wxT("Fabien, Sébastien")));
	days.push_back(NameDay(wxDateTime::Jan, 21, wxT("Agnès, Épiphane")));
	days.push_back(NameDay(wxDateTime::Jan, 22, wxT("Vincent, Anastase")));
	days.push_back(NameDay(wxDateTime::Jan, 23, wxT("Émérentienne, Ildefonse")));
	days.push_back(NameDay(wxDateTime::Jan, 24, wxT("François de Sales, Timothée")));
	days.push_back(NameDay(wxDateTime::Jan, 25, wxT("Paul, Dwynwen")));
	days.push_back(NameDay(wxDateTime::Jan, 26, wxT("Timothée, Tite")));
	days.push_back(NameDay(wxDateTime::Jan, 27, wxT("Angèle, Julien")));
	days.push_back(NameDay(wxDateTime::Jan, 28, wxT("Thomas d'Aquin, Pierre Nolasque")));
	days.push_back(NameDay(wxDateTime::Jan, 29, wxT("Gildas, Valère")));
	days.push_back(NameDay(wxDateTime::Jan, 30, wxT("Martine, Bathilde")));
	days.push_back(NameDay(wxDateTime::Jan, 31, wxT("Jean Bosco, Marcelle")));

	days.push_back(NameDay(wxDateTime::Feb, 1, wxT("Brigitte, Ignace")));
	days.push_back(NameDay(wxDateTime::Feb, 2, wxT("Présentation du Seigneur, Chandeleur")));
	days.push_back(NameDay(wxDateTime::Feb, 3, wxT("Blaise, Anschaire")));
	days.push_back(NameDay(wxDateTime::Feb, 4, wxT("Gilbert, Jeanne de Valois")));
	days.push_back(NameDay(wxDateTime::Feb, 5, wxT("Agathe, Adélaïde")));
	days.push_back(NameDay(wxDateTime::Feb, 6, wxT("Paul Miki, Dorothée")));
	days.push_back(NameDay(wxDateTime::Feb, 7, wxT("Richard, Romuald")));
	days.push_back(NameDay(wxDateTime::Feb, 8, wxT("Jérôme Émilien, Joséphine Bakhita")));
	days.push_back(NameDay(wxDateTime::Feb, 9, wxT("Apolline, Ansbert")));
	days.push_back(NameDay(wxDateTime::Feb, 10, wxT("Scholastique, Guillaume de Maleval")));
	days.push_back(NameDay(wxDateTime::Feb, 11, wxT("Notre-Dame de Lourdes, Benoît d'Aniane")));
	days.push_back(NameDay(wxDateTime::Feb, 12, wxT("Eulalie, Julien l'Hospitalier")));
	days.push_back(NameDay(wxDateTime::Feb, 13, wxT("Catherine de Ricci, Polyeucte")));
	days.push_back(NameDay(wxDateTime::Feb, 14, wxT("Valentin, Cyrille et Méthode")));
	days.push_back(NameDay(wxDateTime::Feb, 15, wxT("Faustin, Sigefroid")));
	days.push_back(NameDay(wxDateTime::Feb, 16, wxT("Julienne, Onésime")));
	days.push_back(NameDay(wxDateTime::Feb, 17, wxT("Fintan, Alexis Falconieri")));
	days.push_back(NameDay(wxDateTime::Feb, 18, wxT("Bernadette, Siméon")));
	days.push_back(NameDay(wxDateTime::Feb, 19, wxT("Conrad de Plaisance, Gabin")));
	days.push_back(NameDay(wxDateTime::Feb, 20, wxT("Éleuthère, Eucher")));
	days.push_back(NameDay(wxDateTime::Feb, 21, wxT("Pierre Damien, Robert Southwell")));
	days.push_back(NameDay(wxDateTime::Feb, 22, wxT("Chaire de Saint Pierre, Marguerite de Cortone")));
	days.push_back(NameDay(wxDateTime::Feb, 23, wxT("Polycarpe, Sérène le Jardinier")));
	days.push_back(NameDay(wxDateTime::Feb, 24, wxT("Matthias, Ethelbert")));
	days.push_back(NameDay(wxDateTime::Feb, 25, wxT("Walburge, Césaire de Nazianze")));
	days.push_back(NameDay(wxDateTime::Feb, 26, wxT("Alexandre d'Alexandrie, Porphyre de Gaza")));
	days.push_back(NameDay(wxDateTime::Feb, 27, wxT("Gabriel de Notre-Dame des Douleurs, Anne Line")));
	days.push_back(NameDay(wxDateTime::Feb, 28, wxT("Oswald, Romain de Condat")));
	days.push_back(NameDay(wxDateTime::Feb, 29, wxT("Jean Cassien, Oswald de Worcester")));

	days.push_back(NameDay(wxDateTime::Mar, 1, wxT("David, Aubin")));
	days.push_back(NameDay(wxDateTime::Mar, 2, wxT("Chad, Agnès de Bohême")));
	days.push_back(NameDay(wxDateTime::Mar, 3, wxT("Katharine Drexel, Cunégonde")));
	days.push_back(NameDay(wxDateTime::Mar, 4, wxT("Casimir, Lucius I")));
	days.push_back(NameDay(wxDateTime::Mar, 5, wxT("Jean-Joseph de la Croix, Kieran")));
	days.push_back(NameDay(wxDateTime::Mar, 6, wxT("Colette, Fridolin de Säckingen")));
	days.push_back(NameDay(wxDateTime::Mar, 7, wxT("Perpétue, Félicité")));
	days.push_back(NameDay(wxDateTime::Mar, 8, wxT("Jean de Dieu, Félix de Bourgogne")));
	days.push_back(NameDay(wxDateTime::Mar, 9, wxT("Françoise de Rome, Dominique Savio")));
	days.push_back(NameDay(wxDateTime::Mar, 10, wxT("Jean Ogilvie, Drostan")));
	days.push_back(NameDay(wxDateTime::Mar, 11, wxT("Aengus, Constantin")));
	days.push_back(NameDay(wxDateTime::Mar, 12, wxT("Fina, Maximilien")));
	days.push_back(NameDay(wxDateTime::Mar, 13, wxT("Euphrasie, Gérald de Mayo")));
	days.push_back(NameDay(wxDateTime::Mar, 14, wxT("Mathilde, Pauline")));
	days.push_back(NameDay(wxDateTime::Mar, 15, wxT("Louise de Marillac, Clément-Marie Hofbauer")));
	days.push_back(NameDay(wxDateTime::Mar, 16, wxT("Herbert, Abraham de Smolensk")));
	days.push_back(NameDay(wxDateTime::Mar, 17, wxT("Patrick, Gertrude de Nivelles")));
	days.push_back(NameDay(wxDateTime::Mar, 18, wxT("Cyrille de Jérusalem, Édouard le Martyr")));
	days.push_back(NameDay(wxDateTime::Mar, 19, wxT("Joseph, Alcmund de Hexham")));
	days.push_back(NameDay(wxDateTime::Mar, 20, wxT("Cuthbert, Wulfram de Sens")));
	days.push_back(NameDay(wxDateTime::Mar, 21, wxT("Benoît de Nursie, Enda d'Aran")));
	days.push_back(NameDay(wxDateTime::Mar, 22, wxT("Léa de Rome, Basile d'Ancyre")));
	days.push_back(NameDay(wxDateTime::Mar, 23, wxT("Turibio de Mogrovejo, Rebecca")));
	days.push_back(NameDay(wxDateTime::Mar, 24, wxT("Catherine de Suède, Aldemar le Sage")));
	days.push_back(NameDay(wxDateTime::Mar, 25, wxT("Annonciation du Seigneur, Dismas")));
	days.push_back(NameDay(wxDateTime::Mar, 26, wxT("Marguerite Clitherow, Braulio de Saragosse")));
	days.push_back(NameDay(wxDateTime::Mar, 27, wxT("Jean d'Égypte, Rupert de Salzbourg")));
	days.push_back(NameDay(wxDateTime::Mar, 28, wxT("Gontran, Tutilo")));
	days.push_back(NameDay(wxDateTime::Mar, 29, wxT("Gladys, Ludolphe de Ratzebourg")));
	days.push_back(NameDay(wxDateTime::Mar, 30, wxT("Jean Climaque, Zosime de Palestine")));
	days.push_back(NameDay(wxDateTime::Mar, 31, wxT("Benjamin, Balbine")));

	days.push_back(NameDay(wxDateTime::Apr, 1, wxT("Hugues de Grenoble, Marie d'Égypte")));
	days.push_back(NameDay(wxDateTime::Apr, 2, wxT("François de Paule, Marie de Cléophas")));
	days.push_back(NameDay(wxDateTime::Apr, 3, wxT("Richard de Chichester, Sixte I")));
	days.push_back(NameDay(wxDateTime::Apr, 4, wxT("Isidore de Séville, Benoît le More")));
	days.push_back(NameDay(wxDateTime::Apr, 5, wxT("Vincent Ferrier, Julienne de Liège")));
	days.push_back(NameDay(wxDateTime::Apr, 6, wxT("Guillaume de Paris, Célestin I")));
	days.push_back(NameDay(wxDateTime::Apr, 7, wxT("Jean-Baptiste de La Salle, Hermann Joseph")));
	days.push_back(NameDay(wxDateTime::Apr, 8, wxT("Julie Billiart, Gautier de Pontoise")));
	days.push_back(NameDay(wxDateTime::Apr, 9, wxT("Marie Cléophas, Démétrius de Thessalonique")));
	days.push_back(NameDay(wxDateTime::Apr, 10, wxT("Fulbert de Chartres, Michel de Sanctis")));
	days.push_back(NameDay(wxDateTime::Apr, 11, wxT("Gemma Galgani, Stanislas de Szczepanów")));
	days.push_back(NameDay(wxDateTime::Apr, 12, wxT("Jules I, Thérèse des Andes")));
	days.push_back(NameDay(wxDateTime::Apr, 13, wxT("Martin I, Herménégilde")));
	days.push_back(NameDay(wxDateTime::Apr, 14, wxT("Lydwine, Pierre Gonzalez")));
	days.push_back(NameDay(wxDateTime::Apr, 15, wxT("Damien de Molokai, Paterne d'Avranches")));
	days.push_back(NameDay(wxDateTime::Apr, 16, wxT("Bernadette Soubirous, Benoît-Joseph Labre")));
	days.push_back(NameDay(wxDateTime::Apr, 17, wxT("Étienne Harding, Kateri Tekakwitha")));
	days.push_back(NameDay(wxDateTime::Apr, 18, wxT("Apollonius l'Apologiste, Parfait")));
	days.push_back(NameDay(wxDateTime::Apr, 19, wxT("Emma de Lesum, Alphège de Cantorbéry")));
	days.push_back(NameDay(wxDateTime::Apr, 20, wxT("Marcellin d'Embrun, Agnès de Montepulciano")));
	days.push_back(NameDay(wxDateTime::Apr, 21, wxT("Anselme de Cantorbéry, Conrad de Parzham")));
	days.push_back(NameDay(wxDateTime::Apr, 22, wxT("Soter, Caïus")));
	days.push_back(NameDay(wxDateTime::Apr, 23, wxT("Georges, Adalbert de Prague")));
	days.push_back(NameDay(wxDateTime::Apr, 24, wxT("Fidèle de Sigmaringen, Marie-Euphrasie Pelletier")));
	days.push_back(NameDay(wxDateTime::Apr, 25, wxT("Marc, Anien d'Alexandrie")));
	days.push_back(NameDay(wxDateTime::Apr, 26, wxT("Clétus, Marcellin de Carthage")));
	days.push_back(NameDay(wxDateTime::Apr, 27, wxT("Zita, Louis-Marie Grignion de Montfort")));
	days.push_back(NameDay(wxDateTime::Apr, 28, wxT("Pierre Chanel, Louis de Montfort")));
	days.push_back(NameDay(wxDateTime::Apr, 29, wxT("Catherine de Sienne, Hugues de Cluny")));
	days.push_back(NameDay(wxDateTime::Apr, 30, wxT("Pie V, Marie de l'Incarnation")));

	days.push_back(NameDay(wxDateTime::May, 1, wxT("Jérémie, Florent")));
	days.push_back(NameDay(wxDateTime::May, 2, wxT("Boris, Zoé")));
	days.push_back(NameDay(wxDateTime::May, 3, wxT("Philippe, Jacques")));
	days.push_back(NameDay(wxDateTime::May, 4, wxT("Sylvain, Monique")));
	days.push_back(NameDay(wxDateTime::May, 5, wxT("Judith, Irène")));
	days.push_back(NameDay(wxDateTime::May, 6, wxT("Prudence, Évode")));
	days.push_back(NameDay(wxDateTime::May, 7, wxT("Gisèle, Domitille")));
	days.push_back(NameDay(wxDateTime::May, 8, wxT("Désiré, Jeanne d'Arc")));
	days.push_back(NameDay(wxDateTime::May, 9, wxT("Pacôme, Isaïe")));
	days.push_back(NameDay(wxDateTime::May, 10, wxT("Solange, Antonin")));
	days.push_back(NameDay(wxDateTime::May, 11, wxT("Estelle, Ignace")));
	days.push_back(NameDay(wxDateTime::May, 12, wxT("Achille, Pancrace")));
	days.push_back(NameDay(wxDateTime::May, 13, wxT("Rolande, Servais")));
	days.push_back(NameDay(wxDateTime::May, 14, wxT("Matthias, Boniface")));
	days.push_back(NameDay(wxDateTime::May, 15, wxT("Denise, Rémi")));
	days.push_back(NameDay(wxDateTime::May, 16, wxT("Honoré, Simon Stock")));
	days.push_back(NameDay(wxDateTime::May, 17, wxT("Pascal, Solange")));
	days.push_back(NameDay(wxDateTime::May, 18, wxT("Éric, Corinne")));
	days.push_back(NameDay(wxDateTime::May, 19, wxT("Yves, Célestin")));
	days.push_back(NameDay(wxDateTime::May, 20, wxT("Bernardin, Baudouin")));
	days.push_back(NameDay(wxDateTime::May, 21, wxT("Constantin, Eugène de Mazenod")));
	days.push_back(NameDay(wxDateTime::May, 22, wxT("Émile, Rita")));
	days.push_back(NameDay(wxDateTime::May, 23, wxT("Didier, Guibert")));
	days.push_back(NameDay(wxDateTime::May, 24, wxT("Donatien, Rogatien")));
	days.push_back(NameDay(wxDateTime::May, 25, wxT("Sophie, Bède le Vénérable")));
	days.push_back(NameDay(wxDateTime::May, 26, wxT("Philippe Néri, Bérenger")));
	days.push_back(NameDay(wxDateTime::May, 27, wxT("Augustin de Cantorbéry, Hildegarde")));
	days.push_back(NameDay(wxDateTime::May, 28, wxT("Germain, Guillaume")));
	days.push_back(NameDay(wxDateTime::May, 29, wxT("Aymar, Maximin")));
	days.push_back(NameDay(wxDateTime::May, 30, wxT("Ferdinand, Jeanne d'Arc")));
	days.push_back(NameDay(wxDateTime::May, 31, wxT("Pétronille, Visitation de la Vierge Marie")));

	days.push_back(NameDay(wxDateTime::Jun, 1, wxT("Justin, Pamphile")));
	days.push_back(NameDay(wxDateTime::Jun, 2, wxT("Blandine, Pothin")));
	days.push_back(NameDay(wxDateTime::Jun, 3, wxT("Kévin, Charles Lwanga")));
	days.push_back(NameDay(wxDateTime::Jun, 4, wxT("Clotilde, François Caracciolo")));
	days.push_back(NameDay(wxDateTime::Jun, 5, wxT("Boniface, Igor")));
	days.push_back(NameDay(wxDateTime::Jun, 6, wxT("Norbert, Claude")));
	days.push_back(NameDay(wxDateTime::Jun, 7, wxT("Gilbert, Willibald")));
	days.push_back(NameDay(wxDateTime::Jun, 8, wxT("Médard, Salomon")));
	days.push_back(NameDay(wxDateTime::Jun, 9, wxT("Diane, Colombe")));
	days.push_back(NameDay(wxDateTime::Jun, 10, wxT("Landry, Félicien")));
	days.push_back(NameDay(wxDateTime::Jun, 11, wxT("Barnabé, Yolande")));
	days.push_back(NameDay(wxDateTime::Jun, 12, wxT("Guy, Onuphre")));
	days.push_back(NameDay(wxDateTime::Jun, 13, wxT("Antoine de Padoue, Aquiline")));
	days.push_back(NameDay(wxDateTime::Jun, 14, wxT("Élisée, Rufin")));
	days.push_back(NameDay(wxDateTime::Jun, 15, wxT("Germaine, Augustin")));
	days.push_back(NameDay(wxDateTime::Jun, 16, wxT("Jean-François Régis, Aurélien")));
	days.push_back(NameDay(wxDateTime::Jun, 17, wxT("Hervé, Rainier")));
	days.push_back(NameDay(wxDateTime::Jun, 18, wxT("Léonce, Marine")));
	days.push_back(NameDay(wxDateTime::Jun, 19, wxT("Romuald, Gervais et Protais")));
	days.push_back(NameDay(wxDateTime::Jun, 20, wxT("Silvère, Florent")));
	days.push_back(NameDay(wxDateTime::Jun, 21, wxT("Louis de Gonzague, Rodolphe")));
	days.push_back(NameDay(wxDateTime::Jun, 22, wxT("Alban, Paulin")));
	days.push_back(NameDay(wxDateTime::Jun, 23, wxT("Audrey, Éthelred")));
	days.push_back(NameDay(wxDateTime::Jun, 24, wxT("Jean-Baptiste, Théodulphe")));
	days.push_back(NameDay(wxDateTime::Jun, 25, wxT("Prosper, Salomon")));
	days.push_back(NameDay(wxDateTime::Jun, 26, wxT("Anthelme, Maxime")));
	days.push_back(NameDay(wxDateTime::Jun, 27, wxT("Fernand, Cyrille d'Alexandrie")));
	days.push_back(NameDay(wxDateTime::Jun, 28, wxT("Irénée, Léon")));
	days.push_back(NameDay(wxDateTime::Jun, 29, wxT("Pierre, Paul")));
	days.push_back(NameDay(wxDateTime::Jun, 30, wxT("Martial, Bertrand")));

	days.push_back(NameDay(wxDateTime::Jul, 1, wxT("Thierry, Esther")));
	days.push_back(NameDay(wxDateTime::Jul, 2, wxT("Martinien, Eugénie")));
	days.push_back(NameDay(wxDateTime::Jul, 3, wxT("Thomas, Anatole")));
	days.push_back(NameDay(wxDateTime::Jul, 4, wxT("Florent, Berthe")));
	days.push_back(NameDay(wxDateTime::Jul, 5, wxT("Antoine-Marie Zaccaria, Zoé")));
	days.push_back(NameDay(wxDateTime::Jul, 6, wxT("Mariette, Maria Goretti")));
	days.push_back(NameDay(wxDateTime::Jul, 7, wxT("Raoul, Thibaut")));
	days.push_back(NameDay(wxDateTime::Jul, 8, wxT("Thibaut, Édgar")));
	days.push_back(NameDay(wxDateTime::Jul, 9, wxT("Amandine, Véronique")));
	days.push_back(NameDay(wxDateTime::Jul, 10, wxT("Ulrich, Félicité")));
	days.push_back(NameDay(wxDateTime::Jul, 11, wxT("Benoît, Olga")));
	days.push_back(NameDay(wxDateTime::Jul, 12, wxT("Olivier, Vivien")));
	days.push_back(NameDay(wxDateTime::Jul, 13, wxT("Henri, Joël")));
	days.push_back(NameDay(wxDateTime::Jul, 14, wxT("Camille, Kateri Tekakwitha")));
	days.push_back(NameDay(wxDateTime::Jul, 15, wxT("Donald, Vladimir")));
	days.push_back(NameDay(wxDateTime::Jul, 16, wxT("Notre-Dame du Mont-Carmel, Hélier")));
	days.push_back(NameDay(wxDateTime::Jul, 17, wxT("Charlotte, Alexis")));
	days.push_back(NameDay(wxDateTime::Jul, 18, wxT("Frédéric, Arnould")));
	days.push_back(NameDay(wxDateTime::Jul, 19, wxT("Arsène, Symphorose")));
	days.push_back(NameDay(wxDateTime::Jul, 20, wxT("Élie, Marina")));
	days.push_back(NameDay(wxDateTime::Jul, 21, wxT("Victor, Daniel")));
	days.push_back(NameDay(wxDateTime::Jul, 22, wxT("Marie-Madeleine, Wandrille")));
	days.push_back(NameDay(wxDateTime::Jul, 23, wxT("Brigitte, Apollinaire")));
	days.push_back(NameDay(wxDateTime::Jul, 24, wxT("Christine, Ségolène")));
	days.push_back(NameDay(wxDateTime::Jul, 25, wxT("Jacques, Christophe")));
	days.push_back(NameDay(wxDateTime::Jul, 26, wxT("Anne, Joachim")));
	days.push_back(NameDay(wxDateTime::Jul, 27, wxT("Nathalie, Célestin")));
	days.push_back(NameDay(wxDateTime::Jul, 28, wxT("Samson, Nazaire")));
	days.push_back(NameDay(wxDateTime::Jul, 29, wxT("Marthe, Béatrix")));
	days.push_back(NameDay(wxDateTime::Jul, 30, wxT("Juliette, Abdallah")));
	days.push_back(NameDay(wxDateTime::Jul, 31, wxT("Ignace de Loyola, Néot")));

	days.push_back(NameDay(wxDateTime::Aug, 1, wxT("Alphonse, Loup")));
	days.push_back(NameDay(wxDateTime::Aug, 2, wxT("Julien, Eusèbe")));
	days.push_back(NameDay(wxDateTime::Aug, 3, wxT("Lydie, Nicodème")));
	days.push_back(NameDay(wxDateTime::Aug, 4, wxT("Jean-Marie Vianney, Dominique")));
	days.push_back(NameDay(wxDateTime::Aug, 5, wxT("Abel, Oswald")));
	days.push_back(NameDay(wxDateTime::Aug, 6, wxT("Transfiguration, Octavien")));
	days.push_back(NameDay(wxDateTime::Aug, 7, wxT("Gaétan, Donat")));
	days.push_back(NameDay(wxDateTime::Aug, 8, wxT("Dominique, Cyprien")));
	days.push_back(NameDay(wxDateTime::Aug, 9, wxT("Amour, Firmat")));
	days.push_back(NameDay(wxDateTime::Aug, 10, wxT("Laurent, Blain")));
	days.push_back(NameDay(wxDateTime::Aug, 11, wxT("Claire, Suzanne")));
	days.push_back(NameDay(wxDateTime::Aug, 12, wxT("Clarisse, Euverte")));
	days.push_back(NameDay(wxDateTime::Aug, 13, wxT("Hippolyte, Cassien")));
	days.push_back(NameDay(wxDateTime::Aug, 14, wxT("Eusèbe, Maximilien")));
	days.push_back(NameDay(wxDateTime::Aug, 15, wxT("Assomption, Marie")));
	days.push_back(NameDay(wxDateTime::Aug, 16, wxT("Armel, Roch")));
	days.push_back(NameDay(wxDateTime::Aug, 17, wxT("Hyacinthe, Claire")));
	days.push_back(NameDay(wxDateTime::Aug, 18, wxT("Hélène, Agapit")));
	days.push_back(NameDay(wxDateTime::Aug, 19, wxT("Jean Eudes, Sébald")));
	days.push_back(NameDay(wxDateTime::Aug, 20, wxT("Bernard, Samuel")));
	days.push_back(NameDay(wxDateTime::Aug, 21, wxT("Christophe, Privat")));
	days.push_back(NameDay(wxDateTime::Aug, 22, wxT("Marie Reine, Fabrice")));
	days.push_back(NameDay(wxDateTime::Aug, 23, wxT("Rose de Lima, Élie")));
	days.push_back(NameDay(wxDateTime::Aug, 24, wxT("Barthélemy, Georges")));
	days.push_back(NameDay(wxDateTime::Aug, 25, wxT("Louis, Joseph de Calasanz")));
	days.push_back(NameDay(wxDateTime::Aug, 26, wxT("Natacha, Césaire")));
	days.push_back(NameDay(wxDateTime::Aug, 27, wxT("Monique, Césaire")));
	days.push_back(NameDay(wxDateTime::Aug, 28, wxT("Augustin, Hermès")));
	days.push_back(NameDay(wxDateTime::Aug, 29, wxT("Sabine, Jean-Baptiste")));
	days.push_back(NameDay(wxDateTime::Aug, 30, wxT("Fiacre, Gaïa")));
	days.push_back(NameDay(wxDateTime::Aug, 31, wxT("Aristide, Raymond")));

	days.push_back(NameDay(wxDateTime::Sep, 1, wxT("Gilles, Loup")));
	days.push_back(NameDay(wxDateTime::Sep, 2, wxT("Ingrid, Juste")));
	days.push_back(NameDay(wxDateTime::Sep, 3, wxT("Grégoire, Basile")));
	days.push_back(NameDay(wxDateTime::Sep, 4, wxT("Rosalie, Moïse")));
	days.push_back(NameDay(wxDateTime::Sep, 5, wxT("Raïssa, Bertin")));
	days.push_back(NameDay(wxDateTime::Sep, 6, wxT("Bertrand, Éleuthère")));
	days.push_back(NameDay(wxDateTime::Sep, 7, wxT("Reine, Clodoald")));
	days.push_back(NameDay(wxDateTime::Sep, 8, wxT("Nativité de la Vierge Marie, Adrien")));
	days.push_back(NameDay(wxDateTime::Sep, 9, wxT("Alain, Pierre Claver")));
	days.push_back(NameDay(wxDateTime::Sep, 10, wxT("Inès, Aubert")));
	days.push_back(NameDay(wxDateTime::Sep, 11, wxT("Adelphe, Hyacinthe")));
	days.push_back(NameDay(wxDateTime::Sep, 12, wxT("Apollinaire, Aimé")));
	days.push_back(NameDay(wxDateTime::Sep, 13, wxT("Aimé, Jean Chrysostome")));
	days.push_back(NameDay(wxDateTime::Sep, 14, wxT("Croix Glorieuse, Materne")));
	days.push_back(NameDay(wxDateTime::Sep, 15, wxT("Roland, Dolorès")));
	days.push_back(NameDay(wxDateTime::Sep, 16, wxT("Édith, Cyprien")));
	days.push_back(NameDay(wxDateTime::Sep, 17, wxT("Renaud, Robert Bellarmin")));
	days.push_back(NameDay(wxDateTime::Sep, 18, wxT("Nadège, Joseph de Cupertino")));
	days.push_back(NameDay(wxDateTime::Sep, 19, wxT("Émilie, Janvier")));
	days.push_back(NameDay(wxDateTime::Sep, 20, wxT("Davy, Eustache")));
	days.push_back(NameDay(wxDateTime::Sep, 21, wxT("Matthieu, Déborah")));
	days.push_back(NameDay(wxDateTime::Sep, 22, wxT("Maurice, Émérance")));
	days.push_back(NameDay(wxDateTime::Sep, 23, wxT("Constant, Thècle")));
	days.push_back(NameDay(wxDateTime::Sep, 24, wxT("Thècle, Andoche")));
	days.push_back(NameDay(wxDateTime::Sep, 25, wxT("Hermann, Firmin")));
	days.push_back(NameDay(wxDateTime::Sep, 26, wxT("Côme, Damien")));
	days.push_back(NameDay(wxDateTime::Sep, 27, wxT("Vincent de Paul, Delphine")));
	days.push_back(NameDay(wxDateTime::Sep, 28, wxT("Venceslas, Saloni")));
	days.push_back(NameDay(wxDateTime::Sep, 29, wxT("Michel, Gabriel")));
	days.push_back(NameDay(wxDateTime::Sep, 30, wxT("Jérôme, Honoré")));

	days.push_back(NameDay(wxDateTime::Oct, 1, wxT("Thérèse de Lisieux, Rémy")));
	days.push_back(NameDay(wxDateTime::Oct, 2, wxT("Léger, Gérard")));
	days.push_back(NameDay(wxDateTime::Oct, 3, wxT("Gérard, Candide")));
	days.push_back(NameDay(wxDateTime::Oct, 4, wxT("François d'Assise, Solange")));
	days.push_back(NameDay(wxDateTime::Oct, 5, wxT("Fleur, Placide")));
	days.push_back(NameDay(wxDateTime::Oct, 6, wxT("Bruno, Marie-Rose")));
	days.push_back(NameDay(wxDateTime::Oct, 7, wxT("Serge, Auguste")));
	days.push_back(NameDay(wxDateTime::Oct, 8, wxT("Pélagie, Thibault")));
	days.push_back(NameDay(wxDateTime::Oct, 9, wxT("Denis, Ghislain")));
	days.push_back(NameDay(wxDateTime::Oct, 10, wxT("Géréon, Ghislain")));
	days.push_back(NameDay(wxDateTime::Oct, 11, wxT("Firmin, Alexandre")));
	days.push_back(NameDay(wxDateTime::Oct, 12, wxT("Wilfrid, Félix")));
	days.push_back(NameDay(wxDateTime::Oct, 13, wxT("Géraud, Édouard")));
	days.push_back(NameDay(wxDateTime::Oct, 14, wxT("Juste, Calliste")));
	days.push_back(NameDay(wxDateTime::Oct, 15, wxT("Thérèse d'Avila, Aurélie")));
	days.push_back(NameDay(wxDateTime::Oct, 16, wxT("Édith, Marguerite-Marie")));
	days.push_back(NameDay(wxDateTime::Oct, 17, wxT("Baudouin, Solenne")));
	days.push_back(NameDay(wxDateTime::Oct, 18, wxT("Luc, Gwladys")));
	days.push_back(NameDay(wxDateTime::Oct, 19, wxT("René, Paul de la Croix")));
	days.push_back(NameDay(wxDateTime::Oct, 20, wxT("Adeline, Caprais")));
	days.push_back(NameDay(wxDateTime::Oct, 21, wxT("Céline, Ursule")));
	days.push_back(NameDay(wxDateTime::Oct, 22, wxT("Élodie, Salomé")));
	days.push_back(NameDay(wxDateTime::Oct, 23, wxT("Jean de Capistran, Domice")));
	days.push_back(NameDay(wxDateTime::Oct, 24, wxT("Florentin, Antoine-Marie")));
	days.push_back(NameDay(wxDateTime::Oct, 25, wxT("Crépin, Daria")));
	days.push_back(NameDay(wxDateTime::Oct, 26, wxT("Dimitri, Évariste")));
	days.push_back(NameDay(wxDateTime::Oct, 27, wxT("Émeline, Antoinette")));
	days.push_back(NameDay(wxDateTime::Oct, 28, wxT("Simon, Jude")));
	days.push_back(NameDay(wxDateTime::Oct, 29, wxT("Narcisse, Bienvenue")));
	days.push_back(NameDay(wxDateTime::Oct, 30, wxT("Bienvenue, Bienvenu")));
	days.push_back(NameDay(wxDateTime::Oct, 31, wxT("Quentin, Lucie")));

	days.push_back(NameDay(wxDateTime::Nov, 1, wxT("Toussaint, Mathurin")));
	days.push_back(NameDay(wxDateTime::Nov, 2, wxT("Défunts, Victorin")));
	days.push_back(NameDay(wxDateTime::Nov, 3, wxT("Hubert, Sylvie")));
	days.push_back(NameDay(wxDateTime::Nov, 4, wxT("Charles Borromée, Émeric")));
	days.push_back(NameDay(wxDateTime::Nov, 5, wxT("Sylvie, Zacharie")));
	days.push_back(NameDay(wxDateTime::Nov, 6, wxT("Bertille, Léonard")));
	days.push_back(NameDay(wxDateTime::Nov, 7, wxT("Carine, Ernest")));
	days.push_back(NameDay(wxDateTime::Nov, 8, wxT("Geoffroy, Claude")));
	days.push_back(NameDay(wxDateTime::Nov, 9, wxT("Théodore, Mathilde")));
	days.push_back(NameDay(wxDateTime::Nov, 10, wxT("Léon, Noé")));
	days.push_back(NameDay(wxDateTime::Nov, 11, wxT("Martin, Véran")));
	days.push_back(NameDay(wxDateTime::Nov, 12, wxT("Christian, Josaphat")));
	days.push_back(NameDay(wxDateTime::Nov, 13, wxT("Brice, Abbon")));
	days.push_back(NameDay(wxDateTime::Nov, 14, wxT("Sidoine, Philémon")));
	days.push_back(NameDay(wxDateTime::Nov, 15, wxT("Albert le Grand, Malo")));
	days.push_back(NameDay(wxDateTime::Nov, 16, wxT("Marguerite d'Écosse, Gertrude")));
	days.push_back(NameDay(wxDateTime::Nov, 17, wxT("Élisabeth de Hongrie, Aignan")));
	days.push_back(NameDay(wxDateTime::Nov, 18, wxT("Aude, Philippine")));
	days.push_back(NameDay(wxDateTime::Nov, 19, wxT("Tanguy, Maxime")));
	days.push_back(NameDay(wxDateTime::Nov, 20, wxT("Edmond, Octave")));
	days.push_back(NameDay(wxDateTime::Nov, 21, wxT("Présentation de Marie, Rufus")));
	days.push_back(NameDay(wxDateTime::Nov, 22, wxT("Cécile, Philémon")));
	days.push_back(NameDay(wxDateTime::Nov, 23, wxT("Clément, Colomban")));
	days.push_back(NameDay(wxDateTime::Nov, 24, wxT("Flora, Virgile")));
	days.push_back(NameDay(wxDateTime::Nov, 25, wxT("Catherine d'Alexandrie, Mercure")));
	days.push_back(NameDay(wxDateTime::Nov, 26, wxT("Delphine, Conrad")));
	days.push_back(NameDay(wxDateTime::Nov, 27, wxT("Séverin, Astrid")));
	days.push_back(NameDay(wxDateTime::Nov, 28, wxT("Jacques de la Marche, Sosthène")));
	days.push_back(NameDay(wxDateTime::Nov, 29, wxT("Saturnin, Radbod")));
	days.push_back(NameDay(wxDateTime::Nov, 30, wxT("André, Tugdual")));

	days.push_back(NameDay(wxDateTime::Dec, 1, wxT("Florence, Éloi")));
	days.push_back(NameDay(wxDateTime::Dec, 2, wxT("Viviane, Sylvestre")));
	days.push_back(NameDay(wxDateTime::Dec, 3, wxT("Xavier, François")));
	days.push_back(NameDay(wxDateTime::Dec, 4, wxT("Barbara, Osmond")));
	days.push_back(NameDay(wxDateTime::Dec, 5, wxT("Gérald, Dalila")));
	days.push_back(NameDay(wxDateTime::Dec, 6, wxT("Nicolas, Évode")));
	days.push_back(NameDay(wxDateTime::Dec, 7, wxT("Ambroise, Gaudens")));
	days.push_back(NameDay(wxDateTime::Dec, 8, wxT("Immaculée Conception, Élise")));
	days.push_back(NameDay(wxDateTime::Dec, 9, wxT("Pierre Fourier, Juan Diego")));
	days.push_back(NameDay(wxDateTime::Dec, 10, wxT("Romaric, Eulalie")));
	days.push_back(NameDay(wxDateTime::Dec, 11, wxT("Daniel, Damase")));
	days.push_back(NameDay(wxDateTime::Dec, 12, wxT("Jeanne-Françoise de Chantal, Corentin")));
	days.push_back(NameDay(wxDateTime::Dec, 13, wxT("Lucie, Jodoc")));
	days.push_back(NameDay(wxDateTime::Dec, 14, wxT("Jean de la Croix, Nicée")));
	days.push_back(NameDay(wxDateTime::Dec, 15, wxT("Ninon, Valérien")));
	days.push_back(NameDay(wxDateTime::Dec, 16, wxT("Alice, Adélaïde")));
	days.push_back(NameDay(wxDateTime::Dec, 17, wxT("Gaël, Yolande")));
	days.push_back(NameDay(wxDateTime::Dec, 18, wxT("Gatien, Gratien")));
	days.push_back(NameDay(wxDateTime::Dec, 19, wxT("Urbain, Anastase")));
	days.push_back(NameDay(wxDateTime::Dec, 20, wxT("Théophile, Philogone")));
	days.push_back(NameDay(wxDateTime::Dec, 21, wxT("Pierre Canisius, Thomas")));
	days.push_back(NameDay(wxDateTime::Dec, 22, wxT("Françoise-Xavière, Démétrie")));
	days.push_back(NameDay(wxDateTime::Dec, 23, wxT("Armand, Thorlak")));
	days.push_back(NameDay(wxDateTime::Dec, 24, wxT("Adèle, Irmine")));
	days.push_back(NameDay(wxDateTime::Dec, 25, wxT("Noël, Emmanuel")));
	days.push_back(NameDay(wxDateTime::Dec, 26, wxT("Étienne, Zénon")));
	days.push_back(NameDay(wxDateTime::Dec, 27, wxT("Jean, Fabiola")));
	days.push_back(NameDay(wxDateTime::Dec, 28, wxT("Innocents, Gaspard")));
	days.push_back(NameDay(wxDateTime::Dec, 29, wxT("David, Thomas Becket")));
	days.push_back(NameDay(wxDateTime::Dec, 30, wxT("Roger, Eugène")));
	days.push_back(NameDay(wxDateTime::Dec, 31, wxT("Sylvestre, Colombe")));
}
