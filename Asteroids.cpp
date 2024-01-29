#include "Game.h"
#include <fstream>

using namespace glm;

namespace Asteroids {
	GameObject* ship;
	std::vector<GameObject*> enemies;
	std::vector<GameObject*> asteroids;
	std::vector<GameObject*> bullets;
	std::vector<GameObject*> stars;

	const std::vector<vec2> __enemyVx = { vec2(-1.0, 0.0), vec2(1.0, 0.0), vec2(-0.6, -0.3), vec2(0.6, -0.3), vec2(-0.6, 0.3), vec2(0.6, 0.3), vec2(-0.5, 0.7), vec2(0.5, 0.7) };
	const std::vector<unsigned int> __enemyInd = { 0, 1, 2, 3, 4, 5, 6, 7, 0, 2, 1, 3, 0, 4, 1, 5, 4, 6, 5, 7 };

	Tekst2d* tScore;
	std::vector<GameObject*> tLives;

	bool isDead;
	bool hasLost;
	bool endingScreen;
	std::string new_username;

	std::vector<GameObject*> debris;
	std::vector<vec2> debrisDirection;
	std::vector<float> debrisRotation;
	float respawnCooldown = 5.0f;
	const float debrisSpeedMultiplier = 25.0f;
	const float maxDebrisRotationMultiplier = 25.0f;
	const float playerDebrisSpeedMultiplier = 10.0f;
	std::vector<int> playersDebris;

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

	float shootCooldown;

	float velocityd;
	float posx;
	float posy;

	std::vector<float> bulletTimeRemain;
	std::vector<bool> isBulletPlayers;

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

	const int bounds = 15;

	bool hasWaveFinished;
	float waveAsteroidsCooldown;

	const int starsAmount = 100;
	const float starsSpeedMultiplier = 2.5f;

	float enemyProb;
	const float enemyDeltaProb = .15f;
	const float enemyMinDelay = 2.5f;
	const float enemyMaxDelay = 5.0f;
	float enemyDelay;

	const float enemySizes[] = { 5.0f, 7.5f };
	std::vector<bool> enemyType;

	const float maxEnemyVelocity = 10.0f;

	const float maxEnemyBulletTime = 2.0f;
	std::vector<float> enemyShootCooldown;
	float _enemyShootCooldown[2] = { 3.0f, 5.0f };
	const float enemyShootCooldownRange = .2f;

	const int maxBigEnemyMoves = 5;
	std::vector<vec3> eBDPos;

	int bigEnemyIterator;

	const int ufoXP[] = { 990,200 }; // small/big
	const int asteroidsXP[] = { 20,50,100 }; // big/normal/small

	bool checkBounds(GameObject* current, vec2 bounds = vec2(170, 95)) {
		bool flag = false;
		if (current->Transform.position.y > bounds.y) {current->MoveGlobal(vec3(0, -bounds.y * 2.0f, 0)); flag = true;}
		if (current->Transform.position.y < -bounds.y) {current->MoveGlobal(vec3(0, bounds.y * 2.0f, 0)); flag = true;}
		if (current->Transform.position.x > bounds.x) {current->MoveGlobal(vec3(-bounds.x * 2.0f, 0, 0)); flag = true;}
		if (current->Transform.position.x < -bounds.x) {current->MoveGlobal(vec3(bounds.x * 2.0f, 0, 0)); flag = true;}
		return flag;
	}

