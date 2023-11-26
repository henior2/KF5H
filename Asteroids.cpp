#include "Game.h"

using namespace glm;

GameObject* ship;
std::vector<GameObject*> enemies;

const float rotationMultiplier = 100.0;

const float maxVelocity = 15;
const float acceleration = 8;
const float deacceleration = 4;

float velocity = 0;

void Game::AsteroidsInit() {
	ship = Create(vec3(0.0f, 0.0f, -99.0f), vec3(0.0f), vec3(5.0f), "AsteroidsShip");
	velocity = 0;
	//enemies.push_back(Create(vec3(0.0f, 0.0f, -10.0f), vec3(0.0f), vec3(.25f), "AsteroidsEnemy"))

	camera->perspective = false;
	camera->cameraHeight = 90;
	camera->cameraWidth = 160;

	PlaySound2d("mus01.mp3", true);
}

void Game::Asteroids(float dt) {
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		velocity += acceleration * dt;
		if (velocity > maxVelocity) velocity = maxVelocity;
		ship->Move(vec3(0, velocity * dt, 0));
	}
	else {
		velocity -= deacceleration * dt;
		if (velocity < 0) velocity = 0;
		ship->Move(vec3(0, velocity * dt, 0));
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		ship->Rotate(vec3(0, 0, 1.0f) * rotationMultiplier * dt);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		ship->Rotate(vec3(0, 0, -1.0f) * rotationMultiplier * dt);
	}

	//todo: add valid condition later
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
		enemies.push_back(Create(vec3(rand() % 100 - 50, rand() % 35 - 10, -80), vec3(0.0f), vec3(5.0f), "AsteroidsEnemy"));
	}

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		Game::ChangeState(Game_Menu);

	// Nie dzia³a
	if (ship->Transform.position.y > 110) ship->MoveTo(vec3(0, -100, 0));
	if (ship->Transform.position.y < -110) ship->MoveTo(vec3(0, 100, 0));
	if (ship->Transform.position.x > 180) ship->MoveTo(vec3(-170, 0, 0));
	if (ship->Transform.position.x < -180) ship->MoveTo(vec3(170, 0, 0));

}