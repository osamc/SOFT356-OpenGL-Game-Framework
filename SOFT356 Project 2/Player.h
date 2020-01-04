#pragma once
#include <glm/glm.hpp> 
#include <glm\ext\matrix_transform.hpp>
#include <GL\glew.h>


enum Controls {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	SPRINT,
	CROUCH,
	JUMP
};


class Player {

public:

	//Stamina values
	GLfloat stamina = 10;
	GLfloat maxStamina = 10;
	GLfloat staminaDec = 0.05;
	GLfloat staminaInc = 0.005;

	//Position of the player
	glm::vec3 position;

	//the height of the player
	GLfloat playerHeight = 2;
	GLfloat viewBobTime = 0;

	GLboolean isJumping;
	GLfloat jumpStart;
	GLfloat jumpHeight = 0;
	GLfloat jumpSize = 0.3;


	//Directions relative to the current direction of the player
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	//Direction facing
	GLfloat yaw;
	GLfloat pitch;

	//Character controls
	GLfloat movementSpeed;
	GLfloat speedModifier = 1;
	GLfloat mouseSensitivity;



	//Initialise with sensible values
	Player() {
		position = glm::vec3(0.0f, 2.0f, 0.0f);
		worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
		up = glm::vec3(0.0f, 1.0f, 0.0f);
		right = glm::vec3(1.0f, 0.0f, 0.0f);
		yaw = -90.0f;
		pitch = 0.0f;
		front = glm::vec3(0.0f, 0.0f, -1.0f);
		movementSpeed = 2.5f;
		mouseSensitivity = 0.1f;

	}

	void setPosition(glm::vec3 pos);

	//Method for returning the view of the player
	glm::mat4 getView();

	//Update loop for dealing with regenerating values
	void update(GLdouble currTime);


	//Processing userInput
	void processControls(Controls control, GLfloat deltaTime);

	//Process mouse movement
	void processMouseMovement(GLfloat xoffset, GLfloat yoffset);

	GLfloat getStaminaAsPercentage();

private:

	//Update local variables to deal with where we are facing
	void updateViewVectors();
};

