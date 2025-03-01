/////////////////////////////////////////////////////////////////////////////
// Name:        shader.h
// Purpose:     My wxWidgets app
// Author:      Name Surname <name.surname@email.org>
// Created:     2025-01-01
// Copyright:   (c) 2025 by Name Surname
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _SHADER_H_
#define _SHADER_H_

#include <string>
#include <cstring>
#include <fstream>
#include <sstream>
#include <iostream>

// GLEW
#include <GL/glew.h>

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

class Shader
{
public:
    Shader(const char* vertex = NULL, const char* fragment = NULL);

    virtual ~Shader() {}

    GLuint GetProgram()
    {
        return this->program;
    }

    void Bind()
    {
        glUseProgram(this->program);
    }

    void Unbind()
    {
        glUseProgram(0);
    }

private:
    GLuint program;
};

#endif
