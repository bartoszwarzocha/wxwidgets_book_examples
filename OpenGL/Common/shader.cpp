/////////////////////////////////////////////////////////////////////////////
// Name:        shader.cpp
// Purpose:     My wxWidgets app
// Author:      Name Surname <name.surname@email.org>
// Created:     2025-01-01
// Copyright:   (c) 2025 by Name Surname
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#include "shader.h"

// Shader
Shader::Shader(const char* vertexShaderSource, const char* fragmentShaderSource)
{
	int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		wxLogError(wxT("ERROR::SHADER::VERTEX::COMPILATION_FAILED"));
	}

	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		wxLogError(wxT("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED"));
	}

	this->program = glCreateProgram();
	glAttachShader(this->program, vertexShader);
	glAttachShader(this->program, fragmentShader);
	glLinkProgram(this->program);
	glGetProgramiv(this->program, GL_LINK_STATUS, &success);
	if (!success) 
	{
		glGetProgramInfoLog(this->program, 512, NULL, infoLog);
		wxLogError(wxT("ERROR::SHADER::PROGRAM::LINKING_FAILED"));
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}
