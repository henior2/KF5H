#include "Game.h"
#include "cmath"

using namespace glm;

GameObject* ship;
std::vector<GameObject*> enemies;

const float rotationMultiplier = 100.0;

const float maxVelocity = 1.5;
const float acceleration = .8;
const float deacceleration = .4;

float velocity = 0;

std::vector<vec3> forces;
vec3 netForce;

void Game::AsteroidsInit() {
	ship = Create(vec3(0.0f, 0.0f, -50.1f), vec3(0.0f), vec3(.25f), "AsteroidsShip");
	//enemies.push_back(Create(vec3(0.0f, 0.0f, -10.0f), vec3(0.0f), vec3(.25f), "AsteroidsEnemy"))

	camera->perspective = false;
	camera->cameraHeight = 800;
	camera->cameraWidth = 600;

	PlaySound2d("mus01.mp3", true);
}

void Game::Asteroids(float dt) {
	netForce = vec3(0.0f, 0.0f, 0.0f);

	for (auto i = forces.rbegin(); i != forces.rend(); ++i) {
		i->x -= deacceleration * dt;
		i->y -= deacceleration * dt;

		i->x = std::max(i->x, 0.0f);
		i->y = std::max(i->y, 0.0f);

		if (i->x == 0.0f && i->y == 0.0f) {
			forces.erase(i.base() - 1);
			continue;
		}

		netForce += i;
	}

	if (netForce.x > maxVelocity) netForce.x = maxVelocity;
	if (netForce.y > maxVelocity) netForce.y = maxVelocity;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		vec3 orientation = ship->Transform.orientation;
		forces.push_back(vec3(acceleration * cos(orientation.x), acceleration * sin(orientation.y), 0.0f));
	}
	ship->MoveGlobal(netForce * dt);
	
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		ship->Rotate(vec3(0, 0, 1.0f) * rotationMultiplier * dt);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		ship->Rotate(vec3(0, 0, -1.0f) * rotationMultiplier * dt);
	}

	//todo: add valid condition later
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
		enemies.push_back(Create(vec3(rand() % 1200 - 600, rand() % 1600 - 800, -50.1f), vec3(0.0f), vec3(.25f), "AsteroidsEnemy"));
	}

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		Game::ChangeState(Game_Menu);

	if (ship->Transform.position.y > 400) ship->MoveTo(vec3(0, -400, 0));
	if (ship->Transform.position.y < -400) ship->MoveTo(vec3(0, -400, 0));
	if (ship->Transform.position.x > 300) ship->MoveTo(vec3(-300, 0, 0));
	if (ship->Transform.position.x < -300) ship->MoveTo(vec3(300, 0, 0));

}