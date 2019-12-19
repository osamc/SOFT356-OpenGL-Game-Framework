#pragma once
#include <vector>
#include <glm/glm.hpp> 
#include <GL/glew.h>
#include <string>
#include <map>
#include "TextRendering.h"

class UIelement {

public:
	std::vector<glm::vec2> vertices;
	std::vector<glm::vec2> textures;
	std::vector<glm::vec4> colours;

	std::vector<GLuint> vertexIndices;

	GLuint textureId;
	bool hasTexture;

	enum VAO_IDs { ModelVAO, NumVAOs = 1 };
	enum Buffer_IDs { Vertices, Colours, Textures, VertexIndices, NumBuffers = 4};

	GLuint VAOs[NumVAOs];
	GLuint Buffers[NumBuffers];

	GLuint usedProgram;

	void createRectangle(glm::vec2 position, GLfloat height, GLfloat width, glm::vec4 colour);
	void createRectangle(glm::vec2 position, GLfloat height, GLfloat width, glm::vec4 colour, std::string textureLocation);

	void init();

	void draw();

	//Bind the texture
	void bindTexture(std::string textureLocation);

};