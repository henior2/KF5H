#include "Battlezone.h"
#include "The real engine/The Real Engine.h"

void Battlezone::Init() {
	SetCursor(NULL);
	thePointer = Game::Create(vec(0,3), vec(0,3), vec(.1,3), L"battlezonePointer");
	thePointer->Stage[0].onTop = true;

	keyCooldown = .25f;
	money = 0;
	

	shopDisplaySquares[0] = Game::Create(vec(-shopXPos, shopYPos, 0), vec(0,3), vec(shopScale,3), L"MenuSquare");
	shopDisplaySquares[1] = Game::Create(vec(-shopXPos, 0, 0), vec(0,3), vec(shopScale,3), L"MenuSquare");
	shopDisplaySquares[2] = Game::Create(vec(-shopXPos, -shopYPos, 0), vec(0,3), vec(shopScale,3), L"MenuSquare");

	shopDisplaySquares[0]->Stage[0].onTop = true; 
	shopDisplaySquares[1]->Stage[0].onTop = true; 
	shopDisplaySquares[2]->Stage[0].onTop = true;

	insertItem(0, true); insertItem(1, true); insertItem(2, true);

	flag = false;
	velocity = 3.0f;
	camSpeed = 1;
	timeMultiplier = 1.0f;
	isMissleSelfTargeting = false;
	timeEffectLeft = 0;
	score = 0;

	scoreMultiplier = 1;
	wavePoints = 1;
	waveFlag = false;
	waveTime = 4.0f;
	bulletsFired = 0;
	TextBox* fala = Game::AddText(-1.0f,0,1.0f,0.9f, L"FALA 0", 0.1f);
	glitchEffectRefreshRate = .1f;

	shot_cool = 2.0f;
	resp_cool = 2.0f;
	hp = 100;
	TextBox* display_hp = Game::AddText(0.6f, 0, 1.0f, 0.9f, std::to_wstring(hp),0.1f);
	isDead = false;
	endingScreen = false;

	new_username = "";



	isUfo = false;
	ufoCooldown = 35.0f;
	ufoMovesLeft = rand() % 3 + 1;
	ufoTargetPos = vec(rand() % 2 * mapSize - mapSize, rand() % 2 * mapSize - mapSize,0);

	planeCooldown = 10.0f;
	isPlane = false;
	planeStartCoords = vec(-1000, 1000, -1000);

	player = Game::Create(vec(0.0f,3), vec(0.0f,3), vec(1.0f,3), L"Tank");
	ufo = Game::Create(vec(-1000.0f,3), vec(0.0f,3), vec(1.0f,3), L"Ufo");
	plane = Game::Create(vec(-1000.0f, 1000, -1000.0f), vec(0.0f,3), vec(1.0f,3), L"BattlezonePlane");

	//making radar , oh I found it
	std::vector<float> rVx;
	std::vector<unsigned int> rInd;

	//Score
	std::wstring scoreStr = std::to_wstring(score);
	while (scoreStr.length() < 3) {
		scoreStr = L"0" + scoreStr;
	}
	TextBox* tScore = Game::AddText(-0.1f, 0, 1.0f, 0.9f, scoreStr,.1f);

	//nvm I think it's not here //bro's having a bipolar disorder 💀

	push_back3(rVx, 0);
	push_back3(rVx, 0, 1, 0);

	for (int i = 0; i < radarPoints; i++) {
		float _angle = (float)(2 * M_PI * i / radarPoints);
		rVx.push_back(radarRadius * cos(_angle));
		rVx.push_back(radarRadius * sin(_angle));
		rVx.push_back(0);

		push_back3(rVx, 0, 1, 0);

		push_back2(rInd, i + 1, i + 2);
	}
	rInd.pop_back();
	rInd.push_back(1);

	push_back3(rVx, radarRadius * (float)cos(3 * M_PI / 4), radarRadius * (float)sin(3 * M_PI / 4), 0);
	push_back3(rVx, 0, 1, 0);

	push_back3(rVx, radarRadius * (float)cos(M_PI / 4), radarRadius * (float)sin(M_PI / 4), 0);
	push_back3(rVx, 0, 1, 0);

	push_back2(rInd, 0, radarPoints + 1);
	push_back2(rInd, 0, radarPoints + 2);

	for (int i = 0; i < 4; i++) {
		float _angle = (float)(M_PI * i / 2);

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
		}

		push_back3(rVx, _x, _y, 0);
		push_back3(rVx, 0, 1, 0);

		push_back2(rInd, radarPoints + 3 + i * 2, radarPoints + 4 + i * 2);
	}

	radar = Game::Create(vec(0.0f,3), vec(0.0f,3), vec(.25f,3), rVx, rInd);

	

	uiElements.push_back(radar);
	targetPos.push_back(vec(0.0f,3));
	targetOri.push_back(vec(0.0f,3));

	for (int i = 0; i < trailLinesNo; i++) {
		GameObject* obj = Game::Create(vec(0.0f,3), vec(0.0f, 0.0f, (90.0f + (float)(trailLinesNo)*linesSpace) - (float)(i)*linesSpace), vec(.25f,3), std::vector<float>{0, 0, 0, 0, 1, 0, 0, radarRadius, 0, 0, 1, 0}, std::vector<unsigned int>{0, 1});
		float modifier = 1.0f - (float)(i) / (float)(trailLinesNo);
		obj->SetColor(vec(0, 1, 0) * modifier);
		obj->Stage[0].lineWidth = modifier;
		spinningLines.push_back(obj);
		uiElements.push_back(obj);
		targetPos.push_back(vec(0.0f,3));
		targetOri.push_back(vec(0.0f,3));
	}
	spinningLines[0]->Stage[0].opacity = 1.25f;
	spinningLines[0]->Stage[0].lineWidth = 1.25f;
	rtp = 0; //was meant to be used for radar, will be used as a global timing unit (no use in radar, used for pu's however)

	//przeciwnicy.clear();
	//enemyShotCooldowns.clear();
	//pociski.clear();

	//radarElements.clear();
	//radarElementsType.clear();

	//powerUpInside.clear();
	//powerUpBox.clear();
	//powerUpType.clear();
	//powerUpAnimation.clear();

	//obstacles.clear();

	//spinningLines.clear();

	//targetPos.clear();
	//targetOri.clear();

	//uiElements.clear();

	//__lines.clear();	
	//
	//bulletTimeRemain.clear();
	//pociski_gracza.clear();

	//randomActionTimeLimit.clear();
	//randomActionTimeCooldown.clear();
	//randomActionType.clear();

	if (!pUSameDirectionRotation) pUBRotationSpeed *= -1;
	currentPUdYoTU = pUdYoTU;
	pU2AnimationCooldown = 0.0f;

	for (auto& current : uiElements) {
		current->Stage[0].onTop = true;
		current->ScaleTo(vec(uiScale, uiScale, 0));
		current->MoveTo(vec(0, uiYOffset, 0));
		current->Rotate(vec(0, 0, 0));
		current->Transform.UseUniversalUnits = true;
	}



	//creating obstacles
	for (int i = 0; i < totalObstacles; i++) {
		makeObstacles(-mapSize + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (2 * mapSize))), -mapSize + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (2 * mapSize))), minObstacleHeight + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (maxObstacleHeight - minObstacleHeight))));
		// x = [-125 ; 125] 
		// z = [-125 ; 125]
		// h = [  5  ;  7 ]
	}
}

