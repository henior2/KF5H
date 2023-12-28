#include "Game.h"
#include <time.h>

using namespace glm;

namespace Battlezone {
	GameObject* player;
	GameObject* model2;

	GameObject* plane;

	const float rotationMultiplier = -500.0f;
	int camSpeed = 1;
	float const velocity = 3.0f;
	float const rotationMultiplier1 = 35;
	std::vector<GameObject*> pociski;
	std::vector<GameObject*> przeciwnicy;
	float shot_cool = 2;
	float resp_cool = 2;
	const float fast_tank_speed = 4;
	const float tank_speed = 2;

	std::vector<float> fastBulletTimeRemain;
	std::vector<int> enemyType;

	const float bulletMaxTime = 4.0f;
	const float bulletSpeed = 28.0f;

	void shot_fast(vec3 pos, vec3 rot, Game* Gra) {
		fastBulletTimeRemain.push_back(bulletMaxTime);
		GameObject* bullet = Gra->Create(pos, rot, vec3(1.0f), "FastBullet");
		pociski.push_back(bullet);
		bullet->Move(vec3(0, 0, -1));
		shot_cool = 2;
	}

	void shot(vec3 pos, vec3 rot, Game* Gra) {
		fastBulletTimeRemain.push_back(bulletMaxTime);
		GameObject* bullet = Gra->Create(pos, rot, vec3(1.0f), "TankBullet");
		pociski.push_back(bullet);
		bullet->Move(vec3(0, 0, -1));
		shot_cool = 2;
	}

	void spawn_enemy(vec3 pos, vec3 rot, Game* Gra, int type) {
		if (type==1) {
			GameObject* enemy = Gra->Create(pos, rot, vec3(1.0f), "Tank");
			przeciwnicy.push_back(enemy);
			enemyType.push_back(type);
		}
		else if (type==2) {
			GameObject* enemy = Gra->Create(pos, rot, vec3(2.0f), "FastTank");
			przeciwnicy.push_back(enemy);
			enemyType.push_back(type);
		}
		else if (type == 3) {
			GameObject* enemy = Gra->Create(pos, rot, vec3(1.0f), "LeonardoTank");
			przeciwnicy.push_back(enemy);
			enemyType.push_back(type);
		}
	}

	const float camFrontOffset = -2.5f;
	const float camYOffset = 1.75f;

	std::vector<GameObject*> enemiesVector;
	std::vector<GameObject*> objectsVector;
	std::vector<GameObject*> obstacles;

	const int minBaseVerticies = 3;
	const int maxBaseVerticies = 9;
	const float maxVertexOffset = .15f;
	const int minLevels = 3;
	const int maxLevels = 6;
	const float backConnChance = .2f;
	const float levelMaxYOffset = .25f;
	const float minRadius = 3.5f;
	const float maxRadius = 5;
	const float minLevelRadiusDecrease = -.1f;
	const float maxLevelRadiusDecrease = .5f;
	const float minScale = .8f;
	const float maxScale = 1.2f;

	const float planeHeight = 30.0f;
	const float planeSpeedMultiplier = 10.0f;
	float planeCooldown = 7.5f;
	bool isPlane = false;
	vec2 planeStartCoords = vec2(-1000,-1000);
	const float planeBounds = 100.0f;

