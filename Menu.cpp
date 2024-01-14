#include "Game.h"

using namespace glm;

namespace Menu {
    GameObject* pointer;

    std::vector<GameObject*> obiekty;
    std::vector<Tekst2d*> tekst;

    const std::string modele[] = { "MenuCube","MenuSquare" };
    const std::string rareModels[] = { "AsteroidsShip","AsteroidsShipFire","AsteroidsEnemy","Tank","FastTank","Ufo","FastBullet","Blaster","Flipper","exclamation-mark","question-mark","RadarT","PowerUpBox","PowerUpHeart" };

    bool esc = false;

    const float pointerPosition[] = { -.05,-.25,-.45 };
    const GameState gameStates[] = { Game_Asteroids,Game_Battlezone,Game_Tempest };
    int pointerState;

    float clickCooldown;
}
using namespace Menu;

void Game::MenuInit() {
    pointer = Create(vec3(-.325,-.05,0), vec3(0, 0, -90), vec3(.1), "AsteroidsBullet");
    pointer->color = vec3(0, 1, 0);
    pointer->Stage[0].onTop = true;
    pointerState = 0;

    obiekty.clear();
    tekst.clear();
    esc = false;
    srand(time(NULL));

    tekst.push_back(CreateTekst(vec2(-0.6, 0.5), 0, vec2(0.1), 1, 0.2f, "GryWektorowe"));
    tekst.push_back(CreateTekst(vec2(-0.225, -0.1), 0, vec2(0.05), 1, 0.2f, "Asteroids"));
    tekst.push_back(CreateTekst(vec2(-0.25, -0.3), 0, vec2(0.05), 1, 0.2f, "Battlezone"));
    tekst.push_back(CreateTekst(vec2(-0.175, -0.5), 0, vec2(0.05), 1, 0.2f, "Tempest"));
    
    for (int i = 0; i < 1500; i++) {
        float x = ((float)(rand() % 100) - 50.0f);
        float y = ((float)(rand() % 100) - 50.0f);
        float z = ((float)(rand() % 150) - 100.0f);
        vec3 rot((float)(rand() % 360), (float)(rand() % 360), (float)(rand() % 360));

        std::string model;
        if (rand() % 10 == 0) {
            int temp = rand() % 3;
            if (temp == 0) model = rareModels[rand() % (sizeof(rareModels) / sizeof(std::string))];
            else if (temp == 1) model = "Upper" + (char)(rand() % 26 + 65);
            else model = std::to_string(rand() % 10);
        }
        else model = modele[rand() % 2];

        obiekty.push_back(Create(vec3(x, y, z), rot, vec3(1.0f), model));
        obiekty[i]->SetColor(vec3((float)(rand()) / ((float)(RAND_MAX / 1.0f)), (float)(rand()) / ((float)(RAND_MAX / 1.0f)), (float)(rand()) / ((float)(RAND_MAX / 1.0f))));
    }

    camera->RotateCamera(0.0f, 90.0f);

    clickCooldown = .25;

    PlaySound2d("mus02.mp3", true);
}

void Game::Menu(float dt) {
    clickCooldown -= dt;

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

    //todo: add that clicking on the UI does something. easy
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) ChangeState(Game_Asteroids);
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) ChangeState(Game_Battlezone);
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) ChangeState(Game_Tempest);
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) ChangeState(Game_TEST);

    if (clickCooldown<=0 && glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) { 
        clickCooldown = .25;
        if (++pointerState > 2) pointerState = 0;
        pointer->Transform.position.y = pointerPosition[pointerState];
    }
    if (clickCooldown<=0 && glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        clickCooldown = .25;
        if (--pointerState < 0) pointerState = 2;
        pointer->Transform.position.y = pointerPosition[pointerState];
    }

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
        ChangeState(gameStates[pointerState]);
    }

    // gdy klikniety esc to wywolaj zamkniecie okna
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS && esc)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_RELEASE)
        esc = true;
}