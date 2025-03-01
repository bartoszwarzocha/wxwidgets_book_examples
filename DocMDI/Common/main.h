/////////////////////////////////////////////////////////////////////////////
// Name:        main.h
// Purpose:     My wxWidgets app
// Author:      Imie Nazwisko <imie.nazwisko@email.org>
// Created:     2018-01-01
// Copyright:   (c) 2018 by Imie Nazwisko
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _MAIN_H_
#define _MAIN_H_

//---------------------------------------------------------------------------
// MyTextFrame class
//---------------------------------------------------------------------------
class MyTextFrame : public wxMDIChildFrame
{
public:
    MyTextFrame(wxMDIParentFrame* parent, wxWindowID id, const wxString& title)
        : wxMDIChildFrame(parent, id, title)
    {
        wxBoxSizer* s = new wxBoxSizer(wxVERTICAL);
        this->SetSizer(s);

        text = new wxTextCtrl(this, wxID_ANY, wxEmptyString);
        s->Add(text, 1, wxEXPAND | wxALL, 0);
    }

    wxTextCtrl* GetTextCtrl() { return this->text; }

private:
    wxTextCtrl* text;
};

//---------------------------------------------------------------------------
// MyDocManager class
//---------------------------------------------------------------------------
class MyDocManager : public wxDocManager
{
public:
    MyDocManager(long flags = 0, bool initialize = true)
        : wxDocManager(flags, initialize), frame(NULL) {
    }

    void SetFrame(wxDocMDIParentFrame* f) { this->frame = f; }
    wxDocMDIParentFrame* GetFrame() { return this->frame; }

private:
    wxDocMDIParentFrame* frame;
};

//---------------------------------------------------------------------------
// MyDocumentElement class
//---------------------------------------------------------------------------
class MyDocumentElement
{
public:
    wxString label;
    wxColour pen;
    wxColour brush;
    wxSize size;
    wxPoint pos;

    void Draw(wxDC* dc)
    {
        dc->SetPen(wxPen(pen));
        dc->SetBrush(wxBrush(brush));
        dc->DrawRectangle(pos.x, pos.y, pos.x + size.x, pos.y + size.y);
        dc->DrawText(label, pos.x + 10, pos.y + 10);
    }
};

//---------------------------------------------------------------------------
// MyDocument class
//---------------------------------------------------------------------------
class MyDocument : public wxDocument
{
public:
    MyDocument() : wxDocument() {}

    std::ostream& SaveObject(std::ostream& stream) wxOVERRIDE { return stream; }
    std::istream& LoadObject(std::istream& stream)
    {
        std::istream& istream = stream;
        wxDocument::LoadObject(istream);

        istream.seekg(0, istream.end);
        int len = istream.tellg();
        istream.seekg(0, istream.beg);

        char* buff = new char[len];
        istream.read(buff, len);

        wxArrayString lines;
        wxStringTokenizer lines_tkzr(wxString(buff), wxT("\n"));
        while (lines_tkzr.HasMoreTokens())
        {
            wxArrayString data;
            wxStringTokenizer data_tkzr(lines_tkzr.GetNextToken(), wxT("|"));
            MyDocumentElement element;

            element.label = data_tkzr.GetNextToken();
            element.pen.Set(data_tkzr.GetNextToken());
            element.brush.Set(data_tkzr.GetNextToken());
            data_tkzr.GetNextToken().ToLong((long*)&element.pos.x);
            data_tkzr.GetNextToken().ToLong((long*)&element.pos.y);
            data_tkzr.GetNextToken().ToLong((long*)&element.size.x);
            data_tkzr.GetNextToken().ToLong((long*)&element.size.y);

            elements.push_back(element);
        }

        delete[] buff;
        return stream;
    }

    wxVector<MyDocumentElement>& GetElements() { return this->elements; }

private:
    wxVector<MyDocumentElement> elements;

    wxDECLARE_DYNAMIC_CLASS(MyDocument);
};

//---------------------------------------------------------------------------
// MyCvs class - canvas, must have the ability to redraw
//---------------------------------------------------------------------------
class MyCvs : public wxScrolledWindow
{
public:
    MyCvs(wxView* v)
        : wxScrolledWindow(v->GetFrame())
    {
        view = v;
        SetCursor(wxCursor(wxCURSOR_CROSS));
        SetVirtualSize(800, 1600);
        SetScrollRate(25, 25);
        SetBackgroundColour(*wxWHITE);
    }

    virtual ~MyCvs() {}

    virtual void OnDraw(wxDC& dc) wxOVERRIDE
    {
        if (view) view->OnDraw(&dc);
    }

private:
    wxView* view;
};

//---------------------------------------------------------------------------
// MyView class
//---------------------------------------------------------------------------
class MyView : public wxView
{
public:
    MyView() : wxView(), cvs(NULL) {}

    bool OnCreate(wxDocument* doc, long flags) wxOVERRIDE
    {
        if (!wxView::OnCreate(doc, flags))
            return false;

        MyDocManager* dm = (MyDocManager*)GetDocumentManager();

        wxDocMDIChildFrame* frame = new wxDocMDIChildFrame
        (
            GetDocument(),
            this,
            dm->GetFrame(),
            wxID_ANY,
            wxT("Document")
        );

        cvs = new MyCvs(this);

        frame->Show(true);
        return true;
    }

    virtual void OnDraw(wxDC* dc) wxOVERRIDE
    {
        MyDocument* doc = (MyDocument*)this->GetDocument();
        for (MyDocumentElement e : doc->GetElements())
        {
            e.Draw(dc);
        }
    }

    virtual void OnUpdate(wxView* sender, wxObject* hint = NULL) wxOVERRIDE
    {
        cvs->Refresh();
    }

    virtual bool OnClose(bool deleteWindow = true) wxOVERRIDE
    {
        cvs->Destroy();
        return true;
    }

private:
    MyCvs* cvs;

    wxDECLARE_DYNAMIC_CLASS(MyView);
};

//---------------------------------------------------------------------------
// MyApp class
//---------------------------------------------------------------------------
class MyApp : public wxApp
{
public:
    virtual bool OnInit();
    virtual int OnExit();

protected:
    wxLocale loc;
};

//---------------------------------------------------------------------------
// MyFrame class
//---------------------------------------------------------------------------
class MyFrame : public wxDocMDIParentFrame
{
public:
    MyFrame(MyDocManager* man, wxFrame* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style);
    ~MyFrame() {}

    void OnAboutProgram(wxCommandEvent& event);
    void OnClose(wxCommandEvent& event);
    void CloseApp(wxCloseEvent& event);
    void OnManage(wxCommandEvent& event);

private:
    unsigned int counter;
    MyDocManager* manager;

    DECLARE_EVENT_TABLE()
};

#endif
