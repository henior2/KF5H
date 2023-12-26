#include "Game.h"
#include <time.h>

using namespace glm;

namespace Battlezone {
	GameObject* model;
	GameObject* model2;

	float rotationMultiplier = -500.0f;
	int camSpeed = 1;
	float const velocity = 3.0f;
	float const rotationMultiplier1 = 35;
	std::vector<GameObject*> pociski;
	std::vector<GameObject*> przeciwnicy;
	float shot_cool = 2;
	float resp_cool = 2;
	float fast_tank_speed = 4;
	float tank_speed = 2;




	std::vector<float> fastBulletTimeRemain;
	std::vector<int> enemyType;

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

	void spawn_enemy(vec3 pos, vec3 rot, Game* game, int type) {
		if (type==1) {
			GameObject* enemy = game->Create(pos, rot, vec3(1.0f), "Tank");
			przeciwnicy.push_back(enemy);
			enemyType.push_back(type);
		}
		else if (type==2) {
			GameObject* enemy = game->Create(pos, rot, vec3(2.0f), "FastTank");
			przeciwnicy.push_back(enemy);
			enemyType.push_back(type);
		}
	}



	std::vector<std::string> objects = {
		"Tank",
		""
	};

	std::vector<GameObject*> enemiesVector;

	std::vector<GameObject*> objectsVector;

	std::vector<float> groundUnitsVx;
	std::vector<unsigned int> groundUnitsInd;
	GameObject* ground;

	const int mapUnitSize = 10;
	const int mapSize = 15; //there will be units in between orginal units, resulting in the map being 2x larger (!!!) - consider this
	const float mapHeightChangeProb = .05f;
	const float stepHeihgt = .5f;

	const vec2 startP = vec2(0, 0);
}

using namespace Battlezone;

void setLevel(int x, int y, int ox, int oy, int lv[mapSize][mapSize], bool collapsed[mapSize][mapSize]) {
	int step = 0;
	if (rand() % (int)(mapHeightChangeProb*100)) {
		switch (rand()%2)
		{
		case 0:
			step = 1;
			break;
		default:
			step = -1;
			break;
		}
	}
	lv[x][y] = lv[ox][oy] + step;
}

int collapse(int x, int y, int lv[mapSize][mapSize], bool collapsed[mapSize][mapSize], int ic[mapSize], int& dc) {
	int cnum = 0;

	if (x > 0 && !collapsed[x - 1][y]) {
		setLevel(x - 1, y, x,y, lv, collapsed);
		ic[x-1]++;
		dc--;
		cnum++;
		collapsed[x - 1][y] = true;
	}
	if (x < mapSize && !collapsed[x + 1][y]) {
		setLevel(x + 1, y, x,y, lv, collapsed);
		ic[x+1]++;
		dc--;
		cnum++;
		collapsed[x + 1][y] = true;
	}
	if (y > 0 && !collapsed[x][y - 1]) {
		setLevel(x, y - 1, x, y, lv, collapsed);
		ic[x]++;
		dc--;
		cnum++;
		collapsed[x][y - 1] = true;
	}
	if (y < mapSize && !collapsed[x][y + 1]) {
		setLevel(x, y + 1, x, y, lv, collapsed);
		ic[x]++;
		dc--;
		cnum++;
		collapsed[x][y + 1] = true;
	}
	
	return cnum;
}

