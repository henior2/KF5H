#include "Game.h"
#include <algorithm>

#define pi 3.14159265359

using namespace glm;

GameObject* ship;
std::vector<GameObject*> enemies;
std::vector<GameObject*> asteroids;
std::vector<GameObject*> bullets;

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

float bposx;
float bposy;

std::vector<float> bulletTimeRemain;

const float bulletMaxTime = 3.0f;
const float bulletSpeed = 50.0f;

int _asteroidsNo;
int score;

const int asteroidRadius = 10;
const int asteroidSide = 4;
const int asteroidSidesNo = 8;

bool hasWaveFinished;

void spawnAsteroids(int asteroidsNum, Game* game) {
	for (int i = 0; i < asteroidsNum; i++) {
		std::vector<float> v;
		std::vector<unsigned int> id;
		std::vector<vec2> points;

		double R = asteroidSide / (2 * sin(pi / asteroidSidesNo));

		for (int i = 0; i < asteroidSidesNo; ++i) {
			double angle = 2 * pi * i / asteroidSidesNo;
			vec2 vertex = { R * cos(angle), R * sin(angle) };
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

		asteroids.push_back(game->Create(vec3(rand()%320-160, rand()%180-90, -99.0f), vec3(0.0f), vec3(5.0f), v, id));
	}
}

void shoot(vec3 _pos, vec3 _rot, Game* game) {
	bulletTimeRemain.push_back(bulletMaxTime);
	GameObject* bullet = game->Create(_pos, _rot, vec3(2.5f), "AsteroidsBullet");
	bullets.push_back(bullet);
	bullet->Move(vec3(0.0f, 6.25f, 0.0f));
}

void wave(int asteroidsNum, Game* game) {
	spawnAsteroids(asteroidsNum, game);
}

void Game::AsteroidsInit() {
	ship = Create(vec3(0.0f, 0.0f, -99.0f), vec3(0.0f), vec3(5.0f), "AsteroidsShip");
	modelShipFire = ship->AddStage("AsteroidsShipFire");

	velocity = vec2(0.0f);
	speed = 0;
	velocityd = maxVelocity * maxVelocity;

	posx = posy = 0.0f;
	bposx = bposy = 0.0f;

	jumpCooldown = 0.5f;
	shipAnimationCooldown = (rand() % 4)/2 + 1;
	shipAnimationCooldown2 = (rand() % 2) / 2 + 0.25;
	shootCooldown = .1f;

	camera->perspective = false;
	camera->cameraHeight = 90;
	camera->cameraWidth = 160;

	_asteroidsNo = 4;
	score = 0;

	hasWaveFinished = false;

	asteroids.clear();
	enemies.clear();
	bullets.clear();

	bulletTimeRemain.clear();

	PlaySound2d("mus01.mp3", true);
}

void Game::Asteroids(float dt) {
	jumpCooldown -= dt;
	shootCooldown -= dt;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		velocity += acceleration * dt * vec2(ship->Up.x, ship->Up.y);
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

	//todo: add valid condition later
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS && jumpCooldown <= 0.0f) {
		jumpCooldown = 0.5f;
		hasWaveFinished = true;
	}

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
				//todo: dodac sprawdzenie, czy nie ma tam asteroidy
				ship->MoveTo(vec3(rand() % (160 - jumpMargin) * 2 - 160 - jumpMargin, rand() % (90 - jumpMargin) * 2 - 90 - jumpMargin, -80));
				velocity = vec2(0.0f, 0.0f);
			}
		}
	}

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && shootCooldown<=0) {
		shootCooldown = .25f;
		shoot(ship->Transform.position, ship->Transform.orientation, this);
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

		bposx = current->Transform.position.x;
		bposy = current->Transform.position.y;
		if (current->Transform.position.y > 100) current->MoveGlobal(vec3(0, -190, 0));
		if (current->Transform.position.y < -100) current->MoveGlobal(vec3(0, 190, 0));
		if (current->Transform.position.x > 170) current->MoveGlobal(vec3(-330, 0, 0));
		if (current->Transform.position.x < -170) current->MoveGlobal(vec3(330, 0, 0));
	}

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		Game::ChangeState(Game_Menu);

	posx = ship->Transform.position.x;
	posy = ship->Transform.position.y;
	if (ship->Transform.position.y > 100) ship->MoveGlobal(vec3(0, -190, 0));
	if (ship->Transform.position.y < -100) ship->MoveGlobal(vec3(0, 190, 0));
	if (ship->Transform.position.x > 170) ship->MoveGlobal(vec3(-330, 0, 0));
	if (ship->Transform.position.x < -170) ship->MoveGlobal(vec3(330, 0, 0));

	if (asteroids.size() == 0) hasWaveFinished = true;

	if (hasWaveFinished) {
		hasWaveFinished = false;
		wave(_asteroidsNo, this);
		if (_asteroidsNo <= 9) _asteroidsNo += 2;
		else _asteroidsNo = 11;
	}
}