void Battlezone::Update(float dt) {

	for (auto& current : uiElements) {
		current->Stage[0].onTop = true;
		current->ScaleTo(vec(uiScale, uiScale, 0));
		current->MoveTo(vec(0, uiYOffset, 0));
		current->Rotate(vec(0, 0, 0));
		current->Transform.UseUniversalUnits = true;
	}

	dt *= timeMultiplier;
	keyCooldown -= dt;
	timeEffectLeft -= dt;
	resp_cool -= dt;
	shot_cool -= dt;
	rtp += dt;
	pU2AnimationCooldown += dt;

	if (hp <= 0) {
		
		display_hp->ChangeText(L"You died");
		isDead = true;
	}

	if (isDead && !endingScreen) {
		//todo: add
	}
	if (endingScreen) {
		//todo: add
	}

	if (Game::KeysPresed[VK_ESCAPE])
	{
		Game::ChangeState(Game_Menu);
		return;
	}

	if (isDead) return; //this stops the game after death. anything above still will be executed

	

	if (timeEffectLeft <= 0) {
		timeEffectLeft = 0;
		timeMultiplier = 1.0f;
	}


	if (!enemyShotCooldowns.empty()) {
		for (auto& cooldown : enemyShotCooldowns) {
			if(cooldown>0)
				cooldown -= dt;
			else	
				cooldown = 0;
		}
	}

	if (rtp >= fullRotationTime) 
		rtp = 0;

	//moving the camera
	
		player->Stage[0].opacity = 0.0f;
		//moving the player
		if (Game::KeysPresed['W']) player->Move(vec(0, 0, -1) * dt * velocity);
		if (Game::KeysPresed['S']) player->Move(vec(0, 0, 1) * dt * velocity);
		if (Game::KeysPresed['A']) player->Rotate(vec(0, 1, 0) * dt * rotationMultiplier1);
		if (Game::KeysPresed['D']) player->Rotate(vec(0, -1, 0) * dt * rotationMultiplier1);

		vec pPos = vec(player->Transform.position);
		vec pOri = vec(player->Transform.orientation);
		vec pFront = vec(player->Front);


		//Game::camera->Front = pFront;
		Game::camera->Position = pPos + vec(0, camYOffset, 0);
		Game::camera->MoveCamera(FORWARD, 0.05);
		Game::camera->Yaw = (-1 * pOri.y) + 90.0f;
		Game::camera->Pitch = 0.0f;
		Game::camera->RotateCamera(0, 0);


	//shoting funtion
		if (Game::KeysPresed[VK_SPACE] && shot_cool <= 0 && bulletsFired <= 4) {
		shot_cool = 2.0f;
		bulletsFired += 1;
		if (player->Transform.orientation.y != 0 && player->Transform.orientation.y != 180)
			shot(player->Transform.position + vec(0, 2.535, 0), player->Transform.orientation, true);
		else
			shot(player->Transform.position + vec(0, 2.535, 1), player->Transform.orientation, true);
	}

	//Moving the bullets
		for (int i = 0; i < pociski.size(); i++) {

			fastBulletTimeRemain[i] -= dt;
			if (fastBulletTimeRemain[i] <= 0) {
				Game::Destroy(pociski[i]);
				pociski.erase(pociski.begin() + i);
				fastBulletTimeRemain.erase(fastBulletTimeRemain.begin() + i);
				i--;
				continue;
			}
			pociski[i]->Move(vec(0, 0, -1) * bulletSpeed * dt);

			//Player bullet collsion
			if (Game::checkCollisions(player,pociski[i], vec(1, 0, 1))) {
				hp -= 25;
				display_hp->ChangeText(std::to_wstring(hp));
				Game::Destroy(pociski[i]);
				pociski.erase(pociski.begin() + i);
				fastBulletTimeRemain.erase(fastBulletTimeRemain.begin() + i);
			}
		}



	//moving the forza horizon


	//spawning the enemies
	float temp_x = (float)(rand() % 51);
	float temp_z = (float)(rand() % 51);
	float temp_y = (float)(rand() % 361);


	if (waveTime <= 0) waveFlag = true;
	else waveTime -= dt;

	if (waveFlag && przeciwnicy.empty() && rakiety.empty() && wavePoints == 0) {
		fala->ChangeText(L"WAVE" + std::to_wstring(wavePoints));
		wavePoints += 1; 
		waveFlag = false;
		wave(wavePoints, dt);
		waveTime = 4.0f;
	}

	//Poruszanie i strzelanie przeciwnikśw
	if (!przeciwnicy.empty()) {
		for (int i = 0; i < przeciwnicy.size(); i++) {
			GameObject* current = przeciwnicy[i];
			vec enemyPos = vec(current->Transform.position.x,0,current->Transform.position.z);
			vec direction = ((pPos - enemyPos).Normalize(),0,0);
			vec distance = vec(pPos - enemyPos);

			//random actions
			randomActionTimeCooldown[i] -= dt;
			if (randomActionTimeCooldown[i] <= 0) {
				if (!randomActionType[i]) randomActionType[i] = rand() % 4 + 1;
				randomActionTimeLimit[i] -= dt;
				if (randomActionTimeLimit[i] <= 0) {
					randomActionTimeCooldown[i] = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / maxRandomActionCooldown));
					randomActionTimeLimit[i] = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / maxRandomActionLimit));
					randomActionType[i] = 0;
				}
			}

			// calculating rotation angle 
			float _angle;
			switch (randomActionType[i]) {
			case 0:
				_angle = 0;
				current->RotateTo(vec(0.0f, atan2(direction.x, direction.z) * 180.0f / M_PI, 0.0f));
				break;
			case 1:
				_angle = -360.0f * rotationsPerSecond * dt; // -360deg * 0.5 = 180deg to the left each second (2s/full rotation)
				break;
			case 2:
				_angle = 360.0f * rotationsPerSecond * dt; // 360deg * 0.5 = 180deg to the right each second
				break;
			default:
				_angle = 0; // for 3 and 4 - no rotation
				break;
			}

			current->Rotate(vec(0, _angle, 0));

			// bro what xDD
			// i'd assume that this is supposed to move the tanks, right?
			if (distance.x * distance.x + distance.z * distance.z > 225 && !randomActionType[i] || randomActionType[i] == 3) { //move only for case 0 or 3
				float _sM = fast_tank_speed;
				if (enemyType[i] != 2) _sM = tank_speed;
				current->MoveGlobal(direction * dt * _sM);
				if (enemyShotCooldowns[i] <= 0) {
					enemyShoot(current);
					enemyShotCooldowns[i] = 4.20f;
				}
			}
			else if (enemyShotCooldowns[i] <= 0 && current->Transform.orientation.y != 0 && current->Transform.orientation.y != 180) {
				enemyShoot(current);
				enemyShotCooldowns[i] = 4.20f;
			}
		}
	}

	// moving the rockets
	if (!rakiety.empty()) {
		for (int i = 0; i < rakiety.size(); i++) {
			GameObject* current = rakiety[i];
			vec enemyPos = vec(current->Transform.position.x,0,current->Transform.position.z);
			vec direction = (vec(pPos - enemyPos).Normalize(),0,0);
			vec distance = vec(pPos - enemyPos);

			// calculating rotation angle 
			current->RotateTo(vec(0.0f, atan2(direction.x, direction.z) * 180.0f / M_PI, 0.0f));

			// move the shit
			current->Move(vec(0, 0, -1) * rocket_speed * dt);

			//Player rocket collision
			if (Game::checkCollisions(current,player,vec(1,0,1))) {
				Game::Destroy(current);
				Game::Destroy(radarElements[i]);
				rakiety.erase(rakiety.begin() + i);
				enemyShotCooldowns.erase(enemyShotCooldowns.begin() + i);
				enemyType.erase(enemyType.begin() + i);

				radarElements.erase(radarElements.begin() + i);
				radarElementsType.erase(radarElementsType.begin() + i);
				uiElements.erase(uiElements.begin() + i);

				hp -= 50;
				display_hp->ChangeText(std::to_wstring(hp));
			}
		}
	}

	//Moving player bullets

	if (bulletsFired > 4) {
		if (reloadTime > 0) {
			reloadTime -= dt;
		}
		else
			bulletsFired = 0;
	}

		for (int i = 0; i < pociski_gracza.size(); i++) {
			

			bulletTimeRemain[i] -= dt;
			if (bulletTimeRemain[i] <= 0) {
				Game::Destroy(pociski_gracza[i]);
				pociski_gracza.erase(pociski_gracza.begin() + i);
				bulletTimeRemain.erase(bulletTimeRemain.begin() + i);
				i--;
				continue;
			}
			pociski_gracza[i]->Move(vec(0, 0, -1) * bulletSpeed * dt);

			//Enemy bullet collision
			if (!przeciwnicy.empty()) {
				for (int j = 0; j < przeciwnicy.size(); j++) {
					if (Game::checkCollisions(pociski_gracza[i],przeciwnicy[j],vec(1,0,1))) {
						switch (enemyType[j]) {
						case 1:
							score += 100 * (int)scoreMultiplier;
							tScore = refreshText(tScore, score);
							break;
						case 2:
							score += 200 * (int)scoreMultiplier;
							tScore = refreshText(tScore, score);
							break;
						case 3:
							score += 300 * (int)scoreMultiplier;
							tScore = refreshText(tScore, score);
							break;
						default:
							throw std::invalid_argument("There is no such a type of enemy!");
							break;
						}
						//Usuwanie pocisku
						Game::Destroy(pociski_gracza[i]);
						pociski_gracza.erase(pociski_gracza.begin() + i);
						bulletTimeRemain.erase(bulletTimeRemain.begin() + i);
						//Usuwanie przeciwnika
						Game::Destroy(przeciwnicy[j]);
						przeciwnicy.clear();
					}
				}
			}
			//Rocket bullet collision
			if (!rakiety.empty()) {
				for (int j = 0; j < rakiety.size(); j++) {
					if (Game::checkCollisions(pociski_gracza[i],rakiety[j],vec(1,0,1))) {
						score += 500 * (int)scoreMultiplier;
						//Usuwanie pocisku
						Game::Destroy(pociski_gracza[i]);
						pociski_gracza.erase(pociski_gracza.begin() + i);
						bulletTimeRemain.erase(bulletTimeRemain.begin() + i);
						//Usuwanie rakiety
						Game::Destroy(rakiety[j]);
						Game::Destroy(radarElements[j]);
						
						rakiety.erase(rakiety.begin() + j);
						radarElements.erase(radarElements.begin() + j);
						radarElementsType.erase(radarElementsType.begin() + j);
						uiElements.erase(uiElements.begin() + j);
					}
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
		inside->Rotate(vec(0, 1, 0) * pUIRotationSpeed * dt);
		box->Rotate(vec(0, 1, 0) * pUBRotationSpeed * dt);

		//up-down thing (?)
		if (inside->Transform.position.y >= 10) inside->Move(vec(0, -powerUpFallingSpeed, 0) * dt);
		else inside->Move(vec(0, 1, 0) * dt * currentPUdYoTU);

		box->MoveTo(inside->Transform.position);

		//aniamation
		if (powerUpType[i] == 2) {
			GameObject* animation = powerUpAnimation[pUAnimationIt];
			animation->Rotate(vec(0, 1, 0) * pUIRotationSpeed * dt);
			animation->MoveTo(inside->Transform.position);

			if (pU2AnimationCooldown >= pUAFCTime2) {
				pU2AnimationCooldown = 0.0f;

				int aState = animation->activeStage;
				if (++aState > 3) aState = 0;
				animation->activeStage = aState;
			}

			pUAnimationIt++;
		}

		//collisions: player/power-up
		if (Game::checkCollisions(player,inside,vec(1,0,1),true)) collectPowerUp(inside, box, powerUpType[i]); //someone optimize this please xD
	}

	//ufo
	if (!isUfo) ufoCooldown -= dt;
	if (ufoCooldown <= 0 && !isUfo) {
		ufo->MoveTo(vec(rand() % 2 * mapSize - mapSize, planeHeight, rand() % 2 * mapSize - mapSize));
		isUfo = true;
	}
	if (isUfo) {
		vec ufoPos = vec(ufo->Transform.position.x,ufo->Transform.position.y,ufo->Transform.position.z);
		if (ufoPos.y > 0.0f)
			ufo->Transform.position.y -= ufoSpeed * dt;
		else if (ufoPos.y < 0.0f)
			ufo->Transform.position.y = 0;
		else {
			if (ufoPos.x > ufoTargetPos.x - 5.0f && ufoPos.x < ufoTargetPos.x + 5.0f && ufoPos.z > ufoTargetPos.y - 5.0f && ufoPos.z < ufoTargetPos.y + 5.0f) {
				if (ufoMovesLeft > 0) {
					ufoMovesLeft -= 1;
					ufoTargetPos = vec(rand() % 2 * mapSize - mapSize,0, rand() % 2 * mapSize - mapSize);
				}
				else {
					isUfo = false;
					ufo->Transform.position = vec(-1000, planeHeight, -1000);
					ufoCooldown = 35.0f;
				}
			}
			else {
				vec dPos = vec(ufoTargetPos - vec(ufoPos.x,0, ufoPos.z));
				if (dPos.x > ufoSpeed) dPos.x = ufoSpeed;
				else if (dPos.x < -ufoSpeed) dPos.x = -ufoSpeed;
				if (dPos.y > ufoSpeed) dPos.y = ufoSpeed;
				else if (dPos.y < -ufoSpeed) dPos.y = -ufoSpeed;
				ufo->Move(vec(dPos.x, 0.0f, dPos.y) * dt);
			}
		}
	}

	//moving the plane
	if (!isPlane) planeCooldown -= dt;
	if (planeCooldown <= 0) {
		planeStartCoords.x = planeBounds;
		if (rand() % 2) planeStartCoords.x *= -1;
		planeStartCoords.y = rand() % (int)(2 * planeBounds) - planeBounds;

		if (rand() % 2) {
			float temp = planeStartCoords.x;
			planeStartCoords.x = planeStartCoords.y;
			planeStartCoords.y = temp;
		}

		plane->MoveTo(vec(planeStartCoords.x, planeHeight, planeStartCoords.y));

		float _angle;

		dropPos = vec(-dropRadius + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (2 * dropRadius))), -dropRadius + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (2 * dropRadius))),0) + vec(pPos.x,0, pPos.z);

		vec direction = vec(dropPos.x - planeStartCoords.x,0,dropPos.z - planeStartCoords.z).Normalize();
		_angle = atan2(direction.x, direction.y);
		_angle = _angle * 180.0f / (float)M_PI;
		plane->RotateTo(vec(0.0f, _angle, 0.0f));

		isPlane = true;
		planeCooldown = 7.5f;
	}
	if (isPlane) {
		plane->Move(plane->Front * dt * planeSpeedMultiplier);

		vec planePos = vec(plane->Transform.position.x,planeHeight, plane->Transform.position.z);


		if (abs(planePos.x) <= dropRadius || abs(planePos.y) <= dropRadius) {										// d = (a+b)/2 = 0/2 = 0
			createPowerUp(planePos.x, planeHeight, planePos.y, rand() % (sizeof(pUModels) / sizeof(std::string)));	// y = b-d = b
		}																											// |x-d|<=y => |x|<=b

		if (fabs(planePos.x) > planeBounds * 1.25 || abs(planePos.y) > planeBounds * 1.25) {
			plane->MoveTo(vec(-1000, 1000, -1000));
			isPlane = false;
		}
	}

	//adjusting ui elements' pos
	//adjusting scanner lines' rotation
	for (int i = 0; i < spinningLines.size(); i++) {
		GameObject* line = spinningLines[i];

		line->Rotate(vec(0, 0, 360.0f / fullRotationTime * dt));
		targetOri[(size_t)i + 1] = line->Transform.orientation & vec(0, 0, 1);
	}

	//adjusting scanner elements' position

	const float radarRange = 50.0f; // todo: move it somewhere else

	float angleRad = pOri.y * (float)M_PI / 180.0f;
	unsigned int radarElementsIterator[] = { 0,0,0,0 }; // 0 - normal / big / vinci, 1 - obstacle, 2 - boost, 3 - intercontinental ballistic missile (aka rocket)
	if (!radarElementsType.empty()) {
		for (int i = 0; i < radarElementsType.size(); i++) {
			int type = (int)radarElementsType[i];
			unsigned int& iterator = radarElementsIterator[type];

			GameObject* current;

			if (type == 0 && !przeciwnicy.empty()) current = przeciwnicy[iterator];
			else if (type == 1 && !obstacles.empty()) current = obstacles[iterator];
			else if (type == 2 && !powerUpInside.empty()) current = powerUpInside[iterator];
			else if (type == 3 && !rakiety.empty()) current = rakiety[iterator];
			else current = przeciwnicy[iterator];//throw std::invalid_argument("check deez values mate");


			float dx = current->Transform.position.x - pPos.x;
			float dz = current->Transform.position.z - pPos.z;


			float angle = signed_angle_between_vectors(player->Front, vec(dx, 0, dz), vec(0, 1, 0));

			float dist = dx * dx + dz * dz;

			dist = sqrt(dist) / radarRange;

			if (dist >= 1) {
				radarElements[iterator]->SetColor(vec(0, 0, 0));
				iterator++;
				continue;
			}
			else {
				radarElements[iterator]->SetColor(vec(0, 1, 0));
			}

			angle = angle*M_PI/180.0f;
			dx = dist * sin(angle);
			dz = -dist * cos(angle);

			dx *= 0.11f;
			dz *= 0.11f;

			radarElements[iterator]->MoveTo(vec(radar->Transform.position.x + dx, radar->Transform.position.y + dz, 0));

			iterator++;

			//checking if out of bounds
			vec absPPos = vec(abs(pPos.x),0, abs(pPos.z)); // bro really said PP
			float dOutofbounds;
			if (absPPos.x > mapSize || absPPos.y > mapSize) {
				glitchEffectRefreshRate -= dt;

				float isNeg = 1.0f;
				if (absPPos.x > mapSize) {
					if (pPos.x < 0) isNeg = -1.0f;
					dOutofbounds = (absPPos.x - mapSize) / maxOutOfBoundsDistance;
					if (dOutofbounds > 1.0f) player->Transform.position.x = (mapSize + maxOutOfBoundsDistance) * isNeg;
				}
				else {
					if (pPos.z < 0) isNeg = -1.0f;
					dOutofbounds = (absPPos.y - mapSize) / maxOutOfBoundsDistance;
					if (dOutofbounds > 1.0f) player->Transform.position.z = (mapSize + maxOutOfBoundsDistance) * isNeg;
				}

				if (glitchEffectRefreshRate <= 0) {
					for (auto& currentLine : __lines) {
						Game::Destroy(currentLine);
					}
					__lines.clear();

					glitchEffectRefreshRate = .1f;
					for (int i = 0; i < (int)(dOutofbounds * maxGlitchLinesNumber); i++) {
						GameObject* current = Game::Create(vec(0,3), vec(0,3), vec(1.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1.1f - 1.0f)))), std::vector<float>{-5 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (5 - -5))), 0, 0, 0, 1, 0, -5 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (5 - -5))), 0, 0, 0, 1, 0}, std::vector<unsigned int>{0, 1});
						current->Stage[0].onTop = true;
						current->MoveTo(vec(-1.1f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1.1f - -1.1f))), -1.1f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1.1f - -1.1f))), -.1));
						__lines.push_back(current);
					}
				}
			}
		}

		//shop
		if (Game::KeysPresed['1'] && keyCooldown <= 0.0f) {
			keyCooldown = .25f;
			shopAction(0);
		}
		if (Game::KeysPresed['2'] && keyCooldown <= 0.0f) {
			keyCooldown = .25f;
			shopAction(1);
		}
		if (Game::KeysPresed['3'] && keyCooldown <= 0.0f) {
			keyCooldown = .25f;
			shopAction(2);
		}
	}
	
}
	

