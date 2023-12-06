#pragma once

#include <GL/glew.h>
#include<string>
#include<fstream>
#include<sstream>
#include<cerrno>

std::string get_file_contents(const char* filename);

class Shader
{
public:
	Shader(const char* vertexFile, const char* fragmentFile);

	void Activate();
	void Delete();

	GLuint getID();

private:
	void checkCompilationStatus(unsigned int shader, const char* type, std::string vertexCode);
	GLuint _ID;
};