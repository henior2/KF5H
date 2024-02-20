#include "Game.h"
#include "fstream"
#include "string"

using namespace glm;

namespace Tempest {

	float debugCooldown = .1f;
	int lastTSN;
	int lvlDif;
	int type;
	
	void zapisywanie(vec3 data) {
		std::ofstream out;
		out.open("move_vector.txt", std::ios_base::app);
		
		std::string datax = std::to_string(data.x);
		std::string datay = std::to_string(data.y);
		std::string dataz = std::to_string(data.z);
		std::string zdanie = datax + ' ' + datay + ' ' + dataz + '\n';
		out << zdanie;
		out.close();
	}
	std::vector <GameObject*> tunnel;
	GameObject* blaster;
	std::vector <GameObject*> bulletsofplayer;
	std::vector <vec3> move;
	std::vector <vec3> point;
	vec3 rotation;
	int position;
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

	int findsmallest(std::vector <vec3> a) {
		int smallest = 0;
		for (int i = 0; i < a.size(); i++) {
			if (a[smallest].y > a[i].y) smallest = i;
		}
		return smallest;
	}
	void points_move_list(std::vector<vec3>& move, int type) {
		move.clear();
		int sid = findsmallest(point);
		bool xd = false;
		move.push_back(point[sid]);
		for (int i = sid - 1; i >= 0; i--) {
			if (!xd && point[i].x == 0) {
				xd = true;
			}
			else {
				move.push_back(point[i]);
			}
		}
		for (int i = sid + 1; i < point.size(); i++) {
			if (!xd && point[i].x == 0) {
				xd = true;
			}
			else {
					move.push_back(point[i]);
			}
		}
	}

	void shipspawn(int& position, int type) {
		vec3 a, b;
		if (type == 0) { 
			a = move[0], b = move[1]; 
			position = 0;
		}
		else  {
			a = move[move.size() - 1], b = move[0];
			position = move.size() - 1;
		}
		if (type==0) rotation = vec3(0, 0, atan2(b.y - a.y, b.x - a.x) * 180.0f / M_PI + 180.0f);
		else rotation = vec3(0, 0, 0);

		blaster = Gra->Create(vec3((b.x + a.x) / 2,(b.y + a.y) / 2, (b.z + a.z) / 2 + .25), rotation, vec3(glm::length(b - a) / 2.25), "tempest_ship");
		//+ .25 so that is't "on top" of the tunnel
		//2.25 so that is doesn't take up the whole space
		}

	void tunelspawn( int& lastTSN, std::vector <vec3>& point, int& type) {
		point.clear();
		std::vector<float> v;
		std::vector<unsigned int> id;
		std::vector<vec2> points;

		int tunnelSidesNo;
		do {
			tunnelSidesNo = rand() % 5 + 5;
		} while (tunnelSidesNo == lastTSN);

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

		point.clear();
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
					if (j == 0) {
						point.push_back(vec3(points[i].x, points[i].y, -16.5f * (j + 1) + 2));
						//zapisywanie(1, vec3(points[i].x, points[i].y, -16.5f * (j + 1) + 2));
					}
					if (lvlDif > 71) push_back3(v, help.x, help.y, help.z);
					else push_back3(v, 0, 0, 1); //color (blue)
				}
				id.pop_back();
				id.push_back(points.size() * j);
			}
			push_back2(id, 0, points.size());
			break;
			int x;
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
						if (k == 0)
							point.push_back(vec3(points[i].x * (1 - (2 * j)), points[i].y, -16.5f * (k + 1) + 2));
								
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

		points_move_list(move, type);
		lastTSN = tunnelSidesNo;
		tunnel.push_back(Gra->Create(vec3(0), vec3(0), vec3(1), v, id));
		shipspawn(position, type);
	}

	void shipmovement(bool right, int& position) {
		vec3 a, b;
		if (right && position == point.size() - 1) { 
			position = 0;
			a = move[position], b = move[position + 1];
		}
		else if (!right && position == 0) {
			position = point.size() - 1;
			a = move[position], b = move[0];
		}
		else if (right) { 
			position++; 
			a = move[position];
			if (position == point.size() - 1) b = move[0];
			else b = move[position + 1];
		}
		else {
			position--;
			a = move[position], b = move[position +1];
		}

		if (type == 0) rotation = vec3(0, 0, atan2(b.y - a.y, b.x - a.x) * 180.0f / M_PI + 180.0f);
		else rotation = vec3(0, 0, atan2(b.y - a.y, b.x - a.x) * 180.0f / M_PI + 360.0f);

		blaster->MoveTo(vec3((b.x + a.x) / 2, (b.y + a.y) / 2, (b.z + a.z) / 2 + .25));
		blaster->ScaleTo(vec3(glm::length(b - a) / 2.25));
		blaster->RotateTo(rotation);
	}
	void shooting(vec3 gun_pos, vec3 rotation) {
		bulletsofplayer.push_back(Gra->Create(gun_pos, vec3(rotation), vec3(0.3), "bulletblaster"));
	}
	void bulletmove(std::vector <GameObject*>& bulletsofplayer, float dt) {
		vec3 help;
		GameObject* xd;
		for (int i = 0; i < bulletsofplayer.size(); i++) {
			help = bulletsofplayer[i]->Transform.position;
			if (help.z > -31)
				bulletsofplayer[i]->Move(vec3(0, 0, 10)*dt);
			else {
				Gra->Destroy(bulletsofplayer[i]);
				bulletsofplayer.erase(bulletsofplayer.begin() + i);
				i--;
			}			
		}

	}
}
using namespace Tempest;

void Game::TempestInit() {
	debugCooldown = .1f;
	lvlDif = 21; //uwa¿aæ na to w przysz³oœci, ma byc 0
	lastTSN = 0;
	tunnel.clear();
	bulletsofplayer.clear();
	position = 0;
	tunelspawn(lastTSN, point, type);
}

void Game::Tempest(float dt) {
	debugCooldown -= dt;

	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS && debugCooldown <= 0.0f) {
		debugCooldown = 0.5f;
		lvlDif++;
		for (auto& c : tunnel) {
			Destroy(c);
		}
		tunnel.clear();
		Destroy(blaster);
		tunelspawn(lastTSN, point, type);
	}

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		Game::ChangeState(Game_Menu);

	if ((glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) && debugCooldown <= 0.0f) {
		debugCooldown = 0.3f;
		shipmovement(false, position);
	}

	if ((glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) && debugCooldown <= 0.0f) {
	debugCooldown = 0.3f;
	shipmovement(true, position);
	}

	if ((glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) && debugCooldown <= 0.0f) {
		debugCooldown = 0.3f;
		shooting(blaster->Transform.position, rotation);
	}

	if (!bulletsofplayer.empty()) {
		bulletmove(bulletsofplayer, dt);
	}
}
