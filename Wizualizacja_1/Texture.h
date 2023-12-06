#pragma once

#include<gl\glew.h>


#include"ShaderClass.h"

class Texture
{
public:
	Texture(const char* image, GLenum texType, GLenum slot, GLenum format, GLenum pixelType);

	//Assigns the texture unit to the texture
	void texUnit(Shader& shader, const char* uniform, GLuint unit);
	void Bind();
	void Unbind();
	void Delete();

	GLuint getID() const;
	GLenum getType() const;

private:
	GLuint _ID;
	GLenum _type;
};