float Battlezone::signed_angle_between_vectors(const vec& A, const vec& B, const vec& axis) {
	float dotProduct = vec::Dot(A, B);
	float magnitudeA = A.Length();
	float magnitudeB = B.Length();

	float cosTheta = dotProduct / (magnitudeA * magnitudeB);
	float sinTheta = (vec::Cross(A, B).Length()) / (magnitudeA * magnitudeB);

	// Calculate the signed angle using the arctangent and the dot product with the axis
	float thetaRad = atan2(sinTheta, cosTheta);

	// Calculate the dot product with the axis to determine the sign
	float dotWithAxis = vec::Dot(vec::Cross(A, B), axis);

	// Adjust the sign of the angle based on the axis
	float signedAngleRad = dotWithAxis >= 0 ? thetaRad : -thetaRad;

	// Convert to degrees and ensure the result is in the range (-180, 180]
	float signedAngleDeg = (signedAngleRad) * 180.0f / M_PI;
	signedAngleDeg = fmod(signedAngleDeg + 180.0f, 360.0f) - 180.0f;

	return signedAngleDeg;
}
void Battlezone::push_back3(std::vector<float>& vec, float a1, float a2, float a3) {
	vec.push_back(a1);
	vec.push_back(a2);
	vec.push_back(a3);
}
void Battlezone::push_back3(std::vector<float>& vec, float a1) {
	vec.push_back(a1);
	vec.push_back(a1);
	vec.push_back(a1);
}
void Battlezone::push_back2(std::vector<unsigned int>& vec, unsigned int a1, unsigned int a2) {
	vec.push_back(a1);
	vec.push_back(a2);
}
void Battlezone::push_back2(std::vector<unsigned int>& vec, unsigned int a1) {
	vec.push_back(a1);
	vec.push_back(a1);
}


