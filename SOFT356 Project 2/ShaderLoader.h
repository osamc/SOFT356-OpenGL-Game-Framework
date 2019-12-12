#pragma once
#ifndef __LOAD_SHADERS_H__
#define __LOAD_SHADERS_H__

#include "GL/glew.h"

#ifdef __cplusplus

#endif 

	typedef struct {
		GLenum       type;
		const char* filename;
		GLuint       shader;
	} ShaderInfo;

	GLuint LoadShaders(ShaderInfo*);

#endif 