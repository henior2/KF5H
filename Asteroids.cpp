#include "Game.h"

using namespace glm;

GameObject* ship;

const float rotationMultiplier = 100.0;
const float moveSpeedMultiplier = 1.0;

void Game::AsteroidsInit() {
	ship = Create(vec3(0.0f, 0.0f, -10.0f), vec3(0.0f), vec3(1.0f), "AsteroidsShip");
}

void Game::Asteroids(float dt) {
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		ship->Move(vec3(0, 1.0f, 0) * moveSpeedMultiplier * dt);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		ship->Rotate(vec3(0, 0, 1.0f) * rotationMultiplier * dt);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		ship->Rotate(vec3(0, 0, -1.0f) * rotationMultiplier * dt);
	}
}