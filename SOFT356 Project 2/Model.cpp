#include "ShaderLoader.h"
#include "Model.h"
#include <glm/glm.hpp> 
#include <glm/ext/matrix_transform.hpp> 
#include <glm/ext/matrix_clip_space.hpp> 
#include <glm/gtc/type_ptr.hpp> 

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <string>
#include <vector>
#include <iostream>

#define BUFFER_OFFSET(a) ((void*)(a))

void Model::init() {

	std::vector<ShaderInfo> shaders;

	shaders.push_back({ GL_VERTEX_SHADER, "media/textured.vert" });
	shaders.push_back({ GL_FRAGMENT_SHADER, "media/textured.frag" });

	//These two checks are done, as dae parsing may result in some values not being present
	//to still be able to render these, we shall add some in to still be able to render it
	if (colours.size() == 0) {
		for (int i = 0; i < vertices.size(); i++) {
			colours.push_back(glm::vec4(1,1,1,1));
		}
	}
	if (textures.size() == 0) {
		for (int i = 0; i < vertices.size(); i++) {
			textures.push_back(glm::vec2(0, 0));
		}
	}

	shaders.push_back({ GL_NONE, NULL });

	usedProgram = LoadShaders(shaders.data());
	glUseProgram(usedProgram);

	glGenVertexArrays(NumVAOs, VAOs);
	glGenBuffers(NumBuffers, Buffers);

	glBindVertexArray(VAOs[ModelVAO]);

	//Populate vertice and it's indices to the buffers
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[Vertices]);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Buffers[VertexIndices]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertexIndices.size() * sizeof(GLuint), vertexIndices.data(), GL_STATIC_DRAW);

	//Tell the shader where the vertex info ends
	glVertexAttribPointer(Vertices, 3, GL_FLOAT,
		GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(Vertices);

	//Colour Binding
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[Colours]);
	glBufferStorage(GL_ARRAY_BUFFER, colours.size() * sizeof(glm::vec4), colours.data(), 0);

	glVertexAttribPointer(Colours, 4, GL_FLOAT,
		GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(Colours);

	//Do the same for normals
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[Normals]);
	glBufferStorage(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), normals.data(), 0);

	glVertexAttribPointer(Normals, 3, GL_FLOAT,
		GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(Normals);

	//And for textures
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[Textures]);
	glBufferData(GL_ARRAY_BUFFER, textures.size() * sizeof(glm::vec2), textures.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(Textures, 2, GL_FLOAT,
		GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(Textures);


	//IF we have determined there's a texture we want to bind it
	if (hasTexture) {
		bindTexture();
	}

	//Apply lighting
	applyLighting();

}

void Model::draw(glm::vec3 playerLoc, glm::mat4 playerView) {

	glUniform1i(glGetUniformLocation(usedProgram, "textured"), hasTexture ? 1 : 0);
	
	//Create a model matrix
	glm::mat4 model = glm::mat4(1.0f);

	model = glm::translate(model, location);

	//Use the scale factor to scale the object
	model = glm::scale(model, glm::vec3(scaleFactor, scaleFactor, scaleFactor));

	//perform rotations 
	model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

	// creating the projection matrix
	glm::mat4 projection = glm::perspective(45.0f, 4.0f / 3, 0.1f, 200.0f);

	// Adding all matrices up to create combined matrix
	glm::mat4 mv = playerView * model;

	//adding the Uniform to the shader
	int mvLoc = glGetUniformLocation(usedProgram, "mv_matrix");
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mv));
	//adding the Uniform to the shader
	int pLoc = glGetUniformLocation(usedProgram, "p_matrix");
	glUniformMatrix4fv(pLoc, 1, GL_FALSE, glm::value_ptr(projection));

	//Bind the vertex array
	glBindVertexArray(VAOs[ModelVAO]);

	//Bind the texture
	glBindTexture(GL_TEXTURE_2D, textureId);

	//We then draw the object
	glDrawElements(GL_TRIANGLES, vertexIndices.size(), GL_UNSIGNED_INT, 0);

}

//Translate the object to a given point
void Model::translate(GLfloat x, GLfloat y, GLfloat z) {
	location.x = x;
	location.y = y;
	location.z = z;
}

void Model::translate(glm::vec3 position) {
	location = position;
}

//Shrink or grow the object by a given scale
void Model::scale(GLfloat scale) {
	this->scaleFactor = scale;
}

