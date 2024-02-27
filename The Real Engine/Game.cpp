#include "Game.h"
#include <windows.h>

std::map<UINT, bool> Game::KeysPresed;
std::vector<GameObject*> Game::Objects;
std::vector<TextBox*> Game::Texts;
Camera* Game::camera = new Camera();

Games Game::games;
GameState Game::State;

POINT Game::MousePosition;

HMODULE Game::hMod = GetModuleHandle(NULL);

void Game::ChangeState(const GameState& state) {
	camera->Position = vec(0, 3);
	camera->Yaw = YAW;
	camera->Pitch = PITCH;
	camera->RotateCamera(0, 0);
	StopSounds();
	DeleteGame();
	State = state;
	if (state == Game_Menu) {
		games.menu = new Menu();
		games.menu->Init();
	}
	else if (state == Game_Init) {
		games.Init = new Initialization();
		games.Init->Init();
	}
	else if (state == Game_TEST) {
		games.test = new TEST();
		games.test->Init();
	}
	else if (state == Game_Asteroids) {
		games.asteroids = new Asteroids();
		games.asteroids->Init();
	}
	else if (state == Game_Battlezone) {
		games.battlezone = new Battlezone();
		games.battlezone->Init();
	}
	else if (state == Game_Tempest) {
		games.tempest = new Tempest();
		games.tempest->Init();
	}
}

void Game::Update(const float& dt) {
	GetCursorPos(&MousePosition);
	if (State == Game_Menu) {
		games.menu->Update(dt);
	}else if (State == Game_Init) {
		games.Init->Update(dt);
	}else if (State == Game_TEST) {
		games.test->Update(dt);
	}else if (State == Game_Asteroids) {
		games.asteroids->Update(dt);
	}else if (State == Game_Battlezone) {
		games.battlezone->Update(dt);
	}else if (State == Game_Tempest) {
		games.tempest->Update(dt);
	}
}

GameObject* Game::Create(const vec& pos, const vec& rot, const vec& scale, const std::wstring& object) {
	int size = Objects.size();
	GameObject* NewObject = new GameObject(pos, rot, scale, object, size);
	Objects.push_back(NewObject);
	return NewObject;
}
GameObject* Game::Create(const vec& pos, const vec& rot, const vec& scale, const std::vector<float>& vertecies, const std::vector<unsigned int>& indecies) {
	int size = Objects.size();
	GameObject* NewObject = new GameObject(pos, rot, scale, vertecies, indecies, size);
	Objects.push_back(NewObject);
	return NewObject;
}
void Game::Destroy(GameObject* Object) {
	for (int i = Object->index + 1; i < Objects.size(); i++) {
		Objects[i]->index--;
	}
	Objects.erase(Objects.begin() + Object->index);
		delete Object;
}

TextBox* Game::AddText(const float& Left, const float Right, const float& Top, const float& Bottom, const float& spacing, const bool& AlignCenterHorizontaly) {
	TextBox* Text = new TextBox(Left, Right, Top, Bottom, spacing, AlignCenterHorizontaly, Texts.size());
	Texts.push_back(Text);
	return Text;
}

void Game::DestroyText(TextBox* Text) {
	int i = Text->Index;
		delete Text;
	Texts.erase(Texts.begin() + i);

	for (int j = i; j < Texts.size(); j++) {
		Texts[j]->Index--;
	}
}

void Game::Sound(const std::wstring& SoundFile, const bool& PlayInLoop) {
	std::wstring sound = L"Sounds\\" + SoundFile + L".wav";
	LPCWSTR lpcstr = sound.c_str();

	if (PlayInLoop)
		PlaySoundW(lpcstr, hMod, SND_FILENAME | SND_ASYNC | SND_LOOP);
	else
		PlaySoundW(lpcstr, hMod, SND_FILENAME | SND_ASYNC);

}
void Game::StopSounds() {
	PlaySoundW(NULL, hMod, SND_PURGE);
}

