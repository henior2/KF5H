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
	const wchar_t* backgroundMusic[3] = { L"asteroidsEndScreenMusic",L"Lobby-Time",L"asteroidsBackgroundMusic" }; // end-screen / pause / normal - live with that

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

	bool checkBounds(GameObject* current, bool stay = false, vec bounds = vec(170, 95));

	void spawnAsteroids(int asteroidsNum, unsigned int type, float _posX = -10000, float _posY = -10000, float rot = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 360.0f)));

	void spawnEnemy(bool type);

	void shoot(vec _pos, vec _rot, bool type, bool eType = 0);

	int wave(int asteroidsNum);

	std::vector<vec> breakIntoPieces(std::vector<unsigned int> ind = std::vector<unsigned int>{}, std::vector<vec> vx = std::vector<vec>{});

	vec pPos; vec pOri;
	//please ignore how messy this code is, i was tired
	void death(vec _pos, vec _rot, GameObject* obj, bool tp = false, bool isShip = true, float scale = 5, std::vector<vec> _vx = {}, std::vector<unsigned int> _ind = {});

	void clearVec(std::vector<GameObject*>& vec);

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
	float clickCooldown = 0.25f;

	bool isEndScreenMusicPlaying = false;

	float haloCoundtown = 0;
	const int haloVxs = 7;
	const float haloRadius = 25.0f;
	const float haloRotation = .25f;
	GameObject* halo;
};