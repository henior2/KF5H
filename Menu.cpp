#include "Game.h"

using namespace glm;

std::vector<GameObject*> obiekty;
std::vector<GameObject*> tekst;

void Game::MenuInit() {
    srand(time(NULL));

    for (int i = 0; i < 696; i++) {
        float x = ((float)(rand() % 100) - 50.0f);
        float y = ((float)(rand() % 100) - 50.0f);
        float z = ((float)(rand() % 150) - 100.0f);
        vec3 rot((float)(rand() % 360), (float)(rand() % 360), (float)(rand() % 360));

        obiekty.push_back(Create(vec3(x, y, z), rot, vec3(1.0f), "MenuCube"));
    }

    for (int i = 0; i < 4; i++) {
        float z = ((float)(rand() % 200) + 100.0f);
        vec3 rot((float)(rand() % 360), (float)(rand() % 360), (float)(rand() % 360));
        obiekty.push_back(Create(vec3(0.0f, 0.0f, z), rot, vec3(1.0f), "MenuCube"));
    }

    PlaySound2d("BeepBox-Song.mp3", true);
}

void Game::Menu(float dt) {
    for (int i = 0; i < obiekty.size(); i++) {
        obiekty[i]->Move(vec3(0.0f, 0.0f, 3 * dt));
        vec3 pos = obiekty[i]->Transform.position;
        obiekty[i]->View.lineWidth = ((obiekty[i]->Transform.position.z / 10.0f) + 10.0f) / 1.5f;
        if (pos.z > 10) {
            float z = ((float)(rand() % 100) + 101.0f);
            obiekty[i]->MoveTo(vec3(pos.x, pos.y, -z));
        }
    }

    ChangeState(Game_Asteroids);
}