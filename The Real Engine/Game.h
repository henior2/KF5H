#pragma once
#include "Camera.h"
#include <map>
#include <vector>
#include <Windows.h>
#include "GameObject.h"
#include "..\Menu.h"
#include "..\Asteroids.h"
#include "..\Battlezone.h"
#include "..\Tempest.h"
#include "..\Initialization.h"
#include "..\TEST.h"
#include "TextBox.h"
#include <stdexcept>

#include "..\irrKlang\irrKlang.h"

struct Games {
	Menu* menu = new Menu();
	Initialization* Init = new Initialization();
	TEST* test = new TEST();
	Asteroids* asteroids = new Asteroids();
	Battlezone* battlezone = new Battlezone();
	Tempest* tempest = new Tempest();
};

//stan gry
enum GameState {
	Game_Init,
	Game_Menu,
	Game_Asteroids,
	Game_Battlezone,
	Game_Tempest,
	Game_TEST,
	Quit_Message
};

class Game
{
public:
	static std::map<UINT, bool> KeysPresed;
	static std::vector<GameObject*> Objects;
	static std::vector<TextBox*> Texts;
	static Camera* camera;

	static Games games;
	static GameState State;

	static POINT MousePosition;
	static POINT ScreenSize;

	static irrklang::ISoundEngine* SoundEngine;

	static void Update(const float& dt);

	static GameObject* Create(const vec& pos, const vec& rot, const vec& scale, const std::wstring& object);
	static GameObject* Create(const vec& pos, const vec& rot, const vec& scale, const std::vector<float>& vertecies, const std::vector<unsigned int>& indecies);
	static void Destroy(GameObject* Object);

	static TextBox* AddText(const float& Left, const float Right, const float& Top, const float& Bottom, const std::wstring& BaseText, const float& spacing = 0.1f, const bool& AlignCenterHorizontaly = false, const vec& Color = vec(1, 1, 1), const unsigned int& Boldicity = 0.1f);
	static void DestroyText(TextBox* Text);

	static void Sound(const std::string& SoundFile, const bool& PlayInLoop = false);
	static void StopSounds();
	
	static void ChangeState(const GameState& state);

	static void FillMesh(std::vector<vec>& mesh, const GameObject* obj, bool simplify = false);
	static vec CalculateAxis(const vec& d, const vec& collisionAxis);
	static void ProjectMesh(const vec& pos, const std::vector<vec>& mesh, const vec& axis, float& min, float& max);
	static void ProjectCircle(const vec& pos, float radius, const vec& axis, float& min, float& max);
	static bool CheckOverlapAndProject(const vec& position1, const std::vector<vec>& mesh1, const vec& position2, const std::vector<vec>& mesh2, const vec& axis, bool simplify = false, float radius = 0.0f);

	static bool collisionCircle(const GameObject* obj1, const GameObject* obj2, const vec& collisionAxis);
	static bool collisionSAT(const GameObject* obj1, const GameObject* obj2, const vec& collisionAxis, const bool simplify = false);

	static vec CalculateBetterVec(const vec& vec, const Transformations& trans);

	// `obj1` and `obj2` are colliding objects
	// \n
	// `collisionAxis` is an axis, from which we chceck for collisions (eg. `vec(1,1,0)` - "2D" collisions (XY plane) are being checked for)
	// \n
	// if `simplify` is set to `true`, the second object will be treated as a circle with no details
	static bool checkCollisions(const GameObject* obj1, const GameObject* obj2, const vec& collisionAxis, const bool simplify = false);

	static std::wstring formatText(std::wstring text, bool type, int length = 3);

	static std::map<wchar_t, wchar_t> accents;
private:
	static void DeleteGame();

	static HMODULE hMod;
};

