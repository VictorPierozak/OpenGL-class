#pragma once

#include <GL/glew.h>
#include<vector>
#include<fstream>

struct GeometryConfig
{
	GLfloat* data;
	GLuint* elements;
	GLuint elementsNum;

	unsigned int verticesNumber;
	unsigned int textureDim;
	unsigned int dimensions;
	unsigned int colorAttribNumber;
	unsigned int norm;
};

class Geometry
{
public:
	Geometry();
	Geometry(GeometryConfig& config);
	Geometry(const char* geometryFile);
	~Geometry();

	bool readGeometryFile(const char* geometryFile);
	bool readElementsFile(const char* eboFile);
	bool isEmpty();

	unsigned int getDataSize() const;
	unsigned int getVerticesNumber() const;
	unsigned int getDim() const;
	unsigned int getColorAttribNum() const;
	unsigned int getTextureDim() const;
	unsigned int getVertexSize() const;
	unsigned int getNorm() const
	{
		return _norm;
	}

	unsigned int coordinatesOffset() const;
	unsigned int colorOffset() const;
	unsigned int textureOffset() const;

	GLfloat* copy();
	GLuint* elements();
	GLuint getElemNum();

	void transition(float x, float y, float z);
	void scale(float x, float y, float z);

	GLfloat& operator[](int pos);

	explicit operator GLfloat* ()
	{
		return _data;
	}
	

private:
	GLfloat* _data;
	GLuint* _elements;
	GLuint _elementsNum;

	unsigned int _verticesNumber;
	unsigned int _textureDim;
	unsigned int _dimensions;
	unsigned int _colorAttribNumber;
	unsigned int _norm;

	friend void compose(Geometry A, Geometry B, Geometry& scene);
};


void compose(Geometry A, Geometry B, Geometry& scene);