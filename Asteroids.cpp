#include "Game.h"

using namespace glm;

GameObject* ship;
std::vector<GameObject*> enemies;

const float rotationMultiplier = 100.0;

const float maxVelocity = 1.5;
const float acceleration = .8;
const float deacceleration = .4;

float velocity = 0;

void Game::AsteroidsInit() {
	ship = Create(vec3(0.0f, 0.0f, -10.0f), vec3(0.0f), vec3(.25f), "AsteroidsShip");
	//enemies.push_back(Create(vec3(0.0f, 0.0f, -10.0f), vec3(0.0f), vec3(.25f), "AsteroidsEnemy"));
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
}