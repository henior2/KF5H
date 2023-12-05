#include "Game.h"

using namespace glm;

std::vector<GameObject*> obiekty;
std::vector<GameObject*> tekst;

std::string modele[] = {"MenuCube","MenuSquare"};

bool esc = false;

void Game::MenuInit() {
    obiekty.clear();
    tekst.clear();
    esc = false;
    srand(time(NULL));
    
    for (int i = 0; i < 1500; i++) {
        float x = ((float)(rand() % 100) - 50.0f);
        float y = ((float)(rand() % 100) - 50.0f);
        float z = ((float)(rand() % 150) - 100.0f);
        vec3 rot((float)(rand() % 360), (float)(rand() % 360), (float)(rand() % 360));
        obiekty.push_back(Create(vec3(x, y, z), rot, vec3(1.0f), modele[rand()%2]));
        obiekty[i]->SetColor(vec3((float)(rand()) / ((float)(RAND_MAX / 1.0f)), (float)(rand()) / ((float)(RAND_MAX / 1.0f)), (float)(rand()) / ((float)(RAND_MAX / 1.0f))));
    }

    //po to to wsm xd
    //for (int i = 0; i < 4; i++) {
    //    float z = ((float)(rand() % 200) - 100.0f);
    //    vec3 rot((float)(rand() % 360), (float)(rand() % 360), (float)(rand() % 360));
    //    obiekty.push_back(Create(vec3(0.0f, 0.0f, z), rot, vec3(1.0f), "MenuCube"));
    //}

    camera->RotateCamera(0.0f, 90.0f);

    PlaySound2d("mus02.mp3", true);
}

void Game::Menu(float dt) {
    for (int i = 0; i < obiekty.size(); i++) {
        obiekty[i]->MoveGlobal(vec3(0.0f, 0.0f, 3 * dt));
        vec3 pos = obiekty[i]->Transform.position;
        obiekty[i]->Stage[Objects[i]->activeStage].lineWidth = ((obiekty[i]->Transform.position.z / 10.0f) + 10.0f) / 3.0f;
        if (pos.z > 10) {
            float z = ((float)(rand() % 100) + 101.0f);
            obiekty[i]->SetColor(vec3((float)(rand()) / ((float)(RAND_MAX / 1.0f)), (float)(rand()) / ((float)(RAND_MAX / 1.0f)), (float)(rand()) / ((float)(RAND_MAX / 1.0f))));
            obiekty[i]->MoveTo(vec3(pos.x, pos.y, -z));
        }
    }


    //todo: add UI
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) ChangeState(Game_Asteroids);
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) ChangeState(Game_Battlezone);
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) ChangeState(Game_Tempest);
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) ChangeState(Game_TEST);

    // gdy klikniety esc to wywolaj zamkniecie okna
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS && esc)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_RELEASE)
        esc = true;
}