	void spawnAsteroids(int asteroidsNum, unsigned int type, float _posX = -10000, float _posY = -10000, float rot = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 360.0f))) {
		float minAsteroidsSize;
		float maxAsteroidsSize;

		for (int i = 0; i < asteroidsNum; i++) {
			std::vector<float> v;
			std::vector<unsigned int> id;
			std::vector<vec2> points;

			asteroidSidesNo = rand() % (maxAsteroidsSidesNo - minAsteroidsSidesno) + minAsteroidsSidesno;

			for (int i = 0; i < asteroidSidesNo; ++i) {
				double angle = 2 * M_PI * i / asteroidSidesNo;
				double radiusModifier = (rand() / (double)RAND_MAX) * 2 * asteroidRadius * asteroidsVertexOffset - asteroidRadius * asteroidsVertexOffset;
				double modifiedRadius = asteroidRadius + radiusModifier;

				vec2 vertex = { modifiedRadius * cos(angle), modifiedRadius * sin(angle) };
				points.push_back(vertex);
			}

			for (int i = 0; i < points.size(); i++) {
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
			id.push_back(asteroidSidesNo - 1);
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

			asteroidRotation.push_back(rot * M_PI / 180.0f);

			float rotM = -maxAsteroidRotationMultiplier + (float)(rand()) / ((float)(RAND_MAX / (maxAsteroidRotationMultiplier - (-maxAsteroidRotationMultiplier))));
			asteroidRotationMultiplier.push_back(rotM);

			int temp;

			vec2 pos = vec2(_posX, _posY);
			if (_posX == -10000 || _posY == -10000) {
				do {
					temp = rand() % (2 * (camW + bounds)) - (camW + bounds);
					pos.x = temp;

					temp = rand() % (2 * (camH + bounds)) - (camH + bounds);
					pos.y = temp;
				} while (pos.x > -camW - 15 && pos.x < camW + 15 && pos.y > -camH - 15 && pos.y < camH + 15);
			}

			asteroids.push_back(Gra->Create(vec3(pos, -90.0f), vec3(0.0f, 0.0f, rot), vec3(minAsteroidsSize + (float)(rand()) / ((float)(RAND_MAX / (maxAsteroidsSize - minAsteroidsSize)))), v, id));
			asteroidSize.push_back(type);
		}
	}

	void spawnEnemy(bool type, Game* Gra) {
		vec2 pos;
		int temp;

		do {
			temp = rand() % (2 * (camW + bounds) - (camW + bounds));
			pos.x = temp;

			temp = rand() % (2 * (camW + bounds) - (camW + bounds));
			pos.y = temp;
		} while (pos.x > -camW - bounds && pos.x < camW + bounds && pos.y > -camH - bounds && pos.y < camH + bounds);

		enemies.push_back(Gra->Create(vec3(pos, -75.0f), vec3(0.0f), vec3(enemySizes[(int)type]), "AsteroidsEnemy"));
		enemyType.push_back(type);
		enemyShootCooldown.push_back((float)((rand() % (int)(2 * enemyShootCooldownRange * 100)) / 100 - enemyShootCooldownRange + _enemyShootCooldown[(int)type]));
		if (type) eBDPos.push_back(vec3(rand() % (2 * (camW - bounds)) - (camW + bounds), rand() % (2 * (camH - bounds)) - (camH + bounds), rand() % maxBigEnemyMoves + 1));
	}

	void shoot(vec3 _pos, vec3 _rot, bool type, Game* Gra, bool eType = 0) {
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
		GameObject* bullet = Gra->Create(_pos, _rot, vec3(_scale), _model);
		bullets.push_back(bullet);
		bullet->Move(vec3(0.0f, _offset, 0.0f));
	}

	bool wave(int asteroidsNum) {
		spawnAsteroids(asteroidsNum, 0);

		int temp = 0;
		while (rand() % 100 <= enemyProb)
			temp += 1;
		return temp;
	}

	std::vector<glm::vec2> breakIntoPieces(std::vector<unsigned int> ind = std::vector<unsigned int>{}, std::vector<glm::vec2> vx = std::vector<glm::vec2>{}) {
		float line[2][2];
		std::vector<glm::vec2> newVx;

		if (ind.size() == 1) {
			int n = ind[0];
			ind.push_back(0);
			for (int i = 0; i < n; i++) {
				ind.push_back(i); ind.push_back(i + 1);
			}
		}
		else if (vx.empty() || ind.empty()) {
			vx = { vec2(0.0, 1.25), vec2(-.3, -.15), vec2(0.3, -.15), vec2(-.25, 0.0), vec2(0.25, 0.0) }; //defaults to AsteroidsShip
			ind = { 0,1,0,2,3,4 }; //same as above
		}

		for (int i = 0; i < ind.size() / 2; i++) {
			line[0][0] = vx[ind[i * 2]].x;      line[0][1] = vx[ind[i * 2]].y;
			line[1][0] = vx[ind[i * 2 + 1]].x;  line[1][1] = vx[ind[i * 2 + 1]].y;

			newVx.push_back(vec2(line[0][0], line[0][1]));

			float t = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
			newVx.push_back(vec2((1 - t) * line[0][0] + t * line[1][0], (1 - t) * line[0][1] + t * line[1][1]));

			newVx.push_back(vec2(line[1][0], line[1][1]));
		}

		return newVx;
	}

	vec2 pPos;
	//please ignore how messy this code is, i was tired
	void death(bool tp = false, vec3 _pos = ship->Transform.position, vec3 _rot = ship->Transform.orientation, bool isShip = true, GameObject* obj = ship, float scale = 5, std::vector<vec2> _vx = {}, std::vector<unsigned int> _ind = {}) {
		if (isShip) {
			isDead = true;
			ship->MoveTo(vec3(-10000, -10000, 0));
		}
		else {
			Gra->Destroy(obj);
		}
		std::vector<vec2> vxs = breakIntoPieces(_ind,_vx);
		int random = rand() % (int)(vxs.size() * .2);
		int size = vxs.size();
		for (int i = 0; i < size / 2; i++) {
			std::vector<float> vx1;
			int it = 1;
			if (rand() % size < random && i<(size/2)-1) it = 2;
			if (tp) _pos = vec3(rand() % (160 - jumpMargin) * 2 - 160 - jumpMargin, rand() % (90 - jumpMargin) * 2 - 90 - jumpMargin, -80);
			for (int k = 0; k < it; k++) {
				vx1.clear();
				for (int j = 0; j < 2; j++) {
					vx1.push_back(vxs[i * 2 + j + k].x); vx1.push_back(vxs[i * 2 + j + k].y); vx1.push_back(0);
					vx1.push_back(1); vx1.push_back(1); vx1.push_back(1);
				}
				debris.push_back(Gra->Create(_pos, _rot, vec3(scale), vx1, std::vector<unsigned int>{0, 1}));
				if (isShip) { debris[debris.size() - 1]->SetColor(vec3(1,0,0)); playersDebris.push_back(debris.size() - 1); }
			}
		}
		for (int i = 0; i < debris.size(); i++) {
			debrisDirection.push_back(vec2(-1 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (2))), -1 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (2))))); //x∈Q:[-1;1]
			debrisRotation.push_back((-maxDebrisRotationMultiplier + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (2 * maxDebrisRotationMultiplier)))));
		}
	}

	void clearVec(std::vector<GameObject*>& vec) {
		for (auto& obj : vec) {
			Gra->Destroy(obj);
		}
	}

	Tekst2d* refreshText(Tekst2d* text, std::string str) {
		Gra->DestroyTekst(text);
		text = Gra->CreateTekst(vec2(-.9, .8), 0, vec2(.025), 1, .5, str);
		return text;
	}
};

