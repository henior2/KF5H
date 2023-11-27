#include "Game.h"

using namespace glm;

GameObject* model;
float rotationMultiplier = -50.0f;

void Game::BattlezoneInit() {
	model = Create(vec3(0.0f, 0.0f, -5.0f), vec3(20.0f, 0.0f, 0.0f), vec3(1.0f), "MenuCube");
}

void Game::Battlezone(float dt) {
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		model->Rotate(vec3(0, 1.0f, 0) * rotationMultiplier * dt);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		model->Rotate(vec3(0, -1.0f, 0) * rotationMultiplier * dt);
	}
	if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		model->Rotate(vec3(-1.0f, 0, 0) * rotationMultiplier * dt);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		model->Rotate(vec3(1.0f, 0, 0) * rotationMultiplier * dt);
	}

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		Game::ChangeState(Game_Menu);
}