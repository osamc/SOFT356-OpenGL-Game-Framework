#pragma once


#include <GL\glew.h>
#include <glm/glm.hpp> 
#include <map>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

//Class to represent the infomation parsed from a .fnt file
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

	//Map containing every character parsed from the fnt file
	std::map<GLuint, Character> characters;
	
	//The scale at which we render the font
	GLint scale;

	//the vao data
	std::vector<glm::vec2> vertices;
	std::vector<glm::vec2> textures;
	std::vector<glm::vec4> colours;

	std::vector<GLuint> vertexIndices;

	//Texture infomation
	GLuint textureId;
	//We need the texture dimensions so we can represent the texture
	//coordinates between -1 and 1 instead of in pixels
	GLint textureWidth, textureHeight;

	//vao definitions
	enum VAO_IDs { ModelVAO, NumVAOs = 1 };
	enum Buffer_IDs { Vertices, Colours, Textures, VertexIndices, NumBuffers = 4 };

	GLuint VAOs[NumVAOs];
	GLuint Buffers[NumBuffers];

	//The used program
	GLuint usedProgram;

public:
	//Method for initialising a font to be used
	void init(std::string fontFile, std::string texture);
	//Create the text vertices and texture coordinates
	void createText(std::string text, glm::vec4 colour, GLfloat scale, glm::vec2 position, GLint width, GLint height);
	//Draw the text on screen
	void draw();
private:
	void bindTexture(std::string texture);
};
