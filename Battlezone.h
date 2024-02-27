#pragma once
#include "The Real Engine/TextBox.h"
class Battlezone
{
public:
    void Init();
    void Update( float& dt);
private:
    float keyCooldown;
    GameObject* thePointer;
    float signed_angle_between_vectors(const vec& A, const vec& B, const vec& axis);
	bool debugCamera = false;
	bool flag;
	bool isDead;
	bool endingScreen;
	float hp;
	int score;
	int wavePoints;
	float waveTime;
	bool waveFlag;
	TextBox* fala;
	TextBox* display_hp;
	std::string new_username;

	std::vector<float> fastBulletTimeRemain;
	std::vector<int> enemyType;
	std::vector<float> enemyShotCooldowns;
	const float enemyCooldown = 8.0f;

	std::vector<GameObject*> pociski;
	std::vector<GameObject*> przeciwnicy;
	std::vector<GameObject*> rakiety;
	float shot_cool = 3;
	float resp_cool = 2;
	const float fast_tank_speed = 3;
	const float tank_speed = 2;
	const float rocket_speed = 5;

	void push_back3(std::vector<float>& vec, float a1, float a2, float a3);
	void push_back3(std::vector<float>& vec, float a1);
	void push_back2(std::vector<unsigned int>& vec, unsigned int a1, unsigned int a2);
	void push_back2(std::vector<unsigned int>& vec, unsigned int a1);

	const float mapSize = 125; //from the middle, so 125 <=> 250x250
	const float maxOutOfBoundsDistance = 25;
	float glitchEffectRefreshRate;
	const unsigned int maxGlitchLinesNumber = 75;
	std::vector<GameObject*> __lines;

	const unsigned int radarPoints = 35;
	const float radarRadius = 5;
	const float radarLineLenght = .5f;
	const float fullRotationTime = 6.0f; //part of the GTU (global timing unit)
	const unsigned int trailLinesNo = 30;
	const float linesSpace = .3f;

	float rtp;
	float pU2AnimationCooldown;

	const float uiYOffset = .75f;
	const float uiScale = .0025f;

	GameObject* player;
	GameObject* model2;
	GameObject* ufo;
	float reloadTime = 5.0f;
	int bulletsFired;

	bool isUfo;
	const float ufoSpeed = 3.0f;
	float ufoCooldown;
	int ufoMovesLeft;
	vec ufoTargetPos;

	GameObject* plane;
	const float dropRadius = 1.0f;
	vec dropPos;
	TextBox* tScore;

	GameObject* radar;
	std::vector<GameObject*> spinningLines;

	std::vector<GameObject*> uiElements;

	std::vector<vec> targetPos;
	std::vector<vec> targetOri;
	std::vector<GameObject*> pociski_gracza;
	std::vector<float> bulletTimeRemain;

	std::vector<GameObject*> radarElements;
	std::vector<unsigned int> radarElementsType; // 0 - normal / big / vinci, 1 - obstacle, 2 - boost, 3 - intercontinental ballistic missile (aka rocket)

	std::vector<float> randomActionTimeLimit; //indicates how much time of performing the random action is left
	std::vector<float> randomActionTimeCooldown; //indicates how much time is left until performing a random action  
	std::vector<int> randomActionType; //0 - none, 1 - left, 2 - right, 3 - forward, 4 - stop

	const float maxRandomActionLimit = 5.0f;
	const float maxRandomActionCooldown = 25.0f;
	const float rotationsPerSecond = .15f;

	const float rPointerScaleDefault = .1f;
	const float rPointerScaleBig = .15f;

	const float rotationMultiplier = -500.0f;
	int camSpeed;
	float velocity;
	float const rotationMultiplier1 = 35;

	const float bulletMaxTime = 4.0f;
	const float bulletSpeed = 14.0f;

	void destroy_enemy(int i);

	void shot_fast(vec pos, vec rot);

	GameObject* auto_bullet;

	void shot(vec pos, vec rot, bool isPlayer = false);

	void shot_leonardo(vec pos, vec rot);

	TextBox* refreshText(TextBox* text, int score);

	void enemyShoot(GameObject* enemy);

	void spawn_enemy(vec pos, vec rot, int type);

	void wave(int wavePoints, float dt);

	const float camFrontOffset = -2.5f;
	const float camYOffset = 2.5f;

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
	float planeCooldown;
	bool isPlane;
	vec planeStartCoords;
	const float planeBounds = 175.0f;

	GameObject* horizon;

	void makeHorizon();

	const int totalObstacles = 100;
	const float maxObstacleHeight = 7.0f;
	const float minObstacleHeight = 5.0f;

	void makeObstacles(float x, float z, float height);