TextBox* Battlezone::refreshText(TextBox* text, int score) {
	text->ChangeText(std::to_wstring(score));
	return text;
}

void Battlezone::destroy_enemy(int i) {
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

void Battlezone::shot_fast(vec pos, vec rot) {
	fastBulletTimeRemain.push_back(bulletMaxTime);
	GameObject* bullet = Game::Create(pos, rot, vec(1.0f, 3), L"FastBullet");
	pociski.push_back(bullet);
	bullet->Move(vec(0, 0, -1));
}


void Battlezone::shot(vec pos, vec rot, bool isPlayer) {
	if (isPlayer)bulletTimeRemain.push_back(bulletMaxTime);
	else fastBulletTimeRemain.push_back(bulletMaxTime);
	GameObject* bullet = Game::Create(pos, rot, vec(1.0f, 3), L"TankBullet");
	if (isPlayer)pociski_gracza.push_back(bullet);
	else pociski.push_back(bullet);
	//if (isPlayer && isMissleSelfTargeting) auto_bullet = bullet;
	bullet->Move(vec(0, 0, -1));
}

void Battlezone::shot_leonardo(vec pos, vec rot) {
	int temp = rand() % 8 + 1;
	GameObject* bullet;

	fastBulletTimeRemain.push_back(bulletMaxTime);
	bullet = Game::Create(pos, rot, vec(1.0f, 3), L"TankBullet");
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

void Battlezone::enemyShoot(GameObject* enemy) {
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

void Battlezone::spawn_enemy(vec pos, vec rot, int type) {
	//Normal tank
	if (type == 1) {
		GameObject* enemy = Game::Create(pos, rot, vec(1.0f, 3), L"Tank");
		przeciwnicy.push_back(enemy);
		enemyType.push_back(type);
		enemyShotCooldowns.push_back(enemyCooldown);

		GameObject* rPointer = Game::Create(vec(0.0f, 3), vec(0.0f, 3), vec(rPointerScaleDefault,3), L"RadarX");
		radarElements.push_back(rPointer);
		radarElementsType.push_back(0);
		uiElements.push_back(rPointer);
		targetPos.push_back(vec(0.0f, 3));
		targetOri.push_back(vec(0.0f, 3));
	}
	//Fast tank
	else if (type == 2) {
		GameObject* enemy = Game::Create(pos, rot, vec(2.0f, 3), L"FastTank");
		przeciwnicy.push_back(enemy);
		enemyType.push_back(type);
		enemyShotCooldowns.push_back(enemyCooldown);

		GameObject* rPointer = Game::Create(vec(0.0f, 3), vec(0.0f, 3), vec(rPointerScaleBig,3), L"RadarX");
		radarElements.push_back(rPointer);
		radarElementsType.push_back(0);
		uiElements.push_back(rPointer);
		targetPos.push_back(vec(0.0f, 3));
		targetOri.push_back(vec(0.0f, 3));
	}
	//Leonardo tank
	else if (type == 3) {
		GameObject* enemy = Game::Create(pos, rot, vec(1.0f, 3), L"LeonardoTank");
		przeciwnicy.push_back(enemy);
		enemyType.push_back(type);
		enemyShotCooldowns.push_back(enemyCooldown);

		GameObject* rPointer = Game::Create(vec(0.0f, 3), vec(0.0f, 3), vec(rPointerScaleDefault,3), L"AsteroidsStar");
		rPointer->SetColor(vec(0, 1, 0));
		radarElements.push_back(rPointer);
		radarElementsType.push_back(0);
		uiElements.push_back(rPointer);
		targetPos.push_back(vec(0.0f, 3));
		targetOri.push_back(vec(0.0f, 3));
	}
	//Rocket
	else if (type == 4) {
		GameObject* enemy = Game::Create(pos, rot, vec(1.0f, 3), L"Rocket");
		rakiety.push_back(enemy);

		GameObject* rPointer = Game::Create(vec(0.0f, 3), vec(0.0f, 3), vec(rPointerScaleDefault,3), L"AsteroidsShip");
		rPointer->SetColor(vec(0, 1, 0));
		radarElements.push_back(rPointer);
		radarElementsType.push_back(3);
		uiElements.push_back(rPointer);
		targetPos.push_back(vec(0.0f, 3));
		targetOri.push_back(vec(0.0f, 3));
	}

	// kamil forgor 💀 //stfu
	auto* current = radarElements.back();
	current->Stage[0].onTop = true;
	current->ScaleTo(vec(uiScale * 9, uiScale * 16, 0));
	current->MoveTo(vec(0, uiYOffset, 0));
	current->Rotate(vec(0, 180, 0));

	randomActionTimeLimit.push_back(static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / maxRandomActionLimit))); //x∈Q: [0;mRAL]
	randomActionTimeCooldown.push_back(static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / maxRandomActionCooldown))); //x∈Q: [0;mRAC]
	randomActionType.push_back(0);
}

