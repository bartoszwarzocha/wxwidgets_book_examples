/////////////////////////////////////////////////////////////////////////////
// Name:        my_dt_format.h
// Purpose:     My wxWidgets app
// Author:      Name Surname <name.surname@email.org>
// Created:     2025-01-01
// Copyright:   (c) 2025 by Name Surname
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _MY_DT_FORMAT_H_
#define _MY_DT_FORMAT_H_

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#define WX_DATE_STR_NOMINATIVE   1 // Nominative case
#define WX_DATE_STR_ACCUSATIVE   2 // Accusative case
#define WX_DATE_STR_FULLNAME     4 // Full name
#define WX_DATE_STR_ABBREVIATION 8 // Abbreviation

wxString wxGetMonthName(wxDateTime::Month month, unsigned int mode);
wxString wxGetWeekDayName(wxDateTime::WeekDay week_day, unsigned int mode);
int wxZodiac(wxDateTime date);
wxString wxZodiacName(wxDateTime date);
wxString wxFormatDateTime(wxDateTime dt, const wxString& format);

// Periods
wxString wxYStr(int y);
wxString wxMStr(int y);
wxString wxDStr(int y);
wxString wxYMDStr(wxDateSpan span);

#endif
