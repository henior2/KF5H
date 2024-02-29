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
	float debugCooldown2 = .1f;
	int lastTSN = 0;
	int lvlDif = 0;
	int type;
	int live;
	float waveCool;
	TextBox* pointsy;
	bool superzapperActive = true;

	const std::vector<std::wstring> enemy_models = {
		L"Tanker",
		L"Spiker",
		L"Fuseball",
		L"Flipper"
	};

	std::vector <unsigned int> ind_spikes { 0,1 };
	std::vector <GameObject*> tunnel;
	GameObject* blaster;
	std::vector <GameObject*> bulletsofplayer;
	std::vector <GameObject*> enemies[4];
	std::vector <int> enemies_position[4];
	std::vector <bool> enemies_bool[4];
	std::vector <int> spikers_max;
	std::vector <float> fusbal_time;
	std::vector <vec> fmove;
	std::vector <vec> fpmove;
	std::vector <vec> fwhere;
	std::vector <vec> fpwhere;
	std::vector <float> cooldown;
	std::vector <float> cooldown2;
	std::vector <std::vector<float>> vx_spike;
	std::vector <GameObject*> spike;
	std::vector <float> help;
	std::vector <vec> move;
	std::vector <vec> move2;
	std::vector <vec> point;
	std::vector <vec> point2;
	vec blok = vec(0, 2);
	int deltaofprize = 80;
	int prize = 15;
	int points_of_player = 0;
	int prize2 = 30;
	int how_much [4] {15,25,35,50};
	vec rotation;
	int position;
	float zhelp;
	float zhelp2;
	std::vector <GameObject*> lives;

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
	void superzapper();

	void tunelspawn();

	void enemies_spawn(int type2);
	void enemies_spawn(int type2, int positionofshipinvec, int typeofspawner, float z, bool where);
	void enemies_spawn(int type2, int positionofshipinvec, int typeofspawner, float z);
	void tanker(float dt);
	void fuseball(float dt);
	void spiker(float dt);
	void flipper(float dt);

	void text1();
	void mechanics(const float dt);
};

