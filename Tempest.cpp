#include "Tempest.h"
#include "The real engine/The Real Engine.h"

void Tempest::Init() {
	SetCursor(NULL);
	debugCooldown = .1f;
	lvlDif = 0; //uwa¿aæ na to w przysz³oœci, ma byc 0
	lastTSN = 0;
	tunnel.clear();
	bulletsofplayer.clear();
	enemies[0].clear();
	enemies[1].clear();
	enemies[2].clear();
	enemies[3].clear();
	position = 0;
	superzapperActive = true;
	tunelspawn(lastTSN, type);
};
void Tempest::Update(const float& dt) {
	debugCooldown -= dt;

	if (Game::KeysPresed['1'] && debugCooldown <= 0.0f) {
		debugCooldown = 0.5f;
		lvlDif++;
		for (auto& c : tunnel) {
			Game::Destroy(c);
		}
		tunnel.clear();
		Game::Destroy(blaster);
		tunelspawn(lastTSN, type);
	}

	if (Game::KeysPresed[VK_ESCAPE]) {
		debugCooldown = 0.3f;
		Game::ChangeState(Game_Menu);
	}
		


	if (Game::KeysPresed['A'] && debugCooldown <= 0.0f) {
		debugCooldown = 0.3f;
		shipmovement(!(type > 0), position, type);
	}

	if (Game::KeysPresed['D'] && debugCooldown <= 0.0f) {
		debugCooldown = 0.3f;
		shipmovement(type > 0, position, type);
	}

	if (Game::KeysPresed[' '] && debugCooldown <= 0.0f) {
		debugCooldown = 0.3f;
		shooting(blaster->Transform.position, rotation);
	}

	if (!bulletsofplayer.empty()) {
		bulletmove(bulletsofplayer, dt);
	}
	if (!enemies[0].empty()) {
		tanker(dt);
	}
	if (!enemies[1].empty()) {
		spiker(dt);
	}
	if (!enemies[2].empty()) {
		fuseball(dt);
	}
	if (!enemies[3].empty()) {
		flipper(dt);
	}
	if (Game::KeysPresed['2'] && debugCooldown <= 0.0f) {
		debugCooldown = 0.5f;
		enemies_spawn(0);
	}
	if (Game::KeysPresed['3'] && debugCooldown <= 0.0f) {
		debugCooldown = 0.5f;
		enemies_spawn(1);
	}
	if (Game::KeysPresed['4'] && debugCooldown <= 0.0f) {
		debugCooldown = 0.5f;
		enemies_spawn(2);
	}
	if (Game::KeysPresed['5'] && debugCooldown <= 0.0f) {
		debugCooldown = 0.5f;
		enemies_spawn(3);
	}
	if (Game::KeysPresed['F'] && debugCooldown <= 0.0f) {
		superzapper();
	}
};

void Tempest::push_back2(std::vector<unsigned int>& Vec, unsigned int a1, unsigned int a2) {
	Vec.push_back(a1);
	Vec.push_back(a2);
}
void Tempest::push_back2(std::vector<unsigned int>& Vec, unsigned int a1) {
	Vec.push_back(a1);
	Vec.push_back(a1);
}
void Tempest::push_back3(std::vector<float>& Vec, float a1, float a2, float a3) {
	Vec.push_back(a1);
	Vec.push_back(a2);
	Vec.push_back(a3);
}
void Tempest::push_back3(std::vector<float>& Vec, float a1) {
	Vec.push_back(a1);
	Vec.push_back(a1);
	Vec.push_back(a1);
}
void Tempest::push_back_point(std::vector<float>& Vec, int startIndex, std::vector<float>& pointVec) {
	for (int i = 0; i < 6; i++) {
		Vec.push_back(pointVec[startIndex + i]);
	}
}

