#include "UIRenderer.h"
#include "Utilities.h"

UIelement staminaBar;

void UIRenderer::createGameElements(GLint windowWidth, GLint windowHeight) {
	
	//We create the stamina text element
	TextRenderer staminaText;
	//We want to use this specific fnt file
	staminaText.init("media/font/font.fnt", "media/font/font.png");
	staminaText.createText("stamina:", convertRGB(glm::vec4(255, 255, 255, 255)), 2.0f, glm::vec2(-0.125f, -1.0f), windowWidth, windowHeight);

	//Add the text to the list of elements
	this->textElements.push_back(staminaText);

	UIelement heart;
	heart.setTransparent(true);
	for (int i = 0; i < 5; i++) {
		heart.createRectangle(glm::vec2(-1.0f + (i * 0.1f), -0.9f), 0.1f, 0.1f, glm::vec4(0.0f, 0.0f, 0.0f, 0.0f), "media/heart.png");
		this->uiElements.push_back(heart);
	}

	//UIelement element;
	//element.createRectangle(glm::vec2(0.6f, 1.0f), 0.4f, 0.4f, glm::vec4(0.29f, 0.0f, 1.0f, 0.5f), "media/test.png");

	//this->uiElements.push_back(element);

}

void UIRenderer::initialiseElements() {

	//We want to initalise all ui elements
	for (int i = 0; i < this->uiElements.size(); i++) {
		this->uiElements[i].init();
	}

	//Due to how the text is made, it is initalised when it is given the text.

}


void UIRenderer::drawElements(Player player)
{

	//loop over and draw all elements
	for (int i = 0; i < this->uiElements.size(); i++) {
		this->uiElements[i].draw();
	}

	for (int i = 0; i < this->textElements.size(); i++) {
		this->textElements[i].draw();
	}

	//We cannot initialise the stamina bar in the loops above as the values used within it get updated, so we need to recalculate and rebind everything
	staminaBar.createRectangle(glm::vec2(0.4f, -0.8f), 0.2f, 0.6f * player.getStaminaAsPercentage(), glm::vec4(0.29f, 0.0f, 1.0f, 0.5f));
	staminaBar.init();
	staminaBar.draw();

}


