#include "TextRendering.h"
#include "stb_image.h"
#include "ShaderLoader.h"

#define BUFFER_OFFSET(a) ((void*)(a))


void TextRenderer::init(std::string fontFile, std::string texture) {

	std::ifstream file;
	std::string line;

	file.open(fontFile);

	//We iterate over each line of the file if it exists
	if (file.is_open()) {
		while (std::getline(file, line)) {

			Character c;
			//we then pull out the relevent infomation to construct the character class
			int matches = sscanf_s(line.c_str(), "char id=%d   x=%d     y=%d     width=%d     height=%d     xoffset=%d     yoffset=%d    xadvance=%d     page=%d  chnl=%d \n",
				&c.id, &c.x, &c.y, &c.width, &c.height, &c.xoffset, &c.yoffset, &c.xadvance, &c.page, &c.channel);
		
			//If we have 10 matches then all the info we need to rendering the character has been collected, we then want to add it to the map
			if (matches == 10) {
				characters.insert(std::pair<GLuint, Character>(c.id, c));
			}

		}
	}
	
	//As this system uses cooridnates and a png image, we then want to bind the texture associated to the font file
	bindTexture(texture);

}


void TextRenderer::createText(std::string text, glm::vec4 colour, GLfloat scale, glm::vec2 position, GLint width, GLint height) {

	//The cursor position is a virtual position on the screen that moves along after each character is drawn
	//this prevents all characters from being drawn in the same location on top of each other
	glm::vec2 cursorPosition;

	GLfloat maxCharHeight = 0;

	//We want to go through each character to find the largest
	for (int i = 0; i < characters.size(); i++) {
		maxCharHeight = maxCharHeight < characters[i].height ? characters[i].height : maxCharHeight;
	}

	//We then offset the y position by the largest character so that all elements should fit on screen
	cursorPosition.x = position.x * width;
	cursorPosition.y = (position.y * height) + (maxCharHeight * scale);

	int count = 0;

	//Iterate over each char in the string
	for (char& c : text) {
		
		try {
			
			//We location the character
			Character character = characters.at(int(c));
			
			//Calculate the vertices
			glm::vec2 topLeft = glm::vec2(cursorPosition.x + (character.xoffset * scale), cursorPosition.y + (character.yoffset * scale));
			glm::vec2 topRight = glm::vec2(topLeft.x + (character.width * scale), topLeft.y);
			glm::vec2 bottomRight = glm::vec2(topRight.x, topLeft.y - (character.height * scale));
			glm::vec2 bottomLeft = glm::vec2(topLeft.x, bottomRight.y);

			vertices.push_back(topLeft);
			vertices.push_back(topRight);
			vertices.push_back(bottomRight);

			vertices.push_back(bottomRight);
			vertices.push_back(bottomLeft);
			vertices.push_back(topLeft);

			vertexIndices.push_back((count * 6) + 0);
			vertexIndices.push_back((count * 6) + 1);
			vertexIndices.push_back((count * 6) + 2);

			vertexIndices.push_back((count * 6) + 3);
			vertexIndices.push_back((count * 6) + 4);
			vertexIndices.push_back((count * 6) + 5);

			//Calculate the texture vertices
			textures.push_back(glm::vec2(character.x, character.y));
			textures.push_back(glm::vec2((character.x + character.width), character.y));
			textures.push_back(glm::vec2((character.x + character.width), (character.y + character.height)));

			textures.push_back(glm::vec2((character.x + character.width), (character.y + character.height)));
			textures.push_back(glm::vec2(character.x, (character.y + character.height)));
			textures.push_back(glm::vec2(character.x, character.y));

			//Put in the same number of colours as vertices to ensure that all the text is the same colour
			for (int i = 0; i < 6; i++) {
				colours.push_back(colour);
			}

			//incrament count
			count++;
			//We then advance the cursor by the characters advance variable
			cursorPosition.x += (character.xadvance * scale);
			

		}
		catch (const std::out_of_range) {
			std::cout << "Unable to parse character: " << c << std::endl;
		}
		
		
	}

	//Everything above worked in pixels, so now we scale it by the size of the window
	for (int i = 0; i < vertices.size(); i++) {
		vertices[i].x /= width;
		vertices[i].y /= height;
	}

	//Same as above, but we scale it to the size of the texture
	for (int i = 0; i < textures.size(); i++) {
		textures[i].x /= this->textureWidth;
		textures[i].y /= this->textureHeight;
	}

	//We then populate the buffers with the correct info
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

void TextRenderer::draw() {
	//Enable png transparency
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glUseProgram(usedProgram);
	glUniform1i(glGetUniformLocation(usedProgram, "textured"), 2);

	glBindVertexArray(VAOs[ModelVAO]);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());
	//Turn it back off
	glDisable(GL_BLEND);
}


void TextRenderer::bindTexture(std::string texture) {
	
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	GLint nrChannels;

	stbi_set_flip_vertically_on_load(false); 

	//We then store the textureWidth and height to be used to calculate the relative size
	unsigned char* data = stbi_load(texture.c_str(), &this->textureWidth, &this->textureHeight, &nrChannels, 0);

	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->textureWidth, this->textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	
	stbi_image_free(data);
}

