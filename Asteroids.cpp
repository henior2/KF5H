#include "Game.h"

#define pi 3.14159265359

using namespace glm;

GameObject* ship;
std::vector<GameObject*> enemies;
std::vector<GameObject*> asteroids;
std::vector<GameObject*> bullets;
std::vector<GameObject*> stars;

const int camW = 160;
const int camH = 90;

int modelShipFire;

const float rotationMultiplier = 100.0;

const float maxVelocity = 25;
const float acceleration = 15;
const float deacceleration = 0.99;

vec2 velocity;
float speed;

const int jumpMargin = 20;
float jumpCooldown;

float shipAnimationCooldown;
float shipAnimationCooldown2;

float shootCooldown = .25f;

float velocityd;
float posx;
float posy;

std::vector<float> bulletTimeRemain;

const float bulletMaxTime = 3.0f;
const float bulletSpeed = 50.0f;

int _asteroidsNo;
int score;
int lives;

bool _return;

const int asteroidRadius = 10;
const int maxAsteroidsSidesNo = 14;
const int minAsteroidsSidesno = 7;

const float asteroidsVertexOffset = .5f;

const float bigAsteroidSize = 1.25f;
const float mediumAsteroidSize = .8f;
const float smallAsteroidSize = .45f;

const float asteroidSizeRange = .2f;

int asteroidSidesNo;

const float bigAsteroidVelocity = 7.5f;
const float mediumAsteroidVelocity = 12.5f;
const float smallAsteroidVelocity = 17.5f;

std::vector<unsigned int> asteroidSize;
std::vector<float> asteroidRotation;
std::vector<float> asteroidRotationMultiplier;

float maxAsteroidRotationMultiplier = 50.0f;

int bounds = 15;

bool hasWaveFinished;
float waveAsteroidsCooldown = 2.5f;

const int starsAmount = 100;
const float starsSpeedMultiplier = 2.5f;

float enemyProb;
const float enemyDeltaProb = .15f;
const float enemyMinDelay = 2.5f;
const float enemyMaxDelay = 5.0f;
float enemyDelay;

float enemySizes[] = { 5.0f, 7.5f };
std::vector<bool> enemyType;

const float maxEnemyVelocity = 10.0f;

const float maxEnemyBulletTime = 2.0f;
std::vector<float> enemyShootCooldown;
float _enemyShootCooldown[] = { 3.0f, 5.0f };
const float enemyShootCooldownRange = .2f;

const int maxBigEnemyMoves = 5;
std::vector<vec3> eBDPos;

int bigEnemyIterator;

void checkBounds(GameObject* current, vec2 bounds = vec2(170,95)) {
	if (current->Transform.position.y > bounds.y) current->MoveGlobal(vec3(0, -bounds.y * 2.0f , 0));
	if (current->Transform.position.y < -bounds.y) current->MoveGlobal(vec3(0, bounds.y * 2.0f, 0));
	if (current->Transform.position.x > bounds.x) current->MoveGlobal(vec3(-bounds.x * 2.0f, 0, 0));
	if (current->Transform.position.x < -bounds.x) current->MoveGlobal(vec3(bounds.x * 2.0f, 0, 0));
}

