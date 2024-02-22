#pragma once

#include "The Real Engine/GameObject.h"
#include "The Real Engine/TextBox.h"

#include <vector>
#include <string>
#include <fstream>

class Asteroids
{
public:
	void Init(bool again = false);
	void Update(const float& dt);

private:
	const wchar_t* backgroundMusic[3] = { L"asteroidsEndScreenMusic.wav",L"Lobby-Time.wav",L"asteroidsBackgroundMusic.wav" }; // end-screen / pause / normal - live with that

	GameObject* ship;
	std::vector<GameObject*> enemies;
	std::vector<GameObject*> asteroids;
	std::vector<GameObject*> bullets;
	std::vector<GameObject*> stars;

	const std::vector<vec> __enemyVx = { vec(-1.0, 0.0), vec(1.0, 0.0), vec(-0.6, -0.3), vec(0.6, -0.3), vec(-0.6, 0.3), vec(0.6, 0.3), vec(-0.5, 0.7), vec(0.5, 0.7) };
	const std::vector<unsigned int> __enemyInd = { 0, 1, 2, 3, 4, 5, 6, 7, 0, 2, 1, 3, 0, 4, 1, 5, 4, 6, 5, 7 };

	TextBox* tScore;
	TextBox* endingUsername;
	TextBox* usernameInfo;
	std::vector<GameObject*> tLives;

	GameObject* pauseIcone;

	TextBox* scoreboard[10];

	bool isDead;
	bool hasLost;
	bool endingScreen;
	std::string new_username;
	int bestScore;

	std::vector<std::string> usernames;
	std::vector<int> scores;

	std::vector<GameObject*> debris;
	std::vector<vec> debrisDirection;
	std::vector<float> debrisRotation;
	float respawnCooldown;
	const float debrisSpeedMultiplier = 25.0f;
	const float maxDebrisRotationMultiplier = 25.0f;
	const float playerDebrisSpeedMultiplier = 10.0f;
	std::vector<int> playersDebris;

	const int camW = 160;
	const int camH = 90;

	const vec animationPos = vec(80, 0);
	//const vec scoreTablePos = vec(-80, 0);

	int modelShipFire;

	const float rotationMultiplier = 80.0;

	const float maxVelocity = 25;
	const float acceleration = 12.5;
	const float deacceleration = 0.75;

	vec velocity;
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

	const float bulletMaxTime = 2.5f;
	const float bulletSpeed = 50.0f;

	const int starsAmount = 100;
	const float starsSpeedMultiplier = 2.5f;
	const float endingScreenAnimationTime = 2.0f;

	const float endingScreenAnimationSize = 40.0f;
	const int endingScreenAnimationNumber = starsAmount * .8;
	const int endingScreenIndicatorNumber = starsAmount - endingScreenAnimationNumber;

	int _asteroidsNo;
	int score;
	int lives;

	int _return;

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

	const float maxAsteroidRotationMultiplier = 50.0f;

	const int bounds = 15;

	bool hasWaveFinished;
	float waveAsteroidsCooldown;
	int wave_num;

	float enemyProb;
	float smallEnemyProb;
	const float enemyDeltaProb = .2f;
	const float enemyMinDelay = 1.5f;
	const float enemyMaxDelay = 5.0f;
	float enemyDelay;

	const float enemySizes[2] = { 5.0f, 7.5f };
	std::vector<bool> enemyType;

	const float maxEnemyVelocity = 10.0f;

	const float maxEnemyBulletTime = 2.0f;
	std::vector<float> enemyShootCooldown;
	const float _enemyShootCooldown[2] = { 3.0f, 5.0f };
	const float enemyShootCooldownRange = .2f;

	float smallEnemyNoise;
	const float smallEnemyNoiseDelta = .1f;

	const int maxBigEnemyMoves = 5;
	std::vector<vec> eBDPos;

	int bigEnemyIterator;

	const int ufoXP[2] = { 500,200 }; // small/big
	const int asteroidsXP[3] = { 25,50,100 }; // big/normal/small

	bool checkBounds(GameObject* current, bool stay = false, vec bounds = vec(170, 95)) {
		bool flag = false;
		if (current->Transform.position.y > bounds.y) { if (!stay) { current->MoveGlobal(vec(0, -bounds.y * 2.0f, 0)); } flag = true; }
		if (current->Transform.position.y < -bounds.y) { if (!stay) { current->MoveGlobal(vec(0, bounds.y * 2.0f, 0)); } flag = true; }
		if (current->Transform.position.x > bounds.x) { if (!stay) { current->MoveGlobal(vec(-bounds.x * 2.0f, 0, 0)); } flag = true; }
		if (current->Transform.position.x < -bounds.x) { if (!stay) { current->MoveGlobal(vec(bounds.x * 2.0f, 0, 0)); } flag = true; }
		return flag;
	}

