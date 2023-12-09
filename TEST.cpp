#include "Game.h"

using namespace glm;

GameObject* model;
GameObject* model2;
float rotationMultiplier = -500.0f;
int camSpeed = 1;
float const velocity = 1;
float const rotationMultiplier1 = 35;
std::vector<GameObject*> pociski;
float shot_cool = 2;


std::vector<float> fastBulletTimeRemain;

const float bulletMaxTime = 3.0f;
const float bulletSpeed = 28.0f;

void shot(vec3 pos,vec3 rot, Game* game) {
	fastBulletTimeRemain.push_back(bulletMaxTime);
	GameObject* bullet = game->Create(pos, rot, vec3(2.5f), "FastBullet");
	pociski.push_back(bullet);
	bullet->Move(vec3(0, 0, -1));
}

std::string objects[]{
	"AsteroidsShip",
	"AsteroidsShipFire",
	"AsteroidsEnemy",
	"AsteroidsBullet",
	"AsteroidsStar",
	"",
	"Tank",
	"FastTank",
	"Rocket",
	"Ufo",
	"LeonardoTank",
	"FastBullet",
	"",
	"MenuCube",
	"MenuSquare",
	"/end"
};

std::vector<GameObject*> objectsVector;

void Game::TESTInit() {
	float shot_cool = 2;
	float zOffset = 0.0f;
	int xOffset = 0;
	for (int i = 0; objects[i] != "/end"; i++)
	{
		if (objects[i] == "")
		{
			zOffset -= 10.0f;
			xOffset = 0;
		}
		else
			objectsVector.push_back(Create(vec3(10.0f * xOffset++, 0.0f, zOffset), vec3(0.0f, 0.0f, 0.0f), vec3(1.0f), objects[i]));

	}
	for (char i = 'A'; i <= 'Z'; i++)
	{
		std::string letter = "Upper";
		letter += i;
		objectsVector.push_back(Create(vec3(10.0f * (i - 'A'), 0.0f, 10.0f), vec3(0.0f), vec3(1.0f), letter));
	}
	for (int i = 0; i <= 9; i++)
	{
		objectsVector.push_back(Create(vec3(10.0f * i, 0.0f, 20.0f), vec3(0.0f), vec3(1.0f), std::to_string(i)));
	}
}

void Game::TEST(float dt) {
	shot_cool -= dt;
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
	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
		if (objectsVector[8]->Transform.orientation.y > 90 || objectsVector[8]->Transform.orientation.y < -90)
			objectsVector[8]->Move(vec3(-1, 0, -1) * objectsVector[8]->Front * dt);
		else
			objectsVector[8]->Move(vec3(1, 0, 1) * objectsVector[8]->Front * dt);
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
		if (objectsVector[8]->Transform.orientation.y > 90 || objectsVector[8]->Transform.orientation.y < -90)
			objectsVector[8]->Move(vec3(-1, 0, -1) * objectsVector[8]->Front * -dt);
		else
			objectsVector[8]->Move(vec3(1, 0, 1) * objectsVector[8]->Front * -dt);

	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
		objectsVector[8]->Rotate(vec3(0, 1, 0) * dt * rotationMultiplier1);
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
		objectsVector[8]->Rotate(vec3(0, -1, 0) * dt * rotationMultiplier1);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && shot_cool<=0) {
		shot_cool = 2;
		shot(vec3(objectsVector[5]->Transform.position.x, objectsVector[5]->Transform.position.y + 1.06, objectsVector[5]->Transform.position.z + 1), vec3(objectsVector[5]->Transform.orientation.y), this);
		
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
		current->Move(vec3(0,0,-1) * bulletSpeed * dt);

		// model->Rotate(vec3(0.0f, 0.0f, 1.0f), 40.0f * dt);
	}
}