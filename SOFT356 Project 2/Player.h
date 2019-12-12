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

	//Position of the player
	glm::vec3 position;

	//Directions relative to the current direction of the player
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	//Direction facing
	float yaw;
	float pitch;

	//Character controls
	float movementSpeed;
	float mouseSensitivity;

	//Initialise with sensible values
	Player() {
		position = glm::vec3(0.0f, 0.0f, 0.0f);
		worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
		up = glm::vec3(0.0f, 1.0f, 0.0f);
		right = glm::vec3(1.0f, 0.0f, 0.0f);
		yaw = -90.0f;
		pitch = 0.0f;
		front = glm::vec3(0.0f, 0.0f, -1.0f);
		movementSpeed = 2.5f;
		mouseSensitivity = 0.1f;
	
	}

	//Method for returning the view of the player
	glm::mat4 getView() {
		return glm::lookAt(position, position + front, up);
	}


	//Processing userInput
	void processControls(Controls control, float deltaTime)
	{
		float velocity = movementSpeed * deltaTime;

		switch (control) {
		case FORWARD:
			position += front * velocity;
			break;
		case BACKWARD:
			position -= front * velocity;
			break;
		case LEFT: 
			position -= right * velocity;
			break;
		case RIGHT:
			position += right * velocity;
			break;
		}

		std::cout << position.x << " : " << position.y << " : " << position.z << std::endl;

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
		} else if (pitch < -89.0f) {
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
		up = glm::normalize(glm::cross(right, front));

	}
};

