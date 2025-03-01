/////////////////////////////////////////////////////////////////////////////
// Name:        texture.cpp
// Purpose:     My wxWidgets app
// Author:      Name Surname <name.surname@email.org>
// Created:     2025-01-01
// Copyright:   (c) 2025 by Name Surname
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#include "texture.h"

Texture::Texture(const std::string& file, GLint wrap, GLint filter, int forceChannels)
{
    Create(file, wrap, wrap, filter, filter, forceChannels);
}

Texture::Texture(const std::string& file, GLint wrapS, GLint wrapT, GLint filterMin, GLint filterMag, int forceChannels)
{
    Create(file, wrapS, wrapT, filterMin, filterMag, forceChannels);
}

void Texture::Create(const std::string& file, GLint wrapS, GLint wrapT, GLint filterMin, GLint filterMag, int forceChannels)
{
    this->file = file;

    // Load and create a texture 
    glGenTextures(1, &this->texture_id);
    glBindTexture(GL_TEXTURE_2D, this->texture_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterMin);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMag);

    int width, height;
    unsigned char* image = SOIL_load_image(this->file.c_str(), &width, &height, 0, forceChannels);

    glTexImage2D(GL_TEXTURE_2D, 0, ((forceChannels == SOIL_LOAD_RGBA) ? (GL_RGBA) : (GL_RGB)), width, height, 0, ((forceChannels == SOIL_LOAD_RGBA) ? (GL_RGBA) : (GL_RGB)), GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);

    SOIL_free_image_data(image);

    this->Unbind();
}

void Texture::Bind(int index)
{
    glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(GL_TEXTURE_2D, this->texture_id);
}

void Texture::Unbind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}