void Battlezone::wave(int wavePoints, float dt) {
	if (wavePoints > 0) {
		float temp_x = (float)(rand() % 51 - 25);
		float temp_z = (float)(rand() % 51 - 25);
		float temp_y = (float)(rand() % 361);
		if (przeciwnicy.empty() && rakiety.empty()) {
			int enemy = rand() % 4 + 1;
			if (wavePoints - enemy >= 0) {
				spawn_enemy(vec(temp_x, 0, temp_z), vec(0, temp_y, 0), enemy);
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
void Battlezone::makeHorizon() {
	std::vector<float> vx;
	std::vector<unsigned int> ind;

	const float distance = 60.0f;
	const float maxMountainHeight = 12.5f;
	const float minMountainHeight = 5.0f;
	const int mountainNumber = 15;

	const int moonPointsNumber = 10;
	const float moonRadius = 2.5f;
	const float moonAboveMountains = 5.0f; //how high the moon is above the mountains //wtf we got the moon? i thought mateusz stole it

	for (int i = 0; i < mountainNumber; i++) {
		double angle = 2 * M_PI * i / mountainNumber;
		double nextAngle = 2 * M_PI * (i + 1) / mountainNumber;

		float yPos = minMountainHeight + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (maxMountainHeight - minMountainHeight)));
		push_back3(vx, distance * (float)cos(angle), yPos, distance * (float)sin(angle));
		push_back3(vx, 0, 1, 0);

		yPos = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / minMountainHeight));
		float randAngle = static_cast <float> (angle + rand()) / (static_cast <float> (RAND_MAX / (nextAngle - angle)));
		push_back3(vx, distance * cos(randAngle), yPos, distance * sin(randAngle));
		push_back3(vx, 0, 1, 0);

		push_back2(ind, 2 * i, 2 * i + 1);
		push_back2(ind, 2 * i + 1, 2 * i + 2);
	}
	ind.pop_back();
	ind.push_back(0);

	int countingOffset = mountainNumber * 2;
	for (int i = 0; i < moonPointsNumber; i++) {
		double angle = 2 * M_PI * i / moonPointsNumber;

		push_back3(vx, moonRadius * (float)cos(angle), moonAboveMountains + maxMountainHeight + moonRadius * (float)sin(angle), distance);
		push_back3(vx, 0, 1, 0);

		push_back2(ind, countingOffset + i, countingOffset + i + 1);
	}
	ind.pop_back();
	ind.push_back(countingOffset);

	countingOffset += moonPointsNumber;
	for (int i = 0; i < mountainNumber * 2; i++) {
		double angle = M_PI * i / mountainNumber;

		push_back3(vx, distance * (float)cos(angle), 0, distance * (float)sin(angle));
		push_back3(vx, 0, 1, 0);

		push_back2(ind, countingOffset + i, countingOffset + i + 1);
	}
	ind.pop_back();
	ind.push_back(countingOffset);

	horizon = Game::Create(vec(0, 3), vec(0, 3), vec(1, 3), vx, ind);
}

