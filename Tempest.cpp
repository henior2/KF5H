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
		
	//const double M_PI = glm::pi<double>();

	int lvlDif;
	std::vector <GameObject*> tunnel;

	void tunelspawn(int lvlDif,int &lastTSN ) { 
		std::vector<float> v;
		std::vector<unsigned int> id;
		std::vector<vec2> points;

		int tunnelSidesNo;
		do { //dla ró¿norodnoœci
			tunnelSidesNo = rand() % 5 + 5;
		} while (tunnelSidesNo == lastTSN);
		
		int type;
		if (lvlDif < 21) type = 0;
		else if (lvlDif < 51) type = 1;
		else if (lvlDif < 71) type = 2;
		else if (lvlDif < 100) type = 3;
		else type = rand() % 3;

		float maxOffset = .3f; //[%]
		float minOffset = -.2f;
		float tunnelRadius = 5.0f;
		vec3 help(-1, -1, -1); 

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
			//conections
			for (int j = 0; j < points.size() - 1 ; j++) {
				push_back2(id, j, j + 1);
				push_back2(id, j + points.size(), j + points.size() + 1);
				push_back2(id, j + points.size() * 2, j + points.size() * 2 + 1);
				push_back2(id, j + points.size() * 3, j + points.size() * 3 + 1);
			}
			push_back2(id, points.size() - 1, points.size() * 2 - 1);
			push_back2(id, points.size() * 3 - 1, points.size() * 4 - 1);

			for (int i = 0; i < points.size() * 2; i++) {
				push_back2(id, i, i + points.size() * 2);
			}
			break;

		case 2: //figury z dziur¹, do poprawy

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

			help.x = rand() % points.size() * 2;

			//conections
			for (int j = 0; j < points.size() - 1; j++) {
				if ( j != help.x ) push_back2(id, j, j + 1);
				if( j + points.size() != help.x ) push_back2(id, j + points.size(), j + points.size() + 1);
				if ( j + points.size() * 2 != help.x ) push_back2(id, j + points.size() * 2, j + points.size() * 2 + 1);
				if (j + points.size() * 3 != help.x) push_back2(id, j + points.size() * 3, j + points.size() * 3 + 1);
			}
			if (points.size() - 1 != help.x && points.size() * 2 - 1 != help.x) push_back2(id, points.size() - 1, points.size() * 2 - 1);
			if (points.size() * 3 - 1 != help.x && points.size() * 4 - 1 - 1 != help.x) push_back2(id, points.size() * 3 - 1, points.size() * 4 - 1);

			for (int i = 0; i < points.size() * 2; i++) {
				push_back2(id, i, i + points.size() * 2);
			}
		
			break;

		case 3: //do zmienienia kolory

			for (int i = 0; i < ceil((float)tunnelSidesNo / 2.0f); i++) {
				double angle = 2 * M_PI * i / tunnelSidesNo;
				float radius = tunnelRadius * (1 + minOffset + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (maxOffset - minOffset))));

				points.push_back(vec2(radius * sin(angle), radius * cos(angle)));
			}
			for (int k = 0; k < 2; k++) {
				for (int j = 0; j < 2; j++) {
					for (int i = 0; i < points.size(); i++) {
						push_back3(v, points[i].x * (1 - (2 * j)), points[i].y, -12.5f * (k + 1)); //points
						push_back3(v, 1, 1, 1); //color (black)
					}
				}
			}
			//conections
			for (int j = 0; j < points.size() - 1; j++) {
				push_back2(id, j, j + 1);
				push_back2(id, j + points.size(), j + points.size() + 1);
				push_back2(id, j + points.size() * 2, j + points.size() * 2 + 1);
				push_back2(id, j + points.size() * 3, j + points.size() * 3 + 1);
			}
			push_back2(id, points.size() - 1, points.size() * 2 - 1);
			push_back2(id, points.size() * 3 - 1, points.size() * 4 - 1);

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

}
using namespace Tempest;

void Game::TempestInit() {
	debugCooldown = .1f;

	lvlDif = 70; //uwa¿aæ na to w przysz³oœci, ma byc 1 lub 0
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
