#version 450 core

in vec4 uiColour;
in vec2 uiText;

out vec4 fColour;

uniform sampler2D ourTexture;
uniform int textured;


void main()
{
	if(textured == 1) {
	    fColour =  texture(ourTexture, uiText);
	} else if (textured == 2) {
	  fColour =  texture(ourTexture, uiText) * uiColour;
	
	} else {
		fColour = uiColour; 
	}
}