void generateGround(int mUS, int mS, float mHV, vec2 sP, Game* gra) {
	//ive just now realised, that ive been calling the 'z' axis 'y' all along, but im too lazy to rename the variables, so just dont get confused lmao 

	int level[mapSize][mapSize];
	bool collapsed[mapSize][mapSize];

	int rowSums[mapSize];
	int left = mS * mS;

	//collapse middle point
	level[mapSize / 2 - 1][mapSize / 2 - 1] = 0;

	//deleting the garbage (i think?)
	for (int i = 0; i < mapSize; i++) {
		rowSums[i] = 0;
		for (int j = 0; j < mapSize; j++) {
			level[i][j] = -100;
			collapsed[i][j] = false;
		}
	}
	collapsed[mapSize / 2 - 1][mapSize / 2 - 1] = true;
	rowSums[mapSize / 2 - 1]++;
	level[mapSize / 2 - 1][mapSize / 2 - 1] = 0;
	left--;

	//find an un-collapsed point neighbouring a collapsed point
	//(going b-t,l-r rn, might be changed at some point later to be more random ig)
	while (left > 0) {
		for (int i = 0; i < mapSize; i++) {
			if (rowSums[i] > 0) {
				for (int j = 0; j < mapSize; j++) {
					if (collapsed[i][j]) {
						left -= collapse(i, j, level, collapsed, rowSums, left);
					}
				}
			}
		}
	}

	//making vx
	int sx = sP.x - mapSize/2 * mUS;
	int sy = sP.y - mapSize/2 * mUS;

	for (int i = 0; i < mapSize; i++) {
		for (int k = 0; k < 2; k++) {
			for (int j = 0; j < mapSize; j++) {
				groundUnitsVx.push_back(sx + j * mUS);
				groundUnitsVx.push_back(0 + level[i][j] * stepHeihgt);
				groundUnitsVx.push_back(sy + i * mUS * 2 + k * mUS);

				groundUnitsVx.push_back(0);
				groundUnitsVx.push_back(1);
				groundUnitsVx.push_back(0);
			}
		}
	}

	//making ind
	for (int i = 0; i < mapSize*2; i++) {
		for (int j = 1; j < mapSize; j++) {
			groundUnitsInd.push_back(j - 1 + i*mapSize);
			groundUnitsInd.push_back(j + i*mapSize);
		}
	}

	for (int i = 0; i < mapSize*(mapSize*2-1); i++) {
		groundUnitsInd.push_back(i);
		groundUnitsInd.push_back(i+mapSize);
	}

	//creating the ground GameObject
	ground = gra->Create(vec3(sx,0.0f,sy), vec3(0.0f), vec3(1.0f), groundUnitsVx, groundUnitsInd);
}

void Game::BattlezoneInit() {
	float shot_cool = 2;
	float resp_cool = 2;
	float zOffset = 0.0f;
	int xOffset = 0;

	//wtf is that
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

	groundUnitsVx.clear();
	groundUnitsInd.clear();

	generateGround(mapUnitSize,mapSize,mapHeightChangeProb,startP,this);
}

void Game::Battlezone(float dt) {
	shot_cool -= dt;
	resp_cool -= dt;
	GameObject* gracz = objectsVector[0];
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
		gracz->Move(vec3(0, 0, -1) * dt * velocity);
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
		gracz->Move(vec3(0, 0, 1) * dt * velocity);

	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
		gracz->Rotate(vec3(0, 1, 0) * dt * rotationMultiplier1);
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
		gracz->Rotate(vec3(0, -1, 0) * dt * rotationMultiplier1);

	//Strzelanie
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && shot_cool <= 0) {
		if (gracz->Transform.orientation.y != 0 && gracz->Transform.orientation.y != 180)
			shot(gracz->Transform.position + vec3(0, 2.535, 0), gracz->Transform.orientation, this);
		else
			shot(gracz->Transform.position + vec3(0, 2.535, 1), gracz->Transform.orientation, this);
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
		spawn_enemy(gracz->Transform.position + vec3(temp_x, 0, temp_z), vec3(0,temp_y,0), this,1);
		resp_cool = 2;
	}
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS && resp_cool <= 0) {
		srand(time(NULL));
		float temp_x = rand() % 51 - 25;
		float temp_z = rand() % 51 - 25;
		float temp_y = rand() % 361;
		spawn_enemy(gracz->Transform.position + vec3(temp_x, 0, temp_z), vec3(0, temp_y, 0), this, 2);
		resp_cool = 2;
	}

	//Poruszanie przeciwników
	for (int i = 0; i < przeciwnicy.size(); i++) {
		GameObject* current = przeciwnicy[i];
		vec3 enemyPos = current->Transform.position;
		vec3 playerPos = gracz->Transform.position;
		vec3 direction = normalize(playerPos - enemyPos);
		if(enemyType[i]==1)
			current->MoveGlobal(direction * dt * tank_speed);
		else if(enemyType[i]==2)
			current->MoveGlobal(direction * dt * fast_tank_speed);

		//rotating the enemy
		float _angle;
		_angle = atan2(direction.x, direction.z);
		_angle = _angle * 180.0f / glm::pi<float>();

		current->RotateTo(vec3(0.0f, _angle, 0.0f));
	}

}