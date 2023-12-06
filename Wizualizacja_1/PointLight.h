#pragma once
#include <GL/glew.h>


class PointLight
{

public:
	PointLight(GLuint brightnessGL, GLuint lightPosGL, GLuint colorGL) : 
		_brightnessGL(brightnessGL), _posGL(lightPosGL), _colorGL(colorGL)
	{
		_brightness = 0.5;
		_pos[0] = 0;
		_pos[1] = 0;
		_pos[2] = 0;
		_color[0] = 0.5;
		_color[1] = 0.5;
		_color[2] = 0.5;
	}

	void setBrightness(float);
	void setColor(float R, float G, float B);
	void setPos(float*);

	float getBrightness();

	void increaseBrightness(float v);
	void decreaeBrightness(float v);

private:

	float _brightness;
	GLuint _brightnessGL;

	float _pos[3];
	GLuint _posGL;

	float _color[3];
	GLuint _colorGL;
};