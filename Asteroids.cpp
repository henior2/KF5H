#include "Game.h"

using namespace glm;

GameObject* ship;
std::vector<GameObject*> enemies;
std::vector<GameObject*> asteroids;

int modelShipFire;

const float rotationMultiplier = 100.0;

const float maxVelocity = 25;
const float acceleration = 15;
const float deacceleration = 0.99;

vec2 velocity;
float speed;

const int jumpMargin = 20;
float jumpCooldown;

float velocityd;

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

		game->Create(vec3(rand()%320-160, rand()%180-90, -99.0f), vec3(0.0f), vec3(25.0f), v, id);
	}
}

void Game::AsteroidsInit() {
	ship = Create(vec3(0.0f, 0.0f, -99.0f), vec3(0.0f), vec3(5.0f), "AsteroidsShip");
	modelShipFire = ship->AddStage("AsteroidsShipFire");

	velocity = vec2(0.0f);
	speed = 0;
	velocityd = maxVelocity * maxVelocity;

	jumpCooldown = 0.5f;
	//enemies.push_back(Create(vec3(0.0f, 0.0f, -10.0f), vec3(0.0f), vec3(.25f), "AsteroidsEnemy"))

	camera->perspective = false;
	camera->cameraHeight = 90;
	camera->cameraWidth = 160;

	score = 0;

	asteroids.clear();
	enemies.clear();

	PlaySound2d("mus01.mp3", true);
}

void Game::Asteroids(float dt) {
	jumpCooldown -= dt;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		velocity += acceleration * dt * vec2(ship->Up.x, ship->Up.y);
		speed = velocity.x * velocity.x + velocity.y * velocity.y;
		if (speed > velocityd) velocity *= velocityd / speed;
		ship->MoveGlobal(vec3(velocity.x * dt, velocity.y * dt, 0));

		ship->activeStage = modelShipFire;
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
		//enemies.push_back(Create(vec3(rand() % 320 - 160, rand() % 180 - 90, -80), vec3(0.0f), vec3(5.0f), "AsteroidsEnemy"));
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
				//czasem wywala poza ekran, nwm czemu
				ship->MoveTo(vec3(rand() % (160 - jumpMargin) * 2 - 160 - jumpMargin, rand() % (90 - jumpMargin) * 2 - 90 - jumpMargin, -80));
				velocity = vec2(0.0f, 0.0f);
			}
		}
	}

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		Game::ChangeState(Game_Menu);

	float posx = ship->Transform.position.x;
	float posy = ship->Transform.position.y;
	if (ship->Transform.position.y > 100) ship->MoveGlobal(vec3(0, -190, 0));
	if (ship->Transform.position.y < -100) ship->MoveGlobal(vec3(0, 190, 0));
	if (ship->Transform.position.x > 170) ship->MoveGlobal(vec3(-330, 0, 0));
	if (ship->Transform.position.x < -170) ship->MoveGlobal(vec3(330, 0, 0));
}