	const float pUScale = 1.15f;
	const float pUIRotationSpeed = 36.0f;
	float pUBRotationSpeed = 12.0f; //not const, cuz might be changed in init(), depending on the value of pUSameDirectionRotation
	const float pUowYOffset = 1.0f;
	const bool pUSameDirectionRotation = false;
	const float pUAFCTime2 = .5f;
	const float powerUpFallingSpeed = 2.0f;

	const std::string pUModels[] = { "Speed","Heart","Reload","Boost","XP","Star" };
	std::vector<GameObject*> powerUpInside;
	std::vector<GameObject*> powerUpBox;
	std::vector<GameObject*> powerUpAnimation;
	std::vector<int> powerUpType; // 0 - speed, 1 - life, 2 - decrease reload time, 3 - increase score multiplier, 4 - increase score (one-time), 5 - boost (no idea for it's purpose)

	const float speedBoost = .05f; //[5%]
	const float healthBoost = 50.0f; //adds 50 to current haelth/healht/health idk how to write it xD
	float timeMultiplier;
	const float timeDecrease = .5f; //[50%]
	const float timeEffectLength = 10.0f; //[10s (NOT real life time)]
	float timeEffectLeft;
	float scoreMultiplier;
	const float scoreMultiplierChange = .1f;
	const int scoreChange = 500; //[500xp]
	bool isMissleSelfTargeting;

	void createPowerUp(float x, float y, float z, int type);


	const float pUdYoTU = pUowYOffset / (.25f * fullRotationTime); //at least im aware that i suck at naming things
	float currentPUdYoTU = pUdYoTU; //...

	// bro AT LEAST LEAVE A COMMENT 😫 //nuh

	void collectPowerUp(GameObject* _inside, GameObject* _box, unsigned int _type);

	int money;
	const std::string shopModels[] = { "PowerUpHeart" };
	int itemsPrice[3];
	int itemsType[3];
	GameObject* shopDisplayIcons[3];
	GameObject* shopDisplaySquares[3];
	const float shopXPos = .825f;
	const float shopYPos = .2f;
	const float shopScale = .15f;

	void insertItem(int item, bool isTheFirstTime = false);

	void buyItem(int type, int cost, int item);

	void shopAction(int item, bool isForced = false);

	void destroy_enemy(int i) {
		Game::Destroy(przeciwnicy[i]);
		Game::Destroy(radarElements[i]);
		Game::Destroy(uiElements[i]);
		przeciwnicy.erase(przeciwnicy.begin() + i);
		enemyType.erase(enemyType.begin() + i);
		enemyShotCooldowns.erase(enemyShotCooldowns.begin() + i);
		radarElements.erase(radarElements.begin() + i);
		radarElementsType.erase(radarElementsType.begin() + i);
		uiElements.erase(uiElements.begin() + i);
	}

	void shot_fast(vec pos, vec rot) {
		fastBulletTimeRemain.push_back(bulletMaxTime);
		GameObject* bullet = Game::Create(pos, rot, vec(1.0f), L"FastBullet");
		pociski.push_back(bullet);
		bullet->Move(vec(0, 0, -1));
	}


	void shot(vec pos, vec rot, bool isPlayer = false) {
		if (isPlayer)bulletTimeRemain.push_back(bulletMaxTime);
		else fastBulletTimeRemain.push_back(bulletMaxTime);
		GameObject* bullet = Game::Create(pos, rot, vec(1.0f), L"TankBullet");
		if (!isPlayer)pociski.push_back(bullet);
		else pociski_gracza.push_back(bullet);
		//if (isPlayer && isMissleSelfTargeting) auto_bullet = bullet;
		bullet->Move(vec(0, 0, -1));
	}

	void shot_leonardo(vec pos, vec rot) {
		int temp = rand() % 8 + 1;
		GameObject* bullet;

		fastBulletTimeRemain.push_back(bulletMaxTime);
		bullet = Game::Create(pos, rot, vec(1.0f), L"TankBullet");
		pociski.push_back(bullet);
		bullet->Move(vec(0, 0, -1));

		switch (temp)
		{
		case 2:
			bullet->RotateTo(rot - vec(0, 180, 0));
			break;
		case 3:
			bullet->RotateTo(rot - vec(0, 90, 0));
			break;
		case 4:
			bullet->RotateTo(rot - vec(0, 270, 0));
			break;
		case 5:
			bullet->RotateTo(rot - vec(0, 45, 0));
			break;
		case 6:
			bullet->RotateTo(rot - vec(0, 225, 0));
			break;
		case 7:
			bullet->RotateTo(rot - vec(0, 135, 0));
			break;
		case 8:
			bullet->RotateTo(rot - vec(0, 315, 0));
			break;
		default:
			break;
		}
	}

