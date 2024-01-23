#include "Game.h"

using namespace glm;

namespace Battlezone {
	const double PI = glm::pi<double>();
	bool flag = false;
	int lives = 3;

	void push_back3(std::vector<float>& vec, float a1, float a2, float a3) {
		vec.push_back(a1);
		vec.push_back(a2);
		vec.push_back(a3);
	}
	void push_back3(std::vector<float>& vec, float a1) {
		vec.push_back(a1);
		vec.push_back(a1);
		vec.push_back(a1);
	}
	void push_back2(std::vector<unsigned int>& vec, unsigned int a1, unsigned int a2) {
		vec.push_back(a1);
		vec.push_back(a2);
	}
	void push_back2(std::vector<unsigned int>& vec, unsigned int a1) {
		vec.push_back(a1);
		vec.push_back(a1);
	}

	const float mapSize = 125; //from the middle, so 125 <=> 250x250
	const float maxOutOfBoundsDistance = 25;
	float glitchEffectRefreshRate = .1f;
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
	int bulletsFired = 0;

	bool isUfo = false;
	const float ufoSpeed = 3.0f;
	float ufoCooldown;
	int ufoMovesLeft;
	vec2 ufoTargetPos;

	GameObject* plane;

	GameObject* radar;
	std::vector<GameObject*> spinningLines;

	std::vector<GameObject*> uiElements;

	std::vector<vec3> targetPos;
	std::vector<vec3> targetOri;

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
	std::vector<float> enemyShotCooldowns;

	const float bulletMaxTime = 4.0f;
	const float bulletSpeed = 28.0f;

	void shot_fast(vec3 pos, vec3 rot, Game* Gra) {
		fastBulletTimeRemain.push_back(bulletMaxTime);
		GameObject* bullet = Gra->Create(pos, rot, vec3(1.0f), "FastBullet");
		pociski.push_back(bullet);
		bullet->Move(vec3(0, 0, -1));
	}

	void shot(vec3 pos, vec3 rot, Game* Gra) {
		fastBulletTimeRemain.push_back(bulletMaxTime);
		GameObject* bullet = Gra->Create(pos, rot, vec3(1.0f), "TankBullet");
		pociski.push_back(bullet);
		bullet->Move(vec3(0, 0, -1));
	}

	void shot_leonardo(vec3 pos, vec3 rot, Game* Gra) {

		int temp = rand() % 8 + 1;

		if (temp == 1) {
			fastBulletTimeRemain.push_back(bulletMaxTime);
			GameObject* bullet1 = Gra->Create(pos, rot, vec3(1.0f), "TankBullet");
			pociski.push_back(bullet1);
			bullet1->Move(vec3(0, 0, -1));
		}

		else if (temp == 2) {
			fastBulletTimeRemain.push_back(bulletMaxTime);
			GameObject* bullet2 = Gra->Create(pos, rot - vec3(0, 180, 0), vec3(1.0f), "TankBullet");
			pociski.push_back(bullet2);
			bullet2->Move(vec3(0, 0, -1));
		}

		else if (temp == 3) {
			fastBulletTimeRemain.push_back(bulletMaxTime);
			GameObject* bullet3 = Gra->Create(pos, rot - vec3(0, 90, 0), vec3(1.0f), "TankBullet");
			pociski.push_back(bullet3);
			bullet3->Move(vec3(0, 0, -1));
		}

		else if (temp == 4) {
			fastBulletTimeRemain.push_back(bulletMaxTime);
			GameObject* bullet4 = Gra->Create(pos, rot - vec3(0, 270, 0), vec3(1.0f), "TankBullet");
			pociski.push_back(bullet4);
			bullet4->Move(vec3(0, 0, -1));
		}

		else if (temp == 5) {
			fastBulletTimeRemain.push_back(bulletMaxTime);
			GameObject* bullet5 = Gra->Create(pos, rot - vec3(0, 45, 0), vec3(1.0f), "TankBullet");
			pociski.push_back(bullet5);
			bullet5->Move(vec3(0, 0, -1));
		}

		else if (temp == 6) {
			fastBulletTimeRemain.push_back(bulletMaxTime);
			GameObject* bullet6 = Gra->Create(pos, rot - vec3(0, 225, 0), vec3(1.0f), "TankBullet");
			pociski.push_back(bullet6);
			bullet6->Move(vec3(0, 0, -1));
		}

		else if (temp == 7) {
			fastBulletTimeRemain.push_back(bulletMaxTime);
			GameObject* bullet7 = Gra->Create(pos, rot - vec3(0, 135, 0), vec3(1.0f), "TankBullet");
			pociski.push_back(bullet7);
			bullet7->Move(vec3(0, 0, -1));
		}

		else if (temp == 8) {
			fastBulletTimeRemain.push_back(bulletMaxTime);
			GameObject* bullet8 = Gra->Create(pos, rot - vec3(0, 315, 0), vec3(1.0f), "TankBullet");
			pociski.push_back(bullet8);
			bullet8->Move(vec3(0, 0, -1));
		}
	}

