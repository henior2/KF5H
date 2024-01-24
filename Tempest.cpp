#include "Game.h"

using namespace glm;

namespace Tempest {
	float debugCooldown = .1f;
	int lastTSN;

	void push_back2(std::vector<unsigned int>& vec, unsigned int a1, unsigned int a2) {
		vec.push_back(a1);
		vec.push_back(a2);
	}
	void push_back2(std::vector<unsigned int>& vec, unsigned int a1) {
		vec.push_back(a1);
		vec.push_back(a1);
	}

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

	int lvlDif;
	std::vector <GameObject*> tunnel;

	void tunelspawn(int lvlDif, int& lastTSN) {
		std::vector<float> v;
		std::vector<unsigned int> id;
		std::vector<vec2> points;

		int tunnelSidesNo;
		do {
			tunnelSidesNo = rand() % 5 + 5;
		} while (tunnelSidesNo == lastTSN);

		int type;
		int type2;
		if (lvlDif < 21) type = 0;
		else if (lvlDif < 51) {
			type = 1;
			type2 = 0;
		}
		else if (lvlDif < 90) {
			type = 1;
			type2 = 1;
		}
		else if (lvlDif < 100) {
			type = 1;
			type2 = rand() % 1;
		}
		else {
			type = rand() % 1;
			type2 = rand() % 1;
		}

		float maxOffset = .2f; //[%]
		float minOffset = -.2f;
		float tunnelRadius = 5.0f;

		vec3 help;
		do {
			help.x = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
			help.y = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
			help.z = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		} while (help.x == 0 && help.y == 0 && help.z == 0);

		switch (type) {
		case 0:

			for (int i = 0; i < tunnelSidesNo; i++) {
				double angle = 2 * M_PI * i / tunnelSidesNo;
				float radius = tunnelRadius * (1 + minOffset + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (maxOffset - minOffset))));

				points.push_back(vec2(radius * cos(angle), radius * sin(angle)));
			}
			for (int j = 0; j < 2; j++) {
				for (int i = 0; i < points.size(); i++) {
					if (j) push_back2(id, i, i + points.size()); //connections
					push_back2(id, i + points.size() * j, i + points.size() * j + 1); //ring

					push_back3(v, points[i].x, points[i].y, -16.5f * (j + 1) + 2); //points
					if (lvlDif > 71) push_back3(v, help.x, help.y, help.z);
					else push_back3(v, 0, 0, 1); //color (blue)
				}
				id.pop_back();
				id.push_back(points.size() * j);
			}
			push_back2(id, 0, points.size());
			break;

		case 1:  // odbicia lustrzane
			for (int i = 0; i < ceil((float)tunnelSidesNo / 2.0f); i++) {
				double angle = 2 * M_PI * i / tunnelSidesNo;
				float radius = tunnelRadius * (1 + minOffset + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (maxOffset - minOffset))));

				points.push_back(vec2(radius * sin(angle), radius * cos(angle)));
			}
			for (int k = 0; k < 2; k++) {
				for (int j = 0; j < 2; j++) {
					for (int i = 0; i < points.size(); i++) {
						push_back3(v, points[i].x * (1 - (2 * j)), points[i].y, -16.5f * (k + 1) + 2); //points
						if (lvlDif > 89 && lvlDif < 100) push_back3(v, 0, 0, 0); //color (black) - be carefull!!!
						else if (lvlDif > 71) push_back3(v, help.x, help.y, help.z); // color (random)
						else push_back3(v, 0, 0, 1); //color (blue)
					}
				}
			}
			//conections
			if (type2 == 0) {//without hole
				for (int j = 0; j < points.size() - 1; j++) {
					push_back2(id, j, j + 1);
					push_back2(id, j + points.size(), j + points.size() + 1);
					push_back2(id, j + points.size() * 2, j + points.size() * 2 + 1);
					push_back2(id, j + points.size() * 3, j + points.size() * 3 + 1);
				}
				push_back2(id, points.size() - 1, points.size() * 2 - 1);
				push_back2(id, points.size() * 3 - 1, points.size() * 4 - 1);
			}
			else {//with hole
				help.x = rand() % points.size() * 2;

				for (int j = 0; j < points.size() - 1; j++) {
					if (j != help.x) {
						push_back2(id, j, j + 1);
						push_back2(id, j + points.size() * 2, j + points.size() * 2 + 1);
					}

					if (j + points.size() != help.x) {
						push_back2(id, j + points.size(), j + points.size() + 1);
						push_back2(id, j + points.size() * 3, j + points.size() * 3 + 1);
					}
				}

				if (points.size() - 1 != help.x && points.size() * 2 - 1 != help.x) {
					push_back2(id, points.size() - 1, points.size() * 2 - 1);
					push_back2(id, points.size() * 3 - 1, points.size() * 4 - 1);
				}
			}
			//conection between rings
			for (int i = 0; i < points.size() * 2; i++) {
				push_back2(id, i, i + points.size() * 2);
			}
				break;

		default:
			throw std::invalid_argument("invalid arg for tunnel type (" + std::to_string(type) + ")");
			}

			lastTSN = tunnelSidesNo;
			tunnel.push_back(Gra->Create(vec3(0), vec3(0), vec3(1), v, id));
		}

	void shipmovement(bool rigorlef) {

	}
	}

using namespace Tempest;

void Game::TempestInit() {
	debugCooldown = .1f;

	lvlDif = 0; //uwa¿aæ na to w przysz³oœci, ma byc 1 lub 0
	lastTSN = 0;

	tunnel.clear();

	GameObject* blaster = Create(vec3(0, 0, -25), vec3(0), vec3(1, 1, 1), "tempest_ship");
}

void Game::Tempest(float dt) {
	debugCooldown -= dt;

	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS && debugCooldown <= 0.0f) {
		debugCooldown = 0.5f;
		lvlDif++;
		if (!tunnel.empty()) {
			for (auto& c : tunnel) {
				Destroy(c);
			}
			tunnel.clear();
		}

		tunelspawn(lvlDif,lastTSN);
	}

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		Game::ChangeState(Game_Menu);
}
