#include"VAO.h"

VAO::VAO()
{
	glGenVertexArrays(1, &_ID);
}

void VAO::setVertexAttribSize(unsigned int dimensions, unsigned int colorAttribNumber, unsigned int textureDim, unsigned int norm)
{
	_textureDim = textureDim;
	_dimensions = dimensions;
	_colorAttribNumber = colorAttribNumber;
	_norm = norm;
}

void VAO::LinkVBO(VBO& vbo, GLuint positionLocation, GLuint colorLocation, GLuint textureLocation, GLuint aNorm)
{
	vbo.Bind();
	unsigned int vertexSize = _textureDim + _dimensions + _colorAttribNumber + _norm;
	glEnableVertexAttribArray(positionLocation);
	glVertexAttribPointer(positionLocation, _dimensions, GL_FLOAT, GL_FALSE,vertexSize* sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(colorLocation);
	glVertexAttribPointer(colorLocation, _colorAttribNumber , GL_FLOAT, GL_FALSE, vertexSize * sizeof(GLfloat), (void*)(_dimensions * sizeof(GLfloat)));
	if (textureLocation != -1)
	{
		glEnableVertexAttribArray(textureLocation);
		glVertexAttribPointer(textureLocation, _textureDim, GL_FLOAT, GL_FALSE, vertexSize * sizeof(GLfloat), (void*)((_dimensions + _colorAttribNumber) * sizeof(GLfloat)));
	}
	if (aNorm != -1)
	{
		glEnableVertexAttribArray(aNorm);
		glVertexAttribPointer(aNorm, _norm, GL_FLOAT, GL_FALSE, vertexSize * sizeof(GLfloat), (void*)((_dimensions + _colorAttribNumber + _textureDim)* sizeof(GLfloat)));
	}
	//vbo.Unbind();
}

void VAO::Bind()
{
	glBindVertexArray(_ID);
}

void VAO::Unbind()
{
	glBindVertexArray(0);
}

void VAO::Delete()
{
	glDeleteVertexArrays(1, &_ID);
}

GLuint VAO::getID()
{
	return _ID;
}