#pragma once
#include <GL/glew.h>

class VBO
{
public:
	VBO(GLfloat* vertices, GLsizeiptr size);
	void Bind();
	void Unbind();
	void Delete();

	GLuint getID();

private:
	GLuint _ID;
};