void Game::FillMesh(std::vector<vec>& mesh, const GameObject* obj, bool simplify) {
	if (!simplify) {
		if (obj->Object.verticies.Colision.edgeSidesNumber == 0) {
			for (int i = 0; i < obj->Object.verticies.iNum; i++) {
				mesh.emplace_back(	obj->Object.verticies.vertecies[i * 6 + 0],
									obj->Object.verticies.vertecies[i * 6 + 1],
									obj->Object.verticies.vertecies[i * 6 + 2]);
			}
		}
		else {
			for (int i = 0; i < obj->Object.verticies.Colision.edgeSidesNumber; i++) {
				mesh.emplace_back(	obj->Object.verticies.vertecies[obj->Object.verticies.Colision.Sides[i] * 6 + 0],
									obj->Object.verticies.vertecies[obj->Object.verticies.Colision.Sides[i] * 6 + 1],
									obj->Object.verticies.vertecies[obj->Object.verticies.Colision.Sides[i] * 6 + 2]);
			}
		}
	}
	else {
		mesh.emplace_back(-1,3);
	}
}
vec Game::CalculateAxis(const vec& d, const vec& collisionAxis) {
	if (collisionAxis == vec(1, 1, 0)) return vec(-d.y, d.x, 0);
	else if (collisionAxis == vec(1, 0, 1)) return vec(-d.z, 0, d.x);
	else if (collisionAxis == vec(0, 1, 1)) return vec(0, -d.z, d.y);
	throw std::invalid_argument("'const vec& collisionAxis' should to be either 'vec(1,1,0)', 'vec(1,0,1)' or 'vec(0,1,1)'");
}
void Game::ProjectMesh(const std::vector<vec>& mesh, const vec& axis, float& min, float& max) {
	min = INFINITE;
	max = -INFINITE;
	for (size_t i = 0; i < mesh.size(); i += 6) {
		vec p(mesh[i]);
		float product = vec::Dot(p, axis);
		if (product < min) min = product;
		if (product > max) max = product;
	}
}

bool Game::collisionCircle(const GameObject* obj1, const GameObject* obj2, const vec& collisionAxis) {
	vec pos1, pos2;
	float r1, r2;

	pos1 = obj1->Transform.position & collisionAxis;
	pos2 = obj2->Transform.position & collisionAxis;

	r1 = obj1->Object.verticies.Colision.farthestVertex;
	r2 = obj2->Object.verticies.Colision.farthestVertex;

	return ((pos2.x - pos1.x) * (pos2.x - pos1.x) + (pos2.y - pos1.y) * (pos2.y - pos1.y) + (pos2.z - pos1.z) * (pos2.z - pos1.z) <= (r1 + r2) * (r1 + r2));
}
bool Game::collsionSAT(const GameObject* obj1, const GameObject* obj2, const vec& collisionAxis, const bool simplify) {
	std::vector<vec> mesh1, mesh2;

	FillMesh(mesh1, obj1, false); // Always fill mesh1
	FillMesh(mesh2, obj2, simplify); // Conditionally fill mesh2 based on simplify

	for (size_t i = 0; i < mesh1.size() / 2; i++) {
		vec d(mesh1[i * 2] - mesh1[i * 2 + 1]);
		vec axis = CalculateAxis(d, collisionAxis);

		float min1, max1, min2, max2;
		ProjectMesh(mesh1, axis, min1, max1);
		if (!simplify) {
			ProjectMesh(mesh2, axis, min2, max2);
			if (min1 - max2 > 0 || min2 - max1 > 0) return false;
		}
	}

	return true;
}
bool Game::checkCollisions(const GameObject* obj1, const GameObject* obj2, const vec& collisionAxis, const bool simplify) {
	return Game::collisionCircle(obj1, obj2, collisionAxis) && Game::collsionSAT(obj1, obj2, collisionAxis, simplify);
}

std::string Game::formatText(std::string text, bool type, int length) {
	char f = ' ';
	if (type) f = '0';
	while (text.length() < length) {
		text = f + text;
	}
	return text;
}

void Game::DeleteGame() {
	for (int i = 0; i < Game::Texts.size(); i++) {
			delete Texts[i];
	}

	for (int i = 0; i < Game::Objects.size(); i++) {
			delete Objects[i];
	}

	Texts.clear();
	Objects.clear();
		if (State == Game_Menu) {
			delete games.menu;
		}
		else if (State == Game_Init) {
			delete games.Init;
		}
		else if (State == Game_TEST) {
			delete games.test;
		}
		else if (State == Game_Asteroids) {
			delete games.asteroids;
		}
		else if (State == Game_Battlezone) {
			delete games.battlezone;
		}
		else if (State == Game_Tempest) {
			delete games.tempest;
		}
}