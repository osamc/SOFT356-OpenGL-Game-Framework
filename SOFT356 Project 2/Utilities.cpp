#include "Utilities.h"

GLuint bindTexture() {
	return 1;
}

glm::vec4 convertRGB(glm::vec4 colourToConvert) {
	colourToConvert.x /= 255;
	colourToConvert.y /= 255;
	colourToConvert.z /= 255;
	return colourToConvert;
}