void spawnAsteroids(int asteroidsNum, unsigned int type, Game* game) {
	float minAsteroidsSize;
	float maxAsteroidsSize;

	for (int i = 0; i < asteroidsNum; i++) {
		std::vector<float> v;
		std::vector<unsigned int> id;
		std::vector<vec2> points;

		asteroidSidesNo = rand() % (maxAsteroidsSidesNo - minAsteroidsSidesno) + minAsteroidsSidesno;

		for (int i = 0; i < asteroidSidesNo; ++i) {
			double angle = 2 * pi * i / asteroidSidesNo;
			double radiusModifier = (rand() / (double)RAND_MAX) * 2 * asteroidRadius * asteroidsVertexOffset - asteroidRadius * asteroidsVertexOffset;
			double modifiedRadius = asteroidRadius + radiusModifier;

			vec2 vertex = { modifiedRadius * cos(angle), modifiedRadius * sin(angle) };
			points.push_back(vertex);
		}

		for(int i=0; i<points.size(); i++){
			v.push_back(points[i].x);
			v.push_back(points[i].y);
			v.push_back(0);
			v.push_back(1);
			v.push_back(1);
			v.push_back(1);
		}

		for (int i = 1; i < asteroidSidesNo; i++) {
			id.push_back(i - 1);
			id.push_back(i);
		}
		id.push_back(asteroidSidesNo -1);
		id.push_back(0);

		switch (type)
		{
		case 0:
			minAsteroidsSize = bigAsteroidSize - bigAsteroidSize * asteroidSizeRange;
			maxAsteroidsSize = bigAsteroidSize + bigAsteroidSize * asteroidSizeRange;
			break;
		case 1:
			minAsteroidsSize = mediumAsteroidSize - mediumAsteroidSize * asteroidSizeRange;
			maxAsteroidsSize = mediumAsteroidSize + mediumAsteroidSize * asteroidSizeRange;
			break;
		case 2:
			minAsteroidsSize = smallAsteroidSize - smallAsteroidSize * asteroidSizeRange;
			maxAsteroidsSize = smallAsteroidSize + smallAsteroidSize * asteroidSizeRange;
			break;
		default:
			throw std::invalid_argument("nuh uh");
			break;
		}

		float rot = (float) (rand()) / ((float) (RAND_MAX / 360.0f));
		asteroidRotation.push_back(rot * pi / 180.0f);

		float rotM = -maxAsteroidRotationMultiplier + (float)(rand()) / ((float)(RAND_MAX / (maxAsteroidRotationMultiplier - (-maxAsteroidRotationMultiplier))));
		asteroidRotationMultiplier.push_back(rotM);

		vec2 pos;
		int temp;

		do {
			temp = rand() % (2 * (camW + bounds)) - (camW + bounds);
			pos.x = temp;

			temp = rand() % (2 * (camH + bounds)) - (camH + bounds);
			pos.y = temp;
		} while (pos.x > -camW - 15 && pos.x < camW + 15 && pos.y > -camH - 15 && pos.y < camH + 15);

		asteroids.push_back(game->Create(vec3(pos, -90.0f), vec3(0.0f,0.0f,rot), vec3(minAsteroidsSize + (float) (rand()) / ((float) (RAND_MAX / (maxAsteroidsSize - minAsteroidsSize)))), v, id));
		asteroidSize.push_back(type);
	}
}

void spawnEnemy(bool type, Game* game) {
	vec2 pos;
	int temp;

	do {
		temp = rand() % (2 * (camW + bounds) - (camW + bounds));
		pos.x = temp;

		temp = rand() % (2 * (camW + bounds) - (camW + bounds));
		pos.y = temp;
	} while (pos.x > -camW - bounds && pos.x < camW + bounds && pos.y > -camH - bounds && pos.y < camH + bounds);

	enemies.push_back(game->Create(vec3(pos, -75.0f), vec3(0.0f), vec3(enemySizes[(int)type]), "AsteroidsEnemy"));
	enemyType.push_back(type);
	enemyShootCooldown.push_back((float)((rand() % (int)(2 * enemyShootCooldownRange * 100))/100 - enemyShootCooldownRange + _enemyShootCooldown[(int)type]));
	if (type) eBDPos.push_back(vec3(rand() % (2 * (camW - bounds)) - (camW + bounds), rand() % (2 * (camH - bounds)) - (camH + bounds), rand() % maxBigEnemyMoves + 1));
}

void shoot(vec3 _pos, vec3 _rot, bool type, Game* game, bool eType = 0) {
	float _time;
	float _scale;
	std::string _model;
	float _offset;

	if (!type) {
		_time = bulletMaxTime;
		_scale = 2.5f;
		_model = "AsteroidsBullet";
		_offset = 6.25f;
	}
	else {
		_time = maxEnemyBulletTime;
		_scale = 1.75f;
		_model = "AsteroidsEnemyBullet";
		_offset = enemySizes[(int)eType] / 2;
	}

	bulletTimeRemain.push_back(_time);
	GameObject* bullet = game->Create(_pos, _rot, vec3(_scale), _model);
	bullets.push_back(bullet);
	bullet->Move(vec3(0.0f, _offset, 0.0f));
}

bool wave(int asteroidsNum, Game* game) {
	spawnAsteroids(asteroidsNum, 0, game);

	int temp = 0;
	while (rand() % 100 <= enemyProb)
		temp += 1;
	return temp;
}

void Game::AsteroidsInit() {
	ship = Create(vec3(0.0f, 0.0f, -99.0f), vec3(0.0f), vec3(5.0f), "AsteroidsShip");
	modelShipFire = ship->AddStage("AsteroidsShipFire");

	velocity = vec2(0.0f);
	speed = 0;
	velocityd = maxVelocity * maxVelocity;

	posx = posy = 0.0f;

	jumpCooldown = 0.5f;
	shipAnimationCooldown = (rand() % 4)/2 + 1;
	shipAnimationCooldown2 = (rand() % 2) / 2 + 0.25;
	shootCooldown = .1f;

	camera->perspective = false;
	camera->cameraHeight = camH;
	camera->cameraWidth = camW;

	_asteroidsNo = 4;
	score = 0;
	lives = 3;

	_return = 0;

	hasWaveFinished = false;
	waveAsteroidsCooldown = 2.5f;

	enemyProb = 15.0f;
	enemyDelay = rand() % (int)(enemyMaxDelay - enemyMinDelay) + enemyMinDelay;

	bigEnemyIterator = 0;

	asteroids.clear();
	enemies.clear();
	bullets.clear();
	stars.clear();

	bulletTimeRemain.clear();
	asteroidSize.clear();
	asteroidRotation.clear();
	asteroidRotationMultiplier.clear();
	enemyType.clear();
	enemyShootCooldown.clear();
	eBDPos.clear();

	for (int i = 0; i < starsAmount; i++) {
		stars.push_back(Create(vec3(rand() % 320 - 160, rand() % 180 - 90, -99.999f), vec3(0.0f, 0.0f, rand() % 45), vec3(.01f), "AsteroidsStar"));
	}

	PlaySound2d("mus01.mp3", true);
}