using namespace Asteroids;


void Game::AsteroidsInit(bool again) {
	if (!again) {
		ship = Create(vec3(0.0f, 0.0f, -99.0f), vec3(0.0f), vec3(5.0f), "AsteroidsShip");
		modelShipFire = ship->AddStage("AsteroidsShipFire");
	}
	ship->MoveTo(vec3(0, 0, -80));
	ship->RotateTo(vec3(0));
	isDead = false;
	hasLost = false;
	endingScreen = false;
	new_username = "";
	respawnCooldown = 5.0f;

	velocity = vec2(0.0f);
	speed = 0;
	velocityd = maxVelocity * maxVelocity;

	posx = posy = 0.0f;

	jumpCooldown = 0.5f;
	shipAnimationCooldown = (rand() % 4) / 2 + 1;
	shipAnimationCooldown2 = (rand() % 2) / 2 + 0.25;
	shootCooldown = .25f;

	if (!again) {
		camera->perspective = false;
		camera->cameraHeight = camH;
		camera->cameraWidth = camW;
	}

	_asteroidsNo = 4;
	lives--;
	if (!again) {
		score = 0;
		lives = 3;
	}
	if (lives <= 0) {
		isDead = true;
		hasLost = true;
	}

	_return = 0;

	hasWaveFinished = false;
	waveAsteroidsCooldown = 2.5f;

	enemyProb = 15.0f;
	enemyDelay = rand() % (int)(enemyMaxDelay - enemyMinDelay) + enemyMinDelay;

	bigEnemyIterator = 0;

	if (again) {
		clearVec(asteroids); clearVec(bullets); clearVec(enemies); clearVec(tLives);
	}

	asteroids.clear();
	enemies.clear();
	bullets.clear();
	if (!again) {
		stars.clear();
		debris.clear();
	}

	bulletTimeRemain.clear();
	isBulletPlayers.clear();
	asteroidSize.clear();
	asteroidRotation.clear();
	asteroidRotationMultiplier.clear();
	enemyType.clear();
	enemyShootCooldown.clear();
	eBDPos.clear();
	debrisDirection.clear();
	playersDebris.clear();
	tLives.clear();

	if (!again) {
		for (int i = 0; i < starsAmount; i++) {
			stars.push_back(Create(vec3(rand() % 320 - 160, rand() % 180 - 90, -99.999f), vec3(0.0f, 0.0f, rand() % 45), vec3(.01f), "AsteroidsStar"));
		}

		PlaySound2d("mus01.mp3", true);
	}

	std::string scoreStr = std::to_string(score);
	while (scoreStr.length() < 3) {
		scoreStr = "0" + scoreStr;
	}
	if (!again) tScore = CreateTekst(vec2(-.9, .8), 0, vec2(.025), 1, .5, scoreStr);
	else tScore = refreshText(tScore, scoreStr);
	tScore->SetColor(vec3(1)); //todo: fix

	for (int i = 0; i < lives; i++) {
		std::string modelName = "AsteroidsShip";
		if (i == lives - 1) modelName = "AsteroidsShipFire";
		GameObject* current = Create(vec3(-.9 + .012 + .035 * i, .7, 0), vec3(0), vec3(.04), modelName); //.012 so that it's centered... .035 is spacing - feel free to change that anytime
		current->Stage[0].onTop = true;
		tLives.push_back(current);
	}
}

