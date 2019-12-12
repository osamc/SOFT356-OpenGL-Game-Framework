
#include "GL/glew.h"
#include "GL/freeglut.h"
#include "GLFW/glfw3.h"
#include <glm/glm.hpp> //includes GLM
#include <string>
#include "ModelLoader.h"
#include "ShaderLoader.h"
#include "Player.h"

Player player;
float deltaTime = 0.0f;
float lastFrame = 0.0f;

float lastX = 800 / 2.0f;
float lastY = 600 / 2.0f;
bool firstMouse = true;

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	player.processMouseMovement(xoffset, yoffset);
}

void processUserInput(GLFWwindow* window)
{

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
}


int main()
{

	//Stores for the models and locations
	std::vector<Model> models;
	std::vector<glm::vec3> modelLocations;

	player = Player();

	models.push_back(loadFromFile("media/creeper.obj"));
	modelLocations.push_back(glm::vec3(0, 0, -2));

	//Initialise glfw
	glfwInit();

	//Create our window
	GLFWwindow* window = glfwCreateWindow(800, 600, "Object Loader", NULL, NULL);

	//Initialise glew
	glfwMakeContextCurrent(window);
	glewInit();

	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//Initialise every loaded model
	for (int i = 0; i < models.size(); i++) {
		models[i].init();
	}

	//Set up values for rotation and scale
	static const float black[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	GLfloat scale = 1.0f;

	std::cout << "You are able to close the window with the Q key" << std::endl;

	glEnable(GL_DEPTH_TEST);

	//main opengl draw loop
	do {
		
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processUserInput(window);

		//Foreach model we want to rotate/translate/scale/draw it
		for (int i = 0; i < models.size(); i++) {
			models[i].translate(modelLocations[i]);
			models[i].draw(player.position, player.getView());
		}

		glfwSwapBuffers(window);
		glfwPollEvents();

		//Then clear ready for next pass
		glClearBufferfv(GL_COLOR, 0, black);
		glClearColor(0.0f, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_Q) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);


	glfwDestroyWindow(window);
	glfwTerminate();

}