	void enemyShoot(GameObject* enemy, Game* Gra) {
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
				shot(enemy->Transform.position + vec3(0, 2.535, 0), enemy->Transform.orientation, Gra);
			}
			else if(enemyType[enemyIndex]==2){
				shot_fast(enemy->Transform.position + vec3(0, 2.12, 0), enemy->Transform.orientation, Gra);
			}
			else if (enemyType[enemyIndex] == 3) {
				shot_leonardo(enemy->Transform.position + vec3(0,.6, 0), enemy->Transform.orientation, Gra);
			}
			enemyShotCooldowns[enemyIndex] = 4.0f;
		}
	}

	void spawn_enemy(vec3 pos, vec3 rot, Game* Gra, int type) {
		if (type == 1) {
			GameObject* enemy = Gra->Create(pos, rot, vec3(1.0f), "Tank");
			przeciwnicy.push_back(enemy);
			enemyType.push_back(type);
			enemyShotCooldowns.push_back(0.0f);

			GameObject* rPointer = Gra->Create(vec3(0.0f), vec3(0.0f), vec3(rPointerScaleDefault), "RadarX");
			radarElements.push_back(rPointer);
			radarElementsType.push_back(0);
			uiElements.push_back(rPointer);
			targetPos.push_back(vec3(0.0f));
			targetOri.push_back(vec3(0.0f));
		}
		else if (type == 2) {
			GameObject* enemy = Gra->Create(pos, rot, vec3(2.0f), "FastTank");
			przeciwnicy.push_back(enemy);
			enemyType.push_back(type);
			enemyShotCooldowns.push_back(0.0f);

			GameObject* rPointer = Gra->Create(vec3(0.0f), vec3(0.0f), vec3(rPointerScaleBig), "RadarX");
			radarElements.push_back(rPointer);
			radarElementsType.push_back(0);
			uiElements.push_back(rPointer);
			targetPos.push_back(vec3(0.0f));
			targetOri.push_back(vec3(0.0f));
		}
		else if (type == 3) {
			GameObject* enemy = Gra->Create(pos, rot, vec3(1.0f), "LeonardoTank");
			przeciwnicy.push_back(enemy);
			enemyType.push_back(type);
			enemyShotCooldowns.push_back(0.0f);

			GameObject* rPointer = Gra->Create(vec3(0.0f), vec3(0.0f), vec3(rPointerScaleDefault), "AsteroidsStar");
			rPointer->color = vec3(0, 1, 0);
			radarElements.push_back(rPointer);
			radarElementsType.push_back(0);
			uiElements.push_back(rPointer);
			targetPos.push_back(vec3(0.0f));
			targetOri.push_back(vec3(0.0f));
		}
		else if (type == 4) {
			GameObject* enemy = Gra->Create(pos, rot, vec3(1.0f), "Rocket");
			przeciwnicy.push_back(enemy);
			enemyType.push_back(type);
			enemyShotCooldowns.push_back(0.0f);

			GameObject* rPointer = Gra->Create(vec3(0.0f), vec3(0.0f), vec3(rPointerScaleDefault), "AsteroidsStar");
			rPointer->color = vec3(0, 1, 0);
			radarElements.push_back(rPointer);
			radarElementsType.push_back(0);
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
	const float planeBounds = 175.0f;

	GameObject* horizon;

	void makeHorizon() {
		std::vector<float> vx;
		std::vector<unsigned int> ind;

		const float distance = 60.0f;
		const float maxMountainHeight = 12.5f;
		const float minMountainHeight = 5.0f;
		const int mountainNumber = 15;

		const int moonPointsNumber = 10;
		const float moonRadius = 2.5f;
		const float moonAboveMountains = 5.0f; //how high the moon is above the mountains

		for (int i = 0; i < mountainNumber; i++) {
			double angle = 2 * PI * i / mountainNumber;
			double nextAngle = 2 * PI * (i + 1) / mountainNumber;
			
			float yPos = minMountainHeight + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (maxMountainHeight - minMountainHeight)));
			push_back3(vx, distance * cos(angle), yPos, distance * sin(angle));
			push_back3(vx, 0, 1, 0);

			yPos = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / minMountainHeight));
			float randAngle = angle + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (nextAngle - angle)));
			push_back3(vx, distance * cos(randAngle), yPos, distance * sin(randAngle));
			push_back3(vx, 0, 1, 0);

			push_back2(ind, 2 * i, 2 * i + 1);
			push_back2(ind, 2 * i + 1, 2 * i + 2);
		}
		ind.pop_back();
		ind.push_back(0);

		int countingOffset = mountainNumber * 2;
		for (int i = 0; i < moonPointsNumber; i++) {
			double angle = 2 * PI * i / moonPointsNumber;

			push_back3(vx, moonRadius * cos(angle), moonAboveMountains + maxMountainHeight + moonRadius * sin(angle), distance);
			push_back3(vx, 0, 1, 0);

			push_back2(ind, countingOffset + i, countingOffset + i + 1);
		}
		ind.pop_back();
		ind.push_back(countingOffset);

		countingOffset += moonPointsNumber;
		for (int i = 0; i < mountainNumber * 2; i++) {
			double angle = PI * i / mountainNumber;

			push_back3(vx, distance * cos(angle), 0, distance * sin(angle));
			push_back3(vx, 0, 1, 0);

			push_back2(ind, countingOffset + i, countingOffset + i + 1);
		}
		ind.pop_back();
		ind.push_back(countingOffset);

		horizon = Gra->Create(vec3(0), vec3(0), vec3(1), vx, ind);
	}

	//where is radar?

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
				double angle = 2 * PI * j / ver;

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

		for (auto const& point : points) {
			vx.push_back(point.x);
			vx.push_back(point.y);
			vx.push_back(point.z);

			vx.push_back(0);
			vx.push_back(1);
			vx.push_back(0);
		}

		obstacles.push_back(Gra->Create(vec3(x, 0.0f, z), vec3(0.0f, rand() % 360, 0.0f), vec3(minScale + (float)(rand()) / ((float)(RAND_MAX / (maxScale - minScale)))), vx, ind));
		
		GameObject* rPointer = Gra->Create(vec3(0.0f), vec3(0.0f), vec3(rPointerScaleDefault), "RadarT");
		radarElements.push_back(rPointer);
		radarElementsType.push_back(1);
		uiElements.push_back(rPointer);
		targetPos.push_back(vec3(0.0f));
		targetOri.push_back(vec3(0.0f));
	}

	const float pUScale = 1.15f;
	const float pUIRotationSpeed = 36.0f;
	float pUBRotationSpeed = 12.0f; //not const, cuz might be changed in init(), depending on the value of pUSameDirectionRotation
	const float pUowYOffset = 1.0f;
	const bool pUSameDirectionRotation = false;
	const float pUAFCTime2 = .5f;

	const std::string pUModels[] = { "Speed","Heart","Reload","Star","XP","Boost" };
	std::vector<GameObject*> powerUpInside;
	std::vector<GameObject*> powerUpBox;
	std::vector<GameObject*> powerUpAnimation;
	std::vector<int> powerUpType; // 0 - speed, 1 - life, 2 - decrease reload time, 3 - increase score multiplier, 4 - increase score (one-time), 5 - boost (no idea for it's purpose)

	void createPowerUp(float x, float y, float z, int type) {
		powerUpInside.push_back(Gra->Create(vec3(x,y,z), vec3(0.0f), vec3(pUScale), "PowerUp"+pUModels[type]));
		powerUpBox.push_back(Gra->Create(vec3(x,y,z), vec3(0.0f), vec3(pUScale), "PowerUpBox"));
		powerUpType.push_back(type);

		if (type == 2) {
			GameObject* obj = Gra->Create(vec3(x, y, z), vec3(0.0f), vec3(pUScale), "Arrow0");
			obj->AddStage("Arrow1");
			obj->AddStage("Arrow2");
			obj->AddStage("Arrow3");
			powerUpAnimation.push_back(obj);
		}

		GameObject* rPointer = Gra->Create(vec3(0.0f), vec3(0.0f), vec3(rPointerScaleDefault), "MenuSquare");
		radarElements.push_back(rPointer);
		radarElementsType.push_back(2);
		uiElements.push_back(rPointer);
		targetPos.push_back(vec3(0.0f));
		targetOri.push_back(vec3(0.0f));
	}

	const float pUdYoTU = pUowYOffset / (.25f * fullRotationTime); //at least im aware that i suck at naming things
	float currentPUdYoTU = pUdYoTU; //...

	// bro AT LEAST LEAVE A COMMENT 😫 //nuh
}
using namespace Battlezone;

