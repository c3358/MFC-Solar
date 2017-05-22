
#pragma once

//Œ∆¿Ì¿‡

#include "gl/glut.h"

class CTextures  
{
public:
	CTextures();
	virtual ~CTextures();

	int load_texture (char *file_name, int width, int height, int depth, GLenum colour_type, GLenum filter_type);
	void LoadTextures(GLuint *texture_id, int MaxNrOfTextures);
};