	void makeObstacles(float x, float z, float height) {
		std::vector<float> vx;
		std::vector<unsigned int> ind;

		std::vector<vec3> points;

		int lv = rand() % (maxLevels - minLevels) + minLevels;

		float lHeight = height / lv;

		int ver = rand() % (maxBaseVerticies - minBaseVerticies) + minBaseVerticies;
		float radius = minRadius + (float)(rand()) / ((float)(RAND_MAX / (maxRadius - minRadius)));

		for (int i = 0; i < lv-1; i++) {
			int noVxLvBw = i * ver;

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

				//last layer => topmost vertex
				if (i == lv - 2) {
					ind.push_back(j + noVxLvBw);
					ind.push_back((lv-1)*ver);
				}

				//every layer before => layer above
				if (i < lv - 2) {
					ind.push_back(j + noVxLvBw);

					int nextVx = j + noVxLvBw + ver;

					ind.push_back(nextVx);

					if (rand() % 100 < backConnChance * 100) {
						ind.push_back(j + noVxLvBw);
						int _rand = rand() % 2;
						if (_rand && --nextVx < 0) nextVx = 2;
						else if (!_rand && ++nextVx > (lv - 1) * ver) nextVx = (lv - 1) * ver - 3;
						ind.push_back(nextVx);
					}
				}


				//every vertex in any given layer => vertex next to
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

		obstacles.push_back(Gra->Create(vec3(x, 0.0f, z), vec3(0.0f, rand() % 360, 0.0f), vec3(minScale + (float)(rand()) / ((float)(RAND_MAX / (maxScale - minScale)))), vx, ind));
	
		//delete[] lVxs;
	}
}
using namespace Battlezone;

void Game::BattlezoneInit() {
	shot_cool = 2;
	resp_cool = 2;

	planeCooldown = 10.0f;
	isPlane = false;
	planeStartCoords = vec2(-1000, -1000);

	player = Create(vec3(0.0f), vec3(0.0f), vec3(1.0f), "Tank");
	plane = Create(vec3(-1000.0f, 1000, -1000.0f), vec3(0.0f), vec3(1.0f), "BattlezonePlane");

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
	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		player->Move(vec3(0, 0, -1) * dt * velocity);
	}


	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		player->Rotate(vec3(0, 1, 0) * dt * rotationMultiplier1);
	}
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		player->Rotate(vec3(0, -1, 0) * dt * rotationMultiplier1);
	}

	//adjusting the cam's pos
	/*vec3 cPos = normalize(pFront) * camFrontOffset;
	cPos.y += camYOffset;

	camera->Position = pPos+cPos;*/
	//todo: make a WORKING cam rot script

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
	float temp_x = rand() % 51 - 25;
	float temp_z = rand() % 51 - 25;
	float temp_y = rand() % 361;

	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS && resp_cool <= 0) {
		spawn_enemy(player->Transform.position + vec3(temp_x, 0, temp_z), vec3(0,temp_y,0), this,1);
		resp_cool = 2;
	}
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS && resp_cool <= 0) {
		spawn_enemy(player->Transform.position + vec3(temp_x, 0, temp_z), vec3(0, temp_y, 0), this, 2);
		resp_cool = 2;
	}
	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS && resp_cool <= 0) {
		spawn_enemy(player->Transform.position + vec3(temp_x, 0, temp_z), vec3(0, temp_y, 0), this, 3);
		resp_cool = 2;
	}

	//Poruszanie przeciwników
	for (int i = 0; i < przeciwnicy.size(); i++) {
		GameObject* current = przeciwnicy[i];
		vec3 enemyPos = current->Transform.position;
		vec3 direction = normalize(pPos - enemyPos);
		vec3 distance = pPos - enemyPos;
		if (int temp = distance.x * distance.x + distance.z * distance.z > 225) {
			if (enemyType[i] == 1 || enemyType[i] == 3)
				current->MoveGlobal(direction * dt * tank_speed);
			else if (enemyType[i] == 2) {
				current->MoveGlobal(direction * dt * fast_tank_speed);
				if (shot_cool <= 0) {
					shot_fast(current->Transform.position + vec3(0, 1.06, 0), current->Transform.orientation, this);
					shot_cool = 2.0;
				}
			}
		}
		else {
			if (enemyType[i] == 1 || enemyType[i] == 3) {
				if (shot_cool <= 0) {
					if (current->Transform.orientation.y != 0 && current->Transform.orientation.y != 180)
						shot(current->Transform.position + vec3(0, 2.535, 0), current->Transform.orientation, this);
					else
						shot(current->Transform.position + vec3(0, 2.535, 1), current->Transform.orientation, this);
				}
				}
			else if (enemyType[i] == 2) {
				if (shot_cool <= 0) {
					shot_fast(current->Transform.position + vec3(0, 1.06, 0), current->Transform.orientation, this);
					shot_cool = 2.0;
				}
			}
			else
				throw std::invalid_argument("co tu zawiodło xD"); //bro's stealing goofy errors 💀
		}
	
		

		//Obracanie przeciwników
		float _angle;
		_angle = atan2(direction.x, direction.z);
		_angle = _angle * 180.0f / glm::pi<float>();

		current->RotateTo(vec3(0.0f, _angle, 0.0f));
	}

	//Poruszanie samolotu
	if (!isPlane) planeCooldown -= dt;
	if (planeCooldown <= 0) {
		planeStartCoords.x = planeBounds;
		if (rand() % 2) planeStartCoords.x *= -1;
		planeStartCoords.y = rand() % (int)(2*planeBounds) - planeBounds;

		if (rand() % 2) {
			float temp = planeStartCoords.x;
			planeStartCoords.x = planeStartCoords.y;
			planeStartCoords.y = temp;
		}

		plane->MoveTo(vec3(planeStartCoords.x, planeHeight, planeStartCoords.y));

		float _angle;
		vec2 direction = normalize(vec2(pPos.x, pPos.z) - planeStartCoords);
		_angle = atan2(direction.x, direction.y);
		_angle = _angle * 180.0f / glm::pi<float>();
		plane->RotateTo(vec3(0.0f, _angle, 0.0f));

		isPlane = true;
		planeCooldown = 7.5f;
	}
	if (isPlane) {
		plane->Move(plane->Front * dt * planeSpeedMultiplier);

		vec2 planePos = vec2(plane->Transform.position.x, plane->Transform.position.z);
		if (abs(planePos.x) > planeBounds * 1.25 || abs(planePos.y) > planeBounds * 1.25) { 
			plane->MoveTo(vec3(-1000, 1000, -1000));
			isPlane = false; 
		}
	}

	//debug ↓
	if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS && resp_cool <= 0) {
		resp_cool = .1f;
		makeObstacles(rand() % 200 - 100, rand() % 200 - 100, 6);
	}
	if (glfwGetKey(window, GLFW_KEY_KP_0) == GLFW_PRESS && resp_cool <= 0) {
		resp_cool = .1f;
		camera->Position = vec3(0, planeHeight, 0);
	}

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		Game::ChangeState(Game_Menu);
}