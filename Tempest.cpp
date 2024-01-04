#include "Game.h"

using namespace glm;

namespace Tempest {

		unsigned long long lvlhardness;
		std::vector <GameObject*> tunel;

		void tunelspawn(unsigned long long lvlhardness) { //mo¿e dodac unsigned int type, zobaczymy

		std::vector<float> v;
		std::vector<unsigned int> id;
		std::vector<vec2> points;

		unsigned int tunelSidesNo;

		unsigned int tunnelstyle;
		if (lvlhardness < 21) tunnelstyle = 1;
		else if (lvlhardness < 51) tunnelstyle = 2;
		else if (lvlhardness < 71) tunnelstyle = 3;
		else if (lvlhardness < 100) tunnelstyle = 4;
		else tunnelstyle = rand() % 3 + 1;

		switch (tunnelstyle) {
			case 1: {
				unsigned int type = rand() % 3;
				//type 0 wielokat foremny, 1 wielokat "losowy", 2 trójkat, 3 kwadrat
				switch (type) {
				case (0 || 1):

					if (lvlhardness < 6) tunelSidesNo = 3 + lvlhardness + rand() % 7 + 2;
					else if (lvlhardness < 10) tunelSidesNo = lvlhardness + rand() % 4 + 1;
					else if (lvlhardness < 16) tunelSidesNo = lvlhardness + rand() % 7 - 2;
					else tunelSidesNo = lvlhardness + rand() % 8 - 3;
					break;

				case 2:

					if (lvlhardness < 6) tunelSidesNo = 3 * (rand() % 2 + 5);
					else if (lvlhardness < 10) tunelSidesNo = 3 * (rand() % 3 + 5);
					else if (lvlhardness < 16) tunelSidesNo = 3 * (rand() % 4 + 5);
					else tunelSidesNo = 3 * (rand() % 4 + 6);
					break;

				case 3:
					if (lvlhardness < 6) tunelSidesNo = 4 * (rand() % 2 + 4);
					else if (lvlhardness < 10) tunelSidesNo = 4 * (rand() % 3 + 4);
					else if (lvlhardness < 16) tunelSidesNo = 4 * (rand() % 4 + 4);
					else tunelSidesNo = 4 * (rand() % 4 + 5);
					break;

				default:
					break;
				}

				/*
				for (int i = 0; i < tunelSidesNo; ++i) {
					double angle = 2 * pi * i / asteroidSidesNo;
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

				float rot = (float)(rand()) / ((float)(RAND_MAX / 360.0f));
				asteroidRotation.push_back(rot * pi / 180.0f);

				float rotM = -maxAsteroidRotationMultiplier + (float)(rand()) / ((float)(RAND_MAX / (maxAsteroidRotationMultiplier - (-maxAsteroidRotationMultiplier))));
				asteroidRotationMultiplier.push_back(rotM);

				vec2 pos;
				int temp;

				do {
					temp = rand() % (2 * (camW + bounds)) - (camW + bounds);
					pos.x = temp;

					temp = rand() % (2 * (camH + bounds)) - (camH + bounds);
					pos.y = temp;
				} while (pos.x > -camW - 15 && pos.x < camW + 15 && pos.y > -camH - 15 && pos.y < camH + 15);

				tunel.push_back(Gra->Create(vec3(pos, -90.0f), vec3(0.0f, 0.0f, rot), vec3(minAsteroidsSize + (float)(rand()) / ((float)(RAND_MAX / (maxAsteroidsSize - minAsteroidsSize)))), v, id));*/
				break;
			}
			case 2:
				break;
			case 3:
				break;
			case 4:
				break;
			
			}


using namespace Tempest;

void Game::TempestInit() {

	tunel.clear();
	lvlhardness = 1; //uwa¿ac, mo¿liwe ze bedzie trzeba zmienic na 0/1 + nie wiem czy wgl to jest potrzebne -  moze dac to wczesniej lub wogóle xd
}

void Game::Tempest(float dt) {

	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
		tunelspawn();
	}
}