#include "Asteroids.h"
#include "The real engine/The Real Engine.h"

void Asteroids::Init(bool again) {

	SetCursor(NULL);

	clickCooldown = .25f;
	isEndScreenMusicPlaying = false;

	haloCoundtown = 0;

	std::wstring scoreStr = std::to_wstring(score);
	scoreStr = Game::formatText(scoreStr, 1);

	if (!again) {
		std::vector<float> haloVx;
		std::vector<unsigned int> haloInd;
		for (int i = 0; i < haloVxs; i++) {
			float angle = 2 * i * M_PI / haloVxs;

			haloVx.push_back(cos(angle) * haloRadius); haloVx.push_back(sin(angle) * haloRadius); haloVx.push_back(0);
			haloVx.push_back(0); haloVx.push_back(0); haloVx.push_back(1);

			haloInd.push_back(i); haloInd.push_back(i + 1);
		}
		haloInd.pop_back(); haloInd.push_back(0);

		halo = Game::Create(vec(0, 0, 100), vec(0, 0, rand() % 360), vec(1, 3), haloVx, haloInd);

		escSoundLen = .25f;
		hasEscd = false;
		isPaused = false;

		pauseIcone = Game::Create(vec(-10, 0, .1), vec(0, 3), vec(.1, .125, 1), L"pauseIcone");
		pauseIcone->Stage[0].onTop = true;

		ship = Game::Create(vec(0.0f, 0.0f, -99.0f), vec(0.0f, 3), vec(5.0f, 3), L"AsteroidsShip");
		modelShipFire = ship->AddStage(L"AsteroidsShipFire");

		wave_num = 0;

		tutorialText = Game::AddText(-.9, 0, -.8, -.9, L"Klikinj W, aby lecieć", .05, 0);
		tutorialStep = 0;

		for (int i = 0; i < 10; i++) {
			scoreboard[i] = nullptr;
		}

		smallEnemyNoise = 2.0f;

		maxSpaceshipCooldown = 60.0f;

		spaceship = Game::Create(vec(-1000, -1000, -80), vec(0, 3), vec(10, 3), L"AsteroidsSpaceship");
		spaceship->SetColor(vec(1, 0, 0));

		std::ifstream file("_data.txt");
		std::vector<std::string> lines;
		std::string line;
		if (!file.good()) {
			file.close();
			std::ofstream file1("_data.txt");
			file1 << "1";
			file1.close();
			file.open("_data.txt");
		}
		while (std::getline(file, line)) {
			lines.push_back(line);
		}
		if ((lines[0][0]) == '0') {
			tutorialStep = 4;
			tutorialText->ChangeText(L"Powodzenia!");
		}
		file.close();
		lines[0][0] = '0';
		std::ofstream ofile("_data.txt");
		for (int i = 0; i < lines.size(); i++) {
			std::string _line = lines[i];
			if (i != lines.size() - 1) _line += '\n';
			ofile << _line;
		}
		ofile.close();

		usernames.clear();
		scores.clear();

		std::ifstream file1("_asteroidsscoredata.txt");
		bestScore = -1;
		if (std::getline(file1, line)) bestScore = std::stoi(line.substr(4));
		file1.close();
		if (bestScore == -1) {
			std::ofstream file2("_asteroidsscoredata.txt");
			file2.close();
		}

		Game::camera->perspective = false;
		Game::camera->cameraHeight = camH;
		Game::camera->cameraWidth = camW;

		_asteroidsNo = 4;
		score = 0;
		lives = 4; //because i do --lives, so its actually 3 lol
		_return = 0;
		hasWaveFinished = false;
		waveAsteroidsCooldown = 2.5f;
		enemyProb = 25.0f;
		smallEnemyProb = 25.0f;
		enemyDelay = rand() % (int)(enemyMaxDelay - enemyMinDelay) + enemyMinDelay;

		stars.clear();
		debris.clear();

		for (int i = 0; i < starsAmount; i++) {
			GameObject* star = Game::Create(vec(rand() % 320 - 160, rand() % 180 - 90, -99.999f), vec(0.0f, 0.0f, rand() % 45), vec(.25f, 3), L"AsteroidsStar");
			star->SetColor(vec(1, 3));
			stars.push_back(star);
		}

		Game::Sound(backgroundMusic[2], true);

		tScore = Game::AddText(-.9, .9, .85, .8, scoreStr, .02, false);

		endingUsername = Game::AddText(-.125, .125, .7, .5, new_username,.1, true);
	}
	endingUsername->ChangeText(L"");

	if (again) {
		clearVec(asteroids); clearVec(bullets); clearVec(enemies); clearVec(tLives);

		_asteroidsNo -= 2;
		if (--wave_num == -1) wave_num = 0;

		scoreStr = Game::formatText(scoreStr, 1);
		tScore->ChangeText(scoreStr);
	}

	spaceshipCooldown = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / maxSpaceshipCooldown));
	isSpaceship = false;
	hasSpaceshipPlayedSound = false;
	spaceship->MoveTo(vec(-1000, -1000, -80));

	ship->MoveTo(vec(0, 0, -80));
	ship->RotateTo(vec(0, 3));
	isDead = false;
	hasLost = false;
	endingScreen = false;
	new_username = L"";
	respawnCooldown = 5.0f;

	velocity = vec(0.0f, 3);
	speed = 0;
	velocityd = maxVelocity * maxVelocity;

	jumpCooldown = 0.5f;
	shipAnimationCooldown = (rand() % 4) / 2 + 1;
	shipAnimationCooldown2 = (rand() % 2) / 2 + 0.25;
	shootCooldown = .5f;

	if (--lives <= 0) {
		isDead = true;
		hasLost = true;

		usernameInfo = Game::AddText(-.9, .9, .6, .5, L"", .1, true);
	}
	else {
		Game::Sound(L"asteroidsStart", false);
	}
	bigEnemyIterator = 0;

	asteroids.clear();
	enemies.clear();
	bullets.clear();

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

	tScore->Color = vec(1, 1, 1);

	for (int i = 0; i < lives; i++) {
		const wchar_t* modelName = L"AsteroidsShip";
		if (i == lives - 1) modelName = L"AsteroidsShipFire";
		GameObject* current = Game::Create(vec(-.9 + .012 + .035 * i, .7, 0), vec(0, 3), vec(.04, 3), modelName); //.012 so that it's centered... .035 is spacing - feel free to change that anytime
		current->Stage[0].onTop = true;
		tLives.push_back(current);
	}
}

