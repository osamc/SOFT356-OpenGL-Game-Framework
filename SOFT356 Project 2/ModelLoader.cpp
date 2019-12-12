#include "Model.h"
#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <sstream>
#include <map>

Model loadFromObj(std::string file) {

	//Create the model to be returned
	Model model;

	//Log to the output that we're making the specified object
	std::cout << "Creating from: " + file << std::endl;

	//Instantiate a variable to hold each line of the string
	std::string line;

	//Create out variables to hold the parsed data untill the end
	std::vector<GLuint> vertexIndices, textureIndices, normalIndices, faceSize, materialUsed;
	std::vector<glm::vec3> tempVertices;
	std::vector<glm::vec2> tempTextures;
	std::vector<glm::vec3> tempNormals;

	std::vector<Material> materials;

	GLuint indexOfCurrentMat;
	Material tempMat; 

	GLuint countF = 0, countT = 0;

	//Create a string to hold the first two characters of each line just so we aren't making a method call each if check
	std::string lineStart;

	//Create a file input stream
	std::ifstream rfile;

	//Look for a material file
	std::string materialFile = file.substr(0, file.length() - 3) + "mtl";

	//We want to process the material file first to grab colours and such
	rfile.open(materialFile);

	//if the file is not open then there is no material file for the object
	if (rfile.is_open()) {
		while (std::getline(rfile, line)) {
			lineStart = line.substr(0, 6);
			if (lineStart._Equal("map_Kd")) {
				model.textureLocation = line.substr(line.find_first_of(" ") + 1, line.length());
				//we want to store the texture location in the same format that the user inputted the info in
				//this means that we will use the same file delimiter. This is done as windows and unix systems
				//use a different directional slash to indicate folder structure
				char fileDelimiter = file.find("\\") != std::string::npos ? '\\' : '/';
				model.textureLocation = file.substr(0, file.find_last_of(fileDelimiter) + 1) + model.textureLocation;
				model.hasTexture = true;
			}
			else if (lineStart._Equal("newmtl")) {
				tempMat.name = line.substr(7, line.size());
			}
			else if (lineStart.substr(0,2)._Equal("Kd")) {
				glm::vec4 colour;
				sscanf_s(line.c_str(), "Kd %f %f %f\n", &colour.x, &colour.y, &colour.z);
				tempMat.colour = colour;
				materials.push_back(tempMat);

			}
		}

	}

	rfile.close();

	//now open the obj file
	rfile.open(file);

	//While the file is open and has more contents 
	if (rfile.is_open()) {
		while (std::getline(rfile, line)) {
			lineStart = line.substr(0, 2);

			//V indicates a vertex
			if (lineStart._Equal("v ")) {
				glm::vec3 vertex;
				sscanf_s(line.c_str(), "v %f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
				tempVertices.push_back(vertex);
			}
			//vt indicates a texture vertex
			else if (lineStart._Equal("vt")) {
				glm::vec2 texture;
				sscanf_s(line.c_str(), "vt %f %f\n", &texture.x, &texture.y);
				tempTextures.push_back(texture);
			}
			//vn indicates a vertex normal
			else if (lineStart._Equal("vn")) {
				glm::vec3 normal;
				sscanf_s(line.c_str(), "vn %f %f %f\n", &normal.x, &normal.y, &normal.z);
				tempNormals.push_back(normal);
			}
			else if (lineStart._Equal("us")) {
				std::string materialName = line.substr(7, line.size());
				for (int i = 0; i < materials.size(); i++) {
					if (materials[i].name._Equal(materialName)) {
						indexOfCurrentMat = i;
						break;
					}
				}
			}
			//f indicates a face
			else if (lineStart._Equal("f ")) {
				int vertexIndex[4], textureIndex[4], normalIndex[4];
				int matches = sscanf_s(line.c_str(),
					"f %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d\n",
					&vertexIndex[0], &textureIndex[0], &normalIndex[0],
					&vertexIndex[1], &textureIndex[1], &normalIndex[1],
					&vertexIndex[2], &textureIndex[2], &normalIndex[2],
					&vertexIndex[3], &textureIndex[3], &normalIndex[3]);

				//We do a simple check that the number of vertices is equal to the amount we load in,
				//this prevents anything from being loaded in and dealt with incorrectly
				if (static_cast<int>(std::count(line.begin(), line.end(), ' ')) * 3 != matches) {
					continue;
				}

				//Obj files support n-agons, but in the majority of the time it is either a triangle or a quadrilateral
				//meaning that we should have either 3x3 or 4x3 of f/f/f per line 
				if (matches != 12 && matches != 9) {
					std::cout << "There was a problem parsing the f data" << std::endl;
					model.createdSuccessfully = false;
					return model;
				}

				//so for triangle 1 we need these
				//we subtract 1 from each index as arrays start from 0 in cpp
				vertexIndices.push_back(vertexIndex[0] - 1);
				vertexIndices.push_back(vertexIndex[1] - 1);
				vertexIndices.push_back(vertexIndex[2] - 1);

				normalIndices.push_back(normalIndex[0] - 1);
				normalIndices.push_back(normalIndex[1] - 1);
				normalIndices.push_back(normalIndex[2] - 1);

				textureIndices.push_back(textureIndex[0] - 1);
				textureIndices.push_back(textureIndex[1] - 1);
				textureIndices.push_back(textureIndex[2] - 1);

				//If we have any materials then we need to add them in,
				//if we don't then attempting to add one will crash the app
				if (materials.size() > 0) {
					model.colours.push_back(materials[indexOfCurrentMat].colour);
					model.colours.push_back(materials[indexOfCurrentMat].colour);
					model.colours.push_back(materials[indexOfCurrentMat].colour);
				}

				
				//If we have 9 matches then we have a triangle
				if (matches == 9) {
					faceSize.push_back(3);
					
				} 
				//Else we might have 12 which means it's a square
				else if (matches == 12) {
					
					//If it's a square then we only need to add one addtional vertex as we can work out the rest
					vertexIndices.push_back(vertexIndex[3] - 1);
					textureIndices.push_back(textureIndex[3] - 1);
					normalIndices.push_back(normalIndex[3] - 1);
					
					if (materials.size() > 0) {
						model.colours.push_back(materials[indexOfCurrentMat].colour);
					}
				
					faceSize.push_back(4);

				}

			}

		}
	}

	//We want to push all of temp vertices into the object
	for (int i = 0; i < vertexIndices.size(); i++) {
		int vertexIndex = vertexIndices[i];
		glm::vec3 vertex = tempVertices[vertexIndex];
		model.vertices.push_back(vertex);
	}

	//We do the same for textures
	for (int i = 0; i < textureIndices.size(); i++) {
		model.textures.push_back(tempTextures[textureIndices[i]]);
	}

	//And for normals
	for (int i = 0; i < normalIndices.size(); i++) {
		model.normals.push_back(tempNormals[normalIndices[i]]);
	}

	int count = 0;

	//We take the value from the facesize and incrament by it to make sure we read
	//and construct objects correctly that used mixed n-gon faces like the low poly boat
	for (int i = 0; i < faceSize.size(); i++) {
		
		model.vertexIndices.push_back(count + 0);
		model.vertexIndices.push_back(count + 1);
		model.vertexIndices.push_back(count + 2);

		if (faceSize[i] == 4) {
			model.vertexIndices.push_back(count + 2);
			model.vertexIndices.push_back(count + 3);
			model.vertexIndices.push_back(count + 0);
		}

		count += faceSize[i];

	}

	rfile.close();

	//Performing a swap with an empty vector should free up the memory used
	std::vector<glm::vec3>().swap(tempVertices);
	std::vector<glm::vec3>().swap(tempNormals);
	std::vector<glm::vec2>().swap(tempTextures);
	std::vector<GLuint>().swap(vertexIndices);
	std::vector<GLuint>().swap(textureIndices);
	std::vector<GLuint>().swap(normalIndices);
	std::vector<Material>().swap(materials);

	//If there are no vertices then we have probably created the object incorrectly
	if (model.vertices.size() == 0) {
		model.createdSuccessfully = false;
	}
	else {
		model.createdSuccessfully = true;
	}

	return model;
}

Model loadFromDae(std::string file) {

	//Create the model to be returned
	Model model;

	//Log to the output that we're making the specified object
	std::cout << "Creating from: " + file << std::endl;

	//Create a file input stream
	std::ifstream rfile;
	rfile.open(file);

	//Instantiate a variable to hold each line of the string
	std::string line;

	//Create out variables to hold the parsed data untill the end
	std::vector<GLuint> vertexIndices, textureIndices, normalIndices, faceSize;
	std::vector<glm::vec3> tempVertices;
	std::vector<glm::vec2> tempTextures;
	std::vector<glm::vec3> tempNormals;

	//Create a string to hold the first two characters of each line just so we aren't making a method call each if check
	std::string lineStart;

	std::string segment = "";
	bool libraryImg = false;
	bool libraryGeo = false;

	if (rfile.is_open()) {
		while (std::getline(rfile, line)) {

			//We want to remove all the white space
			size_t endpos = line.find_last_not_of(" \t");
			size_t startpos = line.find_first_not_of(" \t");
			if (std::string::npos != endpos)
			{
				line = line.substr(0, endpos + 1);
				line = line.substr(startpos);
			}
			
			if (line._Equal("<library_images>")) {
				libraryImg = true;
				
			}
			
			if (libraryImg) {
				segment += line;
			}
			
			if (line._Equal("</library_images>")) {
				libraryImg = false;

				size_t start = segment.find("<init_from>");

				std::regex initFrom(R"(<init_from>(.*?)<\/init_from>)", std::regex::icase);
				std::copy(std::sregex_token_iterator(segment.begin(), segment.end(), initFrom, 1),
					std::sregex_token_iterator(),
					&model.textureLocation);

				//we want to store the texture location in the same format that the user inputted the info in
				//this means that we will use the same file delimiter. This is done as windows and unix systems
				//use a different directional slash to indicate folder structure
				char fileDelimiter = file.find("\\") != std::string::npos ? '\\' : '/';
				model.textureLocation = file.substr(0, file.find_last_of(fileDelimiter) + 1) + model.textureLocation;
				model.hasTexture = model.textureLocation.find("notexture.png") == std::string::npos;
				segment.clear();
			}

			//Similarly as above we want to look for the library geometries
			if (line._Equal("<library_geometries>")) {
				libraryGeo = true;
			}

			if (libraryGeo) {
				segment += line;
			}

			//Once the tag closes we can process it
			if (line._Equal("</library_geometries>")) {
				libraryGeo = false;

				//We want to look for each source id and it's float array,
				//we can then use that to popularte the right set of points (vertices, normals etc)
				std::regex source(R"(<source id=\"(.*?)\")", std::regex::icase);
				std::regex floatArr(R"(<float_array id=\".*?\" count=\".*?\">(.*?)<\/float_array>)", std::regex::icase);

				std::sregex_iterator sourceIter(segment.begin(), segment.end(), source);
				std::sregex_iterator fArrIter(segment.begin(), segment.end(), floatArr);

				std::sregex_iterator end;

				//we want to iterate over what we've found
				while (sourceIter != end) {
				
				std::string id = (*sourceIter)[1];

				std::istringstream iss((*fArrIter)[1]);
				std::vector<std::string> tokens{ std::istream_iterator<std::string>{iss},
				  std::istream_iterator<std::string>{} };

				//If the id contains a certain string, we know that the Array iterater will contain what we need
				if (id.find("positions") != std::string::npos) {
					for (int i = 0; i < tokens.size(); i+=3) {
						glm::vec3 pos = glm::vec3(std::stof(tokens[i]), std::stof(tokens[i + 1]), std::stof(tokens[i + 2]));
						tempVertices.push_back(pos);
					}	
				}
				else if (id.find("normals") != std::string::npos) {
					for (int i = 0; i < tokens.size(); i += 3) {
						glm::vec3 normal = glm::vec3(std::stof(tokens[i]), std::stof(tokens[i + 1]), std::stof(tokens[i + 2]));
						tempNormals.push_back(normal);
					}
				}
				else if (id.find("map") != std::string::npos) {
					for (int i = 0; i < tokens.size(); i += 2) {
						glm::vec2 text = glm::vec2(std::stof(tokens[i]), std::stof(tokens[i + 1]));
						tempTextures.push_back(text);
					}
				}

				sourceIter++;
				fArrIter++;
				}

				//We want to determine what each value in our stride is,
				//so similarly to above we look for the semantic and then it's value
				std::regex semantic(R"(semantic=\"(.*?)\")", std::regex::icase);
				std::regex offset(R"(offset=\"(.*?)\")", std::regex::icase);

				std::sregex_iterator semanticIter(segment.begin(), segment.end(), semantic);
				std::sregex_iterator offsetIter(segment.begin(), segment.end(), offset);

				int vertOff = -1;
				int normalOff = -1;
				int textOff = -1;

				while (semanticIter != end) {

					std::string index = (*semanticIter)[1];

					if (index._Equal("VERTEX")) {
						vertOff = std::stoi((*offsetIter)[1]);
					} else if (index._Equal("NORMAL")) {
						normalOff = std::stoi((*offsetIter)[1]);
					} else if (index._Equal("TEXCOORD")) {
						textOff = std::stoi((*offsetIter)[1]);
					} else if (index._Equal("POSITION")) {
						//as there can be a position semantic without an offset, we just skip that one
						semanticIter++;
						continue;
					}

					semanticIter++;
					offsetIter++;
				}

				//We pull out every point
				std::regex points(R"(<p>(.*?)<\/p>)", std::regex::icase);
				std::sregex_iterator pointIter(segment.begin(), segment.end(), points);

				//iterate over all the points
				while (pointIter != end) {
					std::istringstream iss((*pointIter)[1]);
					std::vector<std::string> tokens{ std::istream_iterator<std::string>{iss},
					  std::istream_iterator<std::string>{} };

					//Then for strides of 3, using each offset we can populate each vertex appropriately
					for (int i = 0; i < tokens.size(); i += 3) {
						
						if (vertOff >= 0) {
							vertexIndices.push_back(std::stoi(tokens[i + vertOff]));
						}

						if (textOff >= 0) {
							textureIndices.push_back(std::stoi(tokens[i + textOff]));
						}

						if (normalOff >= 0) {
							normalIndices.push_back(std::stoi(tokens[i + normalOff]));
						}
					}

					pointIter++;
					
				}
			}
		}
	}

	rfile.close();

	//Then we just process similarly to how we deal with an OBJ
	for (int i = 0; i < vertexIndices.size(); i++) {
		int vertexIndex = vertexIndices[i];
		glm::vec3 vertex = tempVertices[vertexIndex];
		model.vertices.push_back(vertex);
	}

	for (int i = 0; i < textureIndices.size(); i++) {
		model.textures.push_back(tempTextures[textureIndices[i]]);
	}

	for (int i = 0; i < normalIndices.size(); i++) {
		model.normals.push_back(tempNormals[normalIndices[i]]);
	}

	//As Collada files only contain triangles, we don't have to worry about converting anything
	for (int i = 0; i < model.vertices.size(); i += 3) {
		model.vertexIndices.push_back(i + 0);
		model.vertexIndices.push_back(i + 1);
		model.vertexIndices.push_back(i + 2);
	}

	if (model.vertices.size() == 0) {
		model.createdSuccessfully = false;
	} else {
		model.createdSuccessfully = true;
	}

	return model;

}


Model loadFromFile(std::string file) {

	if (file.length() < 4) {
		return Model();
	}

	std::string fileType = file.substr(file.length() - 4, file.length());

	Model newModel;

	if (fileType._Equal(".obj")) {
		newModel = loadFromObj(file);
	}
	else if (fileType._Equal(".dae")) {
		newModel = loadFromDae(file);
	}
	else {
		std::cout << "Sorry this file type is not supported" << std::endl;
	}

	return newModel;

}


