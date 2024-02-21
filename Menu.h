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
    std::vector<GameObject*> obiekty;
    std::vector<TextBox*> Texts;
};

