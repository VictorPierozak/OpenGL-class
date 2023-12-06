#include "PointLight.h"

void PointLight::setBrightness(float b)
{
	_brightness = b;
	glUniform1f(_brightnessGL, _brightness);
}
void PointLight::setColor(float R, float G, float B)
{
	_color[0] = R;
	_color[1] = G;
	_color[2] = B;
	glUniform3fv(_colorGL, 1, _color);
}
void PointLight::setPos(float* pos)
{
	_pos[0] = pos[0];
	_pos[1] = pos[1];
	_pos[2] = pos[2];
	glUniform3fv(_posGL, 1, _pos);
}

float PointLight::getBrightness()
{
	return _brightness;
}

void PointLight::increaseBrightness(float v)
{
	_brightness += v;
	//if (_brightness > 1) _brightness = 1;
	glUniform1f(_brightnessGL, _brightness);
}

void PointLight::decreaeBrightness(float v)
{
	_brightness -= v;
	if (_brightness < 0) _brightness = 0;
	glUniform1f(_brightnessGL, _brightness);
}