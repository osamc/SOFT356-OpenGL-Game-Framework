#version 400 core

layout( location = 0 ) in vec4 inPosition;
layout( location = 1 ) in vec4 inColour;
layout (location = 2) in vec2 inText;

out vec4 uiColour;
out vec2 uiText;


void main()
{
    gl_Position = inPosition;
	uiColour = inColour;
	uiText = inText;
}