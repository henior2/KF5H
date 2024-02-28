#include "Menu.h"
#include "The Real Engine/The Real Engine.h"
#include <thread>

void Menu::Init() {
		for (int i = 0; i < 1500; i++) {
			float x = ((float)(rand() % 100) - 50.0f);
			float y = ((float)(rand() % 100) - 50.0f);
			float z = ((float)(rand() % 150) - 100.0f);
			float scale = (float)(rand() % 100) / 100.0f + 0.5f;
			obiekty.push_back(Game::Create(vec(x, y, z), 
				vec(rand() % 360, rand() % 360, rand() % 360), 
				vec(scale, 3), L"MenuCube"));
			obiekty[i]->SetColor(vec(0, 1, 0));
		}
		for (int i = 0; i < 5; i++) {
			float x = 0;
			float y = 0;
			float z = ((float)(rand() % 150) - 100.0f);
			float scale = (float)(rand() % 100) / 100.0f + 0.5f;
			obiekty.push_back(Game::Create(vec(x, y, z), vec(rand() % 360, rand() % 360, rand() % 360), vec(scale, 3), L"MenuCube"));
			obiekty[i + 1500]->SetColor(vec(0, 1, 0));
		}
		Texts.push_back(Game::AddText(-1, 1, 0.9, 0.8, 0.1f, true));
		Texts[0]->Color = vec(0.5, 0.5, 0.5);
		Texts[0]->Boldicity = 15;
		Texts[0]->Write("Gry Wektorowe");
		Game::Sound(L"mus01", true);
		Game::camera->perspective = true;
		Game::camera->cameraWidth = 16;
		Game::camera->cameraHeight = 9;
	}

void Menu::Update(const float& dt) {
		for (int i = 0; i < obiekty.size() - 1; i++) {
			obiekty[i]->MoveGlobal(vec(0, 0, 3.0f * dt));
			vec pos = obiekty[i]->Transform.position;
			obiekty[i]->SetColor(vec(0, 1.0f + pos.z / 101.0f, 0));
			obiekty[i]->Stage[obiekty[i]->activeStage].lineWidth = (1.0f + pos.z / 101.0f) * 5.0f;
			if (pos.z > 1) {
				float z = ((float)(rand() % 100) + 101.0f);
				obiekty[i]->MoveTo(vec(pos.x, pos.y, -z));
			}
		}

		if (Game::KeysPresed['A'])
			Game::ChangeState(Game_Asteroids);
		else if (Game::KeysPresed['T'])
			Game::ChangeState(Game_Tempest);
		else if (Game::KeysPresed['B'])
			Game::ChangeState(Game_Battlezone);
		
		if (Game::KeysPresed[VK_ESCAPE])
			PostQuitMessage(0);
	}