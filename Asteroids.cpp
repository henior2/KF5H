#include "Asteroids.h"
#include "The real engine/The Real Engine.h"

void Asteroids::Init(bool again) {
	clickCooldown = .25f;
	isEndScreenMusicPlaying = false;

	haloCoundtown = 0;

	std::string scoreStr = std::to_string(score);
	while (scoreStr.length() < 3) {
		scoreStr = "0" + scoreStr;
	}

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

		halo = Game::Create(vec(0, 0, 100), vec(0, 0, rand() % 360), vec(1), haloVx, haloInd);

		escSoundLen = .25f;
		hasEscd = false;
		isPaused = false;

		pauseIcone = Game::Create(vec(-10), vec(0), vec(.1, .125, 1), L"pauseIcone");
		pauseIcone->Stage[0].onTop = true;

		ship = Game::Create(vec(0.0f, 0.0f, -99.0f), vec(0.0f), vec(5.0f), L"AsteroidsShip");
		modelShipFire = ship->AddStage("AsteroidsShipFire");

		wave_num = 0;

		tutorialText = CreateTekst(vec(-.9), 0, vec(.05), 1, .5, "Press W to move");
		tutorialStep = 0;

		for (int i = 0; i < 10; i++) {
			scoreboard[i] = nullptr;
		}

		smallEnemyNoise = 2.0f;

		maxSpaceshipCooldown = 60.0f;

		spaceship = Game::Create(vec(-1000, -1000, -80), vec(0), vec(10), L"AsteroidsSpaceship");
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
			tutorialText = refreshText(tutorialText, "Good luck");
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

		camera->perspective = false;
		camera->cameraHeight = camH;
		camera->cameraWidth = camW;

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
			stars.push_back(Game::Create(vec(rand() % 320 - 160, rand() % 180 - 90, -99.999f), vec(0.0f, 0.0f, rand() % 45), vec(.01f), L"AsteroidsStar"));
		}

		Game::Sound(backgroundMusic[2], true);

		tScore = CreateTekst(vec(-.9, .8), 0, vec(.025), 1, .5, scoreStr);
		endingUsername = CreateTekst(vec(-.125, .3), 0, vec(.06), 1, .5, new_username);
	}
	endingUsername = refreshText(endingUsername, "");

	if (again) {
		clearVec(asteroids); clearVec(bullets); clearVec(enemies); clearVec(tLives);

		_asteroidsNo -= 2;
		if (--wave_num == -1) wave_num = 0;

		tScore = refreshText(tScore, scoreStr, true);
	}

	spaceshipCooldown = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / maxSpaceshipCooldown));
	isSpaceship = false;
	hasSpaceshipPlayedSound = false;
	spaceship->MoveTo(vec(-1000, -1000, -80));

	ship->MoveTo(vec(0, 0, -80));
	ship->RotateTo(vec(0));
	isDead = false;
	hasLost = false;
	endingScreen = false;
	new_username = "";
	respawnCooldown = 5.0f;

	velocity = vec(0.0f);
	speed = 0;
	velocityd = maxVelocity * maxVelocity;

	jumpCooldown = 0.5f;
	shipAnimationCooldown = (rand() % 4) / 2 + 1;
	shipAnimationCooldown2 = (rand() % 2) / 2 + 0.25;
	shootCooldown = .5f;

	if (--lives <= 0) {
		isDead = true;
		hasLost = true;

		usernameInfo = CreateTekst(vec(-.9, .6), 0, vec(.065), 1, .5, "Enter Your Username");
	}
	else {
		Game::Sound(L"asteroidsStart.wav", false);
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

	tScore->SetColor(vec(1));

	for (int i = 0; i < lives; i++) {
		wchar_t* modelName = L"AsteroidsShip";
		if (i == lives - 1) modelName = L"AsteroidsShipFire";
		GameObject* current = Game::Create(vec(-.9 + .012 + .035 * i, .7, 0), vec(0), vec(.04), modelName); //.012 so that it's centered... .035 is spacing - feel free to change that anytime
		current->Stage[0].onTop = true;
		tLives.push_back(current);
	}
}

