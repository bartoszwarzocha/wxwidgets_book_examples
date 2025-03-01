/////////////////////////////////////////////////////////////////////////////
// Name:        main.cpp
// Purpose:     My wxWidgets app
// Author:      Name Surname <name.surname@email.org>
// Created:     2025-01-01
// Copyright:   (c) 2025 by Name Surname
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

// GLEW
#include "GL/glew.h"

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// wxWidgets
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <wx/glcanvas.h>
#include <wx/stopwatch.h>

// OpenGL tools
#include "shader.h"
#include "texture.h"

// App
#include "main.h"

// Coordinates of the cube and texture
float verts[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

// Shaders
const char* solid_vertex_src = "#version 330 core\n"
	"layout(location = 0) in vec3 pos;\n"
	"layout(location = 1) in vec2 texCoord;\n"
	"out vec2 TexCoord;\n"
	"uniform mat4 model;\n"
	"uniform mat4 view;\n"
	"uniform mat4 projection;\n"
	"void main()\n"
	"{\n"
	"    gl_Position = projection * view * model * vec4(pos, 1.0f);\n"
	"    TexCoord = vec2(texCoord.x, texCoord.y);\n"
    "}\n\0";

const char* solid_fragment_src = "#version 330 core\n"
    "out vec4 FragColor;\n"
	"in vec2 TexCoord;\n"
	"uniform sampler2D texture1;\n"
	"void main()\n"
	"{\n"
	"    FragColor = texture(texture1, TexCoord);\n"
    "}\n\0";

const char* mesh_vertex_src = "#version 330 core\n"
	"layout(location = 0) in vec3 pos;\n"
	"uniform mat4 model;\n"
	"uniform mat4 view;\n"
	"uniform mat4 projection;\n"
	"void main()\n"
	"{\n"
	"    gl_Position = projection * view * model * vec4(pos, 1.0f);\n"
	"}\n\0";

const char* mesh_fragment_src = "#version 330 core\n"
	"out vec4 FragColor;\n"
	"void main()\n"
	"{\n"
	"   FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"
	"}\n\0";

const wxString appName = wxT("Test OpenGL 1.0.0");

//---------------------------------------------------------------------------
// Application implementation
//---------------------------------------------------------------------------
IMPLEMENT_APP(MyApp)

//---------------------------------------------------------------------------
// Event IDs
//---------------------------------------------------------------------------
enum
{
	ID_CLOSE = wxID_HIGHEST + 1,
	ID_ABOUT_PROGRAM,
	ID_MESH,
};

//---------------------------------------------------------------------------
// Events table
//---------------------------------------------------------------------------
BEGIN_EVENT_TABLE(MyFrame, wxFrame)
	EVT_MENU(ID_CLOSE, MyFrame::OnClose)
	EVT_MENU(ID_ABOUT_PROGRAM, MyFrame::OnAboutProgram)
	//
	EVT_PAINT(MyFrame::OnPaint)
	EVT_IDLE(MyFrame::OnIdle)
	//
	EVT_MENU(ID_MESH, MyFrame::OnMesh)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(MyCanvas, wxGLCanvas)
	EVT_SIZE(MyCanvas::OnSize)
END_EVENT_TABLE()

//---------------------------------------------------------------------------
// OnInit()
//---------------------------------------------------------------------------
bool MyApp::OnInit()
{
	wxInitAllImageHandlers();

    MyFrame *frame = new MyFrame(NULL, wxID_ANY, appName, wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE);
    frame->Show(true);
    return true;
}

//---------------------------------------------------------------------------
// MyFrame constructor
//---------------------------------------------------------------------------
MyFrame::MyFrame(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &pos, const wxSize &size, long style)
: wxFrame(parent, id, title, pos, size, style)
{
    mb = new wxMenuBar();

    mFile = new wxMenu();
	mView = new wxMenu();
    mHelp = new wxMenu();

	mFile->AppendSeparator();
	mFile->Append(ID_CLOSE, wxT("&Close program\tCtrl-X"), wxT("Closes the program"));

	mView->AppendCheckItem(ID_MESH, wxT("Mesh view\tF5"));

	mHelp->Append(ID_ABOUT_PROGRAM, wxT("&About program\tF1"), wxT("About the program"));

	mb->Append(mFile, wxT("&File"));
	mb->Append(mView, wxT("&View"));
	mb->Append(mHelp, wxT("&Help"));

	this->SetMenuBar(mb);

    sb = new wxStatusBar(this);
    sb->SetFieldsCount(2);
    this->SetStatusBar(sb);
    SetStatusText(title, 0);

	wxBoxSizer* ms = new wxBoxSizer(wxVERTICAL);
	this->SetSizer(ms);

	panel = new wxPanel(this);
	wxBoxSizer* ps = new wxBoxSizer(wxVERTICAL);
	panel->SetSizer(ps);

	timer.Start();

	///////////////////////////////////////

	// Canvas attributes
	wxGLAttributes vAttrs;
	vAttrs.PlatformDefaults().RGBA().DoubleBuffer().Depth(16).EndList();

	if (!wxGLCanvas::IsDisplaySupported(vAttrs))
	{
		wxLogError(wxT("Unsupported OpenGL configuration!"));
		return;
	}

	// Create canvas
	canvas = new MyCanvas(panel, vAttrs, wxID_ANY);
	canvas->SetMinSize(wxSize(500, 500));

	// Context attributes
	wxGLContextAttrs cAttrs;
	cAttrs.PlatformDefaults().CoreProfile().OGLVersion(4, 5).EndList();

	// Create context
	ctx = new wxGLContext(canvas, (wxGLContext*)NULL, &cAttrs);

	if (!(*ctx).IsOK())
	{
		wxLogError(wxT("OpenGL context is invalid."));
		return;
	}

	// Set context
	canvas->SetCurrent(*ctx);

	ps->Add(canvas, 1, wxEXPAND | wxALL, 0);
	ms->Add(panel, 1, wxEXPAND | wxALL, 0);

	//*******************************************
	// OpenGL
	//*******************************************
	// Init GLEW
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (err != GL_NO_ERROR)
	{
		wxLogError(wxString(glewGetErrorString(err)));
	}

	// Load informations about OpenGL and hardware
	this->GLversion = wxString(glGetString(GL_VERSION));
	this->GLvendor = wxString(glGetString(GL_VENDOR));
	this->GLrenderer = wxString(glGetString(GL_RENDERER));

	// Enable 3D supprt
	glEnable(GL_DEPTH_TEST);

	// Init VAO and VBO
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Shaders
	mesh_shader = new Shader(mesh_vertex_src, mesh_fragment_src);
	solid_shader = new Shader(solid_vertex_src, solid_fragment_src);
	active_shader = solid_shader;

	// Load texture
	texture = new Texture("texture.png", GL_REPEAT, GL_LINEAR, SOIL_LOAD_RGBA);

	//
	Fit();
    CenterOnScreen();
}

//---------------------------------------------------------------------------
// MyFrame destructor
//-------------------------------------------------------------------------
MyFrame::~MyFrame()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	wxDELETE(solid_shader);
	wxDELETE(mesh_shader);
	wxDELETE(texture);

	wxDELETE(ctx);
	wxDELETE(canvas);
}

