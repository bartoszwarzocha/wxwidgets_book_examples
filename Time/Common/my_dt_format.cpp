/////////////////////////////////////////////////////////////////////////////
// Name:        my_dt_format.cpp
// Purpose:     My wxWidgets app
// Author:      Name Surname <name.surname@email.org>
// Created:     2025-01-01
// Copyright:   (c) 2025 by Name Surname
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#include "my_dt_format.h"

wxString wxGetMonthName(wxDateTime::Month month, unsigned int mode)
{
    wxString mth = wxEmptyString;

	if (mode & WX_DATE_STR_NOMINATIVE) // Specific for Polish language but ignored after translation
    {
        switch (month)
        {
            case wxDateTime::Jan: mth = wxT("January"); break;
            case wxDateTime::Feb: mth = wxT("February"); break;
            case wxDateTime::Mar: mth = wxT("March"); break;
            case wxDateTime::Apr: mth = wxT("April"); break;
            case wxDateTime::May: mth = wxT("May"); break;
            case wxDateTime::Jun: mth = wxT("June"); break;
            case wxDateTime::Jul: mth = wxT("July"); break;
            case wxDateTime::Aug: mth = wxT("August"); break;
            case wxDateTime::Sep: mth = wxT("September"); break;
            case wxDateTime::Oct: mth = wxT("October"); break;
            case wxDateTime::Nov: mth = wxT("November"); break;
            case wxDateTime::Dec: mth = wxT("December"); break;
        }
    }
    else
    {
        switch (month)
        {
            case wxDateTime::Jan: mth = wxT("of January"); break;
            case wxDateTime::Feb: mth = wxT("of February"); break;
            case wxDateTime::Mar: mth = wxT("of March"); break;
            case wxDateTime::Apr: mth = wxT("of April"); break;
            case wxDateTime::May: mth = wxT("of May"); break;
            case wxDateTime::Jun: mth = wxT("of June"); break;
            case wxDateTime::Jul: mth = wxT("of July"); break;
            case wxDateTime::Aug: mth = wxT("of August"); break;
            case wxDateTime::Sep: mth = wxT("of September"); break;
            case wxDateTime::Oct: mth = wxT("of October"); break;
            case wxDateTime::Nov: mth = wxT("of November"); break;
            case wxDateTime::Dec: mth = wxT("of December"); break;
        }
    }

    if (mode & WX_DATE_STR_ABBREVIATION)
    {
        mth = mth.SubString(0, 2) + wxT(".");
    }

    return mth;
}

wxString wxGetWeekDayName(wxDateTime::WeekDay week_day, unsigned int mode)
{
    wxString wd = wxEmptyString;
    switch (week_day)
    {
        case wxDateTime::Mon: wd = (mode & WX_DATE_STR_ABBREVIATION) ? wxT("Mo") : wxT("Monday"); break;
        case wxDateTime::Tue: wd = (mode & WX_DATE_STR_ABBREVIATION) ? wxT("Tu") : wxT("Tuesday"); break;
        case wxDateTime::Wed: wd = (mode & WX_DATE_STR_ABBREVIATION) ? wxT("We") : wxT("Wednesday"); break;
        case wxDateTime::Thu: wd = (mode & WX_DATE_STR_ABBREVIATION) ? wxT("Th") : wxT("Thursday"); break;
        case wxDateTime::Fri: wd = (mode & WX_DATE_STR_ABBREVIATION) ? wxT("Fr") : wxT("Friday"); break;
        case wxDateTime::Sat: wd = (mode & WX_DATE_STR_ABBREVIATION) ? wxT("Sa") : wxT("Saturday"); break;
        case wxDateTime::Sun: wd = (mode & WX_DATE_STR_ABBREVIATION) ? wxT("Su") : wxT("Sunday"); break;
    }

    return wd;
}

int wxZodiac(wxDateTime date)
{
    int dates[13][4] = {
        { 22, 12, 31, 12 }, // (0) for Capricorn at the end of the year
        { 1, 1, 19, 1 },    // (1) for Capricorn at the beginning of the year
        { 20, 1, 18, 2 },   // (2) Aquarius
        { 19, 2, 20, 3 },   // (3) Pisces
        { 21, 3, 19, 4 },   // (4) Aries
        { 20, 4, 22, 5 },   // (5) Taurus
        { 23, 5, 21, 6 },   // (6) Gemini
        { 22, 6, 22, 7 },   // (7) Cancer
        { 23, 7, 23, 8 },   // (8) Leo
        { 24, 8, 22, 9 },   // (9) Virgo
        { 23, 9, 22, 10 },  // (10) Libra
        { 23, 10, 21, 11 }, // (11) Scorpio
        { 22, 11, 21, 12 }  // (12) Sagittarius
    };

    int year = date.GetYear();

    int zodiac = -1;
    wxDateTime date1 = wxDefaultDateTime;
    wxDateTime date2 = wxDefaultDateTime;

    for (int i = 0; i < 13; i++)
    {
        date1.Set(dates[i][0], static_cast<wxDateTime::Month>(dates[i][1] - 1), year);
        date2.Set(dates[i][2], static_cast<wxDateTime::Month>(dates[i][3] - 1), year);
        if (date.IsBetween(date1, date2))
        {
            zodiac = i;
            break;
        }
    }

    if (zodiac == 0) zodiac = 1;
    return zodiac;
}

