/////////////////////////////////////////////////////////////////////////////
// Name:        main.h
// Purpose:     My wxWidgets app
// Author:      Name Surname <name.surname@email.org>
// Created:     2025-01-01
// Copyright:   (c) 2025 by Name Surname
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _MAIN_H_
#define _MAIN_H_

//---------------------------------------------------------------------------
// MyCanvas class
//---------------------------------------------------------------------------
class MyCanvas : public wxGLCanvas
{
public:
	MyCanvas(wxWindow *parent, const wxGLAttributes &attrs, wxWindowID id = wxID_ANY)
		: wxGLCanvas(parent, attrs, id) {}

	virtual ~MyCanvas() {}

	int x, y;

private:
	void OnSize(wxSizeEvent& event)
	{
		x = event.GetSize().GetWidth();
		y = event.GetSize().GetHeight();
		glViewport(0, 0, x, y);
	}

	DECLARE_EVENT_TABLE();
};

//---------------------------------------------------------------------------
// MyApp class
//---------------------------------------------------------------------------
class MyApp: public wxApp
{
    public:
        virtual bool OnInit();
};

//---------------------------------------------------------------------------
// MyFrame class
//---------------------------------------------------------------------------
class MyFrame: public wxFrame
{
    public:
        MyFrame(wxWindow *parent, wxWindowID id, const wxString &title,
            const wxPoint &pos, const wxSize &size, long style);
		~MyFrame();

		void OnAboutProgram(wxCommandEvent& event);
		void OnClose(wxCommandEvent& event);

		void OnMesh(wxCommandEvent& event);

		void OnPaint(wxPaintEvent& event);
		void OnIdle(wxIdleEvent& event);
		void OnSize(wxSizeEvent& event);

    private:
		void Render();

        wxStatusBar *sb;
        wxMenuBar *mb;
        wxMenu *mFile;
		wxMenu *mView;
        wxMenu *mHelp;

		wxPanel *panel;

		//
		wxStopWatch timer;

		MyCanvas* canvas;
		wxGLContext* ctx;

		wxString GLversion;
		wxString GLvendor;
		wxString GLrenderer;

		unsigned int VBO, VAO;
		Shader* solid_shader;
		Shader* mesh_shader;
		Shader* active_shader;
		Texture* texture;

		DECLARE_EVENT_TABLE()
};



#endif
