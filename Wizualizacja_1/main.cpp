
// Nag³ówki
//#include "stdafx.h"
#include <GL/glew.h>
#include <SFML/Window.hpp>
#include<SFML/Graphics.hpp>
#include<cmath>
#include<iostream>
#include"glm/glm.hpp"
#include"glm/gtc/matrix_transform.hpp"
#include"glm/gtc/type_ptr.hpp"
#include"Camera.h"
#include"VAO.h"
#include"EBO.h"
#include"ShaderClass.h"
#include"Geometry.h"
#include"Texture.h"
#include"ReadBlender.h"
#include"PointLight.h"



#define M_PI 3.14
#define VERT_SIZE 6
#define DIMENSION_NUMBER 3

void changeVerticesNumber(GLfloat*& vertices, unsigned int points, GLuint vbo, const float R = 0.7f, const float z = 0.0f);
int getPrimitiveType(sf::Keyboard::Key key);
void setDefaultCamera(Camera& camera);
int main()
{
	sf::ContextSettings settings;
	settings.depthBits = 24;
	settings.stencilBits = 8;

	// Okno renderingu
	sf::Window window(sf::VideoMode(1000, 1000, 32), "OpenGL", sf::Style::Titlebar | sf::Style::Close, settings);

	// Inicjalizacja GLEW
	glewExperimental = GL_TRUE;
	glewInit();
	GeometryConfig obj = createGeometryObjectFromBlender("chair_texture.obj", 0.1, 0.8, 0.2);
	Geometry chair(obj);
	delete [] obj.data;

	chair.scale(0.1, 0.1, 0.1);
	chair.transition(1, 0, 0);

	obj = createGeometryObjectFromBlender("table.obj", 0.5, 0.5, 0.5);
	Geometry texture(obj);
	delete[] obj.data;

	Geometry scene; 
	compose(chair, texture, scene);

	

	//Geometry cube("cube_texture_norm.geo");
	//cube.readElementsFile("cube.ebo");
	Shader program("default_texture_light.vert", "default_texture_light.fragment");

	float lightPos[] = { 0.0f, 10.0f, 0.0f };

	VAO vao_1;
	vao_1.setVertexAttribSize(scene.getDim(), scene.getColorAttribNum(), scene.getTextureDim(), scene.getNorm());
	vao_1.Bind();

	VBO vbo_1((GLfloat*)scene, scene.getDataSize() * sizeof(GLfloat));
	vao_1.LinkVBO(vbo_1, 0, 1, 2, 3);



	//GLint NorAttrib = glGetAttribLocation(program.getID(), "aNormal");
	//glEnableVertexAttribArray(NorAttrib);
	//glVertexAttribPointer(
	//	NorAttrib, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (void*)(8 * sizeof(GLfloat)));

	//cube.readElementsFile("cube.ebo");
	//EBO ebo_1(cube.elements(), 36);
	
	Texture metal("wood.jpg", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
	metal.texUnit(program, "aTexCoord", 0);

	program.Activate();

	PointLight light(
		glGetUniformLocation(program.getID(), "brigthness"),
		glGetUniformLocation(program.getID(), "lightPos"),
		glGetUniformLocation(program.getID(), "diffColor")
	);

	light.setPos(lightPos);
	light.setColor(0.8, 0.0, 0.2);
	light.setBrightness(1.5);

	/*GLfloat brightness = 0.8f;
	GLint uniLightPos = glGetUniformLocation(program.getID(), "lightPos");
	glUniform3fv(uniLightPos, 1, lightPos);
	GLint uniStr = glGetUniformLocation(program.getID(), "brigthness");
	glUniform1f(uniStr, brightness);

	float diffColor[] = { 1.0, 0.0, 0.0 };
	GLint diffColorPos = glGetUniformLocation(program.getID(), "diffColor");
	glUniform3fv(diffColorPos, 1, diffColor);*/
	

	// Ustawienie kamery 

	Camera camera(program.getID());
	setDefaultCamera(camera);

	// Specifikacja formatu danych wierzcho³kowych
	

	//
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	
	//GLfloat lightPosition[] = {0.0f, 0.0f, 10.0f, 1.0f }; // Position of the light source
	//GLfloat lightDiffuse[] = { 1.0f, 0.0f, 1.0f, 1.0f };  // Diffuse color of the light (white)
	//GLfloat lightAmbient[] = { 0.1f, 0.1f, 0.1f, 1.0f };  // Ambient color of the light
	//GLfloat lightDirection[] = { 0.0f, 0.0f, -1.0f, 0.0f };  // Direction of the light (directional light)

	//glLightfv(GL_LIGHT1, GL_POSITION, lightPosition);
	//glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDiffuse);
	//glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, lightDirection);
	//glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
	//glEnable(GL_LIGHT1);
	// Rozpoczêcie pêtli zdarzeñ
	bool running = true;
	sf::Vector2i mouseLocalPosition;
	bool mouseEnable = false;
	GLfloat xOffset = 0;
	GLfloat yOffset = 0;

	bool lightControl = false;

	sf::Clock clock;
	sf::Time time = clock.getElapsedTime();
	char buffer[10];
	sf::Vector2i center = { (int)window.getSize().x / 2, (int)window.getSize().y / 2 };

	window.setFramerateLimit(60);

	while (running) {
		sf::Event windowEvent;
		mouseLocalPosition = sf::Mouse::getPosition(window);

		time = clock.getElapsedTime();
		_itoa_s((int)(1 / (time.asSeconds())), buffer, 10);
		clock.restart();
		
		window.setTitle(buffer);

	

		while (window.pollEvent(windowEvent)) {
			switch (windowEvent.type) {
			case sf::Event::Closed:
				running = false;
				break;
			case sf::Event::MouseMoved:
				if (mouseEnable)
				{
					xOffset = mouseLocalPosition.x - sf::Mouse::getPosition(window).x;
					yOffset = sf::Mouse::getPosition(window).y - mouseLocalPosition.y;

					//std::cout << xOffset << std::endl;
					camera.mouseHorizontalRotation(xOffset, time);
					camera.mouseVerticalRotation(yOffset, time);
					mouseLocalPosition.x = sf::Mouse::getPosition(window).x;
					mouseLocalPosition.y = sf::Mouse::getPosition(window).y;
					sf::Mouse::setPosition(center, window);
				}
				break;
			case sf::Event::KeyPressed:
				switch (windowEvent.key.code)
				{
				case sf::Keyboard::Up:
					//lightPos[0] += 0.1;
					if (!lightControl) camera.rotateVertical(1);
					else light.increaseBrightness(0.1);
					break;
				case sf::Keyboard::Down:
					//lightPos[0] -= 0.1;
					if (!lightControl) camera.rotateVertical(-1);
					else light.decreaeBrightness(0.1);
					break;
				case sf::Keyboard::Left:
					//lightPos[1] += 0.1;
					camera.rotateHorizontal(-1);
					break;
				case sf::Keyboard::Right:
					//lightPos[1] -= 0.1;
					camera.rotateHorizontal(1);
					break;
				
				case sf::Keyboard::W:
					camera.moveInCameraDirection(1, time);
					break;
				case sf::Keyboard::S:
					camera.moveInCameraDirection(-1, time);
					break;
				case sf::Keyboard::D:
					camera.moveLeftRight(-1, time);
					break;
				case sf::Keyboard::A:
					camera.moveLeftRight(1, time);
					break;
				case sf::Keyboard::M:
					mouseEnable = !mouseEnable;
					window.setMouseCursorGrabbed(mouseEnable);
					window.setMouseCursorVisible(!mouseEnable);
					break;
				case sf::Keyboard::R:
					sf::Mouse::setPosition(center, window);
					mouseLocalPosition.x = sf::Mouse::getPosition(window).x;
					mouseLocalPosition.y = sf::Mouse::getPosition(window).y;
					break;

				case sf::Keyboard::L:
					lightControl = !lightControl;
					break;

				default:
					break;
				}
				break;
			}
		}

		
		// Nadanie scenie koloru czarnego
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		metal.Bind();
		// Narysowanie trójk¹ta na podstawie 3 wierzcho³ków
		glDrawArrays(GL_TRIANGLES, 0, scene.getVerticesNumber());
		//glDrawElements(GL_POLYGON, 36, GL_UNSIGNED_INT, 0);
		// Wymiana buforów tylni/przedni
		window.display();
	}
	// Kasowanie programu i czyszczenie buforów
  	vao_1.Delete();
	vbo_1.Delete();

	// Zamkniêcie okna renderingu
	window.close();
	return 0;
}

void setDefaultCamera(Camera& camera)
{
	// Camera vectors
	glm::vec3 cameraPos = glm::vec3(3.0f, 3.0f, 3.0f);
	glm::vec3 cameraDirection = glm::vec3(-1.0f, -1.0f, -1.0f);

	// Movement 
	GLfloat cameraMoveSpeed = 0.00005f;
	GLfloat cameraRotateSpeed = 0.05f;
	GLfloat rotationAngle = 0.0f;

	// Projection matrix
	glm::mat4 proj = glm::perspective(glm::radians(45.0f), 800.0f / 800.0f, 0.06f, 100.0f);

	// View matrix

	// Model matrix
	glm::mat4 model = glm::identity<glm::mat4>();
	//model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));



	camera.setPosition(cameraPos);
	camera.setProjectionMatrix(proj);
	camera.setDirection(cameraDirection);
	camera.setCameraMovementSpeed(cameraMoveSpeed);
	camera.setCameraRotationSpeed(cameraRotateSpeed);
	camera.setModelMatrix(model);
	camera.set_M_TimeNormalizationFactor(0.05f);
	camera.set_K_TimeNormalizationFactor(0.5f);
	camera.setVerticalAngleUpperLimit(3.14 / 4);
	camera.setVerticalAngleLowerLimit(-3.14 / 4);
}

void changeVerticesNumber(GLfloat*& vertices, unsigned int veritcesNumber, GLuint vbo, const float R, const float z)
{
	float rotationAngle = 2.0f * M_PI / float(veritcesNumber);
	for (int i = 0; i < veritcesNumber; i++) 
	{
		float theta = rotationAngle * float(i) ;
		float x = R * cosf(theta);
		float y = R * sinf(theta);
		vertices[VERT_SIZE * i] = x;
		vertices[VERT_SIZE * i + 1] = y;
		vertices[VERT_SIZE * i + 2] = z;

		vertices[VERT_SIZE * i + 3] = 1.0f/ veritcesNumber; // R
		vertices[VERT_SIZE * i + 4] = (float)(veritcesNumber-i) / (float)veritcesNumber; // G
		vertices[VERT_SIZE * i + 5] = (float)(i) /(float) veritcesNumber; // B
	}
	glBufferSubData(GL_ARRAY_BUFFER, 0, veritcesNumber * sizeof(GLfloat) * VERT_SIZE, vertices);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) *veritcesNumber * VERT_SIZE, vertices, GL_STATIC_DRAW);
}

int getPrimitiveType(sf::Keyboard::Key key)
{
	switch (key) {
	case sf::Keyboard::Num1: return GL_POINTS;
	case sf::Keyboard::Num2: return GL_LINES;
	case sf::Keyboard::Num3: return GL_LINE_STRIP;
	case sf::Keyboard::Num4: return GL_LINE_LOOP;
	case sf::Keyboard::Num5: return GL_TRIANGLES;
	case sf::Keyboard::Num6: return GL_TRIANGLE_STRIP;
	case sf::Keyboard::Num7: return GL_TRIANGLE_FAN;
	case sf::Keyboard::Num8: return GL_QUADS;
	case sf::Keyboard::Num9: return GL_QUAD_STRIP;
	case sf::Keyboard::Num0: return GL_POLYGON;
	default: return GL_POLYGON;
	}
}