wxString wxZodiacName(wxDateTime date)
{
    wxString zodiac = wxEmptyString;

    switch (wxZodiac(date))
    {
        case 1: zodiac = wxT("Capricorn"); break;
        case 2: zodiac = wxT("Aquarius"); break;
        case 3: zodiac = wxT("Pisces"); break;
        case 4: zodiac = wxT("Aries"); break;
        case 5: zodiac = wxT("Taurus"); break;
        case 6: zodiac = wxT("Gemini"); break;
        case 7: zodiac = wxT("Cancer"); break;
        case 8: zodiac = wxT("Leo"); break;
        case 9: zodiac = wxT("Virgo"); break;
        case 10: zodiac = wxT("Libra"); break;
        case 11: zodiac = wxT("Scorpio"); break;
        case 12: zodiac = wxT("Sagittarius"); break;
    }

    return zodiac;
}

wxString wxFormatDateTime(wxDateTime dt, const wxString& format)
{
    int Y = dt.GetYear();
    wxDateTime::Month M = dt.GetMonth();
    int D = dt.GetDay();
    wxDateTime::WeekDay WD = dt.GetWeekDay();
    int h = dt.GetHour();
    int m = dt.GetMinute();
    int s = dt.GetSecond();

    int tmp;

    wxString res = wxEmptyString;
    wxString tmp_str = wxEmptyString;
    wxChar c;

	wxString format_tmp = format;
	format_tmp += ("_____"); // Protection against reading out of memory when i+n is out of string range

    for (int i = 0; (unsigned int)i < (unsigned int)format_tmp.Len(); i++)
    {
        /*
        $YYYY - year in the format 'YYYY year'
        $YYY - year in the format 'YYYY y.'
        $YY - year in the format 'YYYY'
        $Y - year in the format 'YY'
        $MMMM - month name in the nominative case
        $MMM - month name in the accusative case
        $MM - abbreviated month name
        $M - month number
        $DD - day number in the year
        $D - day number in the month
        $W - week number in the year
        $w - week number in the month
        $dd - full name of the day of the week
        $d - abbreviated name of the day of the week
        $hhh - hours in 12-hour format with AM/PM specifier
        $hh - hours in 12-hour format without specifier
        $h - hours in 24-hour format
        $m - minutes
        $s - seconds
        $ss - milliseconds
        $Z, $z - zodiac sign name for the given date
        */

        c = format_tmp.GetChar(i);
        if (c == '$')
        {
            switch ((char)format_tmp.GetChar(i + 1))
            {
            case 'Y': // YEAR
                if (format_tmp.GetChar(i + 2) == 'Y' && format_tmp.GetChar(i + 3) == 'Y' &&  format_tmp.GetChar(i + 4) == 'Y') // YYYY
                {
                    res += (wxString::Format(wxT("%d year"), Y));
                    i += 4;
                }
                else if (format_tmp.GetChar(i + 2) == 'Y' && format_tmp.GetChar(i + 3) == 'Y') // YYY
                {
                    res += (wxString::Format(wxT("%d y."), Y));
                    i += 3;
                }
                else if (format_tmp.GetChar(i + 2) == 'Y') // YY
                {
                    res += (wxString::Format(wxT("%d"), Y));
                    i += 2;
                }
                else // Y
                {
                    res += (wxString::Format(wxT("%d"), (Y % 1000)));
                    i += 1;
                }
                break;

            case 'M': // MONTH
                if (format_tmp.GetChar(i + 2) == 'M' && format_tmp.GetChar(i + 3) == 'M' &&  format_tmp.GetChar(i + 4) == 'M') // MMMM
                {
                    tmp_str = wxGetMonthName(M, WX_DATE_STR_NOMINATIVE | WX_DATE_STR_FULLNAME);
                    tmp_str.LowerCase();
                    res += tmp_str;
                    i += 4;
                }
                else if (format_tmp.GetChar(i + 2) == 'M' && format_tmp.GetChar(i + 3) == 'M') // MMM
                {
                    tmp_str = wxGetMonthName(M, WX_DATE_STR_ACCUSATIVE | WX_DATE_STR_FULLNAME);
                    tmp_str.LowerCase();
                    res += tmp_str;
                    i += 3;
                }
                else if (format_tmp.GetChar(i + 2) == 'M') // MM
                {
                    res += wxGetMonthName(M, WX_DATE_STR_NOMINATIVE | WX_DATE_STR_ABBREVIATION);
                    i += 2;
                }
                else // M
                {
                    res += (wxString::Format(wxT("%02d"), M + 1));
                    i += 1;
                }
                break;

            case 'D': // DAY
                if (format_tmp.GetChar(i + 2) == 'D') // DD
                {
                    tmp = dt.GetDayOfYear();
                    res += (wxString::Format("%03d", tmp));
                    i += 2;
                }
                else // D
                {
                    res += (wxString::Format(wxT("%02d"), D));
                    i += 1;
                }
                break;

            case 'W':
                tmp = dt.GetWeekOfYear();
                res += wxString::Format(wxT("%03d"), tmp);
                i += 1;
                break;

            case 'w':
                tmp = dt.GetWeekOfMonth();
                res += wxString::Format(wxT("%02d"), tmp);
                i += 1;
                break;

            case 'd':
                if (format_tmp.GetChar(i + 2) == 'd') // dd
                {
                    tmp_str = wxGetWeekDayName(WD, WX_DATE_STR_FULLNAME);
                    res += tmp_str;
                    i += 2;
                }
                else // d
                {
                    tmp_str = wxGetWeekDayName(WD, WX_DATE_STR_ABBREVIATION);
                    res += tmp_str;
                    i += 1;
                }
                break;

            case 'h':
                if (format_tmp.GetChar(i + 2) == 'h' && format_tmp.GetChar(i + 3) == 'h') // hhh
                {
                    if (h > 12) tmp = h - 12;
                    else tmp = h;
                    res += (wxString::Format(wxT("%02d"), tmp) + ((h > 12) ? (wxT(" PM")) : (wxT(" AM"))));
                    i += 3;
                }
                else if (format_tmp.GetChar(i + 2) == 'h') // hh
                {
                    if (h > 12) tmp = h - 12;
                    else tmp = h;
                    res += wxString::Format(wxT("%02d"), tmp);
                    i += 2;
                }
                else // h
                {
                    res += wxString::Format(wxT("%02d"), h);
                    i += 1;
                }
                break;

            case 'm':
                res += wxString::Format(wxT("%02d"), m);
                i += 1;
                break;

            case 's':
                if (format_tmp.GetChar(i + 2) == 's') // ss
                {
                    tmp = dt.GetMillisecond();
                    res += wxString::Format(wxT("%03d"), tmp);
                    i += 2;
                }
                else // s
                {
                    res += wxString::Format(wxT("%02d"), s);
                    i += 1;
                }
                break;

            case 'Z':
            case 'z':
                res += wxZodiacName(dt);
                i += 1;
                break;

            default:
                break;
            }
        }
        else
        {
            res += format_tmp.GetChar(i);
        }
    }

    return res.SubString(0, (res.Len() - 1) - 5);
}

