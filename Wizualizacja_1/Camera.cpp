#include "Camera.h"
Camera::Camera(GLuint shaderProgram)
{
	_shaderProgram = shaderProgram;
}

void Camera::setPosition(glm::vec3 pos)
{
	_position = pos;
}
void Camera::setDirection(glm::vec3 direction)
{
	_direction = direction;
	_verticalAngle = asin(_direction.y); //asin(direction.y / (direction.x * direction.x + direction.z * direction.z));
	_horizontalAngle = asin(-direction.z/sqrt(direction.x*direction.x + direction.z *direction.z));
	updateView();
}

void Camera::setProjectionMatrix(glm::mat4 projection)
{
	_projectionMatrix = projection;
	GLint uniProj = glGetUniformLocation(_shaderProgram, "proj");
	glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(_projectionMatrix));
}

void Camera::setModelMatrix(glm::mat4 model)
{
	_modelMatrix = model;
	GLint uniTrans = glGetUniformLocation(_shaderProgram, "model");
	glUniformMatrix4fv(uniTrans, 1, GL_FALSE, glm::value_ptr(model));
}

void Camera::setCameraMovementSpeed(GLfloat speed)
{
	_movementSpeed = speed;
}

void Camera::setCameraRotationSpeed(GLfloat speed)
{
	_rotationSpeed = speed;
}

glm::vec3 Camera::getPosition()
{
	return _position;
}

glm::vec3 Camera::getDirection()
{
	return _direction;
}

glm::vec3 Camera::getUp()
{
	return _up;
}

glm::vec3 Camera::getRight()
{
	return _right;
}

glm::mat4 Camera::getProjectionMatrix()
{
	return _projectionMatrix;
}

glm::mat4 Camera::getModelMatrix()
{
	return _modelMatrix;
}

GLfloat Camera::getMovementSpeed() const
{
	return _movementSpeed;
}

GLfloat Camera::getRotationSpeed() const
{
	return _rotationSpeed;
}

void Camera::updateView()
{
	_direction = glm::normalize(_direction);
	_right = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), _direction));
	_up = glm::normalize(glm::cross(_direction, _right));

	_viewMatrix = glm::lookAt(_position, _position + _direction, _up);
	GLint uniView = glGetUniformLocation(_shaderProgram, "view");
	glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(_viewMatrix));
}

void Camera::moveInCameraDirection(int sign, sf::Time time)
{
	_position += sign * _movementSpeed * _direction * _K_timeNormalizationFactor * (GLfloat)time.asMicroseconds();
	updateView();
}

void Camera::moveForwardBackward(int sign, sf::Time time)
{
	_position.x += sign * _movementSpeed * _direction.x * _K_timeNormalizationFactor * (GLfloat)time.asMicroseconds();
	_position.z += sign * _movementSpeed * _direction.z * _K_timeNormalizationFactor * (GLfloat)time.asMicroseconds();
	updateView();
}

void Camera::moveLeftRight(int sign, sf::Time time)
{
	_position += sign * _movementSpeed * _right * _K_timeNormalizationFactor * (GLfloat)time.asMicroseconds();
	updateView();
}

void Camera::rotateVertical(GLfloat angle)
{
	GLfloat XZlen = sqrt(pow(_direction.x, 2) + pow(_direction.z, 2));
	glm::vec2 verticalPlaneVec = { XZlen, _direction.y };

	verticalPlaneVec.x = verticalPlaneVec.x * cos(angle) - verticalPlaneVec.y * sin(angle);
	verticalPlaneVec.y = verticalPlaneVec.x * sin(angle) + verticalPlaneVec.y * cos(angle);

	_direction.y = verticalPlaneVec.y;
	_direction.x = verticalPlaneVec.x * (_direction.x/XZlen);
	_direction.z = verticalPlaneVec.x *  (_direction.z/XZlen);

	updateView();
}

void Camera::rotateHorizontal(GLfloat rotationAngle)
{
	glm::vec3 direction = _direction;
	_direction.x = direction.x * cos(rotationAngle) - direction.z * sin(rotationAngle);
	_direction.z = direction.x * sin(rotationAngle) + direction.z * cos(rotationAngle);
	
	updateView();
}

void Camera::rotateMouse(int xOffset, int yOffset, sf::Time time)
{
	_verticalAngle += yOffset * _rotationSpeed*_sensitivity;// *_M_timeNormalizationFactor* (GLfloat)time.asMicroseconds();
	_horizontalAngle -= xOffset * _rotationSpeed*_sensitivity;// *_M_timeNormalizationFactor* (GLfloat)time.asMicroseconds();

	_direction.x = cos(glm::radians(_horizontalAngle)) * cos(glm::radians(_verticalAngle));
	_direction.y = sin(glm::radians(_verticalAngle));
	_direction.z = sin(glm::radians(_horizontalAngle)) * cos(glm::radians(_verticalAngle));
	updateView();
}

void Camera::rotateVertical(int sign)
{

	GLfloat angle = _rotationSpeed * sign;
	
	_verticalAngle += angle;

	GLfloat XZlen = sqrt(pow(_direction.x, 2) + pow(_direction.z, 2));
	glm::vec2 verticalPlaneVec = { XZlen, _direction.y };

	verticalPlaneVec.x = verticalPlaneVec.x * cos(angle) - verticalPlaneVec.y * sin(angle);
	verticalPlaneVec.y = verticalPlaneVec.x * sin(angle) + verticalPlaneVec.y * cos(angle);

	_direction.y = verticalPlaneVec.y;
	_direction.x = verticalPlaneVec.x / XZlen * _direction.x;
	_direction.z = verticalPlaneVec.x / XZlen * _direction.z;

	updateView();
}

void Camera::rotateHorizontal(int sign)
{
	GLfloat rotationAngle = sign * _rotationSpeed;
	glm::vec3 direction = _direction;
	_direction.x = direction.x * cos(rotationAngle) - direction.z * sin(rotationAngle);
	_direction.z = direction.x * sin(rotationAngle) + direction.z * cos(rotationAngle);
	updateView();
}

void Camera::printCameraInformation()
{
	std::cout << "Camera Position - X: " << _position.x << " Y: " << _position.y << " Z: " << _position.z << std::endl;
	std::cout << "Camera direction - X: " << _direction.x << " Y: " << _direction.y << " Z: " << _direction.z << std::endl;
}

void Camera::mouseHorizontalRotation(int xOffset, sf::Time time)
{
	rotateHorizontal((GLfloat)xOffset * _rotationSpeed*_sensitivity*_M_timeNormalizationFactor*time.asMilliseconds());
}

void Camera::mouseVerticalRotation(int yOffset, sf::Time time)
{
	rotateVertical((GLfloat)yOffset*  _rotationSpeed*_sensitivity * _M_timeNormalizationFactor * time.asMilliseconds());
}

void Camera::set_M_TimeNormalizationFactor(GLfloat sensitivity)
{
	_M_timeNormalizationFactor = sensitivity;
}

void Camera::set_K_TimeNormalizationFactor(GLfloat sensitivity)
{
	_K_timeNormalizationFactor = sensitivity;
}

void Camera::setVerticalAngleUpperLimit(GLfloat angle)
{
	_verticalAngleUpperLimit = angle;
}

void Camera::setVerticalAngleLowerLimit(GLfloat angle)
{
	_verticalAngleLowerLimit = angle;
}