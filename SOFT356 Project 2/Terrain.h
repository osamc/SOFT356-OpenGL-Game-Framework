#pragma once
#include "Model.h"
#include <GL/glew.h>

//Create a terrain model
Model createTerrain(GLfloat size, GLuint vertexCount, std::string texture) {

	Model model;

	//create a grid of size by size grid points
	for (float i = 0; i < vertexCount; i++) {
		for (float j = 0; j < vertexCount; j++) {
			float x, z;
			x = (j / (vertexCount - 1)) * size;
			z = (i / (vertexCount - 1)) * size;
			//Create the vertex
			glm::vec3 vertex = glm::vec3(x, 0, z);
			//Create the texture point
			glm::vec2 texture = glm::vec2((j / (vertexCount - 1)), (i / (vertexCount - 1)));
			//Get the normal point
			glm::vec3 normal = glm::vec3(0, 1, 0);
			
			//Add points to vectors
			model.vertices.push_back(vertex);
			model.textures.push_back(texture);
			model.normals.push_back(normal);
		}
	}

	//We then iterate over the points to create a square
	for (int i = 0; i < vertexCount - 1; i++) {
		for (int j = 0; j < vertexCount - 1; j++) {
			//Work out the points that create a square
			GLuint topLeft = (i * vertexCount) + j;
			GLuint topRight = topLeft + 1;
			GLuint bottomLeft = ((i + 1) * vertexCount) + j;
			GLuint bottomRight = bottomLeft + 1;

			//from the square we can create two triangles,
			//similar to how the model loader deals with 4 point faces

			//Create triangle 1
			model.vertexIndices.push_back(topLeft);
			model.vertexIndices.push_back(bottomLeft);
			model.vertexIndices.push_back(topRight);

			//Create triangle 2
			model.vertexIndices.push_back(topRight);
			model.vertexIndices.push_back(bottomLeft);
			model.vertexIndices.push_back(bottomRight);
		}
	}

	//We want to assign the texture and tell the model it has a texture
	model.textureLocation = texture;
	model.hasTexture = true;
	return model;

}
