#include "Game.h"

using namespace glm;

namespace Test {
	GameObject* model3;
	GameObject* model4;
	float rotationMultiplier1 = -500.0f;
	int camSpeed1 = 1;
	float const velocity1 = 1;
	float const rotationMultiplier2 = 35;
	std::vector<GameObject*> pociski1;
	float shot_cool1 = 2;

	std::vector<float> fastBulletTimeRemain1;

	const float bulletMaxTime1 = 4.0f;
	const float bulletSpeed1 = 28.0f;

	void shot(vec3 pos, vec3 rot, Game* game) {
		fastBulletTimeRemain1.push_back(bulletMaxTime1);
		GameObject* bullet = game->Create(pos, rot, vec3(1.0f), "FastBullet");
		pociski1.push_back(bullet);
		bullet->Move(vec3(0, 0, -1));
	}

	std::vector<std::string> objects = {
		"AsteroidsShip",
		"AsteroidsShipFire",
		"AsteroidsEnemy",
		"AsteroidsBullet",
		"AsteroidsStar",
		"AsteroidsEnemyBullet",
		"",
		"Tank",
		"FastTank",
		"Rocket",
		"Ufo",
		"LeonardoTank",
		"FastBullet",
		"TankBUllet",
		"BattlezonePlane",
		"RadarX",
		"RadarT",
		"PowerUpBox",
		"PowerUpSpeed",
		"PowerUpHeart",
		"",
		"MenuCube",
		"MenuSquare",
		"",
		"apostrophe",
		"colon",
		"comma",
		"dash",
		"dot",
		"exclamation-mark",
		"left-bracket",
		"right-bracket",
		"percent",
		"semi-colon",
		"question-mark",
		"",
		"Blaster",
		"Flipper", 
		"Tanker",
		"Spiker",
		"Fuseball",
		"Pulsar",
		""
	};

	std::vector<GameObject*> objectsVector;
}

using namespace Test;

void Game::TESTInit() {
	objects.push_back("");
	for (char i = 'A'; i <= 'Z'; i++)
	{
		std::string letter = { i };
		objects.push_back(letter);

		letter = (std::tolower(letter[0]));
		objects.push_back(letter);
	}
	objects.push_back("");
	for (int i = 0; i <= 9; i++)
	{
		objects.push_back(std::to_string(i));
	}

	float shot_cool1 = 2;
	float zOffset1 = 0.0f;
	int xOffset1 = 0;
	for (const std::string& object : objects)
	{
		if (object.empty())
		{
			zOffset1 -= 10.0f;
			xOffset1 = 0;
		}
		else
			objectsVector.push_back(Create(vec3(10.0f * xOffset1++, 0.0f, zOffset1), vec3(0.0f, 0.0f, 0.0f), vec3(1.0f), object));

	}
}

void Game::TEST(float dt) {
	shot_cool1 -= dt;
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		camera->RotateCamera(rotationMultiplier1 * dt * camSpeed1, 0);
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		camera->RotateCamera(-rotationMultiplier1 * dt * camSpeed1, 0);
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		camera->RotateCamera(0, -rotationMultiplier1 * dt * camSpeed1);
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		camera->RotateCamera(0, rotationMultiplier1 * dt * camSpeed1);
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		//camera->Position.z -= 5 * dt;
		camera->MoveCamera(FORWARD, dt / 2 * camSpeed1);
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		//camera->Position.z += 5 * dt;
		camera->MoveCamera(BACKWARD, dt / 2 * camSpeed1);
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		//camera->Position.x -= 5 * dt;
		camera->MoveCamera(LEFT, dt / 2 * camSpeed1);
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		//camera->Position.x += 5 * dt;
		camera->MoveCamera(RIGHT, dt / 2 * camSpeed1);
	}

	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS){
		camera->Position.y += 20 * camSpeed1 * dt / 2;
	}\

	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		camera->Position.y -= 20 * camSpeed1 * dt / 2;
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
		camSpeed1 = 2;
	else
		camSpeed1 = 1;
	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
		objectsVector[7]->Move(vec3(0, 0, -1) * dt);
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
		objectsVector[7]->Move(vec3(0, 0, 1) * dt);

	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
		objectsVector[7]->Rotate(vec3(0, 1, 0) * dt * rotationMultiplier2);
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
		objectsVector[7]->Rotate(vec3(0, -1, 0) * dt * rotationMultiplier2);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && shot_cool1<=0) {
		shot_cool1 = 1.8f;
		if(objectsVector[7]->Transform.orientation.y!=0 && objectsVector[7]->Transform.orientation.y != 180)
			shot(objectsVector[7]->Transform.position + vec3(0, 1.06, 0), objectsVector[7]->Transform.orientation, this);
		else
			shot(objectsVector[7]->Transform.position + vec3(0,1.06,1), objectsVector[7]->Transform.orientation, this);
	}
	for (int i = 0; i < pociski1.size(); i++) {
		GameObject* current = pociski1[i];

		fastBulletTimeRemain1[i] -= dt;
		if (fastBulletTimeRemain1[i] <= 0) {
			Destroy(current);
			pociski1.erase(pociski1.begin() + i);
			fastBulletTimeRemain1.erase(fastBulletTimeRemain1.begin() + i);
			i--;
			continue;
		}
		current->Move(vec3(0,0,-1) * bulletSpeed1 * dt);

		// model->Rotate(vec3(0.0f, 0.0f, 1.0f), 40.0f * dt);
	}


}