//Rotate the object by X-radians on a given axis
void Model::rotate(GLfloat xdelta, GLfloat ydelta, GLfloat zdelta) {
	rotation.x = xdelta;
	rotation.y = ydelta;
	rotation.z = zdelta;
}

void Model::rotate(glm::vec3 rotation) {
	this->rotation = rotation;
}

//Clear up as much memory as we possibly can
void Model::destroy() {

	glDeleteVertexArrays(NumVAOs, &VAOs[0]);
	glDeleteBuffers(NumBuffers, &Buffers[0]);

	std::vector<glm::vec3>().swap(vertices);
	std::vector<glm::vec3>().swap(normals);
	std::vector<glm::vec2>().swap(textures);
	std::vector<glm::vec4>().swap(colours);
	std::vector<GLuint>().swap(vertexIndices);

}

//Simple debug method that prints out info 
void Model::debug(bool verbose) {

	std::cout << "Program: " << usedProgram << std::endl;
	std::cout << "VAO:" << VAOs[0] << std::endl;
	std::cout << "VAO Model: " << VAOs[ModelVAO] << std::endl;
	std::cout << "Buffer 0: " << Buffers[0] << std::endl;
	std::cout << "Buffer 1: " << Buffers[1] << std::endl;
	std::cout << "Buffer 2: " << Buffers[2] << std::endl;
	std::cout << "Buffer 3: " << Buffers[3] << std::endl;
	std::cout << "Buffer 4: " << Buffers[4] << std::endl;
	std::cout << "Location: " << location.x << ":" << location.y << ":" << location.z << std::endl;
	std::cout << "Rotation: " << rotation.x << ":" << rotation.y << ":" << rotation.z << std::endl;
	std::cout << "TextureID: " << textureId << std::endl;
	std::cout << "Vertex Count: " << vertices.size() << std::endl;
	std::cout << "Texture Count: " << textures.size() << std::endl;
	std::cout << "Normal Count: " << normals.size() << std::endl;
	std::cout << "Colour count:" << colours.size() << std::endl;
	std::cout << "Index Count: " << vertexIndices.size() << std::endl;

	if (verbose) {
		for (int i = 0; i < vertices.size(); i++) {
			std::cout << vertices[i].x << "f," << vertices[i].y << "f," << vertices[i].z << "f," << std::endl;
		}

		std::cout << "Textures: " << std::endl;


		for (int i = 0; i < textures.size(); i++) {
			std::cout << textures[i].x << "f," << textures[i].y << "f," << std::endl;
		}

		std::cout << "Normals: " << std::endl;

		for (int i = 0; i < normals.size(); i++) {
			std::cout << normals[i].x << ".0f," << normals[i].y << ".0f," << normals[i].z << ".0f," << std::endl;
		}

		std::cout << "Vertex indices: " << std::endl;

		for (int i = 0; i < vertexIndices.size(); i++) {
			std::cout << vertexIndices[i] << ",";
		}
	}

	std::cout << std::endl;
}

//Apply lighting to the model
void Model::applyLighting() {
	
	//Create Ambient lighting and add to shader
	glm::vec4 ambient = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	GLuint aLoc = glGetUniformLocation(usedProgram, "ambient");
	glUniform4fv(aLoc, 1, glm::value_ptr(ambient));

	//Create the light object and apply to shader
	glm::vec3 lightPos = glm::vec3(5.0f, 0.0f, 0.0f);
	GLuint dLightPosLoc = glGetUniformLocation(usedProgram, "lightPos");
	glUniform3fv(dLightPosLoc, 1, glm::value_ptr(lightPos));

	//Create light diffusion and add to shader
	glm::vec3 diffuseLight = glm::vec3(0.5f, 0.2f, 0.7f);
	GLuint dLightLoc = glGetUniformLocation(usedProgram, "dLight");
	glUniform3fv(dLightLoc, 1, glm::value_ptr(diffuseLight));

	//Create specular light and add to shader
	glm::vec3 specularLight = glm::vec3(0.7f);
	GLfloat shininess = 256; //128 is max value
	GLuint sLightLoc = glGetUniformLocation(usedProgram, "sLight");
	GLuint sShineLoc = glGetUniformLocation(usedProgram, "sShine");
	glUniform3fv(sLightLoc, 1, glm::value_ptr(specularLight));
	glUniform1fv(sShineLoc, 1, &shininess);
}

//bind a texture
void Model::bindTexture() {

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

	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		hasTexture = true;
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
		hasTexture = false;
	}

	stbi_image_free(data);
}