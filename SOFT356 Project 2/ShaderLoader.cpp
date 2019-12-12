#include "ShaderLoader.h"

#include <cstdlib>
#include <iostream>

#include "GL/glew.h"
#include "ShaderLoader.h"

/**
Given a file name, load and return the correct shader if the file exists
*/
static const GLchar* ReadShader(const char* filename) {

	FILE* infile;
	fopen_s(&infile, filename, "rb");

	//if the file doesn't open we want to return a null value
	if (!infile) {
		return NULL;
	}

	//We go to the end of the file so we are able to work out it's length
	fseek(infile, 0, SEEK_END);
	int len = ftell(infile);
	//then we reset the seek to the beginning again so we are able to read the data properly
	fseek(infile, 0, SEEK_SET);

	GLchar* source = new GLchar[len + 1];

	//We load the data from the file into the source
	fread(source, 1, len, infile);
	//We close the file to make sure we don't cause any permissions errors down the road
	fclose(infile);

	source[len] = 0;

	return const_cast<const GLchar*>(source);
}

/**
Load shaders from a given shaderinfo object
*/
GLuint LoadShaders(ShaderInfo* shaders) {
	if (shaders == NULL) {
		return 0;
	}

	GLuint program = glCreateProgram();

	ShaderInfo* entry = shaders;

	//The final shader in the shader array should be glNone, so we need to iterate over
	//until we get to a shader with that type
	while (entry->type != GL_NONE) {

		GLuint shader = glCreateShader(entry->type);
		entry->shader = shader;
		//We read the shader from the file name
		const GLchar* source = ReadShader(entry->filename);
		//if the source is null then we want to delete what we have created so far to clear up memory
		if (source == NULL) {
			for (entry = shaders; entry->type != GL_NONE; ++entry) {
				glDeleteShader(entry->shader);
				entry->shader = 0;
			}

			return 0;
		}

		glShaderSource(shader, 1, &source, NULL);
		delete[] source;

		glCompileShader(shader);

		GLint compiled;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

		//if there was an issue we want to abort and log a message in the console
		if (!compiled) {
			std::cout << "There was an issue compiling shaders";
			return 0;
		}

		//If we get to here everything has worked as assumed so we attach the shader
		glAttachShader(program, shader);

		//iterate onto the next entry
		entry++;
	}

	glLinkProgram(program);

	GLint linked;
	glGetProgramiv(program, GL_LINK_STATUS, &linked);
	if (!linked) {

		//If linking fails we want to clear down everything we have done
		for (entry = shaders; entry->type != GL_NONE; ++entry) {
			glDeleteShader(entry->shader);
			entry->shader = 0;
		}

		return 0;
	}

	return program;
}