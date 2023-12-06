#include"ShaderClass.h"
#include<iostream>

std::string get_file_contents(const char* filename)
{
	std::ifstream in(filename, std::ios::binary);
	if (in)
	{
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());

		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		return (contents);
	}
	throw(errno);
}

Shader::Shader(const char* vertexFile, const char* fragmentFile)
{
	std::string vertexCode = get_file_contents(vertexFile);
	std::string fragmentCode = get_file_contents(fragmentFile);

	const char* vertexSource = vertexCode.c_str();
	const char* fragmentSource = fragmentCode.c_str();

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);
	
	checkCompilationStatus(vertexShader, "VERTEX", vertexCode);


	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);

	checkCompilationStatus(fragmentShader, "FFRAGMENT", fragmentCode);

	_ID = glCreateProgram();

	glAttachShader(_ID, vertexShader);
	glAttachShader(_ID, fragmentShader);

	glLinkProgram(_ID);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

}

void Shader::Activate()
{
	glUseProgram(_ID);
}

void Shader::Delete()
{
	glDeleteProgram(_ID);
}

GLuint Shader::getID()
{
	return _ID;
}

void Shader::checkCompilationStatus(unsigned int shader, const char* type, std::string shaderCode)
{
	GLint hasCompiled;
	char infoLog[1024];

	if (strcmp(type, "PROGRAM") != 0)
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE) glGetShaderInfoLog(shader, 1024, NULL, infoLog);
		std::cout << type << " SHADER ---> COMPILATION STATUS: ";
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE) glGetShaderInfoLog(shader, 1024, NULL, infoLog);
		std::cout << type << " ---> LINKING STATUS: ";
	}

	if (hasCompiled == GL_TRUE)
	{
		std::cout << "OK" << std::endl;
	}
	else
	{
		std::cout << "BAD" << std::endl;
		std::cout << shaderCode << std::endl;
	}
}