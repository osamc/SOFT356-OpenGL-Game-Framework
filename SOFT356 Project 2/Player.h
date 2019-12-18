#pragma once
#include <glm/glm.hpp> 
#include <glm\ext\matrix_transform.hpp>
#include <GL\glew.h>


enum Controls {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	SPRINT
};


class Player {

public:

	//Stamina values
	GLfloat stamina = 10;
	GLfloat maxStamina = 10;
	GLfloat sDec = 0.05;
	GLfloat sInc = 0.005;

	//Position of the player
	glm::vec3 position;

	//the height of the player
	GLfloat playerHeight = 2;

	//Directions relative to the current direction of the player
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	//Gametime
	GLfloat gameTime = 0;

	//Direction facing
	float yaw;
	float pitch;

	//Character controls
	float movementSpeed;
	float speedModifier = 1;
	float mouseSensitivity;

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

	void setPosition(glm::vec3 pos) {
		position = pos;
	}

	//Method for returning the view of the player
	glm::mat4 getView() {
		return glm::lookAt(position, position + front, up);
	}

	void update() {
		//Every loop we want to regen some stamina
		stamina += sInc;

		//Don't go higher than the max value
		if (stamina > maxStamina) {
			stamina = maxStamina;
		}

	}



	//Processing userInput
	void processControls(Controls control, GLfloat deltaTime) {

		//work out how  far to travel
		float distanceTraveled = movementSpeed * speedModifier * deltaTime;

		if (control != SPRINT) {
			gameTime += deltaTime;
			//Calculate the new height, we do this to allow
			//for view bobbing within the world,
			//this so it looks less smooth and more "immersive"
			position.y = (0.1 * (glm::sin(gameTime / 0.1))) + playerHeight;
		}

		//We want to either calculate the new distance traveled or change the speed modifier
		switch (control) {
		case FORWARD:
			position += front * distanceTraveled;
			break;
		case BACKWARD:
			position -= front * distanceTraveled;
			break;
		case LEFT:
			position -= right * distanceTraveled;
			break;
		case RIGHT:
			position += right * distanceTraveled;
			break;
		case SPRINT:

			stamina -= sDec;

			//We don't want negative stamina
			if (stamina < 0) {
				stamina = 0;
			}

			//If we're above this stamina value then we want to "sprint"
			if (stamina > 3) {
				speedModifier = 5;
			}

			break;
		}

		//if we're below the threshold then we want to slow down
		if (control != SPRINT) {
			speedModifier = stamina > 3 ? 2.5 : 0.5;
		}

	}

	//Process mouse movement
	void processMouseMovement(float xoffset, float yoffset)
	{
		xoffset *= mouseSensitivity;
		yoffset *= mouseSensitivity;

		yaw += xoffset;
		pitch += yoffset;

		if (pitch > 89.0f) {
			pitch = 89.0f;
		}
		else if (pitch < -89.0f) {
			pitch = -89.0f;
		}

		updateViewVectors();
	}


private:

	//Update local variables to deal with where we are facing
	void updateViewVectors()
	{
		glm::vec3 newFront;
		newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		newFront.y = sin(glm::radians(pitch));
		newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		front = glm::normalize(newFront);
		right = glm::normalize(glm::cross(front, worldUp));

	}
};