	void enemyShoot(GameObject* enemy) {
		int enemyIndex = -1;
		if (!przeciwnicy.empty()) {
			for (int i = 0; i < przeciwnicy.size(); ++i) {
				if (przeciwnicy[i] == enemy) {
					enemyIndex = i;
					break;
				}
			}
		}

		//nice ChatGPT lmao
		if (enemyIndex != -1 && enemyShotCooldowns[enemyIndex] <= 0) {
			if (enemyType[enemyIndex] == 1) {
				shot(enemy->Transform.position + vec(0, 2.535, 0), enemy->Transform.orientation);
			}
			else if (enemyType[enemyIndex] == 2) {
				shot_fast(enemy->Transform.position + vec(0, 2.12, 0), enemy->Transform.orientation);
			}
			else if (enemyType[enemyIndex] == 3) {
				shot_leonardo(enemy->Transform.position + vec(0, .6, 0), enemy->Transform.orientation);
			}
			enemyShotCooldowns[enemyIndex] = enemyCooldown;
		}
	}

	void spawn_enemy(vec pos, vec rot, int type) {
		//Normal tank
		if (type == 1) {
			GameObject* enemy = Game::Create(pos, rot, vec(1.0f), L"Tank");
			przeciwnicy.push_back(enemy);
			enemyType.push_back(type);
			enemyShotCooldowns.push_back(enemyCooldown);

			GameObject* rPointer = Game::Create(vec(0.0f), vec(0.0f), vec(rPointerScaleDefault), L"RadarX");
			radarElements.push_back(rPointer);
			radarElementsType.push_back(0);
			uiElements.push_back(rPointer);
			targetPos.push_back(vec(0.0f));
			targetOri.push_back(vec(0.0f));
		}
		//Fast tank
		else if (type == 2) {
			GameObject* enemy = Game::Create(pos, rot, vec(2.0f), L"FastTank");
			przeciwnicy.push_back(enemy);
			enemyType.push_back(type);
			enemyShotCooldowns.push_back(enemyCooldown);

			GameObject* rPointer = Game::Create(vec(0.0f), vec(0.0f), vec(rPointerScaleBig), L"RadarX");
			radarElements.push_back(rPointer);
			radarElementsType.push_back(0);
			uiElements.push_back(rPointer);
			targetPos.push_back(vec(0.0f));
			targetOri.push_back(vec(0.0f));
		}
		//Leonardo tank
		else if (type == 3) {
			GameObject* enemy = Game::Create(pos, rot, vec(1.0f), L"LeonardoTank");
			przeciwnicy.push_back(enemy);
			enemyType.push_back(type);
			enemyShotCooldowns.push_back(enemyCooldown);

			GameObject* rPointer = Game::Create(vec(0.0f), vec(0.0f), vec(rPointerScaleDefault), L"AsteroidsStar");
			rPointer->color = vec3(0, 1, 0);
			radarElements.push_back(rPointer);
			radarElementsType.push_back(0);
			uiElements.push_back(rPointer);
			targetPos.push_back(vec3(0.0f));
			targetOri.push_back(vec3(0.0f));
		}
		//Rocket
		else if (type == 4) {
			GameObject* enemy = Gra->Create(pos, rot, vec3(1.0f), "Rocket");
			rakiety.push_back(enemy);
			enemyType.push_back(type);
			enemyShotCooldowns.push_back(enemyCooldown);

			GameObject* rPointer = Gra->Create(vec3(0.0f), vec3(0.0f), vec3(rPointerScaleDefault), "AsteroidsShip");
			rPointer->color = vec3(0, 1, 0);
			radarElements.push_back(rPointer);
			radarElementsType.push_back(3);
			uiElements.push_back(rPointer);
			targetPos.push_back(vec3(0.0f));
			targetOri.push_back(vec3(0.0f));
		}

		// kamil forgor 💀 //stfu
		auto* current = radarElements.back();
		current->Stage[0].onTop = true;
		current->ScaleTo(vec3(uiScale * 9, uiScale * 16, 0));
		current->MoveTo(vec3(0, uiYOffset, 0));
		current->Rotate(vec3(0, 180, 0));

		randomActionTimeLimit.push_back(static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / maxRandomActionLimit))); //x∈Q: [0;mRAL]
		randomActionTimeCooldown.push_back(static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / maxRandomActionCooldown))); //x∈Q: [0;mRAC]
		randomActionType.push_back(0);
	}

	void wave(int wavePoints, float dt) {
		if (wavePoints > 0) {
			float temp_x = (float)(rand() % 51 - 25);
			float temp_z = (float)(rand() % 51 - 25);
			float temp_y = (float)(rand() % 361);
			if (przeciwnicy.empty() && rakiety.empty()) {
				int enemy = rand() % 4 + 1;
				if (wavePoints - enemy >= 0) {
					spawn_enemy(vec3(temp_x, 0, temp_z), vec3(0, temp_y, 0), enemy);
					wavePoints -= enemy;
				}
			}
		}
		else {
			while (waveTime > 0) {
				waveTime -= dt;
			}
		}
	}

};