void Game::Asteroids(float dt) {
	jumpCooldown -= dt;
	shootCooldown -= dt;

	bigEnemyIterator = 0;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		vec2 shipUp = ship->Up;

		velocity += acceleration * dt * vec2(shipUp.x, shipUp.y);
		speed = velocity.x * velocity.x + velocity.y * velocity.y;
		if (speed > velocityd) velocity *= velocityd / speed;
		ship->MoveGlobal(vec3(velocity.x * dt, velocity.y * dt, 0));

		ship->activeStage = modelShipFire;
		
		shipAnimationCooldown -= dt;
		if (shipAnimationCooldown <= 0) {
			ship->activeStage = 0;

			shipAnimationCooldown2 -= dt;
			if (shipAnimationCooldown2 <= 0) {
				shipAnimationCooldown = (rand() % 4) / 2 + 1;
				shipAnimationCooldown2 = (rand() % 2) / 2 + 0.25;
			}
		}
		
		for (int i = 0; i < starsAmount; i++) {
			stars[i]->MoveGlobal(vec3(shipUp.x * -starsSpeedMultiplier, shipUp.y * -starsSpeedMultiplier, 0) * dt);
			checkBounds(stars[i], vec2(160, 90));
		}
	}
	else {
		velocity -= velocity * deacceleration * dt;
		speed = velocity.x * velocity.x + velocity.y * velocity.y;
		ship->MoveGlobal(vec3(velocity.x * dt, velocity.y * dt, 0));

		ship->activeStage = 0;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		ship->Rotate(vec3(0, 0, 1.0f) * rotationMultiplier * dt);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		ship->Rotate(vec3(0, 0, -1.0f) * rotationMultiplier * dt);
	}

	//debug - dont touch please :)
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS && jumpCooldown <= 0.0f) {
		jumpCooldown = 0.5f;
		hasWaveFinished = true;
	}
	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS && jumpCooldown <= 0.0f) {
		jumpCooldown = 0.5f;
		spawnAsteroids(5, 2, this);
	}
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS && jumpCooldown <= 0.0f) {
		jumpCooldown = 0.5f;
		spawnAsteroids(5, 1, this);
	}
	if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS && jumpCooldown <= 0.0f) {
		jumpCooldown = .5f;
		spawnEnemy(0, this);
	}
	if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS && jumpCooldown <= 0.0f) {
		jumpCooldown = .5f;
		spawnEnemy(1, this);
	}
	//end of debug :)

	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS && jumpCooldown <= 0.0f) {
		jumpCooldown = 0.5f;

		int random = rand() % 32 - 1;
		if (random >= 24 && random <= 31) {
			//niepowodzenie - smierc
			Game::ChangeState(Game_Menu);
		}
		else {
			random = rand() % 8 - 1;
			random = (random*2)+4;

			if (random < asteroids.size()) {
				//niepowodzenie - smierc
				Game::ChangeState(Game_Menu);
			}
			else {
				ship->MoveTo(vec3(rand() % (160 - jumpMargin) * 2 - 160 - jumpMargin, rand() % (90 - jumpMargin) * 2 - 90 - jumpMargin, -80));
				velocity = vec2(0.0f, 0.0f);
			}
		}
	}

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && shootCooldown<=0) {
		shootCooldown = .25f;
		shoot(ship->Transform.position, ship->Transform.orientation, 0, this);
	}

	for (int i = 0; i < enemies.size(); i++) {
		GameObject* current = enemies[i];
		bool type = enemyType[i];

		vec2 sPos;

		vec2 pos = vec2(current->Transform.position.x, current->Transform.position.y);

		if (!type) sPos = vec2(ship->Transform.position.x, ship->Transform.position.y);
		else {
			sPos = vec2(eBDPos[bigEnemyIterator].x, eBDPos[bigEnemyIterator].y);
			eBDPos[bigEnemyIterator].z--;
		}

		if (sPos.x + 5 > pos.x && sPos.x - 5 < pos.x && sPos.y + 5 > pos.y && sPos.y - 5 < pos.y) {
			if (eBDPos[bigEnemyIterator].z > 0) {
				eBDPos[bigEnemyIterator] = vec3(rand() * (2 * (camW + bounds)) - (camW + bounds), rand() % (2 * (camH + bounds)) - (camH + bounds), eBDPos[bigEnemyIterator].z);
			}
			else {
				eBDPos[bigEnemyIterator] = vec3(rand() % (2 * camW) - camW, -2 * camH - bounds, eBDPos[bigEnemyIterator].z);
			}
		}

		vec2 dMov = vec2(sPos.x - pos.x, sPos.y - pos.y);
		
		if (dMov.x > maxEnemyVelocity) dMov.x = maxEnemyVelocity;
		else if (dMov.x < -maxEnemyVelocity) dMov.x = -maxEnemyVelocity;

		if (dMov.y > maxEnemyVelocity) dMov.y = maxEnemyVelocity;
		else if (dMov.y < -maxEnemyVelocity) dMov.y = -maxEnemyVelocity;

		current->MoveGlobal(vec3(dMov, 0.0f)*dt);

		float _angle;

		enemyShootCooldown[i] -= dt;
		if (enemyShootCooldown[i] <= 0) {
			if (!type) {
				enemyShootCooldown[i] = (float)((rand() % (int)(2 * enemyShootCooldownRange * 100)) / 100 - enemyShootCooldownRange + _enemyShootCooldown[(int)type]);

				_angle = atan2(sPos.y - pos.y, sPos.x - pos.x);
				_angle = _angle * 180.0f / pi - 90.0f;
			}
			else _angle = rand() % 360;

			shoot(current->Transform.position, vec3(0.0f, 0.0f, _angle), 1, this, type);
			enemyShootCooldown[i] = _enemyShootCooldown[(int)type];
		}

		if (pos.y <= -camH - bounds && eBDPos[bigEnemyIterator].z <= 0) {
			Destroy(current);
			enemies.erase(enemies.begin() + i);
			enemyType.erase(enemyType.begin() + i);
			eBDPos.erase(eBDPos.begin() + bigEnemyIterator);
			enemyShootCooldown.erase(enemyShootCooldown.begin() + i);
			
			bigEnemyIterator--; i--;
		}
		bigEnemyIterator++;
	}

	for (int i = 0; i < bullets.size(); i++) {
		GameObject* current = bullets[i];

		bulletTimeRemain[i] -= dt;
		if (bulletTimeRemain[i] <= 0) {
			Destroy(current);
			bullets.erase(bullets.begin() + i);
			bulletTimeRemain.erase(bulletTimeRemain.begin() + i);
			i--;
			continue;
		}
		current->Move(vec3(0.0f,1.0f,0.0f) * bulletSpeed * dt);

		checkBounds(current);
	}

	for (int i = 0; i < asteroids.size(); i++) {
		GameObject* current = asteroids[i];

		float _velocity;
		switch (asteroidSize[i]) {
		case 0:
			_velocity = bigAsteroidVelocity;
			break;
		case 1:
			_velocity = mediumAsteroidVelocity;
			break;
		case 2:
			_velocity = smallAsteroidVelocity;
			break;
		default:
			throw std::invalid_argument("how did you manage to mess up this bad lmao?");
			break;
		}

		float deg = asteroidRotation[i];
		
		current->Rotate(vec3(0.0f, 0.0f, asteroidRotationMultiplier[i]) * dt);
		current->MoveGlobal(vec3(cos(deg), sin(deg), 0.0f) * _velocity* dt);

		checkBounds(current,vec2(camera->cameraWidth+ bounds,camera->cameraHeight+ bounds));
	}

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		Game::ChangeState(Game_Menu);

	checkBounds(ship);

	if (asteroids.size() == 0) hasWaveFinished = true;

	if (hasWaveFinished) {
		waveAsteroidsCooldown -= dt;

		if (waveAsteroidsCooldown <= 0) {
			waveAsteroidsCooldown = 2.5f;
			hasWaveFinished = false;
			_return = wave(_asteroidsNo, this);
			if (_asteroidsNo <= 9) _asteroidsNo += 2;
			else _asteroidsNo = 11;
		}
	}

	if (_return != 0) {
		enemyDelay -= dt;
		if (enemyDelay <= 0) {
			for (int i = 0; i < _return; i++) 
				spawnEnemy(0,this);

			enemyDelay = rand() % (int)(enemyMaxDelay - enemyMinDelay) - enemyMinDelay;
			enemyProb += enemyProb * enemyDeltaProb;
			_return = 0;
		}
	}
}