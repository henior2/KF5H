#include "Game.h"
#include <time.h>
using namespace glm;

GameObject* model;
GameObject* model2;
float rotationMultiplier = -500.0f;
int camSpeed = 1;
float const velocity = 1;
float const rotationMultiplier1 = 35;
std::vector<GameObject*> pociski;
std::vector<GameObject*> przeciwnicy;
float shot_cool = 2;
float resp_cool = 2;

std::vector<float> fastBulletTimeRemain;

const float bulletMaxTime = 3.0f;
const float bulletSpeed = 28.0f;

void shot(vec3 pos, vec3 rot, Game* game) {
	fastBulletTimeRemain.push_back(bulletMaxTime);
	GameObject* bullet = game->Create(pos, rot, vec3(1.0f), "FastBullet");
	pociski.push_back(bullet);
	bullet->Move(vec3(0, 0, -1));
	shot_cool = 2;
}

void spawn_tank(vec3 pos, vec3 rot, Game* game){
		GameObject* enemy = game->Create(pos, rot, vec3(.5f), "Tank");
		przeciwnicy.push_back(enemy);
}

std::vector<std::string> objects = {
	"FastTank",
	""
};

std::vector<GameObject*> enemiesVector;

std::vector<GameObject*> objectsVector;

void Game::BattlezoneInit() {

	float shot_cool = 2;
	float resp_cool = 2;
	float zOffset = 0.0f;
	int xOffset = 0;
	for (const std::string& object : objects)
	{
		if (object.empty())
		{
			zOffset -= 10.0f;
			xOffset = 0;
		}
		else
			objectsVector.push_back(Create(vec3(10.0f * xOffset++, 0.0f, zOffset), vec3(0.0f, 0.0f, 0.0f), vec3(1.0f), object));

	}
}

void Game::Battlezone(float dt) {
	shot_cool -= dt;
	resp_cool -= dt;

	//Poruszanie kamer¹
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		camera->RotateCamera(rotationMultiplier * dt * camSpeed, 0);
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		camera->RotateCamera(-rotationMultiplier * dt * camSpeed, 0);
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		camera->RotateCamera(0, -rotationMultiplier * dt * camSpeed);
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		camera->RotateCamera(0, rotationMultiplier * dt * camSpeed);
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		//camera->Position.z -= 5 * dt;
		camera->MoveCamera(FORWARD, dt / 2 * camSpeed);
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		//camera->Position.z += 5 * dt;
		camera->MoveCamera(BACKWARD, dt / 2 * camSpeed);
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		//camera->Position.x -= 5 * dt;
		camera->MoveCamera(LEFT, dt / 2 * camSpeed);
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		//camera->Position.x += 5 * dt;
		camera->MoveCamera(RIGHT, dt / 2 * camSpeed);
	}

	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) camera->Position.y += 2 * camSpeed * dt;
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) camera->Position.y -= 2 * camSpeed * dt;

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

	// Poruszanie modelem
	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
		objectsVector[0]->Move(vec3(0, 0, -1) * dt);
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
		objectsVector[0]->Move(vec3(0, 0, 1) * dt);

	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
		objectsVector[0]->Rotate(vec3(0, 1, 0) * dt * rotationMultiplier1);
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
		objectsVector[0]->Rotate(vec3(0, -1, 0) * dt * rotationMultiplier1);

	//Strzelanie
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && shot_cool <= 0) {
		if (objectsVector[0]->Transform.orientation.y != 0 && objectsVector[0]->Transform.orientation.y != 180)
			shot(objectsVector[0]->Transform.position + vec3(0, 1.06, 0), objectsVector[0]->Transform.orientation, this);
		else
			shot(objectsVector[0]->Transform.position + vec3(0, 1.06, 1), objectsVector[0]->Transform.orientation, this);
	}
	for (int i = 0; i < pociski.size(); i++) {
		GameObject* current = pociski[i];

		fastBulletTimeRemain[i] -= dt;
		if (fastBulletTimeRemain[i] <= 0) {
			Destroy(current);
			pociski.erase(pociski.begin() + i);
			fastBulletTimeRemain.erase(fastBulletTimeRemain.begin() + i);
			i--;
			continue;
		}
		current->Move(vec3(0, 0, -1) * bulletSpeed * dt);

		// model->Rotate(vec3(0.0f, 0.0f, 1.0f), 40.0f * dt);
	}
	//Spawnowanie przeciwników
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS && resp_cool <= 0) {
		srand(time(NULL));
		float temp_x = rand() % 51 -25;
		float temp_z = rand() % 51 -25;
		float temp_y = rand() % 361;
		spawn_tank(objectsVector[0]->Transform.position + vec3(temp_x, 0, temp_z), vec3(0,temp_y,0), this);
		resp_cool = 2;
	}
}