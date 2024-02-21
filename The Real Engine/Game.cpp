#include "Game.h"
#include <windows.h>

std::map<UINT, bool> Game::KeysPresed;
std::vector<GameObject*> Game::Objects;
std::vector<TextBox*> Game::Texts;
Camera* Game::camera = new Camera();

Games Game::games;
GameState Game::State;

POINT Game::MousePosition;

void Game::ChangeState(const GameState& state) {
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
		PlaySound(lpcstr, NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
	else
		PlaySound(lpcstr, NULL, SND_FILENAME | SND_ASYNC);

}
void Game::StopSounds() {
	PlaySound(NULL, NULL, SND_PURGE);
}


void Game::DeleteGame() {
	for (int i = 0; i < Game::Objects.size(); i++) {
		delete Objects[i];
	}

	for (int i = 0; i < Game::Texts.size(); i++) {
		delete Texts[i];
	}

	Texts.clear();
	Objects.clear();
	if (State == Game_Menu) {
		delete games.menu;
	}
}