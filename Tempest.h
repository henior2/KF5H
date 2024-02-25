#pragma once
#include "The Real Engine/GameObject.h"
#include "The Real Engine/TextBox.h"

#include <vector>
#include <string>
#include <fstream>

class Tempest
{
public:
    void Init();
    void Update(const float& dt);
private:
	float debugCooldown = .1f;
	int lastTSN;
	int lvlDif;
	int type;

	const std::vector<std::wstring> enemy_models = {
		L"Tanker",
		L"Spiker",
		L"Fuseball",
		L"Flipper"
	};

	std::vector <GameObject*> tunnel;
	GameObject* blaster;
	std::vector <GameObject*> bulletsofplayer;
	std::vector <GameObject*> enemies[4];
	std::vector <int> enemies_position[4];
	std::vector <vec> move;
	std::vector <vec> move2;
	std::vector <vec> point;
	std::vector <vec> point2;
	vec rotation;
	int position;

	void push_back2(std::vector<unsigned int>& vec, unsigned int a1, unsigned int a2);
	void push_back2(std::vector<unsigned int>& vec, unsigned int a1);
	void push_back3(std::vector<float>& vec, float a1, float a2, float a3);
	void push_back3(std::vector<float>& vec, float a1);
	void push_back_point(std::vector<float>& vec, int startIndex, std::vector<float>& pointVec);

	int findsmallest(std::vector <vec> a);
	void points_move_list(std::vector<vec>& moving, int type, std::vector<vec> pointing);

	void shipspawn(int& position, int type);
	void shipmovement(bool right, int& position, int type);
	void shooting(vec gun_pos, vec rotation);
	void bulletmove(std::vector <GameObject*>& bulletsofplayer, float dt);

	void tunelspawn(int& lastTSN, int& type);

	void enemies_spawn(int type2);
	void tanker(float dt);
	void fuseball(float dt);
};