void Game::BattlezoneInit() {
	shot_cool = 2;
	resp_cool = 2;
	lives = 3;

	glitchEffectRefreshRate = .1f;
	__lines.clear();

	isUfo = false;
	ufoCooldown = 35.0f;
	ufoMovesLeft = rand() % 3 + 1;
	ufoTargetPos = vec2(rand() % 2 * mapSize - mapSize, rand() % 2 * mapSize - mapSize);

	planeCooldown = 10.0f;
	isPlane = false;
	planeStartCoords = vec2(-1000, -1000);

	player = Create(vec3(0.0f), vec3(0.0f), vec3(1.0f), "Tank");
	ufo = Create(vec3(-1000.0f), vec3(0.0f), vec3(1.0f), "Ufo");
	plane = Create(vec3(-1000.0f, 1000, -1000.0f), vec3(0.0f), vec3(1.0f), "BattlezonePlane");

	//making radar , oh I found it
	std::vector<float> rVx;
	std::vector<unsigned int> rInd;

	//nvm I think it's not here
	
	push_back3(rVx, 0);
	push_back3(rVx, 0, 1, 0);

	for (int i = 0; i < radarPoints; i++) {
		float _angle = 2 * PI * i / radarPoints;
		rVx.push_back(radarRadius * cos(_angle));
		rVx.push_back(radarRadius * sin(_angle));
		rVx.push_back(0);

		push_back3(rVx, 0, 1, 0);

		push_back2(rInd, i+1, i+2);
	}
	rInd.pop_back();
	rInd.push_back(1);

	push_back3(rVx, radarRadius * cos(3*PI/4), radarRadius * sin(3*PI/4), 0);
	push_back3(rVx, 0, 1, 0);

	push_back3(rVx, radarRadius * cos(PI/4), radarRadius * sin(PI/4), 0);
	push_back3(rVx, 0, 1, 0);

	push_back2(rInd, 0, radarPoints+1);
	push_back2(rInd, 0, radarPoints+2);

	for (int i = 0; i < 4; i++) {
		float _angle = PI * i / 2;

		float _x = radarRadius * cos(_angle);
		float _y = radarRadius * sin(_angle);

		push_back3(rVx, _x, _y, 0);
		push_back3(rVx, 0, 1, 0);

		switch (i)
		{
		case 0:
			_x -= radarLineLenght;
			break;
		case 1:
			_y -= radarLineLenght;
			break;
		case 2:
			_x += radarLineLenght;
			break;
		case 3:
			_y += radarLineLenght;
			break;
		default:
			throw(std::invalid_argument("how did you manage to go out of bounds of for-loop?!"));
			break;
		}
		
		push_back3(rVx, _x, _y, 0);
		push_back3(rVx, 0, 1, 0);

		push_back2(rInd, radarPoints + 3 + i * 2, radarPoints + 4 + i * 2);
	}

	radar = Create(vec3(0.0f), vec3(0.0f), vec3(.25f), rVx, rInd);

	obstacles.clear();

	spinningLines.clear();

	targetPos.clear();
	targetOri.clear();

	uiElements.clear();

	uiElements.push_back(radar);
	targetPos.push_back(vec3(0.0f));
	targetOri.push_back(vec3(0.0f));

	for (int i = 0; i < trailLinesNo; i++) {
		GameObject* obj = Create(vec3(0.0f), vec3(0.0f, 0.0f, (90.0f + (float)(trailLinesNo) * linesSpace) - (float)(i) * linesSpace), vec3(.25f), std::vector<float>{0, 0, 0, 0, 1, 0, 0, radarRadius, 0, 0, 1, 0}, std::vector<unsigned int>{0, 1});
		float modifier = 1.0f - (float)(i) / (float)(trailLinesNo);
		obj->Stage[0].opacity = modifier; 
		obj->Stage[0].lineWidth = modifier; 
		spinningLines.push_back(obj);
		uiElements.push_back(obj);
		targetPos.push_back(vec3(0.0f));
		targetOri.push_back(vec3(0.0f));
	}
	spinningLines[0]->Stage[0].opacity = 1.25f;
	spinningLines[0]->Stage[0].lineWidth = 1.25f;
	rtp = 0; //was meant to be used for radar, will be used as a global timing unit (no use in radar, used for pu's however)

	przeciwnicy.clear();
	enemyShotCooldowns.clear();
	pociski.clear();

	radarElements.clear();
	radarElementsType.clear();

	powerUpInside.clear();
	powerUpBox.clear();
	powerUpType.clear();
	powerUpAnimation.clear();

	if (!pUSameDirectionRotation) pUBRotationSpeed *= -1;
	currentPUdYoTU = pUdYoTU;
	pU2AnimationCooldown = 0.0f;

	for (auto& current : uiElements) {
		current->Stage[0].onTop = true;
		current->ScaleTo(vec3(uiScale*9,uiScale*16,0));
		current->MoveTo(vec3(0, uiYOffset, 0));
		current->Rotate(vec3(0, 180, 0));
	}

	randomActionTimeLimit.clear();
	randomActionTimeCooldown.clear();
	randomActionType.clear();
	makeHorizon();
}

