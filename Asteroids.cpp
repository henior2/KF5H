#include "Game.h"

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

float shootCooldown = .1f;

float velocityd;
float posx;
float posy;

float bposx;
float bposy;

std::vector<float> bulletTimeRemain;

const float bulletMaxTime = 3.0f;
const float bulletSpeed = 20.0f;

int8_t score;

void wave(int asteroidsNum, Game* game) {
	int vertexesNo;

	for (int i = 0; i < asteroidsNum; i++) {
		vertexesNo = rand() % 5 + 5;

		std::vector<float> v;
		std::vector<unsigned int> id;

		for (int j = 0; j < vertexesNo; j++) {
			v.push_back((float)(rand()) / (float)(RAND_MAX / 2) - 1);
			v.push_back((float)(rand()) / (float)(RAND_MAX / 2) - 1);
			v.push_back(0);
			v.push_back(1);
			v.push_back(1);
			v.push_back(1);

			id.push_back((j + vertexesNo - 1) % vertexesNo);
			id.push_back((j + vertexesNo) % vertexesNo);
		}

		asteroids.push_back(game->Create(vec3(rand()%320-160, rand()%180-90, -99.0f), vec3(0.0f), vec3(25.0f), v, id));
	}
}

void shoot(vec3 _pos, vec3 _rot, Game* game) {
	bulletTimeRemain.push_back(bulletMaxTime);
	bullets.push_back(game->Create(_pos, _rot, vec3(5.0f), "AsteroidsBullet"));
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

	score = 0;

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
		wave(1, this);
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
		shootCooldown = .1f;
		shoot(ship->Transform.position, ship->Transform.orientation, this);
	}

	//crashes the game - don't do it :D
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
		current->Move(vec3(current->Up.x, current->Up.y, 0.0f) * bulletSpeed * dt);

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
}