void Asteroids::Update(const float& dt) { 
	clickCooldown -= dt;

	if (Game::KeysPresed[VK_ESCAPE]) {
		hasEscd = true;
		if (escSoundLen == .25f) Game::Sound(L"asteroidsExit");
	}
	if (hasEscd) {
		escSoundLen -= dt;
	}
	if (escSoundLen <= 0) {
		Game::ChangeState(Game_Menu);
	}

	if (!isDead && clickCooldown <= 0 && Game::KeysPresed['P']) {
		pauseIcone->MoveTo(vec(-10 * (float)isPaused, 0, .1)); //what this essentially means is go to either (-10,-10) or (0,0)

		Game::StopSounds();
		Game::Sound(backgroundMusic[(int)isPaused + 1], true);

		isPaused = !isPaused;
		clickCooldown = .25f;
	}

	if (isPaused) return;

	for (int i = 0; i < debris.size(); i++) {
		GameObject* current = debris[i];
		float multiplier = debrisSpeedMultiplier;
		bool isPlayers = false;
		if (std::find(playersDebris.begin(), playersDebris.end(), i) != playersDebris.end()) { multiplier = playerDebrisSpeedMultiplier; isPlayers = true; }
		current->MoveGlobal(vec(debrisDirection[i].x, debrisDirection[i].y, 0) * dt * multiplier);
		current->Rotate(vec(0, 0, 1) * debrisRotation[i] * dt);

		if (checkBounds(current) && !isPlayers) {
			Game::Destroy(current);
			debris.erase(debris.begin() + i);
			debrisDirection.erase(debrisDirection.begin() + i);
			debrisRotation.erase(debrisRotation.begin() + i);
		}
	}

	if (respawnCooldown <= 0 && !hasLost && !endingScreen) {
		Asteroids::Init(true);
		clearVec(debris);
		debris.clear();
	}

	if (isDead) {
		respawnCooldown -= dt;

		int n = 0;

		if (hasLost && !endingScreen) {
			if (!isEndScreenMusicPlaying) { //this happens only once
				isEndScreenMusicPlaying = true;

				Game::StopSounds();
				Game::Sound(backgroundMusic[0], true);

				tutorialText->ChangeText(L"");

				std::wifstream file("_asteroidsscoredata.txt"); // reading the file

				std::wstring line;

				while (std::getline(file, line)) {
					usernames.push_back((line.substr(0, 3)));
					scores.push_back(std::stoi(line.substr(4)));
				}
				file.close();
			}

			if (new_username.length() < 3) {
				for (int key = 'A'; key <= 'Z'; key++) {
					if (Game::KeysPresed[key] && clickCooldown <= 0) {
						Game::Sound(L"asteroidsInput", false);

						clickCooldown = .25f;
						new_username += (char)('A' + (key - 'A'));

						endingUsername->ChangeText(new_username);
					}
				}
				if ((Game::KeysPresed[VK_BACK] || Game::KeysPresed[VK_DELETE]) && clickCooldown <= 0) {
					Game::Sound(L"asteroidsInputBackspace", false);

					clickCooldown = .25f;
					if (new_username.length() != 0) new_username.pop_back();

					endingUsername->ChangeText(new_username);
				}
			}
			else { //this happens only once
				for (n; n < scores.size(); n++) {
					if (score >= scores[n]) break;
				}

				usernames.insert(usernames.begin() + n, new_username); // inserting the new username
				scores.insert(scores.begin() + n, score); // and score

				if (usernames.size() > 10) {
					usernames.pop_back(); // removing the last (worst) username
					scores.pop_back(); // and score (there can only be <= 10)
				}

				std::wofstream file_out("_asteroidsscoredata.txt"); // opening the file

				for (int i = 0; i < usernames.size(); i++) {
					file_out << (usernames[i] + L" " + std::to_wstring(scores[i]) + L"\n"); // writing the scores ('ABC1234', where 'ABC' is the username, and '1234' is the score)
				}

				file_out.close(); // closing the file

				endingScreen = true;
			}
		}
		if (endingScreen && hasLost) {
			for (int i = 0; i < starsAmount; i++) {
				GameObject* current = stars[i];
				float angle = -M_PI / 2;
				float size = (i % endingScreenIndicatorNumber) * endingScreenAnimationSize / (endingScreenIndicatorNumber * .5) - endingScreenAnimationSize;
				float scoredAngle = (90.0f - (360.0f * score / bestScore)) * M_PI / 180.0f;

				if (bestScore < score) bestScore = score;

				if (i < endingScreenAnimationNumber) {
					angle = 2 * M_PI * i / endingScreenAnimationNumber;
					size = endingScreenAnimationSize;
				}
				else if (i >= starsAmount - (.5 * endingScreenIndicatorNumber) && bestScore != -1) angle = scoredAngle;

				float coefficient = dt / endingScreenAnimationTime; //probably multiplied the values wrong but still looks cool
				current->Move(vec(size * cos(angle) - current->Transform.position.x + animationPos.x, size * sin(angle) - current->Transform.position.y + animationPos.y, 0) * coefficient);
			}

			if (!scoreboard[0]) {
				for (int i = 0; i < 10; i++) {
					std::wstring place = std::to_wstring(i + 1);
					if (i != 9) place = L"0" + place;

					std::wstring nick = L"xxx"; //migh be changed to '-' or unknow character for empty space
					std::wstring score = L"000";

					if (usernames.size() > i) {
						nick = usernames[i];
						score = std::to_wstring(scores[i]);
					}

					score = Game::formatText(Game::formatText(score, 1), 0, std::to_string(scores[0]).length()); //crazy operations lol

					scoreboard[i] = Game::AddText(-animationPos.x * 1.5 / camW, -animationPos.x * 1.0f, .35f - (.125f * i), .35f - (.125f * (i + .5)) - .05f, place + L" " + nick + L" " + score,.1f, false);
				}

				ship->RotateTo(vec(0, 0, 90.0f));
				ship->MoveTo(vec(-animationPos.x * .6, (.35 - (.125 * n)) * camH, -80.0f));
			}
		}
	}

	if (isDead) return;

	pPos = ship->Transform.position;
	pOri = ship->Transform.orientation;

	jumpCooldown -= dt;
	shootCooldown -= dt;

	for (int i = 0; i < bullets.size(); i++) {
		GameObject* current = bullets[i];
		bool shouldSkip = false;

		bulletTimeRemain[i] -= dt;
		if (bulletTimeRemain[i] <= 0) {
			Game::Destroy(current);
			bullets.erase(bullets.begin() + i);
			bulletTimeRemain.erase(bulletTimeRemain.begin() + i);
			isBulletPlayers.erase(isBulletPlayers.begin() + i);
			i--;
			continue;
		}
		current->Move(vec(0.0f, 1.0f, 0.0f) * bulletSpeed * dt);

		checkBounds(current);

		vec currentPos = current->Transform.position;

		//collisions - player/bullets
		if (!isBulletPlayers[i] && Game::collisionCircle(ship, current, vec(1,1,0))) {
			death(ship->Transform.position, ship->Transform.orientation, ship);
		}

		//collisions - enemy/bullets
		for (int j = enemies.size() - 1; j >= 0; j--) {
			GameObject* enemy = enemies[j];
			if (isBulletPlayers[i] && Game::collisionCircle(current, enemy, vec(1, 1, 0))) {
				death(enemy->Transform.position, vec(0, 3), enemy, false, false, enemySizes[0], __enemyVx, __enemyInd);
				enemies.erase(enemies.begin() + j);
				if (enemyType[j]) eBDPos.erase(eBDPos.begin() + j);
				enemyShootCooldown.erase(enemyShootCooldown.begin() + j);

				score += ufoXP[enemyType[j]];
				enemyType.erase(enemyType.begin() + j);

				std::wstring temp = Game::formatText(std::to_wstring(score), 1);
				tScore->ChangeText(temp);

				bulletTimeRemain[i] = 0;
				shouldSkip = true;
				break;
			}
		}
		if (shouldSkip) continue;

		//collisions - asteroids/bullets
		for (int j = asteroids.size() - 1; j >= 0; j--) {
			GameObject* asteroid = asteroids[j];
			if (Game::collisionCircle(asteroid, current, vec(1, 1, 0)))
			{
				current->SetColor(vec(1, 0, 0));
			}
			if (Game::checkCollisions(asteroid, current, vec(1,1,0), true)) {
				int type = asteroidSize[j];
				if (type < 2) {
					float ori = asteroid->Transform.orientation.z;
					vec pos = asteroid->Transform.position;
					float randomChange = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 45.0f)); // [-45;45]

					spawnAsteroids(1, type + 1, pos.x, pos.y, ori + randomChange);
					spawnAsteroids(1, type + 1, pos.x, pos.y, ori - randomChange);
				}

				Game::Sound(L"asteroidsDestroy", false);

				Game::Destroy(asteroid);
				asteroids.erase(asteroids.begin() + j);
				asteroidSize.erase(asteroidSize.begin() + j);
				asteroidRotation.erase(asteroidRotation.begin() + j);
				asteroidRotationMultiplier.erase(asteroidRotationMultiplier.begin() + j);

				score += asteroidsXP[type];

				std::wstring temp = Game::formatText(std::to_wstring(score), 1);
				tScore->ChangeText(temp);

				bulletTimeRemain[i] = 0;
				break;
			}
		}
	}

	if (Game::KeysPresed['W']) {
		if (tutorialStep == 0) {
			tutorialStep++;

			tutorialText->ChangeText(L"Użyj A i D, aby się obracać");
		}

		vec shipUp = ship->Up;

		velocity += vec(shipUp.x, shipUp.y, 0) * acceleration * dt;
		speed = velocity.x * velocity.x + velocity.y * velocity.y;
		if (speed > velocityd) velocity = velocity * (velocityd / speed);
		ship->MoveGlobal(vec(velocity.x * dt, velocity.y * dt, 0));

		ship->activeStage = modelShipFire;

		shipAnimationCooldown -= dt;
		if (shipAnimationCooldown <= 0) {
			ship->activeStage = 0;

			shipAnimationCooldown2 -= dt;
			if (shipAnimationCooldown2 <= 0) {
				shipAnimationCooldown = (rand() % 4) / 2 + 2;
				shipAnimationCooldown2 = (rand() % 2) / 2 + 0.25;

				Game::Sound(L"asteroidsWoosh", false);
			}
		}

		for (auto& star : stars) {
			star->MoveGlobal(vec(shipUp.x, shipUp.y, 0) * -starsSpeedMultiplier * dt);
			checkBounds(star, false, vec(160, 90, 0));
		}
	}
	else {
		velocity -= velocity * deacceleration * dt;
		speed = velocity.x * velocity.x + velocity.y * velocity.y;
		ship->MoveGlobal(vec(velocity.x * dt, velocity.y * dt, 0));

		ship->activeStage = 0;
	}
	if (tutorialStep > 0 && (Game::KeysPresed['A'])) {
		if (tutorialStep == 1) {
			tutorialStep++;

			tutorialText->ChangeText(L"Kliknij SPACA, aby strzelać");
		}

		ship->Rotate(vec(0, 0, 1.0f) * rotationMultiplier * dt);
	}
	if (tutorialStep > 0 && (Game::KeysPresed['D'])) {
		if (tutorialStep == 1) {
			tutorialStep++;

			tutorialText->ChangeText(L"Kliknij SPACA, aby strzelać");
		}

		ship->Rotate(vec(0, 0, -1.0f) * rotationMultiplier * dt);
	}

	if (tutorialStep > 2 && Game::KeysPresed['E'] && jumpCooldown <= 0.0f) {
		bool forceTeleport = false;

		if (tutorialStep == 3) {
			tutorialStep++;

			tutorialText->ChangeText(L"Uważaj! To może być niebezpieczne!");
			forceTeleport = true;
		}

		Game::Sound(L"asteroidsPlayerTeleport", false);

		jumpCooldown = 0.5f;

		int random = rand() % 32 - 1;
		if (!forceTeleport && random >= 24 && random <= 31) {
			death(ship->Transform.position, ship->Transform.orientation, ship, true);
		}
		else {
			random = rand() % 8 - 1;
			random = (random * 2) + 4;

			if (!forceTeleport && random < asteroids.size()) {
				death(ship->Transform.position, ship->Transform.orientation, ship, true);
			}
			else {
				ship->MoveTo(vec(rand() % (160 - jumpMargin) * 2 - 160 - jumpMargin, rand() % (90 - jumpMargin) * 2 - 90 - jumpMargin, -80));
				velocity = vec(0.0f, 3);

				haloCoundtown = 5.0f;
			}
		}
	}

	if (tutorialStep > 1 && Game::KeysPresed[VK_SPACE] && shootCooldown <= 0) {
		if (tutorialStep == 2) {
			tutorialStep++;

			tutorialText->ChangeText(L"Kliknij E, żeby się teleportować");
		}

		Game::Sound(L"asteroidsPlayerShoot", false);

		shootCooldown = .5f;
		shoot(ship->Transform.position, ship->Transform.orientation, 0, this);
		isBulletPlayers.push_back(true);
	}

	if (tutorialStep < 4) return;

	bigEnemyIterator = 0;

	for (int i = 0; i < enemies.size(); i++) {
		GameObject* current = enemies[i];
		bool type = enemyType[i];

		vec sPos;

		vec pos = vec(current->Transform.position.x, current->Transform.position.y, 0);

		if (!type) sPos = vec(ship->Transform.position.x, ship->Transform.position.y, 0);
		else {
			sPos = vec(eBDPos[bigEnemyIterator].x, eBDPos[bigEnemyIterator].y, 0);
			eBDPos[bigEnemyIterator].z--;
		}

		if (type) {
			if (sPos.x + 5 > pos.x && sPos.x - 5 < pos.x && sPos.y + 5 > pos.y && sPos.y - 5 < pos.y) {
				if (eBDPos[bigEnemyIterator].z > 0) {
					eBDPos[bigEnemyIterator] = vec(rand() * (2 * (camW + bounds)) - (camW + bounds), rand() % (2 * (camH + bounds)) - (camH + bounds), eBDPos[bigEnemyIterator].z);
				}
				else {
					eBDPos[bigEnemyIterator] = vec(rand() % (2 * camW) - camW, -2 * camH - bounds, eBDPos[bigEnemyIterator].z);
				}
			}
		}

		vec dMov = vec(sPos.x - pos.x, sPos.y - pos.y, 0);

		if (dMov.x > maxEnemyVelocity) dMov.x = maxEnemyVelocity;
		else if (dMov.x < -maxEnemyVelocity) dMov.x = -maxEnemyVelocity;

		if (dMov.y > maxEnemyVelocity) dMov.y = maxEnemyVelocity;
		else if (dMov.y < -maxEnemyVelocity) dMov.y = -maxEnemyVelocity;

		current->MoveGlobal(vec(dMov.x, dMov.y, 0.0f) * dt);

		float _angle;

		enemyShootCooldown[i] -= dt;
		if (enemyShootCooldown[i] <= 0 && !checkBounds(current, true)) {
			if (!type) {
				enemyShootCooldown[i] = (float)((rand() % (int)(2 * enemyShootCooldownRange * 100)) / 100 - enemyShootCooldownRange + _enemyShootCooldown[(int)type]);

				_angle = atan2(sPos.y - pos.y, sPos.x - pos.x);

				float noise = -smallEnemyNoise + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (2 * smallEnemyNoise)));

				_angle = (_angle + noise) * 180.0f / M_PI - 90.0f;
			}
			else _angle = rand() % 360;

			Game::Sound(L"asteroidsEnemyShoot", false);

			shoot(current->Transform.position, vec(0.0f, 0.0f, _angle), 1, type);
			enemyShootCooldown[i] = _enemyShootCooldown[(int)type];
			isBulletPlayers.push_back(false);
		}

		//collisions - player/enemy
		if (Game::checkCollisions(current,ship, vec(1, 1, 0), true)) {
			death(ship->Transform.position, ship->Transform.orientation, ship);
		}

		if (type) {
			if (pos.y <= -camH - bounds && eBDPos[bigEnemyIterator].z <= 0) {
				Game::Destroy(current);
				enemies.erase(enemies.begin() + i);
				enemyType.erase(enemyType.begin() + i);
				eBDPos.erase(eBDPos.begin() + bigEnemyIterator);
				enemyShootCooldown.erase(enemyShootCooldown.begin() + i);

				bigEnemyIterator--; i--;
			}
			bigEnemyIterator++;
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

		current->Rotate(vec(0.0f, 0.0f, asteroidRotationMultiplier[i]) * dt);
		current->MoveGlobal(vec(cos(deg), sin(deg), 0.0f) * _velocity * dt);

		checkBounds(current, false, vec(Game::camera->cameraWidth + bounds, Game::camera->cameraHeight + bounds, 0));

		//collisions - player/asteroid
		if (Game::checkCollisions(current,ship, vec(1, 1, 0),true)) {
			death(ship->Transform.position, ship->Transform.orientation, ship);
		}
	}

	checkBounds(ship);

	if (asteroids.empty()) hasWaveFinished = true;

	if (hasWaveFinished) {
		waveAsteroidsCooldown -= dt;

		if (waveAsteroidsCooldown <= 0) {
			waveAsteroidsCooldown = 2.5f;
			hasWaveFinished = false;
			_return = wave(_asteroidsNo);
			if (_asteroidsNo <= 13) _asteroidsNo += 2;
			else _asteroidsNo = 15;

			tutorialText->ChangeText(L"Poziom " + std::to_wstring(wave_num));
		}
	}
	if (_return != 0) {
		enemyDelay -= dt;
		if (enemyDelay <= 0) {
			//Game::Sound(L"asteroidsEnemySpawn", false);

			for (int i = _return; i > 0; i--) {
				bool type = 0;
				if (rand() % 100 > smallEnemyProb) type = 1;
				spawnEnemy(type);
			}
			_return = 0;

			enemyDelay = rand() % (int)(enemyMaxDelay - enemyMinDelay) - enemyMinDelay;
			enemyProb += enemyProb * enemyDeltaProb;
			smallEnemyProb += smallEnemyProb * enemyDeltaProb;
		}
	}

	if (wave_num >= 3) {
		if (!isSpaceship) spaceshipCooldown -= dt;
		if (spaceshipCooldown <= 0) {
			spaceshipCooldown = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / maxSpaceshipCooldown));

			vec pos;
			int temp;
			do {
				temp = rand() % (2 * (camW + bounds)) - (camW + bounds);
				pos.x = temp;

				temp = rand() % (2 * (camH + bounds)) - (camH + bounds);
				pos.y = temp;
			} while (pos.x > -camW && pos.x < camW && pos.y > -camH && pos.y < camH);

			spaceship->MoveTo(vec(pos.x, pos.y, -80));

			float _angle = atan2(pPos.y - pos.y, pPos.x - pos.x);
			_angle = _angle * 180 / M_PI - 90;

			spaceship->RotateTo(vec(0, 0, _angle));

			isSpaceship = true;

			maxSpaceshipCooldown *= (1 - spaceshipCooldownDelta / 100.0f);
		}
		if (isSpaceship) {
			spaceship->Move(vec(0, 1, 0) * spaceshipSpeed * dt);

			//collisions - spaceship/player
			if (!hasSpaceshipPlayedSound && Game::checkCollisions(spaceship,ship, vec(1, 1, 0),true)) {
				hasSpaceshipPlayedSound = true;
				Game::Sound(L"asteroidsLoudWoosh", false);
			}
			if (Game::checkCollisions(spaceship,ship, vec(1, 1, 0),true)) death(ship->Transform.position, ship->Transform.orientation, ship);

			if (checkBounds(spaceship, false, vec(camW + 2 * bounds, camH + 2 * bounds, 0))) {
				spaceship->MoveTo(vec(-1000, -1000, -80));
				isSpaceship = false;
				score += 10;
				tScore->ChangeText(std::to_wstring(score));
			}
		}
	}

	if (haloCoundtown <= 0 && haloCoundtown != 5.0f) { //so that it doesnt appear right on
		halo->Transform.position.z = 100; //hide
	}
	else {
		haloCoundtown -= dt;

		halo->MoveTo(vec(pPos.x, pPos.y, -80));
		halo->Rotate(vec(0, 0, 1) * dt * speed * haloRotation);
	}
}