//---------------------------------------------------------------------------
// Function to initiate program closure
//---------------------------------------------------------------------------
void MyFrame::OnClose(wxCommandEvent& WXUNUSED(event))
{
    this->Close(true);
}

//---------------------------------------------------------------------------
// Function to display the "About program..." window
//---------------------------------------------------------------------------
void MyFrame::OnAboutProgram(wxCommandEvent& WXUNUSED(event))
{
	wxString msg = appName + wxT("\nCopyright (c) 2025 by ...\n\n");
	msg += GLversion + wxT("\n");
	msg += GLvendor + wxT("\n");
	msg += GLrenderer + wxT("\n");
    wxMessageBox(msg, wxT("About program..."), wxOK|wxICON_INFORMATION);
}

//---------------------------------------------------------------------------
// Function to switch between normal and mesh view
//---------------------------------------------------------------------------
void MyFrame::OnMesh(wxCommandEvent& event)
{
	if (event.IsChecked())
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		active_shader = mesh_shader;
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		active_shader = solid_shader;
	}
}

//---------------------------------------------------------------------------
// Function to handle window repaint event
//---------------------------------------------------------------------------
void MyFrame::OnPaint(wxPaintEvent& WXUNUSED(event))
{
	Render();
}

//---------------------------------------------------------------------------
// Function to perform drawing during window idle time
//---------------------------------------------------------------------------
void MyFrame::OnIdle(wxIdleEvent& event)
{
	Render();

	// Ensure continuity...
	event.RequestMore();
}

//---------------------------------------------------------------------------
// Drawing function
//---------------------------------------------------------------------------
void MyFrame::Render()
{
	if (!this->IsShown()) return;

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//
	texture->Bind(0);
	active_shader->Bind();

	//
	glUseProgram(active_shader->GetProgram());
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;
	model = glm::rotate(model, (GLfloat)timer.TimeInMicro().ToDouble() / 1000000, glm::vec3(0.5f, 1.0f, 0.0f));
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
	projection = glm::perspective(glm::radians(45.0f), (float)canvas->x / (float)canvas->y, 0.1f, 100.0f);

	glUniformMatrix4fv(glGetUniformLocation(active_shader->GetProgram(), "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(active_shader->GetProgram(), "view"), 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(active_shader->GetProgram(), "projection"), 1, GL_FALSE, &projection[0][0]);

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	canvas->SwapBuffers();
}