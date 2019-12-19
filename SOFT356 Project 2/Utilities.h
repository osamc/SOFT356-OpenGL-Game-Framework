#pragma once
#include <glm/glm.hpp> 
#include <GL\glew.h>

//Convert a typical rgb value into an opengl compatible one
glm::vec4 convertRGB(glm::vec4 colourToConvert);
