#pragma once
#include <GL/glew.h>
#include"VBO.h"

#define DIMENSION 3
#define VERTEX_SIZE 6
#define COLOR_ATTRIB 3

class VAO
{
public:
	VAO();
	void setVertexAttribSize(unsigned int _dimensions, unsigned int _colorAttribNumber, unsigned int _textureDim = 0, unsigned int norm = 0);
	void LinkVBO(VBO& vbo, GLuint positionLocation, GLuint colorLocation, GLuint textureLocation = -1, GLuint aNorm  = -1);
	void Bind();
	void Unbind();
	void Delete();

	GLuint getID();
private:
	GLuint _ID;
	unsigned int _textureDim;
	unsigned int _dimensions;
	unsigned int _colorAttribNumber;
	unsigned int _norm;
};