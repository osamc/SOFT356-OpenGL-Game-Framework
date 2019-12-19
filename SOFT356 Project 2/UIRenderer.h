#pragma once
#include <vector>
#include "UIelement.h"
#include "Player.h"

class UIRenderer{

//List of UIElements
std::vector<UIelement> uiElements;
//List of TextElements
std::vector<TextRenderer> textElements;

public:

//Method for creating the game elements predefined
void createGameElements(GLint windowWidth, GLint windowHeight);
//initialise the game elements
void initialiseElements();
//draw the elements
void drawElements(Player player);


};
