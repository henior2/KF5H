#include "Game.h"
#include <time.h>

using namespace glm;

namespace Battlezone {
	GameObject* player;
	GameObject* model2;

	float rotationMultiplier = -500.0f;
	int camSpeed = 1;
	float const velocity = 1.5f;
	float const rotationMultiplier1 = 35;
	std::vector<GameObject*> pociski;
	std::vector<GameObject*> przeciwnicy;
	float shot_cool = 2;
	float resp_cool = 2;
	float fast_tank_speed = 4;
	float tank_speed = 1;

	std::vector<float> fastBulletTimeRemain;

	const float bulletMaxTime = 4.0f;
	const float bulletSpeed = 28.0f;

	void shot_fast(vec3 pos, vec3 rot, Game* game) {
		fastBulletTimeRemain.push_back(bulletMaxTime);
		GameObject* bullet = game->Create(pos, rot, vec3(1.0f), "FastBullet");
		pociski.push_back(bullet);
		bullet->Move(vec3(0, 0, -1));
		shot_cool = 2;
	}

	void shot(vec3 pos, vec3 rot, Game* game) {
		fastBulletTimeRemain.push_back(bulletMaxTime);
		GameObject* bullet = game->Create(pos, rot, vec3(1.0f), "TankBullet");
		pociski.push_back(bullet);
		bullet->Move(vec3(0, 0, -1));
		shot_cool = 2;
	}

	void spawn_tank(vec3 pos, vec3 rot, Game* game) {
		GameObject* enemy = game->Create(pos, rot, vec3(.5f), "Tank");
		przeciwnicy.push_back(enemy);
	}
	
	float camFrontOffset = -2.5f;
	float camYOffset = 1.75f;

	std::vector<GameObject*> enemiesVector;

	std::vector<GameObject*> objectsVector;

	std::vector<GameObject*> obstacles;

	const int minBaseVerticies = 3;
	const int maxBaseVerticies = 7;
	const int minLevelVerticiesNumberDecrease = 1;
	const int maxLevelVerticiesNumberDecrease = 2;
	const float maxVertexOffset = .15f;
	const int minLevels = 2;
	const int maxLevels = 4;
	const float levelMaxYOffset = .15f;
	const float minRadius = 5;
	const float maxRadius = 7.5f;
	const float minLevelRadiusDecrease = .25f;
	const float maxLevelRadiusDecrease = .5f;
	const float minScale = .8f;
	const float maxScale = 1.2f;

	int sumUp(int arr[], int i) {
		int sum = 0;
		while (i >= 0) {
			sum += arr[i];
			i--;
		}
		return sum;
	}

	void makeObstacles(float x, float z, float height, Game* game) {
		std::vector<float> vx;
		std::vector<unsigned int> ind;

		std::vector<vec3> points;

		int lv = rand() % (maxLevels - minLevels) + minLevels;

		float lHeight = height / lv;

		int ver = rand() % (maxBaseVerticies - minBaseVerticies) + minBaseVerticies;
		float radius = minRadius + (float)(rand()) / ((float)(RAND_MAX / (maxRadius - minRadius)));

		int* lVxs = new int[lv-1];

		for (int i = 0; i < lv-1; i++) {
			if (i != 0) ver -= rand() % (maxLevelVerticiesNumberDecrease - minLevelVerticiesNumberDecrease) + minLevelVerticiesNumberDecrease;
			if (ver < minBaseVerticies) ver = minBaseVerticies;

			lVxs[i] = ver;
			int noVxLvBw = sumUp(lVxs, i - 1);

			float yModifier = (float)(rand()) / (static_cast <float> (RAND_MAX / levelMaxYOffset));
			if (rand() % 2) yModifier *= -1;

			if (i != 0) radius -= (minLevelRadiusDecrease + (float)(rand()) / ((float)(RAND_MAX / (maxLevelRadiusDecrease - minLevelRadiusDecrease))));
			if (radius < minRadius) radius = minRadius;

			for (int j = 0; j < ver; j++) {
				double angle = 2 * glm::pi<float>() * j / ver;

				float mxvtr = maxVertexOffset * radius;
				float radiusModifier = -mxvtr + (float)(rand()) / ((float)(RAND_MAX / (mxvtr + mxvtr))); //i must have been high when i wrote this lmao

				float tempRadius = radius + radiusModifier;

				vec3 point = vec3(tempRadius * cos(angle), (i * lHeight) + yModifier, tempRadius * sin(angle));
				points.push_back(point);

				if (i == lv - 2) {
					ind.push_back(j + noVxLvBw);
					ind.push_back(sumUp(lVxs,i));
				}

				ind.push_back(j + noVxLvBw);
				ind.push_back(j + noVxLvBw + 1);
			}
			ind.pop_back();
			ind.push_back(noVxLvBw);
		}
		points.push_back(vec3(0.0f, lv * lHeight, 0.0f));

		for (int i = 0; i < points.size(); i++) {
			vx.push_back(points[i].x);
			vx.push_back(points[i].y);
			vx.push_back(points[i].z);

			vx.push_back(0);
			vx.push_back(1);
			vx.push_back(0);
		}

		obstacles.push_back(game->Create(vec3(x, 0.0f, z), vec3(0.0f, rand() % 360, 0.0f), vec3(minScale + (float)(rand()) / ((float)(RAND_MAX / (maxScale - minScale)))), vx, ind));
	
		delete[] lVxs;
	}
}
using namespace Battlezone;

