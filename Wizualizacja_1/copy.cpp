
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

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define M_PI 3.14
#define VERT_SIZE 6
#define DIMENSION_NUMBER 3

// Kody shaderów
const GLchar* vertexSource = R"glsl(
#version 150 core
in vec3 position;
in vec3 color;
in vec2 aTexCoord;
out vec2 TexCoord;
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
out vec3 Color;
void main(){
Color = color;
gl_Position = proj*view*model*vec4(position, 1.0);
TexCoord = aTexCoord;
}
)glsl";

const GLchar* fragmentSource = R"glsl(
#version 150 core
in vec3 Color;
in vec2 TexCoord;
out vec4 outColor;
uniform sampler2D texture1;
void main()
{
outColor=texture(texture1, TexCoord);
}
)glsl";

void changeVerticesNumber(GLfloat*& vertices, unsigned int points, GLuint vbo, const float R = 0.7f, const float z = 0.0f);
int getPrimitiveType(sf::Keyboard::Key key);
void setDefaultCamera(Camera& camera);
int copy()
{
	sf::ContextSettings settings;
	settings.depthBits = 24;
	settings.stencilBits = 8;

	// Okno renderingu
	sf::Window window(sf::VideoMode(1000, 1000, 32), "OpenGL", sf::Style::Titlebar | sf::Style::Close, settings);

	// Inicjalizacja GLEW
	glewExperimental = GL_TRUE;
	glewInit();

	//Geometry cube("cube.geo");
	//Shader program("default.vert", "default.fragment");
	//VAO vao_1;
	//vao_1.Bind();

	//VBO vbo_1((GLfloat*)cube, cube.getDataSize() * sizeof(GLfloat));
	//vao_1.LinkVBO(vbo_1, 0, 1);

	// Utworzenie VAO (Vertex Array Object)
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//	// Utworzenie VBO (Vertex Buffer Object)
	//	// i skopiowanie do niego danych wierzcho³kowych
	GLuint vbo;
	glGenBuffers(1, &vbo);

	Geometry cube;
	cube.readGeometryFile("cube_ texture.geo");

	unsigned int maxVerticesNumber = cube.getVerticesNumber();
	unsigned int verticesNumber = cube.getVerticesNumber();
	unsigned int vertexSize = cube.getVertexSize();
	unsigned int coorOffset = cube.coordinatesOffset();
	unsigned int colorOffset = cube.colorOffset();
	unsigned int textureOffset = cube.textureOffset();

	int currentPrimitive = GL_POLYGON;

	unsigned int texture1; //Jak do wszystkich obiektów w OpenGL do tekstury mo¿na odwo³aæ siê poprzez
	//identyfikator ID // texture 1
	glGenTextures(1, &texture1); // Generuje tekstury
	glBindTexture(GL_TEXTURE_2D, texture1); //Ustawienie tekstury jako bie¿¹cej (powi¹zanie)
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	unsigned char* data = stbi_load("metal.bmp", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * maxVerticesNumber * vertexSize, (GLfloat*)cube, GL_DYNAMIC_DRAW);

	// Utworzenie i skompilowanie shadera wierzcho³ków
	GLuint vertexShader =
		glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);

	GLint compilationSucces;
	GLchar log[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compilationSucces);
	if (!compilationSucces)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, log);
		std::cerr << "Compilation vertexShader Error\n" << log << std::endl;
	}
	else
	{
		std::cout << "Compilation vertexShader OK\n";
	}
	//
	//	// Utworzenie i skompilowanie shadera fragmentów
	GLuint fragmentShader =
		glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compilationSucces);
	if (!compilationSucces)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, log);
		std::cerr << "Compilation fragmentShader Error\n" << log << std::endl;
	}
	else
	{
		std::cout << "Compilation fragmentShader OK\n";
	}

	//	// Zlinkowanie obu shaderów w jeden wspólny program
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glBindFragDataLocation(shaderProgram, 0, "outColor");
	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);

	// Ustawienie kamery 

	Camera camera(shaderProgram);
	setDefaultCamera(camera);

	// Specifikacja formatu danych wierzcho³kowych
	GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, vertexSize * sizeof(GLfloat), (void*)(coorOffset * sizeof(GLfloat)));
	GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
	glEnableVertexAttribArray(colAttrib);
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, vertexSize * sizeof(GLfloat), (void*)(colorOffset * sizeof(GLfloat)));
	GLint TexCoord = glGetAttribLocation(shaderProgram, "aTexCoord");
	glEnableVertexAttribArray(TexCoord);
	glVertexAttribPointer(TexCoord, 2, GL_FLOAT, GL_FALSE, vertexSize * sizeof(GLfloat), (void*)(textureOffset * sizeof(GLfloat)));

	//
	glEnable(GL_DEPTH_TEST);
	// Rozpoczêcie pêtli zdarzeñ
	bool running = true;
	sf::Vector2i mouseLocalPosition;
	bool mouseEnable = false;
	GLfloat xOffset = 0;
	GLfloat yOffset = 0;

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
					camera.rotateVertical(1);
					break;
				case sf::Keyboard::Down:
					camera.rotateVertical(-1);
					break;
				case sf::Keyboard::Left:
					camera.rotateHorizontal(-1);
					break;
				case sf::Keyboard::Right:
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

				default:
					break;
				}
				break;
			}
		}

		// Nadanie scenie koloru czarnego
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBindTexture(GL_TEXTURE_2D, texture1);
		// Narysowanie trójk¹ta na podstawie 3 wierzcho³ków
		glDrawArrays(GL_POLYGON, 0, verticesNumber);
		// Wymiana buforów tylni/przedni
		window.display();
	}
	// Kasowanie programu i czyszczenie buforów
	//vao_1.Delete();
	//vbo_1.Delete();
	glDeleteProgram(shaderProgram);
	glDeleteShader(fragmentShader);
	glDeleteShader(vertexShader);
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
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
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));



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
		float theta = rotationAngle * float(i);
		float x = R * cosf(theta);
		float y = R * sinf(theta);
		vertices[VERT_SIZE * i] = x;
		vertices[VERT_SIZE * i + 1] = y;
		vertices[VERT_SIZE * i + 2] = z;

		vertices[VERT_SIZE * i + 3] = 1.0f / veritcesNumber; // R
		vertices[VERT_SIZE * i + 4] = (float)(veritcesNumber - i) / (float)veritcesNumber; // G
		vertices[VERT_SIZE * i + 5] = (float)(i) / (float)veritcesNumber; // B
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