bool Asteroids::checkBounds(GameObject* current, bool stay, vec bounds) {
	bool flag = false;
	if (current->Transform.position.y > bounds.y) { if (!stay) { current->MoveGlobal(vec(0, -bounds.y * 2.0f, 0)); } flag = true; }
	if (current->Transform.position.y < -bounds.y) { if (!stay) { current->MoveGlobal(vec(0, bounds.y * 2.0f, 0)); } flag = true; }
	if (current->Transform.position.x > bounds.x) { if (!stay) { current->MoveGlobal(vec(-bounds.x * 2.0f, 0, 0)); } flag = true; }
	if (current->Transform.position.x < -bounds.x) { if (!stay) { current->MoveGlobal(vec(bounds.x * 2.0f, 0, 0)); } flag = true; }
	return flag;
}

void Asteroids::spawnAsteroids(int asteroidsNum, unsigned int type, float _posX, float _posY, float rot) {
	float minAsteroidsSize;
	float maxAsteroidsSize;

	for (int i = 0; i < asteroidsNum; i++) {
		std::vector<float> v;
		std::vector<unsigned int> id;
		std::vector<vec> points;

		asteroidSidesNo = rand() % (maxAsteroidsSidesNo - minAsteroidsSidesno) + minAsteroidsSidesno;

		for (int i = 0; i < asteroidSidesNo; ++i) {
			float angle = 2 * M_PI * i / asteroidSidesNo;
			float radiusModifier = (rand() / (double)RAND_MAX) * 2 * asteroidRadius * asteroidsVertexOffset - asteroidRadius * asteroidsVertexOffset;
			float modifiedRadius = asteroidRadius + radiusModifier;

			vec vertex = { modifiedRadius * cos(angle), modifiedRadius * sin(angle), 0 };
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

		vec pos = vec(_posX, _posY, -10);
		if (_posX == -10000 || _posY == -10000) {
			do {
				temp = rand() % (2 * (camW + bounds)) - (camW + bounds);
				pos.x = temp;

				temp = rand() % (2 * (camH + bounds)) - (camH + bounds);
				pos.y = temp;
			} while (pos.x > -camW - bounds && pos.x < camW + bounds && pos.y > -camH - bounds && pos.y < camH + bounds);
		}

		asteroids.push_back(Game::Create(vec(pos.x, pos.y, -90.0f), vec(0.0f, 0.0f, rot), vec(minAsteroidsSize + (float)(rand()) / ((float)(RAND_MAX / (maxAsteroidsSize - minAsteroidsSize))), 3), v, id));
		//asteroids[asteroids.size() - 1]->SetColor(vec(1, 1, 1));
		asteroidSize.push_back(type);
	}
}

void Asteroids::spawnEnemy(bool type) {
	vec pos;
	int temp;

	do {
		temp = rand() % (2 * (camW + bounds)) - (camW + bounds);
		pos.x = temp;

		temp = rand() % (2 * (camW + bounds)) - (camW + bounds);
		pos.y = temp;
	} while (pos.x > -camW - bounds && pos.x < camW + bounds && pos.y > -camH - bounds && pos.y < camH + bounds);

	enemies.push_back(Game::Create(vec(pos.x, pos.y, -75.0f), vec(0.0f, 3), vec(enemySizes[(int)type], 3), L"AsteroidsEnemy"));
	enemyType.push_back(type);
	enemyShootCooldown.push_back((float)((rand() % (int)(2 * enemyShootCooldownRange * 100)) / 100 - enemyShootCooldownRange + _enemyShootCooldown[(int)type]));
	if (type) eBDPos.push_back(vec(rand() % (2 * (camW - bounds)) - (camW + bounds), rand() % (2 * (camH - bounds)) - (camH + bounds), rand() % maxBigEnemyMoves + 1));
}

void Asteroids::shoot(vec _pos, vec _rot, bool type, bool eType) {
	float _time;
	float _scale;
	const wchar_t* _model;
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
	GameObject* bullet = Game::Create(_pos, _rot, vec(_scale, 3), _model);
	bullets.push_back(bullet);
	bullet->Move(vec(0.0f, _offset, 0.0f));
}

int Asteroids::wave(int asteroidsNum) {
	Game::Sound(L"asteroidsNewWave", false);

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

std::vector<vec> Asteroids::breakIntoPieces(std::vector<unsigned int> ind, std::vector<vec> vx) {
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
		vx = { vec(0.0, 1.25, 0), vec(-.3, -.15, 0), vec(0.3, -.15, 0), vec(-.25, 0.0, 0), vec(0.25, 0.0, 0) }; //defaults to AsteroidsShip
		ind = { 0,1,0,2,3,4 }; //same as above
	}

	for (int i = 0; i < ind.size() / 2; i++) {
		line[0][0] = vx[ind[i * 2]].x;      line[0][1] = vx[ind[i * 2]].y;
		line[1][0] = vx[ind[i * 2 + 1]].x;  line[1][1] = vx[ind[i * 2 + 1]].y;

		newVx.push_back(vec(line[0][0], line[0][1], 0));

		float t = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		newVx.push_back(vec((1 - t) * line[0][0] + t * line[1][0], (1 - t) * line[0][1] + t * line[1][1], 0));

		newVx.push_back(vec(line[1][0], line[1][1], 0));
	}

	return newVx;
}

void Asteroids::death(vec _pos, vec _rot, GameObject* obj, bool tp, bool isShip, float scale, std::vector<vec> _vx, std::vector<unsigned int> _ind) {
	if (isShip) {
		Game::Game::Sound(L"asteroidsPlayerDeath", false);

		isDead = true;
		ship->MoveTo(vec(-10000, -10000, 0));
	}
	else {
		Game::Game::Sound(L"asteroidsEnemyDeath", false);

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
			debris.push_back(Game::Create(_pos, _rot, vec(scale, 3), vx1, std::vector<unsigned int>{0, 1}));
			if (isShip) { debris[debris.size() - 1]->SetColor(vec(1, 0, 0)); playersDebris.push_back(debris.size() - 1); }
		}
	}
	for (int i = 0; i < debris.size(); i++) {
		debrisDirection.push_back(vec(-1 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (2))), -1 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (2))), 0)); //x∈Q:[-1;1]
		debrisRotation.push_back((-maxDebrisRotationMultiplier + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (2 * maxDebrisRotationMultiplier)))));
	}
}

void Asteroids::clearVec(std::vector<GameObject*>& vec) {
	for (auto& obj : vec) {
		Game::Destroy(obj);
	}
}