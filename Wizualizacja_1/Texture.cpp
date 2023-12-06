#include"Texture.h"
#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#endif // !1


Texture::Texture(const char* image, GLenum texType, GLenum slot, GLenum format, GLenum pixelType)
{
	int widthImg, heightImg, numColCh;
	
	unsigned char* bytes = stbi_load(image, &widthImg, &heightImg, &numColCh, 0);

	glGenTextures(1, &_ID);
	glActiveTexture(slot);
	glBindTexture(texType, _ID);
	_type = texType;

	glBindTexture(texType, _ID);

	glTexParameteri(texType, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(texType, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glTexParameteri(texType, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(texType, GL_TEXTURE_WRAP_T, GL_REPEAT);

	GLint mipMapLevel = 0;
	GLint border = 0;
	glTexImage2D(texType, mipMapLevel, GL_RGBA, widthImg, heightImg, border, format, pixelType, (void*)bytes);
	glGenerateMipmap(texType);

	stbi_image_free(bytes);

	glBindTexture(texType, 0);
}

void Texture::texUnit(Shader& shader, const char* uniform, GLuint unit)
{
	GLuint texUni = glGetUniformLocation(shader.getID(), uniform);
	shader.Activate();
	glUniform1i(texUni, unit);
}

void Texture::Bind()
{
	glBindTexture(_type, _ID);
}

void Texture::Unbind()
{
	glBindTexture(_type, 0);
}

void Texture::Delete()
{
	glDeleteTextures(1, &_ID);
}

GLuint Texture::getID() const
{
	return _ID;
}

GLenum Texture::getType() const
{
	return _type;
}