#include "Menu.h"
#include "The Real Engine/The Real Engine.h"
#include <thread>

void Menu::Init() {
		SetCursor(LoadCursor(NULL, IDC_ARROW));
		Sign = Game::AddText(-1, 1, 0.9, 0.8, "Gry Wektorowe", 0.1f, true, vec(.5, .5, .5), 15);
		PlayButton = Game::AddText(-0.4f, 0.4f, 0.15, 0.1, "Play", 0.1, true, vec(.5, 1, .5), 8);
		ExitButton = Game::AddText(-0.4f, 0.4f, -0.1, -0.15, "Exit", 0.1, true, vec(.5, 1, .5), 8);
		AsteroidsButton = Game::AddText(-0.4f, 0.4f, 0.25, 0.20, "", 0.1, true, vec(.5, 1, .5), 8);
		BattlezoneButton = Game::AddText(-0.4f, 0.4f, 0.025, -0.025, "", 0.1, true, vec(.5, 1, .5), 8);
		TempestButton = Game::AddText(-0.4f, 0.4f, -0.20, -0.25, "", 0.1, true, vec(.5, 1, .5), 8);
		BackButton = Game::AddText(-0.4f, 0.4f, -0.75, -0.80, "", 0.1, true, vec(1, .5, .5), 8);
		Buttons = { PlayButton, ExitButton, AsteroidsButton, BattlezoneButton, TempestButton, BackButton };
		//Pointer = Game::Create(vec3(-.325, -.05, 0), vec3(0, 0, -90), vec3(.1), "AsteroidsBullet");
		//Pointer = Game::Create()
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
		Game::Sound(L"mus01", true);
		Game::camera->perspective = true;
		Game::camera->cameraWidth = 16;
		Game::camera->cameraHeight = 9;
	}

void Menu::Update(const float& dt) {
		for (int i = 0; i < obiekty.size(); i++) {
			obiekty[i]->MoveGlobal(vec(0, 0, 3.0f * dt));
			vec pos = obiekty[i]->Transform.position;
			obiekty[i]->SetColor(vec(0, 1.0f + pos.z / 101.0f, 0));
			obiekty[i]->Stage[obiekty[i]->activeStage].lineWidth = (1.0f + pos.z / 101.0f) * 5.0f;
			if (pos.z > 1) {
				float z = ((float)(rand() % 100) + 101.0f);
				obiekty[i]->MoveTo(vec(pos.x, pos.y, -z));
			}
		}

		for (int i = 0; i < Buttons.size(); i++) {
			if (Buttons[i]->Hovered(0.05f) && Buttons[i]->Boldicity == 8) {
				Buttons[i]->ChangeSize(2, 0.05);
			}
			else if (!Buttons[i]->Hovered(0.025f) && Buttons[i]->Boldicity != 8) {
				Buttons[i]->ChangeSize(-2, -0.05);
			}
		}

		if (!Play && Wait <= 0) {
			if (PlayButton->Hovered(0.025f) && Game::KeysPresed[VK_LBUTTON]) {
					Play = true;
					Wait = 1;
					PlayButton->ChangeText("");
					ExitButton->ChangeText("");
					AsteroidsButton->ChangeText("Asteroids");
					BattlezoneButton->ChangeText("Battlezone");
					TempestButton->ChangeText("Tempest");
					BackButton->ChangeText("Go Back");
			}

			if (ExitButton->Hovered(0.025f) && Game::KeysPresed[VK_LBUTTON])
				PostQuitMessage(0);
		}
		else if(Wait <= 0) {
			if (AsteroidsButton->Hovered(0.025f) && Game::KeysPresed[VK_LBUTTON])
				Game::ChangeState(Game_Asteroids);
			else if (BattlezoneButton->Hovered(0.025f) && Game::KeysPresed[VK_LBUTTON])
				Game::ChangeState(Game_Battlezone);
			else if (TempestButton->Hovered(0.025f) && Game::KeysPresed[VK_LBUTTON])
				Game::ChangeState(Game_Tempest);
			else if (BackButton->Hovered(0.025f) && Game::KeysPresed[VK_LBUTTON]) {
				Play = false;
				Wait = 1;
				PlayButton->ChangeText("Play");
				ExitButton->ChangeText("Exit");
				AsteroidsButton->ChangeText("");
				BattlezoneButton->ChangeText("");
				TempestButton->ChangeText("");
				BackButton->ChangeText("");
			}
		}
		else {
			Wait -= dt;
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