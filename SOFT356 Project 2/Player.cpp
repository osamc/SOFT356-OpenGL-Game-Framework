#include "Player.h"
#include <GLFW\glfw3.h>


void Player::setPosition(glm::vec3 pos) {
	position = pos;
}

glm::mat4 Player::getView() {
	return glm::lookAt(position, position + front, up);
}

void Player::update(GLdouble currTime) {
	//Every loop we want to regen some stamina
	stamina += staminaInc;

	//Don't go higher than the max value
	if (stamina > maxStamina) {
		stamina = maxStamina;
	}


	if (isJumping) {

		GLdouble timeElapsed = currTime - jumpStart;
		

		if (jumpStart + 0.4 > currTime) {
			jumpHeight += (jumpSize * timeElapsed);
		}
		else if (jumpStart + 0.8 >= currTime) {
			jumpHeight -= (jumpSize * (timeElapsed));
		}
		else {
			isJumping = false;
			jumpHeight = 0;
		}

		jumpHeight = jumpHeight < 0 ? 0 : jumpHeight;
		

		position.y = playerHeight + jumpHeight;
	}
	else {
		position.y = (0.1 * (glm::sin(viewBobTime / 0.1))) + ((playerHeight + jumpHeight));

	}
		 
}

void Player::processControls(Controls control, GLfloat deltaTime) {
	
	GLfloat heightModifier = 0;

	if (control == CROUCH) {
		heightModifier = 0.66;
		speedModifier = 0.6;
		position.y = playerHeight * heightModifier;
	}
	else {
		heightModifier = 1;
	}
	
	//work out how  far to travel
	float distanceTraveled = movementSpeed * speedModifier * deltaTime;

	if (control != SPRINT && control != CROUCH) {
		viewBobTime += deltaTime;
	}

	if (control != SPRINT) {
		//Calculate the new height, we do this to allow
		//for view bobbing within the world,
		//this so it looks less smooth and more "immersive"
		position.y = (0.1 * (glm::sin(viewBobTime / 0.1))) + ((playerHeight + jumpHeight) * heightModifier);
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

		stamina -= staminaDec;

		//We don't want negative stamina
		if (stamina < 0) {
			stamina = 0;
		}

		//If we're above this stamina value then we want to "sprint"
		if (stamina > 3) {
			speedModifier = 5;
		}

		break;

	case JUMP: 
		if (!isJumping) {
			this->isJumping = true;
			this->jumpStart = glfwGetTime();
		}
	}
	

	//if we're below the threshold then we want to slow down
	if (control != SPRINT) {
		speedModifier = stamina > 3 ? 2.5 : 1.5;
	}

}

void Player::processMouseMovement(GLfloat xoffset, GLfloat yoffset) {
	xoffset *= mouseSensitivity;
	yoffset *= mouseSensitivity;

	//offset the cameras position by how much the mouse moved
	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f) {
		pitch = 89.0f;
	}
	else if (pitch < -89.0f) {
		pitch = -89.0f;
	}

	//recalcualte the new view vectors
	updateViewVectors();
}

void Player::updateViewVectors() {
	glm::vec3 newFront;
	//we calculate the new front meaning that after we turn, the camera location
	//has relative controls on the keyboard
	newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	newFront.y = sin(glm::radians(pitch));
	newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(newFront);
	right = glm::normalize(glm::cross(front, worldUp));
}

GLfloat Player::getStaminaAsPercentage() {
	return this->stamina / this->maxStamina;
}