int Tempest::findsmallest(std::vector <vec> a) {
	int smallest = 0;
	for (int i = 0; i < a.size(); i++) {
		if (a[smallest].y > a[i].y) smallest = i;
	}
	return smallest;
}
void Tempest::points_move_list(std::vector<vec>& moving, int type, std::vector<vec> pointing) {
	moving.clear();
	int sid = findsmallest(pointing);
	bool xd = false;
	moving.push_back(pointing[sid]);
	for (int i = sid - 1; i >= 0; i--) {
		if (!xd && pointing[i].x == 0) {
			xd = true;
		}
		else {
			moving.push_back(pointing[i]);
		}
	}
	for (int i = point.size()-1; i > sid; i--) {
		if (!xd && pointing[i].x == 0) {
			xd = true;
		}
		else {
			moving.push_back(pointing[i]);
		}
	}
}

void Tempest::shipspawn(int& position, int type) {
	vec a, b;
	if (type == 0) {
		a = move[0], b = move[1];
		position = 0;
	}
	else {
		a = move[move.size() - 1], b = move[0];
		position = move.size() - 1;
	}
	if (type == 0) rotation = vec(0, 0, atan2(b.y - a.y, b.x - a.x) * 180.0f / M_PI + 180.0f);
	else rotation = vec(0, 0, 0);

	if (type == 0) blaster = Game::Create(vec((b.x + a.x) / 2, (b.y + a.y) / 2, (-8 + .25)), rotation, vec((b - a).Length() / 2.25, 3), L"tempest_ship");
	else blaster = Game::Create(vec((b.x + a.x) / 2, (b.y + a.y) / 2, (-8 + .25)), rotation, vec((b - a).Length() / 2.25, 3), L"tempest_ship2");
	//+ .25 so that is't "on top" of the tunne
	//2.25 so that is doesn't take up the whole space
}
void Tempest::shipmovement(bool right, int& position, int type) {

	vec a, b;
	if (right && position == move.size() - 1) {
		position = 0;
		a = move[position]; b = move[position + 1];
	}
	else if (!right && position == 0) {
		position = move.size() - 1;
		a = move[position]; b = move[0];
	}
	else if (right) {
		position++;
		a = move[position];
		if (position == move.size() - 1) b = move[0];
		else b = move[position + 1];
	}
	else {
		position--;
		a = move[position]; b = move[position + 1];
	}


	if (type == 0) rotation = vec(0, 0, atan2(b.y - a.y, b.x - a.x) * 180.0f / M_PI + 180.0f);
	else rotation = vec(0, 0, atan2(b.y - a.y, b.x - a.x) * 180.0f / M_PI + 360.0f);

	blaster->MoveTo(vec((b.x + a.x) / 2, (b.y + a.y) / 2, (-8 + .25)));
	blaster->ScaleTo(vec((b - a).Length() / 2.25, 3));
	blaster->RotateTo(rotation);

}
void Tempest::shooting(vec gun_pos, vec rotation) {
	bulletsofplayer.push_back(Game::Create(gun_pos, vec(rotation), vec(0.2, 3), L"bulletblaster"));
}
void Tempest::bulletmove(std::vector <GameObject*>& bulletsofplayer, float dt) {
	vec help;
	for (int i = 0; i < bulletsofplayer.size(); i++) {
		help = bulletsofplayer[i]->Transform.position;
		if (help.z > -31) {
			bulletsofplayer[i]->Move(vec(0, 0, 15) * dt);
			bulletsofplayer[i]->Rotate(vec(0, 0, 10));
		}
			
		else {
			Game::Destroy(bulletsofplayer[i]);
			bulletsofplayer.erase(bulletsofplayer.begin() + i);
			i--;
		}
	}

}
void Tempest::superzapper() {
	if (superzapperActive) {
		for (int i = 0; i < 4; i++) {
			if (!enemies[i].empty()) {
				for (int j = 0; j < enemies[i].size(); j++) {
					Game::Destroy(enemies[i][j]);
					if (i == 1) Game::Destroy(spike[j]);
				}
				enemies[i].clear();
				enemies_position[i].clear();
				enemies_bool[i].clear();
				if (i == 1) {
					spikers_max.clear();
					spike.clear();
					vx_spike.clear();
				}
			}
			
		}
		superzapperActive = false;
	}
}