void Game::Battlezone(float dt) {
	if (!enemyShotCooldowns.empty()) {
		for (auto& cooldown : enemyShotCooldowns) {
			cooldown -= dt;
			if (cooldown < 0) cooldown = 0;
		}
	}
	resp_cool -= dt;
	shot_cool -= dt;

	rtp += dt;
	if (rtp >= fullRotationTime) rtp = 0;
	pU2AnimationCooldown += dt;

	vec3 pPos = player->Transform.position;
	vec3 pOri = player->Transform.orientation;
	vec3 pFront = player->Front;

	//moving the camera
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) camera->RotateCamera(rotationMultiplier * dt * camSpeed, 0);
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) camera->RotateCamera(-rotationMultiplier * dt * camSpeed, 0);
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) camera->RotateCamera(0, -rotationMultiplier * dt * camSpeed);
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) camera->RotateCamera(0, rotationMultiplier * dt * camSpeed);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) camera->MoveCamera(FORWARD, dt / 2 * camSpeed);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) camera->MoveCamera(BACKWARD, dt / 2 * camSpeed);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) camera->MoveCamera(LEFT, dt / 2 * camSpeed);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) camera->MoveCamera(RIGHT, dt / 2 * camSpeed);

	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) camera->Position.y += 2 * camSpeed * dt;
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) camera->Position.y -= 2 * camSpeed * dt;

	if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS) {
		camera->Position = vec3(0.0f, 0.0f, 0.0f);
		camera->Yaw = -90.0f;
		camera->Pitch = 0.0f;
		camera->MoveCamera(FORWARD, 0.0f);
		camera->RotateCamera(0.0f, 0.0f);
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) camSpeed = 2;
	else camSpeed = 1;

	//moving the player
	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) player->Move(vec3(0, 0, -1) * dt * velocity);
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) player->Rotate(vec3(0, 1, 0) * dt * rotationMultiplier1);
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) player->Rotate(vec3(0, -1, 0) * dt * rotationMultiplier1);

	//adjusting the cam's pos
	/*vec3 cPos = normalize(pFront) * camFrontOffset;
	cPos.y += camYOffset;

	camera->Position = pPos+cPos;*/
	//todo: make a WORKING cam rot script

	//shooting funtion
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && shot_cool <= 0 && bulletsFired<=4) {
		shot_cool = 2;
		bulletsFired += 1;
		if (player->Transform.orientation.y != 0 && player->Transform.orientation.y != 180)
			shot(player->Transform.position + vec3(0, 2.535, 0), player->Transform.orientation, this);
		else
			shot(player->Transform.position + vec3(0, 2.535, 1), player->Transform.orientation, this);
	}
	if (!pociski.empty()) {
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
	}
	else if (bulletsFired > 4) {
		if (reloadTime > 0) {
			reloadTime -= dt;
		}
		else
			bulletsFired = 0;
	}

	//moving the horizon
	horizon->MoveTo(pPos);

	//spawning the enemies
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
	if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS && resp_cool <= 0) {
		spawn_enemy(player->Transform.position + vec3(temp_x, 0, temp_z), vec3(0, temp_y, 0), this, 4);
		resp_cool = 2;
	}

