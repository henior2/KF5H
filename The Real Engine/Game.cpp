#include "Game.h"
#include <windows.h>
#include "ModelMenager.h"

std::map<UINT, bool> Game::KeysPresed;
std::vector<GameObject*> Game::Objects;
std::vector<TextBox*> Game::Texts;
Camera* Game::camera = new Camera();

Games Game::games;
GameState Game::State;

POINT Game::MousePosition;
POINT Game::ScreenSize;

HMODULE Game::hMod = GetModuleHandle(NULL);

irrklang::ISoundEngine* Game::SoundEngine;

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
	else {
		return;
	}
}

void Game::Update(const float& dt) {
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
	else {
		return;
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

TextBox* Game::AddText(const float& Left, const float Right, const float& Top, const float& Bottom,const std::wstring& BaseText, const float& spacing, const bool& AlignCenterHorizontaly, const vec& Color, const unsigned int& Boldicity) {
	TextBox* Text = new TextBox(Left, Right, Top, Bottom, spacing, AlignCenterHorizontaly, Texts.size());
	Text->Color = Color;
	Text->Boldicity = Boldicity;
	Text->ChangeText(BaseText);
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

void Game::Sound(const std::string& SoundFile, const bool& PlayInLoop) {
	std::string sound = "Sounds\\" + SoundFile + ".wav";
	const char *lpcstr = sound.c_str();

	SoundEngine->play2D(lpcstr, PlayInLoop);

}
void Game::StopSounds() {
	SoundEngine->stopAllSounds();
}

vec Game::CalculateBetterVec(const vec& vector, const Transformations& trans) {
	mat ModelMatrix = mat(1, 4);
	mat Tra = ModelMatrix.Translate(trans.position);
	ModelMatrix = ModelMatrix * Tra;
	ModelMatrix.Rotate(Kmath::Radians(trans.orientation));
	mat Sc = ModelMatrix.Scale(trans.scale);
	ModelMatrix = ModelMatrix * Sc;

	vec result(4);
	result = ModelMatrix * vector;

	vec r;

	for (int j = 0; j < 3; j++) {
		r.array[j] /= result.array[3];
		r.array[j] /= result.array[3];
	}

	return r;
}
void Game::FillMesh(std::vector<vec>& mesh, const GameObject* obj, bool simplify) {
	if (!simplify) {
		unsigned int edges = (obj->Object.doVerex) ? obj->Object.verticies.Colision.edgeSidesNumber : ModelMenager::ObjectsDatas[obj->Object.name].Colision.edgeSidesNumber;
		if (edges == 0) {
			for (int i = 0; i < obj->Object.verticies.iNum; i++) {
				float x = (obj->Object.doVerex) ? obj->Object.verticies.vertecies[obj->Object.verticies.indecies[i] * 6 + 0] : ModelMenager::ObjectsDatas[obj->Object.name].vertecies[obj->Object.verticies.indecies[i] * 6 + 0];
				float y = (obj->Object.doVerex) ? obj->Object.verticies.vertecies[obj->Object.verticies.indecies[i] * 6 + 1] : ModelMenager::ObjectsDatas[obj->Object.name].vertecies[obj->Object.verticies.indecies[i] * 6 + 1];
				float z = (obj->Object.doVerex) ? obj->Object.verticies.vertecies[obj->Object.verticies.indecies[i] * 6 + 2] : ModelMenager::ObjectsDatas[obj->Object.name].vertecies[obj->Object.verticies.indecies[i] * 6 + 2];
				vec Pos = CalculateBetterVec(vec(x, y, z), obj->Transform);
				mesh.emplace_back(Pos);
			}
		}
		else {
			for (int i = 0; i < edges; i++) {
				float x = (obj->Object.doVerex) ? obj->Object.verticies.vertecies[obj->Object.verticies.Colision.Sides[i] * 6 + 0] : ModelMenager::ObjectsDatas[obj->Object.name].vertecies[obj->Object.verticies.Colision.Sides[i] * 6 + 0];
				float y = (obj->Object.doVerex) ? obj->Object.verticies.vertecies[obj->Object.verticies.Colision.Sides[i] * 6 + 1] : ModelMenager::ObjectsDatas[obj->Object.name].vertecies[obj->Object.verticies.Colision.Sides[i] * 6 + 1];
				float z = (obj->Object.doVerex) ? obj->Object.verticies.vertecies[obj->Object.verticies.Colision.Sides[i] * 6 + 2] : ModelMenager::ObjectsDatas[obj->Object.name].vertecies[obj->Object.verticies.Colision.Sides[i] * 6 + 2];
				vec Pos = CalculateBetterVec(vec(x,y,z),obj->Transform);
				mesh.emplace_back(Pos);
			}
		}
	}
	else {
		mesh.emplace_back(-1,3);
	}
}
vec Game::CalculateAxis(const vec& d, const vec& collisionAxis) {
	if		(collisionAxis == vec(1, 1, 0)) return vec(-d.y,  d.x,   0);
	else if (collisionAxis == vec(1, 0, 1)) return vec(-d.z,    0, d.x);
	else if (collisionAxis == vec(0, 1, 1)) return vec(   0, -d.z, d.y);
	throw std::invalid_argument("'const vec& collisionAxis' should to be either 'vec(1,1,0)', 'vec(1,0,1)' or 'vec(0,1,1)'");
}
void Game::ProjectMesh(const vec& pos, const std::vector<vec>& mesh, const vec& axis, float& min, float& max) {
	min = INFINITE;
	max = -min;
	for (size_t i = 0; i < mesh.size(); i += 6) {
		vec p = mesh[i] + pos;
		float product = vec::Dot(p, axis);
		if (product < min) min = product;
		if (product > max) max = product;
	}
}
void Game::ProjectCircle(const vec& pos, float radius, const vec& axis, float& min, float& max) {
	float centerProjection = vec::Dot(pos, axis);
	min = centerProjection - radius;
	max = centerProjection + radius;
}
bool Game::CheckOverlapAndProject(const vec& position1, const std::vector<vec>& mesh1, const vec& position2, const std::vector<vec>& mesh2, const vec& axis, bool simplify, float radius) {
	float min1, max1, min2, max2;

	ProjectMesh(position1, mesh1, axis, min1, max1);

	if (!simplify) ProjectMesh(position2, mesh2, axis, min2, max2);
	else ProjectCircle(position2, radius, axis, min2, max2);

	return !(min1 > max2 || min2 > max1);
}

bool Game::collisionCircle(const GameObject* obj1, const GameObject* obj2, const vec& collisionAxis) {
	vec pos1, pos2;
	float r1, r2;

	pos1 = obj1->Transform.position & collisionAxis;
	pos2 = obj2->Transform.position & collisionAxis;

	float f1 = (obj1->Object.doVerex) ? obj1->Object.verticies.Colision.farthestVertex : ModelMenager::ObjectsDatas[obj1->Object.name].Colision.farthestVertex;
	float f2 = (obj2->Object.doVerex) ? obj2->Object.verticies.Colision.farthestVertex : ModelMenager::ObjectsDatas[obj2->Object.name].Colision.farthestVertex;

	if (f1 == 0) {
		obj1->Object.verticies.CreateCollision();
	}

	if (f2 == 0) {
		obj2->Object.verticies.CreateCollision();
	}

	r1 = f1 * obj1->Transform.scale.x;
	r2 = f2 * obj2->Transform.scale.x;

	return ((pos2.x - pos1.x) * (pos2.x - pos1.x) + (pos2.y - pos1.y) * (pos2.y - pos1.y) + (pos2.z - pos1.z) * (pos2.z - pos1.z) <= (r1 + r2) * (r1 + r2));
}
bool Game::collisionSAT(const GameObject* obj1, const GameObject* obj2, const vec& collisionAxis, const bool simplify) {
	std::vector<vec> mesh1, mesh2;

	FillMesh(mesh1, obj1, false);
	FillMesh(mesh2, obj2, simplify);

	float f2 = (obj2->Object.doVerex) ? obj2->Object.verticies.Colision.farthestVertex : ModelMenager::ObjectsDatas[obj2->Object.name].Colision.farthestVertex;

	for (int i = 0; i < mesh1.size() / 2; i++) {
		vec d = mesh1[i * 2 + 1] - mesh1[i * 2];
		vec axis = CalculateAxis(d, collisionAxis);

		if (!CheckOverlapAndProject(obj1->Transform.position, mesh1, obj2->Transform.position, mesh2, axis, simplify, f2 * obj2->Transform.scale.x)) {
			return false;
		}
	}

	if (!simplify) {
		for (int i = 0; i < mesh2.size() / 2; i++) {
			vec d = mesh2[i * 2] - mesh2[i * 2 + 1];
			vec axis = CalculateAxis(d, collisionAxis);

			if (!CheckOverlapAndProject(obj1->Transform.position, mesh1, obj2->Transform.position, mesh2, axis, simplify, f2 * obj2->Transform.scale.x)) {
				return false;
			}
		}
	}
	else {
		float minLen = INFINITE;
		vec d;

		for (int i = 0; i < mesh1.size(); i++) {
			vec len = obj2->Transform.position - (obj1->Transform.position + mesh1[i]);
			if (len.Length() < minLen) {
				minLen = len.Length();
				d = len;
			}
		}

		vec axis = CalculateAxis(d, collisionAxis);

		if (!CheckOverlapAndProject(obj1->Transform.position, mesh1, obj2->Transform.position, mesh2, axis, simplify, f2 * obj2->Transform.scale.x)) {
			return false;
		}
	}

	return true;
}
bool Game::checkCollisions(const GameObject* obj1, const GameObject* obj2, const vec& collisionAxis, const bool simplify) {
	return Game::collisionCircle(obj1, obj2, collisionAxis) && Game::collisionSAT(obj1, obj2, collisionAxis, simplify);
}

std::wstring Game::formatText(std::wstring text, bool type, int length) {
	wchar_t f = ' ';
	if (type) f = L'0';
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