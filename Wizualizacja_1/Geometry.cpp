#include"Geometry.h"


Geometry::Geometry()
{
	_data = nullptr;
	_elements = nullptr;
	_verticesNumber = 0;
	_dimensions = 0;
	_colorAttribNumber = 0;
}

Geometry::Geometry(GeometryConfig& config) 
{
	_data = new GLfloat[config.verticesNumber * (config.dimensions + config.textureDim + config.colorAttribNumber + config.norm)];
	std::memcpy(_data, config.data, sizeof(GLfloat) * config.verticesNumber * (config.dimensions + config.textureDim + config.colorAttribNumber + config.norm));
	_elements = config.elements;
	_elementsNum = config.elementsNum;

	_verticesNumber = config.verticesNumber;
	_textureDim = config.textureDim;
	_dimensions = config.dimensions;
	_colorAttribNumber = config.colorAttribNumber;
	_norm = config.norm;
}

Geometry::Geometry(const char* geometryFile)
{
	_data = nullptr;
	_elements = nullptr;
	readGeometryFile(geometryFile);
}

Geometry::~Geometry()
{
	delete[] _data;
}

bool Geometry::readGeometryFile(const char* geometryFile)
{
	std::ifstream in(geometryFile);
	if (_data != nullptr)
		delete[] _data;

	if (!in)
	{
		_data = nullptr;
		_verticesNumber = 0;
		_dimensions = 0;
		_colorAttribNumber = 0;
		return false;
	}

	in >> _verticesNumber;
	in >> _dimensions;
	in >> _colorAttribNumber;
	in >> _textureDim;
	in >> _norm;

	unsigned int dataSize = _verticesNumber * (_dimensions + _colorAttribNumber + _textureDim + _norm);
	_data = new GLfloat[dataSize];
	for (unsigned int i = 0; i < dataSize; i++)
		in >> _data[i];
	return true;
}

bool Geometry::isEmpty()
{
	return _data == nullptr;
}

unsigned int Geometry::getDataSize() const
{
	return _verticesNumber * (_dimensions + _colorAttribNumber + _textureDim + _norm);
}

unsigned int Geometry::getVerticesNumber() const
{
	return _verticesNumber;
}

unsigned int Geometry::getDim() const
{
	return _dimensions;
}

unsigned int  Geometry::getColorAttribNum() const
{
	return _colorAttribNumber;
}

unsigned int Geometry::getTextureDim() const
{
	return _textureDim;
}

unsigned int Geometry::getVertexSize() const
{
	return _textureDim + _colorAttribNumber + _dimensions + _norm;
}

unsigned int Geometry::coordinatesOffset() const
{
	return 0;
}

unsigned int Geometry::colorOffset() const
{
	return _dimensions;
}

unsigned int Geometry::textureOffset() const
{
	return _dimensions + _colorAttribNumber;
}

GLfloat* Geometry::copy()
{
	GLfloat* copy = new GLfloat[_verticesNumber * (_dimensions + _colorAttribNumber)];
	std::memcpy(copy, _data, _verticesNumber * (_dimensions + _colorAttribNumber) * sizeof(GLfloat));
	return copy;
}

GLfloat& Geometry::operator[](int pos)
{
	return _data[pos];
}

bool Geometry::readElementsFile(const char* eboFile)
{
	std::ifstream in(eboFile);
	if (_elements != nullptr)
		delete[] _elements;

	if (!in)
	{
		_elements = nullptr;
	}

	GLuint elementsNumber;
	in >> elementsNumber;
	/*if (elementsNumber != (_verticesNumber / (_dimensions + _colorAttribNumber + _textureDim)))
	{
		in.close();
		return false;
	}*/
	_elementsNum = elementsNumber;

	_elements = new GLuint[elementsNumber * 3];
	for (unsigned int i = 0; i < elementsNumber * 3; i++)
		in >> _elements[i];
	in.close();
	return true;
}

GLuint* Geometry::elements()
{
	return _elements;
}

GLuint Geometry::getElemNum()
{
	return _elementsNum;
}

void Geometry::transition(float x, float y, float z)
{
	unsigned int vertex = (_textureDim + _colorAttribNumber + _dimensions + _norm);
	unsigned int size = _verticesNumber * vertex;
	for (unsigned int i = 0; i < size; i += vertex)
	{
		_data[i] += x;
		_data[i + 1] += y;
		_data[i + 2] += z;
	}
}

void Geometry::scale(float x, float y, float z)
{
	unsigned int vertex = (_textureDim + _colorAttribNumber + _dimensions + _norm);
	unsigned int size = _verticesNumber * vertex;
	for (unsigned int i = 0; i < size; i += vertex)
	{
		_data[i] *= x;
		_data[i + 1] *= y;
		_data[i + 2] *= z;
	}
}

void compose(Geometry A, Geometry B, Geometry& scene)
{

	if (A._dimensions != B._dimensions);
	if (A._colorAttribNumber != B._colorAttribNumber);
	if (A._textureDim != B._textureDim);
	if (A._norm != B._norm);
	
	scene._data = new GLfloat[A.getDataSize() + B.getDataSize()];
	std::memcpy(scene._data, A._data, sizeof(GLfloat) * A.getDataSize());
	std::memcpy(scene._data +  A.getDataSize(), B._data, sizeof(GLfloat) * B.getDataSize());
	scene._dimensions = A._dimensions;
	scene._colorAttribNumber = A._colorAttribNumber;
	scene._textureDim = A._textureDim;
	scene._norm = A._norm;
	scene._verticesNumber = A._verticesNumber + B._verticesNumber;
}

