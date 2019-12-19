#include "UIelement.h"
#include "ShaderLoader.h"
#include <glm\gtc\type_ptr.hpp>
#include "stb_image.h"
#include <iostream>

#define BUFFER_OFFSET(a) ((void*)(a))

void UIelement::createRectangle(glm::vec2 position, GLfloat height, GLfloat width, glm::vec4 colour) {

	//Clear previous data in model
	std::vector<glm::vec2>().swap(vertices);
	std::vector<glm::vec2>().swap(textures);
	std::vector<glm::vec4>().swap(colours);
	std::vector<GLuint>().swap(vertexIndices);

	glm::vec2 topLeft = position;
	glm::vec2 topRight = glm::vec2(position.x + width, position.y);
	glm::vec2 bottomRight = glm::vec2(position.x + width, position.y - height);
	glm::vec2 bottomLeft = glm::vec2(position.x, position.y - height);

	//Triangle 1
	vertices.push_back(topRight);
	vertices.push_back(bottomRight);
	vertices.push_back(bottomLeft);

	vertexIndices.push_back(0);
	vertexIndices.push_back(1);
	vertexIndices.push_back(2);

	//Triangle 2
	vertices.push_back(bottomLeft);
	vertices.push_back(topLeft);
	vertices.push_back(topRight);

	vertexIndices.push_back(3);
	vertexIndices.push_back(4);
	vertexIndices.push_back(5);

	//We add an empty texture just so it doesn't explode
	textures.push_back(glm::vec2(0.0f, 0.0f));

	//Set each vertex to the desired colour
	for (int i = 0; i < vertices.size(); i++) {
		colours.push_back(colour);
	}

}

void UIelement::createRectangle(glm::vec2 position, GLfloat height, GLfloat width, glm::vec4 colour, std::string textureLocation) {
	//Create rectangle as we world normally would
	createRectangle(position, height, width, colour);

	std::vector<glm::vec2>().swap(textures);

	//Calculate new texture data
	//create texture triangle for triangle 1
	textures.push_back(glm::vec2(0.0f, 0.0f));
	textures.push_back(glm::vec2(0.0f, -1.0f));
	textures.push_back(glm::vec2(-1.0f, -1.0f));

	//create texture triangle for triangle 2
	textures.push_back(glm::vec2(0.0f, 0.0f));
	textures.push_back(glm::vec2(0.0f, 1.0f));
	textures.push_back(glm::vec2(1.0f, 1.0f));
	
	//Bind the texture
	bindTexture(textureLocation);

}


void UIelement::init() {
	
	std::vector<ShaderInfo> shaders;

	shaders.push_back({ GL_VERTEX_SHADER, "media/2d.vert" });
	shaders.push_back({ GL_FRAGMENT_SHADER, "media/2d.frag" });
	shaders.push_back({ GL_NONE, NULL });

	usedProgram = LoadShaders(shaders.data());
	glUseProgram(usedProgram);

	glGenVertexArrays(NumVAOs, VAOs);
	glGenBuffers(NumBuffers, Buffers);

	glBindVertexArray(VAOs[ModelVAO]);

	//Bind vertex data
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[Vertices]);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec2), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Buffers[VertexIndices]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertexIndices.size() * sizeof(GLuint), vertexIndices.data(), GL_STATIC_DRAW);


	//Tell the shader where the vertex info ends
	glVertexAttribPointer(Vertices, 2, GL_FLOAT,
		GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(Vertices);

	//Bind colour data
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[Colours]);
	glBufferStorage(GL_ARRAY_BUFFER, colours.size() * sizeof(glm::vec4), colours.data(), 0);

	glVertexAttribPointer(Colours, 4, GL_FLOAT,
		GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(Colours);

	//Bind texture data
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[Textures]);
	glBufferData(GL_ARRAY_BUFFER, textures.size() * sizeof(glm::vec2), textures.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(Textures, 2, GL_FLOAT,
		GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(Textures);
}


void UIelement::draw() {
	
	glUseProgram(usedProgram);
	glUniform1i(glGetUniformLocation(usedProgram, "textured"), hasTexture ? 1 : 0);

	glBindVertexArray(VAOs[ModelVAO]);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());

}

void UIelement::bindTexture(std::string textureLocation) {

	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	GLint width, height, nrChannels;

	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	unsigned char* data = stbi_load(textureLocation.c_str(), &width, &height, &nrChannels, 0);

	int imageMode = nrChannels == 4 ? GL_RGBA: GL_RGB;

	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, imageMode, width, height, 0, imageMode, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		hasTexture = true;
	}
	else {
		hasTexture = false;
	}

	stbi_image_free(data);
}