void Tempest::tunelspawn(int& lastTSN, int& type) {
	point.clear();
	point2.clear();
	std::vector<float> v;
	std::vector<unsigned int> id;
	std::vector<vec> points;

	int tunnelSidesNo ;
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

	vec help;
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

			points.push_back(vec(radius * cos(angle), radius * sin(angle), 0));
		}
		for (int j = 0; j < 2; j++) {
			for (int i = 0; i < points.size(); i++) {
				if (j) push_back2(id, i, i + points.size()); //connections
				push_back2(id, i + points.size() * j, i + points.size() * j + 1); //ring

				push_back3(v, points[i].x, points[i].y, -25 * j - 8); //points
				if (j == 0) point.push_back(vec(points[i].x, points[i].y, -25 * j - 8));
				else point2.push_back(vec(points[i].x, points[i].y, -25 * j - 8));

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

			points.push_back(vec(radius * sin(angle), radius * cos(angle), 0));
		}
		for (int k = 0; k < 2; k++) {
			for (int j = 0; j < 2; j++) {
				for (const auto& c_point : points) { //refactored this loop into the for-each loop
					push_back3(v, c_point.x * (1 - (2 * j)), c_point.y, -25 * k - 8); //points
					if (k == 0) point.push_back(vec(c_point.x * (1 - (2 * j)), c_point.y, -25 * k - 8));
					else point2.push_back(vec(c_point.x * (1 - (2 * j)), c_point.y, -25 * k - 8));

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

	points_move_list(move, type, point);
	points_move_list(move2, type, point2);
	lastTSN = tunnelSidesNo;
	zhelp = move2[0].z;
	zhelp2 = move[0].z;
		
	if (type == 1) tunnel.push_back(Game::Create(vec(0, 3), vec(0, 3), vec(1, 3), v, id));
	else {
		std::vector<float> vxVec;
		std::vector<unsigned int> indVec = { 0,1,1,2,2,3,3,0 }; //connections
		for (int i = 0; i < tunnelSidesNo; i++) {
			vxVec.clear();

			int tryPush = i + 1;
			if (i == tunnelSidesNo - 1) tryPush = 0; //so that it loops over

			push_back_point(vxVec, i * 6, v); //point #1 (front)
			push_back_point(vxVec, tryPush * 6, v); //point #2 (front)
			push_back_point(vxVec, (tryPush + tunnelSidesNo) * 6, v); //point #3 (aka #2 back)
			push_back_point(vxVec, (i + tunnelSidesNo) * 6, v); //point #4 (#1 back)

			tunnel.push_back(Game::Create(vec(0, 3), vec(0, 3), vec(1, 3), vxVec, indVec));
		}
	}

	shipspawn(position, type);
	superzapperActive = true;
}

void Tempest::enemies_spawn(int type2) {
	std::wstring model = enemy_models[type2];

	int spawn1 = rand() % move2.size();
	int spawn2;

	vec a = move2[spawn1];
	if (spawn1 == move2.size() - 1) spawn2 = 0;
	else spawn2 = spawn1 + 1;
	vec b = move2[spawn2];

	vec place;
	vec scale((b - a).Length() / 1.5, 3);
	if (type2 == 2) {
		place = a;
		fusbal_time.push_back(4.0f);
		fmove.push_back(vec(0,3));
		fwhere.push_back(vec(0, 3));
		cooldown.push_back(0);
	}
		
	else if (type2 == 3) {
		place = vec((b.x + a.x) / 2, (b.y + a.y) / 2, b.z);
		scale.x /= 1.3;
		scale.y /= 1.3;
		scale.z /= 1.3;
	}
	else {
		place = vec((b.x + a.x) / 2, (b.y + a.y) / 2, b.z);
		scale.x /= 1.85;
		scale.y /= 1.85;
		scale.z /= 1.85;
	}

	vec rotation;
	if (type == 0) rotation = vec(0, 0, atan2(b.y - a.y, b.x - a.x) * 180.0f / M_PI + 180.0f);
	else rotation = vec(0, 0, atan2(b.y - a.y, b.x - a.x) * 180.0f / M_PI);

	enemies[type2].push_back(Game::Create(place, rotation, scale, model));
	enemies_position[type2].push_back(spawn1);
	enemies_bool[type2].push_back(false);

	if (type2 == 1) {
		spikers_max.push_back(rand() % 5 - 14);

		help.clear();
		push_back3(help, place.x, place.y, place.z);
		push_back3(help, 0, 1, 0);
		push_back3(help, place.x, place.y, place.z);
		push_back3(help, 1, 1, 1);
		vx_spike.push_back(help);

		spike.push_back(Game::Create(vec(0, 3), vec(0, 3), vec(1, 3), vx_spike[vx_spike.size() - 1], ind_spikes));
		spike[spike.size() - 1]->SetColor(vec(0, 1, 0));
	}

}

void Tempest::enemies_spawn(int type2, int positionofshipinvec, int typeofspawner, float z) {
	std::wstring model = enemy_models[type2];

	int spawn1 = enemies_position[typeofspawner][positionofshipinvec];
	int spawn2;

	vec a = move2[spawn1];
	if (spawn1 == move2.size() - 1) spawn2 = 0;
	else spawn2 = spawn1 + 1;
	vec b = move2[spawn2];

	vec place;
	vec scale((b - a).Length() / 1.5, 3);
	if (type2 == 2)
		place = vec(a.x, a.y, z);
	else if (type2 == 3) {
		place = vec((b.x + a.x) / 2, (b.y + a.y) / 2, z);
		scale.x /= 1.3;
		scale.y /= 1.3;
		scale.z /= 1.3;
	}
	else {
		place = vec((b.x + a.x) / 2, (b.y + a.y) / 2, z);
		scale.x /= 1.85;
		scale.y /= 1.85;
		scale.z /= 1.85;
	}

	vec rotation;
	if (type == 0) rotation = vec(0, 0, atan2(b.y - a.y, b.x - a.x) * 180.0f / M_PI + 180.0f);
	else rotation = vec(0, 0, atan2(b.y - a.y, b.x - a.x) * 180.0f / M_PI);

	enemies[type2].push_back(Game::Create(place, rotation, scale, model));
	enemies_position[type2].push_back(spawn1);
	enemies_bool[type2].push_back(false);

}

void Tempest::tanker(float dt) {
	vec help, xd;
	for (int i = 0; i < enemies[0].size(); i++) {
		help = enemies[0][i]->Transform.position;
		if (help.z < zhelp2)
			enemies[0][i]->Move(vec(0, 0, -3) * dt);
		else {
			xd = enemies[0][i]->Transform.position;
			Game::Destroy(enemies[0][i]);
			enemies_spawn(3, i, 0, xd.z);
			enemies[0].erase(enemies[0].begin() + i);
			enemies_position[0].erase(enemies_position[0].begin() + i);
			enemies_bool[0].erase(enemies_bool[0].begin() + i);
			i--;
		}
	}
}
void Tempest::fuseball(float dt) {
	vec help;
	int direction, pos, speed;

	for (int i = 0; i < enemies[2].size(); i++) {
		help = enemies[2][i]->Transform.position;


		if (help.z < -7.5 && !enemies_bool[2][i]) {
			speed = -1;
		}
		else if (help.z >= -7.5 && !enemies_bool[2][i]) {
			enemies_bool[2][i] = true;
			speed = 1;
		}
		else if (help.z > zhelp && enemies_bool[2][i]) {
			speed = 1;
		}
		else if (help.z <= -7.5 && enemies_bool[2][i]) {
			enemies_bool[2][i] = false;
			speed = -1;
		}


		if (fusbal_time[i] == 4.0f) {
			direction = rand() % 41 + 1; //1 20 do przodu,  21 30 do ty³u, 31 35 w prawo, 36 40 w lewo 
			if (cooldown[i] <= 0) {
				if ((direction >= 31 && direction <= 35) || (direction >= 36 && direction <= 40)) {
					pos = enemies_position[2][i];
					vec b, a = move[pos];

					if (direction >= 31 && direction <= 35) {
						if (pos == move2.size() - 1)  pos = 0;
						else pos++;
						b = move2[pos];
					}
					else if ((direction >= 36 && direction <= 40)) {
						if (pos == 0) pos = move2.size() - 1;
						else pos--;
						b = move2[pos];
					}
					enemies_position[2][i] = pos;

					fmove[i] = b - a;
					fwhere[i] = b;
					fusbal_time[i] -= dt;
					cooldown[i] = 10.0f;

				}
				else {

					if (direction >= 1 && direction <= 20) {
						speed = speed * (rand() % 6 + 15);
						enemies[2][i]->Move(vec(0, 0, speed) * dt);

					}
					else if (direction >= 21 && direction <= 30) {
						speed = speed * (rand() % 20 + 35);
						enemies[2][i]->Move(vec(0, 0, speed) * dt);

					}
					speed = rand() % 3 - 1;
					enemies[2][i]->Rotate(vec(0, 0, speed));
				}
			}
			else {

				if (direction >= 1 && direction <= 20) {
					speed = speed * (rand() % 6 + 15);
					enemies[2][i]->Move(vec(0, 0, speed) * dt);

				}
				else if (direction >= 21 && direction <= 30) {
					speed = speed * (rand() % 20 + 35);
					enemies[2][i]->Move(vec(0, 0, speed) * dt);

				}
				speed = rand() % 3 - 1;
				enemies[2][i]->Rotate(vec(0, 0, speed));
				cooldown[i]--;

			}
		}
		else if (fusbal_time[i] > 0.0f) {
			fusbal_time[i] -= dt;
			direction = 4 / dt;
			enemies[2][i]->MoveGlobal(vec(fmove[i].x / direction, fmove[i].y / direction, 0));
			vec xd = fmove[i] & (fwhere[i] - enemies[2][i]->Transform.position);
			if (xd.x <= 0 && xd.y <= 0 && xd.z <= 0) {
				enemies[2][i]->MoveTo(fwhere[i]);
				speed = speed * (rand() % 10 + 15);
				enemies[2][i]->Move(vec(0, 0, speed) * dt);
			}
			speed = rand() % 3 - 1;
			enemies[2][i]->Rotate(vec(0, 0, speed));
		}
		else {
			fusbal_time[i] = 4.0f;
			speed = speed * (rand() % 10 + 15);
			enemies[2][i]->Move(vec(0, 0, speed) * dt);
			cooldown[i] = 5;
		}

	}
	
	
	
}

void Tempest::spiker(float dt) {

	for (int i = 0; i < enemies[1].size(); i++) {

		vec help = enemies[1][i]->Transform.position;

		if (help.z < spikers_max[i] && enemies_bool[1][i] == false) {

			enemies[1][i]->Move(vec(0, 0, -3) * dt);
			enemies[1][i]->Rotate(vec(0, 0, 2));

			help = enemies[1][i]->Transform.position;
			spike[i]->Object.verticies.vertecies[6] = help.x;
			spike[i]->Object.verticies.vertecies[7] = help.y;
			spike[i]->Object.verticies.vertecies[8] = help.z; 
		}
		else if (enemies_bool[1][i] == false) {
			enemies_bool[1][i] = true;
		}

		else if (help.z > zhelp ) {
			enemies[1][i]->Move(vec(0, 0, 3) * dt);
			enemies[1][i]->Rotate(vec(0, 0, -2));
		} 
		else {
			Game::Destroy(enemies[1][i]);
			enemies_spawn(0, i, 1, help.z);
			enemies[1].erase(enemies[1].begin() + i);
			enemies_position[1].erase(enemies_position[1].begin() + i);
			enemies_bool[1].erase(enemies_bool[1].begin() + i);
			spikers_max.erase(spikers_max.begin() + i);
			spike.erase(spike.begin() + i);
			vx_spike.erase(vx_spike.begin() + i);

		}
	}
}

void Tempest::flipper(float dt) {
		bool d;
		for (int i = 0; i < enemies[3].size(); i++) {
			vec help = enemies[3][i]->Transform.position;
			if (help.z < zhelp2) {
				enemies[3][i]->Move(vec(0, 0, -3) * dt); 
			}
			else {
				d = rand() % 2 + 1;
			}
		}
	}
