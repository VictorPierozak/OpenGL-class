#pragma once
#include <GL/glew.h>
#include <SFML/Window.hpp>
#include<cmath>
#include<iostream>
#include"glm/glm.hpp"
#include"glm/gtc/matrix_transform.hpp"
#include"glm/gtc/type_ptr.hpp"


class Camera
{
public:
	Camera(GLuint shaderProgram);

	void setPosition(glm::vec3 pos);
	void setDirection(glm::vec3 direction);
	void setProjectionMatrix(glm::mat4 projection);
	void setModelMatrix(glm::mat4 model);

	glm::vec3 getPosition();
	glm::vec3 getDirection();
	glm::vec3 getUp();
	glm::vec3 getRight();
	glm::mat4 getProjectionMatrix();
	glm::mat4 getModelMatrix();

	void setCameraMovementSpeed(GLfloat speed);
	void setCameraRotationSpeed(GLfloat speed);
	GLfloat getMovementSpeed() const;
	GLfloat getRotationSpeed() const;

	void setVerticalAngleUpperLimit(GLfloat angle);
	void setVerticalAngleLowerLimit(GLfloat angle);
	void set_M_TimeNormalizationFactor(GLfloat sensitivity);
	void set_K_TimeNormalizationFactor(GLfloat sensitivity);

	void mouseHorizontalRotation(int xOffset, sf::Time time);
	void mouseVerticalRotation(int yOffset, sf::Time time);

	void updateView();
	void moveInCameraDirection(int sign, sf::Time time);
	void moveForwardBackward(int sign, sf::Time time);
	void moveLeftRight(int sign, sf::Time time);
	void rotateVertical(GLfloat angle);
	void rotateHorizontal(GLfloat angle);
	void rotateMouse(int xOffset, int yOffset, sf::Time time);

	void rotateVertical(int sign);
	void rotateHorizontal(int sign);

	void printCameraInformation();

private:

	glm::vec3 _position;
	glm::vec3 _direction;
	glm::vec3 _up;
	glm::vec3 _right;

	glm::mat4 _viewMatrix;
	glm::mat4 _projectionMatrix;
	glm::mat4 _modelMatrix;

	GLfloat _movementSpeed;
	GLfloat _rotationSpeed;

	GLfloat _sensitivity{ 0.01 };

	GLfloat _verticalAngle;
	GLfloat _horizontalAngle;
	GLfloat _verticalAngleUpperLimit;
	GLfloat _verticalAngleLowerLimit;

	GLfloat _M_timeNormalizationFactor;
	GLfloat _K_timeNormalizationFactor;

	GLuint _shaderProgram;
};

