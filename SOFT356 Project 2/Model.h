#pragma once

#include <glm/glm.hpp> 
#include <GL/glew.h>

#include <string>
#include <vector>
#include <iostream>

class Material {
public: 
	std::string name;
	glm::vec4 colour;
};

class Model
{

public:
	//Vertex info
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> textures;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec4> colours;


	//Indice info
	std::vector<GLuint> vertexIndices;

	//how the object is rotated
	glm::vec3 rotation;
	//the location of the object
	glm::vec3 location;

	//the size of the object
	GLfloat scaleFactor = 1;

	//Texture info
	bool hasTexture = false;
	std::string textureLocation;

	//Indicator if the model was loaded properly
	bool createdSuccessfully = false;

	enum VAO_IDs { ModelVAO, NumVAOs = 1 };
	enum Buffer_IDs { Vertices, Colours, Normals, Textures, VertexIndices, NumBuffers = 5 };

	GLuint VAOs[NumVAOs];
	GLuint Buffers[NumBuffers];
	GLuint textureId;

	GLuint usedProgram;

	//Initialise
	void init();

	//Draw model
	void draw(glm::vec3 playerLoc, glm::mat4 playerView);

	//Move the model
	void translate(GLfloat x, GLfloat y, GLfloat z);
	void translate(glm::vec3 position);

	//Change the size of the model
	void scale(GLfloat scale);

	//Rotate the model
	void rotate(GLfloat xdelta, GLfloat ydelta, GLfloat zdelta);
	void rotate(glm::vec3 rotation);

	
	//clear up memory of model
	void destroy();
	
	//Print the variables to the console
	void debug(bool verbose);
	
private: 

	//Apply the lighting
	void applyLighting();

	//Bind the texture
	void bindTexture();

};