void Asteroids::Update(const float& dt) {
	clickCooldown -= dt;

	if (Game::KeysPresed['Esc']) {
		hasEscd = true;
		if (escSoundLen == .25f) Game::Sound(L"asteroidsExit.wav", false);
	}
	if (hasEscd) {
		escSoundLen -= dt;
	}
	if (escSoundLen <= 0) {
		Game::ChangeState(Game_Menu);
	}

	if (!isDead && clickCooldown <= 0 && Game::KeysPresed['P']) {
		pauseIcone->MoveTo(vec(-10) * (float)isPaused); //what this essentially means is go to either (-10,-10) or (0,0)

		Game::engine->stopAllSounds();
		Game::Sound(backgroundMusic[(int)isPaused + 1].c_str(), true);

		isPaused = !isPaused;
		clickCooldown = .25f;
	}

	if (isPaused) return;

	for (int i = 0; i < debris.size(); i++) {
		GameObject* current = debris[i];
		float multiplier = debrisSpeedMultiplier;
		bool isPlayers = false;
		if (std::find(playersDebris.begin(), playersDebris.end(), i) != playersDebris.end()) { multiplier = playerDebrisSpeedMultiplier; isPlayers = true; }
		current->MoveGlobal(vec(debrisDirection[i], 0) * dt * multiplier);
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

				Game::engine->stopAllSounds();
				Game::Sound(backgroundMusic[0].c_str(), true);

				tutorialText = refreshText(tutorialText, "");

				std::ifstream file("_asteroidsscoredata.txt"); // reading the file

				std::string line;

				while (std::getline(file, line)) {
					usernames.push_back(line.substr(0, 3));
					scores.push_back(std::stoi(line.substr(4)));
				}
				file.close();
			}

			if (new_username.length() < 3) {
				for (int key = 'A'; key <= 'Z'; key++) {
					if (Game::KeysPresed[key] && clickCooldown <= 0) {
						Game::Sound(L"asteroidsInput.wav", false);

						clickCooldown = .25f;
						new_username += (char)('A' + (key - 'A'));

						endingUsername = refreshText(endingUsername, new_username);
					}
				}
				if ((Game::KeysPresed['Back'] || Game::KeysPresed['Del']) && clickCooldown <= 0) {
					Game::Sound(L"asteroidsInputBackspace.wav", false);

					clickCooldown = .25f;
					if (new_username.length() != 0) new_username.pop_back();

					endingUsername = refreshText(endingUsername, new_username);
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

				std::ofstream file_out("_asteroidsscoredata.txt"); // opening the file

				for (int i = 0; i < usernames.size(); i++) {
					file_out << (usernames[i] + " " + std::to_string(scores[i]) + "\n"); // writing the scores ('ABC1234', where 'ABC' is the username, and '1234' is the score)
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
					std::string place = std::to_string(i + 1);
					if (i != 9) place = "0" + place;

					std::string nick = "xxx"; //migh be changed to '-' or unknow character for empty space
					std::string score = "000";

					if (usernames.size() > i) {
						nick = usernames[i];
						score = std::to_string(scores[i]);
					}

					while (score.length() < 3) { score = "0" + score; }
					while (score.length() < std::to_string(scores[0]).length()) { score = " " + score; }

					scoreboard[i] = Game::CreateTekst(vec(-animationPos.x * 1.5 / camW, .35 - (.125 * i)), 0, vec(.0215), 1, .5, (place + " " + nick + " " + score)); // these "" will be changed to ". " and " " respectively
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
		if (!isBulletPlayers[i] && Game::collisionCircle(pPos, current->Transform.position)) {
			death();
		}

		//collisions - enemy/bullets
		for (int j = enemies.size() - 1; j >= 0; j--) {
			GameObject* enemy = enemies[j];
			if (isBulletPlayers[i] && Game::collisionCircle(currentPos, enemy->Transform.position)) {
				death(false, enemy->Transform.position, vec(0), false, enemy, enemySizes[0], __enemyVx, __enemyInd);
				enemies.erase(enemies.begin() + j);
				if (enemyType[j]) eBDPos.erase(eBDPos.begin() + j);
				enemyShootCooldown.erase(enemyShootCooldown.begin() + j);

				score += ufoXP[enemyType[j]];
				enemyType.erase(enemyType.begin() + j);
				tScore = refreshText(tScore, std::to_string(score), true);

				bulletTimeRemain[i] = 0;
				shouldSkip = true;
				break;
			}
		}
		if (shouldSkip) continue;

		//collisions - asteroids/bullets
		for (int j = asteroids.size() - 1; j >= 0; j--) {
			GameObject* asteroid = asteroids[j];
			if (Game::collisionCircle(currentPos, asteroid->Transform.position)) {
				int type = asteroidSize[j];
				if (type < 2) {
					float ori = asteroid->Transform.orientation.z;
					vec pos = asteroid->Transform.position;
					float randomChange = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 45.0f)); // [-45;45]

					spawnAsteroids(1, type + 1, pos.x, pos.y, ori + randomChange);
					spawnAsteroids(1, type + 1, pos.x, pos.y, ori - randomChange);
				}

				Game::Sound(L"asteroidsDestroy.wav", false);

				Game::Destroy(asteroid);
				asteroids.erase(asteroids.begin() + j);
				asteroidSize.erase(asteroidSize.begin() + j);
				asteroidRotation.erase(asteroidRotation.begin() + j);
				asteroidRotationMultiplier.erase(asteroidRotationMultiplier.begin() + j);

				score += asteroidsXP[type];
				tScore = refreshText(tScore, std::to_string(score), true);

				bulletTimeRemain[i] = 0;
				break;
			}
		}
	}

	if (Game::KeysPresed['W']) {
		if (tutorialStep == 0) {
			tutorialStep++;

			tutorialText = refreshText(tutorialText, "Use A and D to rotate");
		}

		vec shipUp = ship->Up;

		velocity += acceleration * dt * vec(shipUp.x, shipUp.y);
		speed = velocity.x * velocity.x + velocity.y * velocity.y;
		if (speed > velocityd) velocity *= velocityd / speed;
		ship->MoveGlobal(vec(velocity.x * dt, velocity.y * dt, 0));

		ship->activeStage = modelShipFire;

		shipAnimationCooldown -= dt;
		if (shipAnimationCooldown <= 0) {
			ship->activeStage = 0;

			shipAnimationCooldown2 -= dt;
			if (shipAnimationCooldown2 <= 0) {
				shipAnimationCooldown = (rand() % 4) / 2 + 2;
				shipAnimationCooldown2 = (rand() % 2) / 2 + 0.25;

				Game::Sound(L"asteroidsWoosh.wav", false);
			}
		}

		for (auto& star : stars) {
			star->MoveGlobal(vec(shipUp.x * -starsSpeedMultiplier, shipUp.y * -starsSpeedMultiplier, 0) * dt);
			checkBounds(star, false, vec(160, 90));
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

			tutorialText = refreshText(tutorialText, "Press SPACE to shoot");
		}

		ship->Rotate(vec(0, 0, 1.0f) * rotationMultiplier * dt);
	}
	if (tutorialStep > 0 && (Game::KeysPresed['D'])) {
		if (tutorialStep == 1) {
			tutorialStep++;

			tutorialText = refreshText(tutorialText, "Press SPACE to shoot");
		}

		ship->Rotate(vec(0, 0, -1.0f) * rotationMultiplier * dt);
	}

	if (tutorialStep > 2 && Game::KeysPresed['E'] && jumpCooldown <= 0.0f) {
		bool forceTeleport = false;

		if (tutorialStep == 3) {
			tutorialStep++;

			tutorialText = refreshText(tutorialText, "Dont use it too much");
			forceTeleport = true;
		}

		Game::Sound(L"asteroidsPlayerTeleport.wav", false);

		jumpCooldown = 0.5f;

		int random = rand() % 32 - 1;
		if (!forceTeleport && random >= 24 && random <= 31) {
			death(true);
		}
		else {
			random = rand() % 8 - 1;
			random = (random * 2) + 4;

			if (!forceTeleport && random < asteroids.size()) {
				death(true);
			}
			else {
				ship->MoveTo(vec(rand() % (160 - jumpMargin) * 2 - 160 - jumpMargin, rand() % (90 - jumpMargin) * 2 - 90 - jumpMargin, -80));
				velocity = vec(0.0f, 0.0f);

				haloCoundtown = 5.0f;
			}
		}
	}

	if (tutorialStep > 1 && Game::KeysPresed['Spc'] && shootCooldown <= 0) {
		if (tutorialStep == 2) {
			tutorialStep++;

			tutorialText = refreshText(tutorialText, "Press E to teleport");
		}

		Game::Sound(L"asteroidsPlayerShoot.wav", false);

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

		vec pos = vec(current->Transform.position.x, current->Transform.position.y);

		if (!type) sPos = vec(ship->Transform.position.x, ship->Transform.position.y);
		else {
			sPos = vec(eBDPos[bigEnemyIterator].x, eBDPos[bigEnemyIterator].y);
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

		vec dMov = vec(sPos.x - pos.x, sPos.y - pos.y);

		if (dMov.x > maxEnemyVelocity) dMov.x = maxEnemyVelocity;
		else if (dMov.x < -maxEnemyVelocity) dMov.x = -maxEnemyVelocity;

		if (dMov.y > maxEnemyVelocity) dMov.y = maxEnemyVelocity;
		else if (dMov.y < -maxEnemyVelocity) dMov.y = -maxEnemyVelocity;

		current->MoveGlobal(vec(dMov, 0.0f) * dt);

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

			Game::Sound(L"asteroidsEnemyShoot.wav", false);

			shoot(current->Transform.position, vec(0.0f, 0.0f, _angle), 1, type);
			enemyShootCooldown[i] = _enemyShootCooldown[(int)type];
			isBulletPlayers.push_back(false);
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

		//collisions - player/enemy
		if (Game::collisionCircle(pPos, pos)) {
			death();
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

		checkBounds(current, false, vec(camera->cameraWidth + bounds, camera->cameraHeight + bounds));

		//collisions - player/asteroid
		if (Game::collisionCircle(pPos, current->Transform.position)) {
			death();
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

			tutorialText = refreshText(tutorialText, ("Wave " + std::to_string(wave_num)));
		}
	}
	if (_return != 0) {
		enemyDelay -= dt;
		if (enemyDelay <= 0) {
			//Game::Sound(L"asteroidsEnemySpawn.wav", false);

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

			spaceship->MoveTo(vec(pos, -80));

			float _angle = atan2(pPos.y - pos.y, pPos.x - pos.x);
			_angle = _angle * 180 / M_PI - 90;

			spaceship->RotateTo(vec(0, 0, _angle));

			isSpaceship = true;

			maxSpaceshipCooldown *= (1 - spaceshipCooldownDelta / 100.0f);
		}
		if (isSpaceship) {
			spaceship->Move(vec(0, 1, 0) * spaceshipSpeed * dt);

			//collisions - spaceship/player
			if (!hasSpaceshipPlayedSound && Game::collisionCircle(vec(spaceship->Transform.position), pPos, 17.5f, 17.5f)) {
				hasSpaceshipPlayedSound = true;
				Game::Sound(L"asteroidsLoudWoosh.wav", false);
			}
			if (Game::collisionCircle(vec(spaceship->Transform.position), pPos)) death();

			if (checkBounds(spaceship, false, vec(camW + 2 * bounds, camH + 2 * bounds))) {
				spaceship->MoveTo(vec(-1000, -1000, -80));
				isSpaceship = false;
				score += 10;
				tScore = refreshText(tScore, std::to_string(score));
			}
		}
	}

	if (haloCoundtown <= 0 && haloCoundtown != 5.0f) { //so that it doesnt appear right on
		halo->Transform.position.z = 100; //hide
	}
	else {
		haloCoundtown -= dt;

		halo->MoveTo(vec(pPos, -80));
		halo->Rotate(vec(0, 0, 1) * dt * speed * haloRotation);
	}
}