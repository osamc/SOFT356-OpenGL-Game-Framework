#pragma once
#include "ModelLoader.h"
#include "Terrain.h"
#include "Player.h"


//Generate world
void generateWorld(std::vector<Model> &models, Player &player) {

	GLfloat size = 100;
	//Create the floor plane with the texture of grass
	Model floor = createTerrain(size, 2, "media/sand.png");
	floor.location = glm::vec3(0.0f, 0.0f, 0.0f);
	models.push_back(floor);

	//Locate the centre of the plane
	glm::vec3 furthestPoint = floor.vertices[floor.vertices.size() - 1];
	glm::vec3 centre = glm::vec3(furthestPoint.x / 2, 2, furthestPoint.z / 2);

	//Place the player there
	player.setPosition(centre);

	//Load a tree
	Model tree = loadFromFile("media/Christmas Tree.obj");
	tree.scale(0.5f);

	//Place 20 trees in random locations on the map
	for (int i = 0; i < 20; i++) {

		//Generate random numbers from 1-100
		GLfloat randomX = ((rand() % 100));
		GLfloat randomZ = ((rand() % 100));

		//Then we make them a percentage of the total size
		randomX = (randomX / 100) * size;
		randomZ = (randomZ / 100) * size;

		tree.location = glm::vec3(randomX, 0, randomZ);
		models.push_back(tree);
	
	}

	//Load a rock
	Model rock = loadFromFile("media/rock.obj");

	//Place a rock in random space
	for (int i = 0; i < 10; i++) {

		//Change the scale of the rock randomly
		rock.scale(rand() % 10 + 1);
		//Rotate the rock
		rock.rotate(glm::vec3(rand() % 10 + 1, 0, 0));

		GLfloat randomX = ((rand() % 100));
		GLfloat randomZ = ((rand() % 100));
		randomX = (randomX / 100) * size;
		randomZ = (randomZ / 100) * size;

		rock.location = glm::vec3(randomX, 0, randomZ);
		models.push_back(rock);
	}

	//Do the same for another rock model
	rock = loadFromFile("media/rock2.obj");

	for (int i = 0; i < 10; i++) {

		rock.scale(rand() % 10 + 1);
		rock.rotate(glm::vec3(rand() % 10 + 1, 0, 0));

		//Generate random numbers from 1-100
		GLfloat randomX = ((rand() % 100));
		GLfloat randomZ = ((rand() % 100));

		//Then we make them a percentage of the total size
		randomX = (randomX / 100) * size;
		randomZ = (randomZ / 100) * size;

		rock.location = glm::vec3(randomX, 0, randomZ);
		models.push_back(rock);
		
	}

}