#include "Game.h"

using namespace glm;

GameObject* model;
GameObject* model2;
float rotationMultiplier = -500.0f;
int camSpeed = 1;
int velocity = 1;
float const rotationMultiplier1 = 35;

std::string objects[] = {
	"LeonardoTank",
	"MenuCube",
	"FastTank",
	"AsteroidsEnemy",
	"AsteroidsShip",
	"AsteroidsShipFire",
	"Ufo",
	"AsteroidsBullet",
	"AsteroidsBullet2",
	"LeonardoTank",
	""
};

std::vector<GameObject*> objectsVector;

void Game::TESTInit() {
	for(int i = 0; objects[i] != ""; i++)
		objectsVector.push_back(Create(vec3(10.0f * i, 0.0f, -5.0f), vec3(0.0f, 0.0f, 0.0f), vec3(1.0f), objects[i]));
	for (char i = 'A'; i <= 'Z'; i++)
	{
		std::string letter = "Upper";
		letter += i;
		objectsVector.push_back(Create(vec3(10.0f * (i - 'A'), 0.0f, -15.0f), vec3(0.0f), vec3(1.0f), letter));
	}
}

void Game::TEST(float dt) {
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		camera->RotateCamera(rotationMultiplier * dt*camSpeed, 0);
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		camera->RotateCamera(-rotationMultiplier * dt*camSpeed, 0);
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		camera->RotateCamera(0, -rotationMultiplier * dt*camSpeed);
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		camera->RotateCamera(0, rotationMultiplier * dt*camSpeed);
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		//camera->Position.z -= 5 * dt;
		camera->MoveCamera(FORWARD, dt/2*camSpeed);
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		//camera->Position.z += 5 * dt;
		camera->MoveCamera(BACKWARD, dt/2*camSpeed);
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		//camera->Position.x -= 5 * dt;
		camera->MoveCamera(LEFT, dt/2*camSpeed);
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		//camera->Position.x += 5 * dt;
		camera->MoveCamera(RIGHT, dt/2*camSpeed);
	}

	if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS) {
		camera->Position = vec3(0.0f, 0.0f, 0.0f);
		camera->Yaw = -90.0f;
		camera->Pitch = 0.0f;
		camera->MoveCamera(FORWARD, 0.0f);
		camera->RotateCamera(0.0f, 0.0f);
	}

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		Game::ChangeState(Game_Menu);

	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		camSpeed = 2;
	else
		camSpeed = 1;
	if (glfwGetKey(window,GLFW_KEY_T) == GLFW_PRESS)
		objectsVector[0]->Move(vec3(0, 0, -velocity * dt));
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
		objectsVector[0]->Move(vec3(0,0, velocity * dt));
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
		objectsVector[0]->Rotate(vec3(0, 1, 0)  * dt * rotationMultiplier1);
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
		objectsVector[0]->Rotate(vec3(0,-1,0)  * dt * rotationMultiplier1);

	// model->Rotate(vec3(0.0f, 0.0f, 1.0f), 40.0f * dt);
}