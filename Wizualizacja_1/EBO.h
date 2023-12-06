#pragma once
#include <GL/glew.h>

class EBO
{
public:
	EBO(GLuint* indeces, GLsizeiptr size);
	void Bind();
	void Unbind();
	void Delete();

	GLuint getID();

private:
	GLuint _ID;
};