	void spawnAsteroids(int asteroidsNum, unsigned int type, float _posX = -10000, float _posY = -10000, float rot = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 360.0f))) {
		float minAsteroidsSize;
		float maxAsteroidsSize;

		for (int i = 0; i < asteroidsNum; i++) {
			std::vector<float> v;
			std::vector<unsigned int> id;
			std::vector<vec> points;

			asteroidSidesNo = rand() % (maxAsteroidsSidesNo - minAsteroidsSidesno) + minAsteroidsSidesno;

			for (int i = 0; i < asteroidSidesNo; ++i) {
				double angle = 2 * M_PI * i / asteroidSidesNo;
				double radiusModifier = (rand() / (double)RAND_MAX) * 2 * asteroidRadius * asteroidsVertexOffset - asteroidRadius * asteroidsVertexOffset;
				double modifiedRadius = asteroidRadius + radiusModifier;

				vec vertex = { modifiedRadius * cos(angle), modifiedRadius * sin(angle) };
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

			vec pos = vec(_posX, _posY);
			if (_posX == -10000 || _posY == -10000) {
				do {
					temp = rand() % (2 * (camW + bounds)) - (camW + bounds);
					pos.x = temp;

					temp = rand() % (2 * (camH + bounds)) - (camH + bounds);
					pos.y = temp;
				} while (pos.x > -camW - bounds && pos.x < camW + bounds && pos.y > -camH - bounds && pos.y < camH + bounds);
			}

			asteroids.push_back(Game::Create(vec(pos.x, pos.y, -90.0f), vec(0.0f, 0.0f, rot), vec(minAsteroidsSize + (float)(rand()) / ((float)(RAND_MAX / (maxAsteroidsSize - minAsteroidsSize)))), v, id));
			asteroidSize.push_back(type);
		}
	}

	void spawnEnemy(bool type) {
		vec pos;
		int temp;

		do {
			temp = rand() % (2 * (camW + bounds)) - (camW + bounds);
			pos.x = temp;

			temp = rand() % (2 * (camW + bounds)) - (camW + bounds);
			pos.y = temp;
		} while (pos.x > -camW - bounds && pos.x < camW + bounds && pos.y > -camH - bounds && pos.y < camH + bounds);

		enemies.push_back(Game::Create(vec(pos.x, pos.y, -75.0f), vec(0.0f), vec(enemySizes[(int)type]), L"AsteroidsEnemy"));
		enemyType.push_back(type);
		enemyShootCooldown.push_back((float)((rand() % (int)(2 * enemyShootCooldownRange * 100)) / 100 - enemyShootCooldownRange + _enemyShootCooldown[(int)type]));
		if (type) eBDPos.push_back(vec(rand() % (2 * (camW - bounds)) - (camW + bounds), rand() % (2 * (camH - bounds)) - (camH + bounds), rand() % maxBigEnemyMoves + 1));
	}

	void shoot(vec _pos, vec _rot, bool type, bool eType = 0) {
		float _time;
		float _scale;
		wchar_t* _model;
		float _offset;

		if (!type) {
			_time = bulletMaxTime;
			_scale = 2.5f;
			_model = L"AsteroidsBullet";
			_offset = 6.25f;
		}
		else {
			_time = maxEnemyBulletTime;
			_scale = 1.75f;
			_model = L"AsteroidsEnemyBullet";
			_offset = enemySizes[(int)eType] / 2;
		}

		bulletTimeRemain.push_back(_time);
		GameObject* bullet = Game::Create(_pos, _rot, vec(_scale), _model);
		bullets.push_back(bullet);
		bullet->Move(vec(0.0f, _offset, 0.0f));
	}

	int wave(int asteroidsNum) {
		Game::Sound(L"asteroidsNewWave.wav", false);

		spawnAsteroids(asteroidsNum, 0);
		wave_num++;
		smallEnemyNoise *= (1 - smallEnemyNoiseDelta);

		int temp = 0;
		float tempProb = enemyProb;
		while (rand() % 100 <= tempProb)
			tempProb -= 100;
		temp += 1;
		return temp;
	}

	std::vector<vec> breakIntoPieces(std::vector<unsigned int> ind = std::vector<unsigned int>{}, std::vector<vec> vx = std::vector<vec>{}) {
		float line[2][2];
		std::vector<vec> newVx;

		if (ind.size() == 1) {
			int n = ind[0];
			ind.push_back(0);
			for (int i = 0; i < n; i++) {
				ind.push_back(i); ind.push_back(i + 1);
			}
		}
		else if (vx.empty() || ind.empty()) {
			vx = { vec(0.0, 1.25), vec(-.3, -.15), vec(0.3, -.15), vec(-.25, 0.0), vec(0.25, 0.0) }; //defaults to AsteroidsShip
			ind = { 0,1,0,2,3,4 }; //same as above
		}

		for (int i = 0; i < ind.size() / 2; i++) {
			line[0][0] = vx[ind[i * 2]].x;      line[0][1] = vx[ind[i * 2]].y;
			line[1][0] = vx[ind[i * 2 + 1]].x;  line[1][1] = vx[ind[i * 2 + 1]].y;

			newVx.push_back(vec(line[0][0], line[0][1]));

			float t = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
			newVx.push_back(vec((1 - t) * line[0][0] + t * line[1][0], (1 - t) * line[0][1] + t * line[1][1]));

			newVx.push_back(vec(line[1][0], line[1][1]));
		}

		return newVx;
	}

	vec pPos; vec pOri;
	//please ignore how messy this code is, i was tired
	void death(bool tp = false, vec _pos = ship->Transform.position, vec _rot = ship->Transform.orientation, bool isShip = true, GameObject* obj = ship, float scale = 5, std::vector<vec> _vx = {}, std::vector<unsigned int> _ind = {}) {
		if (isShip) {
			Game::Game::Sound(L"asteroidsPlayerDeath.wav", false);

			isDead = true;
			ship->MoveTo(vec(-10000, -10000, 0));
		}
		else {
			Game::Game::Sound(L"asteroidsEnemyDeath.wav", false);

			Game::Destroy(obj);
		}
		std::vector<vec> vxs = breakIntoPieces(_ind, _vx);
		int random = rand() % (int)(vxs.size() * .2);
		int size = vxs.size();
		for (int i = 0; i < size / 2; i++) {
			std::vector<float> vx1;
			int it = 1;
			if (rand() % size < random && i < (size / 2) - 1) it = 2;
			if (tp) _pos = vec(rand() % (160 - jumpMargin) * 2 - 160 - jumpMargin, rand() % (90 - jumpMargin) * 2 - 90 - jumpMargin, -80);
			for (int k = 0; k < it; k++) {
				vx1.clear();
				for (int j = 0; j < 2; j++) {
					vx1.push_back(vxs[i * 2 + j + k].x); vx1.push_back(vxs[i * 2 + j + k].y); vx1.push_back(0);
					vx1.push_back(1); vx1.push_back(1); vx1.push_back(1);
				}
				debris.push_back(Game::Create(_pos, _rot, vec(scale), vx1, std::vector<unsigned int>{0, 1}));
				if (isShip) { debris[debris.size() - 1]->SetColor(vec(1, 0, 0)); playersDebris.push_back(debris.size() - 1); }
			}
		}
		for (int i = 0; i < debris.size(); i++) {
			debrisDirection.push_back(vec(-1 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (2))), -1 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (2))))); //x∈Q:[-1;1]
			debrisRotation.push_back((-maxDebrisRotationMultiplier + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (2 * maxDebrisRotationMultiplier)))));
		}
	}

	void clearVec(std::vector<GameObject*>& vec) {
		for (auto& obj : vec) {
			Game::Destroy(obj);
		}
	}

	float escSoundLen;
	bool hasEscd;

	TextBox* tutorialText;
	int tutorialStep;

	float spaceshipCooldown;
	float maxSpaceshipCooldown;
	const float spaceshipCooldownDelta = 5.0f; //[%]

	GameObject* spaceship;
	bool isSpaceship;
	bool hasSpaceshipPlayedSound;

	const float spaceshipSpeed = 40.0f;

	bool isPaused;
	float clickCooldown;

	bool isEndScreenMusicPlaying;

	float haloCoundtown;
	const int haloVxs = 7;
	const float haloRadius = 25.0f;
	const float haloRotation = .25f;
	GameObject* halo;
};