// PERIODS 
// Specific in Polish language but partially ignored after translation
// In Polish language different numbers have different forms but
// in English they are always two only. Functions were not changed
// because they may be useful for your language.
// They demonstrates how to use different forms of numbers in different cases.
// Years
wxString wxYStr(int y)
{
    wxString s = wxEmptyString;
    if (y < 0) y = -y;

    if (y == 0) s = wxT("0 years");
    else if (y == 1) s = wxT("1 year");
    else if (y == 2 || y == 3 || y == 4) s = wxString::Format(wxT("%d years"), y);
    else if (y > 4 && y <= 19) s = wxString::Format(wxT("%d years"), y);
    else if (y >= 20)
    {
        if ((y % 10) == 2 || (y % 10) == 3 || (y % 10) == 4)
        {
            s = wxString::Format(wxT("%d years"), y);
        }
        else
        {
            s = wxString::Format(wxT("%d years"), y);
        }
    }

    return s;
}

// Months
wxString wxMStr(int m)
{
    wxString s = wxEmptyString;
    if (m < 0) m = -m;

    if (m == 0) s = wxT("0 months");
    else if (m == 1) s = wxT("1 month");
    else if (m == 2 || m == 3 || m == 4) s = wxString::Format(wxT("%d months"), m);
    else if (m > 4 && m <= 12) s = wxString::Format(wxT("%d months"), m);
    else if (m >= 20)
    {
        if ((m % 10) == 2 || (m % 10) == 3 || (m % 10) == 4)
        {
            s = wxString::Format(wxT("%d months"), m);
        }
        else
        {
            s = wxString::Format(wxT("%d months"), m);
        }
    }

    return s;
}

// Days
wxString wxDStr(int d)
{
    wxString s = wxEmptyString;
    if (d < 0) d = -d;

    if (d == 1)
    {
        s = wxT("1 day");
    }
    else
    {
        s = wxString::Format(wxT("%d days"), d);
    }

    return s;
}

// General function reading data from wxDateSpan
wxString wxYMDStr(wxDateSpan span)
{
    return wxYStr(span.GetYears()) + ", "
        + wxMStr(span.GetMonths()) + wxT(" and ")
        + wxDStr((span.GetWeeks() * 7) + span.GetDays());
}
