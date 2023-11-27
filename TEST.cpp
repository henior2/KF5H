#include "Game.h"

using namespace glm;

GameObject* model;
float rotationMultiplier = -50.0f;

float _delta = 0.0f;

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	model->MoveGlobal(vec3(0, 0, yoffset * _delta * 50));
	model->Rotate(vec3(0, xoffset * _delta * -rotationMultiplier * 50, 0));
}
void Game::TESTInit() {

	model = Create(vec3(0.0f, 0.0f, -5.0f), vec3(20.0f, 0.0f, 0.0f), vec3(1.0f), "MenuCube");
	glfwSetScrollCallback(window, scroll_callback);
}

void Game::TEST(float dt) {
	_delta = dt;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		model->Rotate(vec3(0, 1.0f, 0) * rotationMultiplier * dt);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		model->Rotate(vec3(0, -1.0f, 0) * rotationMultiplier * dt);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		model->Rotate(vec3(-1.0f, 0, 0) * rotationMultiplier * dt);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		model->Rotate(vec3(1.0f, 0, 0) * rotationMultiplier * dt);

	}
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		Game::ChangeState(Game_Menu);


}