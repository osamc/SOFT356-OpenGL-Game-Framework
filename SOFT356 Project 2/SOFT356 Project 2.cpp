#pragma once
#include "Player.h"
#include "Model.h"
#include <vector>
#include "GL/glew.h"
#include "GL/freeglut.h"
#include "GLFW/glfw3.h"
#include <glm/glm.hpp> //includes GLM
#include "UIRenderer.h"
#include "WorldGeneration.h"

//The opengl window
GLFWwindow* window;

//The Game elements
Player player;
UIRenderer uiRenderer;
std::vector<Model> worldModels;

//Variables for describing the game
GLint wWidth = 1280, wHeight = 960;

//Variables for keeping track of how much time has past
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

//Variables for dealing with mouse inputs
GLfloat lastX = wWidth / 2.0f;
GLfloat lastY = wHeight / 2.0f;
GLboolean firstMouse = true;

//Method for dealing with mouse movements
void mouseHook(GLFWwindow* window, GLdouble xpos, GLdouble ypos) {

	//If it is the first loop round, then we set to the positions to be the same
	//this prevents a massive initial jump/spike when the player first moves
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	//We then want to know how much to offset the player view
	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	//We then deal with the movement
	player.processMouseMovement(xoffset, yoffset);
}

//A method for dealing with keyboard inputs
void processUserInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		player.processControls(FORWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		player.processControls(BACKWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		player.processControls(LEFT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		player.processControls(RIGHT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		player.processControls(SPRINT, deltaTime);
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
		player.processControls(CROUCH, deltaTime);
	}

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		player.processControls(JUMP, deltaTime);
	}

}

//Initialise OpenGL
void initialiseOpenGL(GLint windowWidth, GLint windowHeight, std::string title) {

	wWidth = windowWidth;
	wHeight = windowHeight;

	//Initialise glfw
	glfwInit();

	//Create our window
	window = glfwCreateWindow(windowWidth, windowHeight, title.c_str(), NULL, NULL);

	//Initialise glew
	glfwMakeContextCurrent(window);
	glewInit();

	glfwSetCursorPosCallback(window, mouseHook);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glEnable(GL_DEPTH_TEST);

}

//We generate the world and initialise it
void initialiseWorld() {
	//Generate the world
	generateWorld(worldModels, player);

	for (int i = 0; i < worldModels.size(); i++) {
		worldModels[i].init();
	}

}

//Initialise the UI
void initialiseUI() {
	uiRenderer.createGameElements(wWidth, wHeight);
	uiRenderer.initialiseElements();
}

//Initialise everything
void initialise(GLint windowWidth, GLint windowHeight, std::string title) {

	initialiseOpenGL(windowWidth, windowHeight, title);
	initialiseWorld();
	initialiseUI();

}

//Render method to draw all objects needed to be drawn
void render() {

	GLfloat currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	processUserInput(window);

	for (int i = 0; i < worldModels.size(); i++) {
		worldModels[i].draw(player.position, player.getView());
	}

	uiRenderer.drawElements(player);
	player.update(currentFrame);

	glfwSwapBuffers(window);
	glfwPollEvents();

	float black[] = { 0.0f, 0.0f, 0.0f, 0.0f };

	//Then clear ready for next pass
	glClearBufferfv(GL_COLOR, 0, black);
	glClearColor(0.1f, 0.1f, 0.3f, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}


//simple loop to run the game
int main() {
	initialise(wWidth, wHeight, "SOFT356 Game Demo");
	do {
		render();
	} while (glfwGetKey(window, GLFW_KEY_Q) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

	return 0;
}