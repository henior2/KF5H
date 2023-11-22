#ifndef GAME_H
#define GAME_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "GameObject.h"
#include "Shader.h"

//stan gry
enum GameState {
	Game_Menu,
	Game_Battlezone,
	Game_Tempest,
	Game_Asteroids
};

struct Color {
	float Red = 0.0f;
	float Green = 0.0f;
	float Blue = 0.0f;
	float Alpha = 0.0f;
};

class Game
{
public:
	std::vector<GameObject*> Objects;
	std::vector<int> nulls;

	GameState State;
	bool Keys[1024];
	GLFWwindow* window;

	Color Buffer;

	Shader program;
	
	Game(GLFWwindow* win, Shader* prog);
	~Game();

	void ProcessInput(float dt);
	void Update(float dt);

	GameObject* Create(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale, std::string Object);
	void Destroy(GameObject* Object);

	void Menu(float dt);
	void MenuInit();

	void Battlezone(float dt);
	void BattlezoneInit();

	void Tempest(float dt);
	void TempestInit();

	void Asteroids(float dt);
	void AsteroidsInit();

	void ChangeState();
	void Render(float dt);
};

#endif
