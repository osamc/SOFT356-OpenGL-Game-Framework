#pragma once


#include <GL\glew.h>
#include <glm/glm.hpp> 
#include <map>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>


class Character {
public:
	GLint id;  
	GLint x;
	GLint y;
	GLint width;
	GLint height;
	GLint xoffset;
	GLint yoffset;
	GLint xadvance;
	GLint page;
	GLint channel;
};

class TextRenderer {

	GLint scale;

	std::vector<glm::vec2> vertices;
	std::vector<glm::vec2> textures;
	std::vector<glm::vec4> colours;

	std::vector<GLuint> vertexIndices;

	GLint textureWidth, textureHeight;

	std::map<GLuint, Character> characters;
	GLuint textureId;

	enum VAO_IDs { ModelVAO, NumVAOs = 1 };
	enum Buffer_IDs { Vertices, Colours, Textures, VertexIndices, NumBuffers = 4 };

	GLuint VAOs[NumVAOs];
	GLuint Buffers[NumBuffers];

	GLuint usedProgram;

public:
	void init(std::string fontFile, std::string texture);
	void renderText(std::string, GLfloat scale, glm::vec2 position, GLint width, GLint height);
	void draw();
private:
	void bindTexture(std::string texture);
};