//Poruszanie i strzelanie przeciwników
	if (!przeciwnicy.empty()) {
		for (int i = 0; i < przeciwnicy.size(); i++) {
			GameObject* current = przeciwnicy[i];
			vec3 enemyPos = current->Transform.position;
			vec3 direction = normalize(pPos - enemyPos);
			vec3 distance = pPos - enemyPos;

			//random actions
			randomActionTimeCooldown[i] -= dt;
			if (randomActionTimeCooldown[i] <= 0) {
				if(!randomActionType[i]) randomActionType[i] = rand() % 4 + 1;
				randomActionTimeLimit[i] -= dt;
				if (randomActionTimeLimit[i] <= 0) {
					randomActionTimeCooldown[i] = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / maxRandomActionCooldown));
					randomActionTimeLimit[i] = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / maxRandomActionLimit));
					randomActionType[i] = 0;
				}
			}  

			if (collisionCircle(vec2(pPos.x, pPos.z), vec2(enemyPos.x, enemyPos.z))) {
				if (lives > 1) {
					lives -= 1;
				}
				else {
					CreateTekst(vec2(0, 0), 0, vec2(0.1), 2, 1, "UMARLES");
				}
			}

			// calculating rotation angle 
			float _angle;
			switch (randomActionType[i]) {
			case 0:
				_angle = 0;
				current->RotateTo(vec3(0.0f, atan2(direction.x, direction.z) * 180.0f / PI, 0.0f));
				break;
			case 1:
				_angle = -360.0f * rotationsPerSecond * dt; // -360deg * 0.5 = 180deg to the left each second (2s/full rotation)
				break;
			case 2:
				_angle = 360.0f * rotationsPerSecond * dt; // 360deg * 0.5 = 180deg to the right each second
				break;
			default:
				_angle = 0; // for 3 and 4 - no rotation
			}

			current->Rotate(vec3(0, _angle, 0));

			// bro what xDD
			// i'd assume that this is supposed to move the tanks, rigth?
			if (distance.x * distance.x + distance.z * distance.z > 225 && !randomActionType[i] || randomActionType[i] == 3) { //move only for case 0 or 3
				float _sM = fast_tank_speed;
				if (enemyType[i] != 2) _sM = tank_speed;
				current->MoveGlobal(direction * dt * _sM);
				if (enemyShotCooldowns[i] <= 0) {
					enemyShoot(current, this);
					enemyShotCooldowns[i] = 4.0;
				}
			}
			else if (enemyShotCooldowns[i] <= 0 && current->Transform.orientation.y != 0 && current->Transform.orientation.y != 180) {
				enemyShoot(current, this);
				enemyShotCooldowns[i] = 4.0;
			}
		}
	}

	//power-ups' animations
	bool isTimestamp = false;
	if (rtp > .25f * fullRotationTime && rtp < .75f * fullRotationTime) isTimestamp = true;
	
	currentPUdYoTU = pUdYoTU;
	if (!isTimestamp) currentPUdYoTU = -pUdYoTU;

	int pUAnimationIt = 0;
	for (int i = 0; i < powerUpInside.size(); i++) {
		GameObject* inside = powerUpInside[i];
		GameObject* box = powerUpBox[i];

		//rotato :D
		inside->Rotate(vec3(0, 1, 0) * pUIRotationSpeed * dt);
		box->Rotate(vec3(0, 1, 0) * pUBRotationSpeed * dt);

		//up-down thing (?)
		inside->Move(vec3(0, 1, 0) * dt * currentPUdYoTU);
		box->MoveTo(inside->Transform.position);

		//aniamation
		if (powerUpType[i] == 2) {
			GameObject* animation = powerUpAnimation[pUAnimationIt];
			animation->Rotate(vec3(0, 1, 0) * pUIRotationSpeed * dt);
			animation->MoveTo(inside->Transform.position);

			if (pU2AnimationCooldown >= pUAFCTime2) {
				pU2AnimationCooldown = 0.0f;
				
				int aState = animation->activeStage;
				if (++aState > 3) aState = 0;
				animation->activeStage = aState;
			}

			pUAnimationIt++;
		}
	}

	//ufo
	if (!isUfo) ufoCooldown -= dt;
	if (ufoCooldown <= 0 && !isUfo) {
		ufo->MoveTo(vec3(rand() % 2 * mapSize - mapSize, planeHeight, rand() % 2 * mapSize - mapSize));
		isUfo = true;
	}
	if (isUfo) {
		vec3 ufoPos = ufo->Transform.position;
		if (ufoPos.y > 0.0f)
			ufo->Transform.position.y -= ufoSpeed * dt;
		else if (ufoPos.y < 0.0f) 
			ufo->Transform.position.y = 0;
		else {
			if (ufoPos.x > ufoTargetPos.x - 5.0f && ufoPos.x < ufoTargetPos.x + 5.0f && ufoPos.z > ufoTargetPos.y - 5.0f && ufoPos.z < ufoTargetPos.y + 5.0f) {
				if (ufoMovesLeft > 0) {
					ufoMovesLeft -= 1;
					ufoTargetPos = vec2(rand() % 2*mapSize - mapSize, rand() % 2 * mapSize - mapSize);
				}
				else {
					isUfo = false;
					ufo->Transform.position = vec3(-1000, planeHeight, -1000);
					ufoCooldown = 35.0f; 
				}
			}
			else {
				vec2 dPos = vec2(ufoTargetPos - vec2(ufoPos.x, ufoPos.z));
				if (dPos.x > ufoSpeed) dPos.x = ufoSpeed;
				else if (dPos.x < -ufoSpeed) dPos.x = -ufoSpeed;
				if (dPos.y > ufoSpeed) dPos.y = ufoSpeed;
				else if (dPos.y < -ufoSpeed) dPos.y = -ufoSpeed;
				ufo->Move(vec3(dPos.x,0.0f,dPos.y) * dt);
			}
		}
	}

	//moving the plane
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
		_angle = _angle * 180.0f / PI;
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

	//adjusting ui elements' pos
	//adjusting scanner lines' rotation
	for (int i = 0; i < spinningLines.size(); i++) {
		GameObject* line = spinningLines[i];

		line->Rotate(vec3(0, 0, 360.0f / fullRotationTime * dt));
		targetOri[i + 1] = line->Transform.orientation * vec3(0, 0, 1);
	}

	//adjusting scanner elements' position... or do we? *vsauce music*
	//(YES WE NEED TO DO IT SOMEONE PLEASE SEND PROFFESIONAL PSYCHICAL HELP) *intense music*
	//Profesional help decending from sky
	//Don't warry, be happy

	const float radarRange = 10.0f; // todo: move it somewhere else

	float angleRad = pOri.y * PI / 180.0f;
	unsigned int radarElementsIterator[] = { 0,0,0,0 }; // 0 - normal / big / vinci, 1 - obstacle, 2 - boost, 3 - intercontinental ballistic missile (aka rocket)
	for (int i = 0; i < radarElements.size(); i++) {
		int type = radarElementsType[i];
		unsigned int& iterator = radarElementsIterator[type];

		GameObject* current;

		if (type == 0 && !przeciwnicy.empty()) current = przeciwnicy[iterator];
		else if (type == 1 && !obstacles.empty()) current = obstacles[iterator];
		else if (type == 2 && !powerUpInside.empty()) current = powerUpInside[iterator];
		//else if (type == 3 && !rockets.empty()) current = rockets[iterator];
		else throw std::invalid_argument("check deez values mate");

		float dx = current->Transform.position.x - pPos.x;
		float dz = current->Transform.position.z - pPos.z;

		// github copilot moment
		//dx = dx * cos(angleRad) - dz * sin(angleRad);
		//dz = dx * sin(angleRad) + dz * cos(angleRad);

		// todo: discover why it looks wrong without radarRange
		//dx = (dx / mapSize) * radarRadius / radarRange;
		//dz = (dz / mapSize) * radarRadius / radarRange;

		/*dx /= 50;
		dz /= 50;

		float dist = dx * dx + dz * dz;

		if (dist >= 1) {
			radarElements[iterator]->Stage[radarElements[iterator]->activeStage].opacity = 0;
			continue;
		}
		else {
			radarElements[iterator]->Stage[radarElements[iterator]->activeStage].opacity = 1;
		}

		float deg = glm::dot(player->Front, vec3(dx, 0, dz));

		

		//bomboclat - Adam Cisek

		dist *= radarRadius;

		dx = dist * cos(deg);
		dz = dist * sin(deg);*/

		dx /= 10;
		dz /= 10;



		//todo: add out-of-bounds checking condition

		//targetPos[1 + trailLinesNo + i].x = dx; 
		//targetPos[1 + trailLinesNo + i].y = dz;
		//targetPos[1 + trailLinesNo + i].z = 0.0f;
		
		// fun fact: kamil forgot to add this line so I spent like 2 hours trying to figure out why the radar is broken
		//radarElements[iterator]->MoveTo(vec3(-dx, uiYOffset + dz, 0.0f));
		radarElements[iterator]->MoveTo(vec3(radar->Transform.position.x + dx, radar->Transform.position.y + dz, 0));
		
		iterator++;
	}

	//checking if out of bounds
	vec2 absPPos = vec2(abs(pPos.x), abs(pPos.z)); // bro really said PP
	float dOutofbounds;
	if (absPPos.x > mapSize || absPPos.y > mapSize) {
		glitchEffectRefreshRate -= dt;
		
		float isNeg = 1.0f;
		if (absPPos.x > mapSize) {
			if (pPos.x < 0) isNeg = -1.0f;
			dOutofbounds = (absPPos.x - mapSize) / maxOutOfBoundsDistance;
			if (dOutofbounds > 1.0f) player->Transform.position.x = (mapSize + maxOutOfBoundsDistance)*isNeg;
		}
		else {
			if (pPos.z < 0) isNeg = -1.0f;
			dOutofbounds = (absPPos.y - mapSize) / maxOutOfBoundsDistance;
			if (dOutofbounds > 1.0f) player->Transform.position.z = (mapSize + maxOutOfBoundsDistance)*isNeg;
		}

		if (glitchEffectRefreshRate <= 0) {
			for (auto& currentLine : __lines) {
				Destroy(currentLine);
			}
			__lines.clear();

			glitchEffectRefreshRate = .1f;
			for (int i = 0; i < (int)(dOutofbounds * maxGlitchLinesNumber); i++) {
				GameObject* current = Gra->Create(vec3(0), vec3(0), vec3(1.0f+static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1.1f - 1.0f)))), std::vector<float>{-5+static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (5 - -5))), 0, 0, 0, 1, 0, -5+static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (5 - -5))), 0, 0, 0, 1, 0}, std::vector<unsigned int>{0, 1});
				current->Stage[0].onTop = true;
				current->MoveTo(vec3(-1.1f+ static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1.1f - -1.1f))), -1.1f+static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1.1f - -1.1f))),-.1));
				__lines.push_back(current);
			}
		}
	}

	//debug ↓
	if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS && resp_cool <= 0) {
		resp_cool = .5f;
		makeObstacles(rand() % 200 - 100, rand() % 200 - 100, 6);
	}
	if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS && resp_cool <= 0) {
		resp_cool = .5f;
		createPowerUp(rand() % 100 - 50, 0, rand() % 100 - 50, rand()%(sizeof(pUModels)/sizeof(std::string)));
	}

	if (glfwGetKey(window, GLFW_KEY_KP_0) == GLFW_PRESS && resp_cool <= 0) {
		resp_cool = .1f;
		camera->Position = vec3(0, planeHeight, 0);
	}
	if (glfwGetKey(window, GLFW_KEY_KP_5) == GLFW_PRESS && resp_cool <= 0) {
		resp_cool = .1f;
		player->Transform.position = vec3(120,0,-120);
		camera->Position = vec3(120,0,-120);
	}

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		Game::ChangeState(Game_Menu);
}