void Game::Asteroids(float dt) {
	pPos = ship->Transform.position;

	jumpCooldown -= dt;
	shootCooldown -= dt;

	bigEnemyIterator = 0;

	if (!isDead && (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)) {
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

		for (auto& star : stars) {
			star->MoveGlobal(vec3(shipUp.x * -starsSpeedMultiplier, shipUp.y * -starsSpeedMultiplier, 0) * dt);
			checkBounds(star, vec2(160, 90));
		}
	}
	else {
		velocity -= velocity * deacceleration * dt;
		speed = velocity.x * velocity.x + velocity.y * velocity.y;
		ship->MoveGlobal(vec3(velocity.x * dt, velocity.y * dt, 0));

		ship->activeStage = 0;
	}
	if (!isDead && (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)) {
		ship->Rotate(vec3(0, 0, 1.0f) * rotationMultiplier * dt);
	}
	if (!isDead && (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)) {
		ship->Rotate(vec3(0, 0, -1.0f) * rotationMultiplier * dt);
	}

	//debug - dont touch please :)
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS && jumpCooldown <= 0.0f) {
		jumpCooldown = 0.5f;
		hasWaveFinished = true;
	}
	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS && jumpCooldown <= 0.0f) {
		jumpCooldown = 0.5f;
		spawnAsteroids(5, 2);
	}
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS && jumpCooldown <= 0.0f) {
		jumpCooldown = 0.5f;
		spawnAsteroids(5, 1);
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

	if (!isDead && glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS && jumpCooldown <= 0.0f) {
		jumpCooldown = 0.5f;

		int random = rand() % 32 - 1;
		if (random >= 24 && random <= 31) {
			death(true);
		}
		else {
			random = rand() % 8 - 1;
			random = (random * 2) + 4;

			if (random < asteroids.size()) {
				death(true);
			}
			else {
				ship->MoveTo(vec3(rand() % (160 - jumpMargin) * 2 - 160 - jumpMargin, rand() % (90 - jumpMargin) * 2 - 90 - jumpMargin, -80));
				velocity = vec2(0.0f, 0.0f);
			}
		}
	}

	if (!isDead && glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && shootCooldown <= 0) {
		shootCooldown = .25f;
		shoot(ship->Transform.position, ship->Transform.orientation, 0, this);
		isBulletPlayers.push_back(true);
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

		if (type) {
			if (sPos.x + 5 > pos.x && sPos.x - 5 < pos.x && sPos.y + 5 > pos.y && sPos.y - 5 < pos.y) {
				if (eBDPos[bigEnemyIterator].z > 0) {
					eBDPos[bigEnemyIterator] = vec3(rand() * (2 * (camW + bounds)) - (camW + bounds), rand() % (2 * (camH + bounds)) - (camH + bounds), eBDPos[bigEnemyIterator].z);
				}
				else {
					eBDPos[bigEnemyIterator] = vec3(rand() % (2 * camW) - camW, -2 * camH - bounds, eBDPos[bigEnemyIterator].z);
				}
			}
		}

		vec2 dMov = vec2(sPos.x - pos.x, sPos.y - pos.y);

		if (dMov.x > maxEnemyVelocity) dMov.x = maxEnemyVelocity;
		else if (dMov.x < -maxEnemyVelocity) dMov.x = -maxEnemyVelocity;

		if (dMov.y > maxEnemyVelocity) dMov.y = maxEnemyVelocity;
		else if (dMov.y < -maxEnemyVelocity) dMov.y = -maxEnemyVelocity;

		if (!isDead) current->MoveGlobal(vec3(dMov, 0.0f) * dt);

		float _angle;

		enemyShootCooldown[i] -= dt;
		if (!isDead && enemyShootCooldown[i] <= 0) {
			if (!type) {
				enemyShootCooldown[i] = (float)((rand() % (int)(2 * enemyShootCooldownRange * 100)) / 100 - enemyShootCooldownRange + _enemyShootCooldown[(int)type]);

				_angle = atan2(sPos.y - pos.y, sPos.x - pos.x);
				_angle = _angle * 180.0f / M_PI - 90.0f;
			}
			else _angle = rand() % 360;

			shoot(current->Transform.position, vec3(0.0f, 0.0f, _angle), 1, this, type);
			enemyShootCooldown[i] = _enemyShootCooldown[(int)type];
			isBulletPlayers.push_back(false);
		}

		if (!isDead && type) {
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

		//collisions - player/enemy
		if (!isDead && Gra->collisionCircle(pPos, pos)) {
			death();
		}
	}

	for (int i = 0; i < bullets.size(); i++) {
		GameObject* current = bullets[i];
		bool shouldSkip = false;

		bulletTimeRemain[i] -= dt;
		if (bulletTimeRemain[i] <= 0) {
			Destroy(current);
			bullets.erase(bullets.begin() + i);
			bulletTimeRemain.erase(bulletTimeRemain.begin() + i);
			isBulletPlayers.erase(isBulletPlayers.begin() + i);
			i--;
			continue;
		}
		if (!isDead) {
			current->Move(vec3(0.0f, 1.0f, 0.0f) * bulletSpeed * dt);

			checkBounds(current);
		}

		vec2 currentPos = current->Transform.position;

		//collisions - player/bullets
		if (!isDead && !isBulletPlayers[i] && Gra->collisionCircle(pPos, current->Transform.position)) {
			death();
		}

		//collisions - enemy/bullets
		for (int j = enemies.size() - 1; j >= 0; j--) {
			GameObject* enemy = enemies[j];
			if (!isDead && isBulletPlayers[i] && Gra->collisionCircle(currentPos, enemy->Transform.position)) {
       			death(false, enemy->Transform.position, vec3(0), false, enemy, enemySizes[0], __enemyVx, __enemyInd);
				enemies.erase(enemies.begin() + j);
				if (enemyType[j]) eBDPos.erase(eBDPos.begin() + j);
				enemyShootCooldown.erase(enemyShootCooldown.begin() + j);

				score += ufoXP[enemyType[j]];
				enemyType.erase(enemyType.begin() + j);
				tScore = refreshText(tScore, std::to_string(score));

				bulletTimeRemain[i] = 0;
				shouldSkip = true;
				break;
			}
		}
		if (shouldSkip) continue;

		//collisions - asteroids/bullets
		for (int j = asteroids.size() - 1; j >= 0; j--) {
			GameObject* asteroid = asteroids[j];
			if (!isDead && Gra->collisionCircle(currentPos, asteroid->Transform.position)) {
				int type = asteroidSize[j];
				if (type < 2) {
					float ori = asteroid->Transform.orientation.z;
					vec3 pos = asteroid->Transform.position;
					float randomChange = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 45.0f)); // [-45;45]

					spawnAsteroids(1, type + 1, pos.x, pos.y, ori + randomChange);
					spawnAsteroids(1, type + 1, pos.x, pos.y, ori - randomChange);
				}

				Destroy(asteroid);
				asteroids.erase(asteroids.begin() + j);
				asteroidSize.erase(asteroidSize.begin() + j);
				asteroidRotation.erase(asteroidRotation.begin() + j);
				asteroidRotationMultiplier.erase(asteroidRotationMultiplier.begin() + j);

				score += asteroidsXP[type];
				tScore = refreshText(tScore, std::to_string(score));

				bulletTimeRemain[i] = 0;
				shouldSkip = true;
				break;
			}
		}
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

		if (!isDead) {
			current->Rotate(vec3(0.0f, 0.0f, asteroidRotationMultiplier[i]) * dt);
			current->MoveGlobal(vec3(cos(deg), sin(deg), 0.0f) * _velocity * dt);
		}

		checkBounds(current, vec2(camera->cameraWidth + bounds, camera->cameraHeight + bounds));

		//collisions - player/asteroid
		if (!isDead && Gra->collisionCircle(pPos, current->Transform.position)) {
			death();
		}
	}

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		Game::ChangeState(Game_Menu);

	if (!isDead) checkBounds(ship);

	if (!isDead && asteroids.empty()) hasWaveFinished = true;

	if (!isDead && hasWaveFinished) {
		waveAsteroidsCooldown -= dt;

		if (waveAsteroidsCooldown <= 0) {
			waveAsteroidsCooldown = 2.5f;
			hasWaveFinished = false;
			_return = wave(_asteroidsNo);
			if (_asteroidsNo <= 9) _asteroidsNo += 2;
			else _asteroidsNo = 11;
		}
	}

	if (!isDead && _return != 0) {
		enemyDelay -= dt;
		if (enemyDelay <= 0) {
			for (int i = 0; i < _return; i++)
				spawnEnemy(0, this);

			enemyDelay = rand() % (int)(enemyMaxDelay - enemyMinDelay) - enemyMinDelay;
			enemyProb += enemyProb * enemyDeltaProb;
			_return = 0;
		}
	}

	for (int i = 0; i < debris.size(); i++) {
		GameObject* current = debris[i];
		float multiplier = debrisSpeedMultiplier;
		bool isPlayers = false;
		if (std::find(playersDebris.begin(), playersDebris.end(), i) != playersDebris.end()) { multiplier = playerDebrisSpeedMultiplier; isPlayers = true; }
		current->MoveGlobal(vec3(debrisDirection[i], 0) * dt * multiplier);
		current->Rotate(vec3(0, 0, 1) * debrisRotation[i] * dt);

		if (checkBounds(current) && !isPlayers) {
			Destroy(current);
			debris.erase(debris.begin() + i);
			debrisDirection.erase(debrisDirection.begin() + i);
			debrisRotation.erase(debrisRotation.begin() + i);
		}
	}
	if (isDead) {
		if (hasLost && !endingScreen) {
			std::ifstream file("_scoredata.txt"); // reading the file

			std::vector<std::string> usernames; // char[3] would be enough, but it's not letting me do it
			std::vector<int> scores;
			std::string line;

			while (std::getline(file, line)) {
				usernames.push_back(line.substr(0, 3));
				scores.push_back(std::stoi(line.substr(3)));
			}
			file.close();

			if (new_username.length() < 3) {
				for (int key = GLFW_KEY_A; key <= GLFW_KEY_Z; key++) {
					if (glfwGetKey(window, key) == GLFW_PRESS && jumpCooldown <= 0) {
						jumpCooldown = .25f; //im using this variable on purpose
						new_username += (char)('A' + (key - GLFW_KEY_A));
					}
				}
			}
			else {
				int n = 0;
				for (int i = 0; i < scores.size(); i++) {
					n = i;
					if (score >= scores[i]) break;
				}

				usernames.insert(usernames.begin() + n, new_username); // inserting the new username
				scores.insert(scores.begin() + n, score); // and score

				if (usernames.size() >= 10) {
					usernames.pop_back(); // removing the last (worst) username
					scores.pop_back(); // and score (there can only be <= 10)
				}

				std::ofstream file_out("_scoredata.txt"); // opening the file

				for (int i = 0; i < usernames.size(); i++) {
					file_out << (usernames[i] + std::to_string(scores[i]) + "\n"); // writing the scores ('ABC1234', where 'ABC' is the username, and '1234' is the score)
				}

				file_out.close(); // closing the file

				endingScreen = true;
			}
		}
		if (endingScreen) {
			//todo: add
		}
		respawnCooldown -= dt;
		if (respawnCooldown <= 0 && !hasLost) {
			AsteroidsInit(true);
			clearVec(debris);
			debris.clear();
		}
	}
}