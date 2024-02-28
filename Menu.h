#pragma once
#include <vector>
#include "The Real Engine/GameObject.h"
#include "The Real Engine/TextBox.h"

class Menu
{
public:
    void Init();
    void Update(const float& dt);
private:
    GameObject* Pointer;
    std::vector<GameObject*> obiekty;
    TextBox* Sign;
    TextBox* PlayButton;
    TextBox* ExitButton;
    TextBox* AsteroidsButton;
    TextBox* BattlezoneButton;
    TextBox* TempestButton;
    TextBox* BackButton;

    std::vector<TextBox*> Buttons;

    float Wait = 1;

    bool Play = false;
};