//where is radar? //no idea, mate. try using ctrl+f


void Battlezone::makeObstacles(float x, float z, float height) {
	 std::vector<float> vx;
	 std::vector<unsigned int> ind;

	 std::vector<vec> points;

	 int lv = rand() % (maxLevels - minLevels) + minLevels;

	 float lHeight = height / lv;

	 int ver = rand() % (maxBaseVerticies - minBaseVerticies) + minBaseVerticies;
	 float radius = minRadius + (float)(rand()) / ((float)(RAND_MAX / (maxRadius - minRadius)));

	 for (int i = 0; i < lv - 1; i++) {
	 	int noVxLvBw = i * ver;

	 	float yModifier = (float)(rand()) / (static_cast <float> (RAND_MAX / levelMaxYOffset));
	 	if (rand() % 2) yModifier *= -1;

	 	if (i != 0) radius -= (minLevelRadiusDecrease + (float)(rand()) / ((float)(RAND_MAX / (maxLevelRadiusDecrease - minLevelRadiusDecrease))));
	 	if (radius < minRadius) radius = minRadius;

	 	for (int j = 0; j < ver; j++) {
	 		double angle = 2 * M_PI * j / ver;

	 		float mxvtr = maxVertexOffset * radius;
	 		float radiusModifier = -mxvtr + (float)(rand()) / ((float)(RAND_MAX / (mxvtr + mxvtr)));  //i must have been high when i wrote this lmao

	 		float tempRadius = radius + radiusModifier;

	 		vec point = vec(tempRadius * cos(angle), (i * lHeight) + yModifier, tempRadius * sin(angle));
	 		points.push_back(point);

	 		 //last layer => topmost vertex
	 		if (i == lv - 2) {
	 			ind.push_back(j + noVxLvBw);
	 			ind.push_back((lv - 1) * ver);
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
	points.push_back(vec(0.0f, lv * lHeight, 0.0f));

	for (auto const& point : points) {
		vx.push_back(point.x);
		vx.push_back(point.y);
		vx.push_back(point.z);

		vx.push_back(0);
		vx.push_back(1);
		vx.push_back(0);
	}

	obstacles.push_back(Game::Create(vec(x, 0.0f, z), vec(0.0f, rand() % 360, 0.0f), vec(minScale + (float)(rand()) / ((float)(RAND_MAX / (maxScale - minScale)))), vx, ind));

	GameObject* rPointer = Game::Create(vec(0.0f, 3), vec(0.0f, 3), vec(rPointerScaleDefault,3), L"RadarT");
	radarElements.push_back(rPointer);
	radarElementsType.push_back(1);
	uiElements.push_back(rPointer);
	targetPos.push_back(vec(0.0f, 3));
	targetOri.push_back(vec(0.0f, 3));

	for (auto current : uiElements) {
		current->Stage[0].onTop = true;
		current->ScaleTo(vec(uiScale, uiScale, 0));
		//current->MoveTo(vec(0, uiYOffset, 0));
	}
}


void Battlezone::createPowerUp(float x, float y, float z, int type) {
	powerUpInside.push_back(Game::Create(vec(x, y, z), vec(0.0f, 3), vec(pUScale,3), L"PowerUp" + pUModels[type]));
	powerUpBox.push_back(Game::Create(vec(x, y, z), vec(0.0f, 3), vec(pUScale,3), L"PowerUpBox"));
	powerUpType.push_back(type);

	if (type == 2) {
		GameObject* obj = Game::Create(vec(x, y, z), vec(0.0f, 3), vec(pUScale,3), L"Arrow0");
		obj->AddStage(L"Arrow1");
		obj->AddStage(L"Arrow2");
		obj->AddStage(L"Arrow3");
		powerUpAnimation.push_back(obj);
	}

	GameObject* rPointer = Game::Create(vec(0.0f, 3), vec(0.0f, 3), vec(rPointerScaleDefault,3), L"MenuSquare");
	radarElements.push_back(rPointer);
	radarElementsType.push_back(2);
	uiElements.push_back(rPointer);
	targetPos.push_back(vec(0.0f, 3));
	targetOri.push_back(vec(0.0f, 3));
}


void Battlezone::collectPowerUp(GameObject* _inside, GameObject* _box, unsigned int _type) {
	Game::Destroy(_inside);
	Game::Destroy(_box);

	switch (_type) {
	case 0:
		velocity *= (1 + speedBoost);
		break;
	case 1:
		hp += healthBoost;
		break;
	case 2:
		timeMultiplier *= (1 - timeDecrease);
		timeEffectLeft = timeEffectLength;
		break;
	case 3:
		scoreMultiplier *= (1 + scoreMultiplierChange);
		break;
	case 4:
		score += scoreChange;
		break;
	case 5:
		isMissleSelfTargeting = true;
		break;
	default:
		throw std::invalid_argument("You might have forgotten to code what happens after collecting the PU. Chceck the `colleckPowerUp()` function.");
	}
}



void Battlezone::insertItem(int item, bool isTheFirstTime) {
	itemsType[item] = rand() % (sizeof(shopModels) / sizeof(std::string));
	itemsPrice[0] = 500;
	itemsPrice[1] = 100;
	itemsPrice[2] = 300;
	if (!isTheFirstTime) Game::Destroy(shopDisplayIcons[item]);
	shopDisplayIcons[item] = Game::Create(vec(-shopXPos, shopYPos - item * shopYPos, 0), vec(0, 3), vec(shopScale,3), shopModels[itemsType[item]]);
	shopDisplayIcons[item]->Stage[0].onTop = true;
}

void Battlezone::buyItem(int type, int cost, int item) {
	money -= cost;

	switch (type) {
	case 0:
		if (hp < 100) hp = 100;
		else hp += 25;
		break;
	case 1:
		bulletsFired = 0;
		break;
	case 2:
		velocity += 0.3f;
	default:
		throw std::invalid_argument("the shop is out of stock");
	}

	insertItem(item);
}

void Battlezone::shopAction(int item, bool isForced) {
	if (money >= itemsPrice[item] || isForced) 
		buyItem(itemsType[item], itemsPrice[item], item);
}