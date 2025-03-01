/////////////////////////////////////////////////////////////////////////////
// Name:        MySplashScreen.h
// Purpose:     My wxWidgets app
// Author:      Name Surname <name.surname@email.org>
// Created:     2025-01-01
// Copyright:   (c) 2025 by Name Surname
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _MYSPLASHSCREEN_H_
#define _MYSPLASHSCREEN_H_

#include <wx/splash.h>

struct MySplashLabel
{
    wxString label;
    wxPoint pos;
    wxFont font;
    wxColour col;
};

class MySplashScreen : public wxSplashScreen
{
public:
    MySplashScreen(const wxBitmap &bitmap, long splashStyle, int milliseconds, wxWindow *parent, wxWindowID id, const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxDefaultSize, long style = wxBORDER_SIMPLE | wxFRAME_NO_TASKBAR | wxSTAY_ON_TOP)
        : wxSplashScreen(bitmap, splashStyle, milliseconds, parent, id, pos, size, style), background(bitmap) {}
    virtual ~MySplashScreen() {}

    void AddLabelField(const wxPoint& pos, const wxFont& font, const wxColour& col)
    {
        MySplashLabel l;
        l.label = wxEmptyString;
        l.pos = pos;
        l.font = font;
        l.col = col;

        fields.push_back(l);
    }

    void SetLabel(int index, const wxString& label)
    {
        if (index < 0 || index > fields.size() - 1) return;

        fields[index].label = label;
        Draw();
    }

private:
    void Draw()
    {
        if (fields.size() > 0) 
        {
            wxBitmap splash(background);
            wxMemoryDC dc;

            dc.SelectObject(splash);
            dc.SetBackgroundMode(wxTRANSPARENT);

            for (MySplashLabel l : fields)
            {
                dc.SetTextForeground(l.col);
                dc.SetFont(l.font);
                dc.DrawText(l.label, l.pos.x, l.pos.y);
            }

            dc.SelectObject(wxNullBitmap);

            GetSplashWindow()->SetBitmap(splash);
            GetSplashWindow()->Refresh();
            GetSplashWindow()->Update();
        }
    }

    wxBitmap background;
    wxVector<MySplashLabel> fields;
};



#endif