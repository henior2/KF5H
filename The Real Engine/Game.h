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
	Game_TEST
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

	static void Update(const float& dt);

	static GameObject* Create(const vec& pos, const vec& rot, const vec& scale, const std::wstring& object);
	static GameObject* Create(const vec& pos, const vec& rot, const vec& scale, const std::vector<float>& vertecies, const std::vector<unsigned int>& indecies);
	static void Destroy(GameObject* Object);

	static TextBox* AddText(const float& Left, const float Right, const float& Top, const float& Bottom, const float& spacing = 0.1f, const bool& AlignCenterHorizontaly = false);
	static void DestroyText(TextBox* Text);

	static void Sound(const std::wstring& SoundFile, const bool& PlayInLoop = false);
	static void StopSounds();
	
	static void ChangeState(const GameState& state);

	static bool collisionCircle(vec pos1, vec pos2, float r1 = 5.0f, float r2 = 5.0f);
	static std::string formatText(std::string text, bool type, int length = 3);
private:
	static void DeleteGame();

	static HMODULE hMod;
};

