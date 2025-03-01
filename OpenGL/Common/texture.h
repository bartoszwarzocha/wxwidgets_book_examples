/////////////////////////////////////////////////////////////////////////////
// Name:        texture.h
// Purpose:     My wxWidgets app
// Author:      Name Surname <name.surname@email.org>
// Created:     2025-01-01
// Copyright:   (c) 2025 by Name Surname
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include <string>

// Loads images for OpenGL textures
#include <SOIL.h>

// GLEW
#include <GL/glew.h>

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

#include "shader.h"

struct Texture_data
{
	GLuint id;
	std::string path;
};

class Texture
{
public:
    Texture(const std::string& file, GLint wrap = GL_REPEAT, GLint filter = GL_LINEAR, int forceCHannels = SOIL_LOAD_RGB);
    Texture(const std::string& file, GLint wrapS, GLint wrapT, GLint filterMin, GLint filterMag, int forceChannels = SOIL_LOAD_RGB);
    virtual ~Texture() {}

    void Bind(int index = 0);
    void Unbind();

    std::string GetFile() { return this->file; }

private:
    void Create(const std::string& file, GLint wrapS, GLint wrapT, GLint filterMin, GLint filterMag, int forceChannels);

    GLuint texture_id;
    std::string file;
};

#endif

