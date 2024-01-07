#include "Game.h"

using namespace glm;

namespace Tempest {
	float debugCooldown = .1f;
	int lastTSN;

	//to make the code a bit cleaner
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

	const double M_PI = glm::pi<double>();

	int lvlDif;
	std::vector <GameObject*> tunnel;

	void tunelspawn(int lvlDif,int &lastTSN ) { 
		std::vector<float> v;
		std::vector<unsigned int> id;
		std::vector<vec2> points;

		int tunnelSidesNo;
		
		do { //dla ró¿norodnoœci
			tunnelSidesNo = rand() % 7 + 5;
		} while (tunnelSidesNo == lastTSN);
	
		int type; //do not touch :) //i wont if you stop being obsessed with putting unsigned everywhere :)
		if (lvlDif < 21) type = 0;
		else if (lvlDif < 51) type = 1;
		else if (lvlDif < 71) type = 3;
		else if (lvlDif < 100) type = 4;
		else type = rand() % 4;

		const float maxOffset = .1f; //[%]
		const float minOffset = -.1f;
		const float tunnelRadius = 5.0f;

		switch (type) { 
		case 0:
			// podcase 2 i 3  to trojkat i kwadrat - bed¹ w ifach

			/*
			if (tunnelSidesNo % 3 == 0 && lastTSN % 3 == 0 && tunnelSidesNo / 3 > 2){}
			else if (tunnelSidesNo % 4 == 0 && lastTSN % 2 == 0 && tunnelSidesNo / 4 > 2){}
			else{}
			*/

			for (int i = 0; i < tunnelSidesNo; i++) {
				double angle = 2 * M_PI * i / tunnelSidesNo;
				float radius = tunnelRadius * (1 + minOffset + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (maxOffset - minOffset))));

				points.push_back(vec2(radius * cos(angle), radius * sin(angle)));
			}
			for (int j = 0; j < 2; j++) {
				for (int i = 0; i < points.size(); i++) {
					if (j) push_back2(id, i, i + points.size()); //connections
					push_back2(id, i + points.size() * j, i + points.size() * j + 1); //ring

					push_back3(v, points[i].x, points[i].y, -12.5f * (j + 1)); //points
					push_back3(v, 0, 0, 1); //color (blue)
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
						push_back3(v, points[i].x * (1 - (2 * j)), points[i].y, -12.5f * (k + 1)); //points
						push_back3(v, 0, 0, 1); //color (blue)
					}
				}
			}
			//still work in progress!!
			for (int j = 0; j < 2; j++) {
				for (int i = 0; i < points.size() * 2; i++) {
					if (j) push_back2(id, i, i + points.size() * 2);
					push_back2(id, i, i + 1);
				}
				id.pop_back();
				id.push_back(points.size());
			}
			break;
		default:
			throw std::invalid_argument("invalid arg for tunnel type (" + std::to_string(type) + ")");
		}

		lastTSN = tunnelSidesNo;
		tunnel.push_back(Gra->Create(vec3(0), vec3(0), vec3(1), v, id));
	}

}
using namespace Tempest;

void Game::TempestInit() {
	debugCooldown = .1f;

	lvlDif = 19; //uwa¿aæ na to w przysz³oœci
	lastTSN = 0;

	tunnel.clear();
}

void Game::Tempest(float dt) {
	debugCooldown -= dt;

	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS && debugCooldown <= 0.0f) {
		debugCooldown = 0.5f;

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