void Game::BattlezoneInit() {
	float shot_cool = 2;
	float resp_cool = 2;
	float zOffset = 0.0f;
	int xOffset = 0;

	player = Create(vec3(0.0f), vec3(0.0f), vec3(1.0f), "Tank");

	//bruv you have to clear the vectors here ↓ otherwise it wont work
	obstacles.clear();
}

void Game::Battlezone(float dt) {
	shot_cool -= dt;
	resp_cool -= dt;

	vec3 pPos = player->Transform.position;
	vec3 pOri = player->Transform.orientation;
	vec3 pFront = player->Front;

	//Poruszanie kamerą
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

	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		camSpeed = 2;
	else
		camSpeed = 1;

	// Poruszanie modelem
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		player->Move(vec3(0, 0, -1) * dt * velocity);
	}

	//no moving backwards in the orginal game
	/*if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
		gracz->Move(vec3(0, 0, 1) * dt * velocity);*/

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		player->Rotate(vec3(0, 1, 0) * dt * rotationMultiplier1);
		//todo: make camera rotation script
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		player->Rotate(vec3(0, -1, 0) * dt * rotationMultiplier1);
		//todo: make camera rotation script
	}

	//adjusting the cam's pos
	/*/vec3 cPos = normalize(pFront) * camFrontOffset;
	cPos.y += camYOffset;

	camera->Position = pPos+cPos;*/
	//todo: make camera rotation script

	//Strzelanie
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && shot_cool <= 0) {
		if (player->Transform.orientation.y != 0 && player->Transform.orientation.y != 180)
			shot(player->Transform.position + vec3(0, 2.535, 0), player->Transform.orientation, this);
		else
			shot(player->Transform.position + vec3(0, 2.535, 1), player->Transform.orientation, this);
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
		float temp_x = rand() % 51 -25;
		float temp_z = rand() % 51 -25;
		float temp_y = rand() % 361;
		spawn_tank(player->Transform.position + vec3(temp_x, 0, temp_z), vec3(0,temp_y,0), this);
		resp_cool = 2;
	}

	//Poruszanie przeciwników
	for (int i = 0; i < przeciwnicy.size(); i++) {
		GameObject* current = przeciwnicy[i];
		vec3 enemyPos = current->Transform.position;
		//vec3 playerPos = gracz->Transform.position;

		vec3 direction = normalize(pPos - enemyPos);

		current->MoveGlobal(direction * dt * tank_speed);

		//rotating the enemy
		float _angle;
		_angle = atan2(direction.x, direction.z);
		_angle = _angle * 180.0f / glm::pi<float>();

		current->RotateTo(vec3(0.0f, _angle, 0.0f));
	}

	//debug ↓
	if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS && resp_cool <= 0) {
		resp_cool = .1f;
		makeObstacles(rand() % 200 - 100, rand() % 200 - 100, 6, this);
	}

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		Game::ChangeState(Game_Menu);
}