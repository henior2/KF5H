#ifndef GAME_H
#define GAME_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "GameObject.h"
#include "Shader.h"
#include "Camera.h"
#include "Tekst2d.h"

#include<irrKlang/irrKlang.h>

//stan gry
enum GameState {
	Game_Init,
	Game_Menu,
	Game_Asteroids,
	Game_Battlezone,
	Game_Tempest,
	Game_TEST
};

struct Color {
	float Red = 0.0f;
	float Green = 0.0f;
	float Blue = 0.0f;
	float Alpha = 0.0f;
};

struct ScreenPosition
{
	double x;
	double y;
};

class Game
{
public:

	irrklang::ISoundEngine* engine;

	std::vector<GameObject*> Objects;
	std::vector<Tekst2d*> Teksts;

	GameState State;
	bool Keys[1024];
	GLFWwindow* window;

	Color Buffer;

	unsigned int SCR_WIDTH, SCR_HEIGHT;

	Shader* program;

	Camera* camera;

	ScreenPosition MousePosition;
	
	Game(GLFWwindow* win, unsigned int width, unsigned int height, irrklang::ISoundEngine* SoundEngine);
	~Game();

	void ProcessInput(float dt);
	void Update(float dt);

	GameObject* Create(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale, std::string Object);
	GameObject* Create(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale, std::vector<float> vertecies, std::vector<unsigned int> indecies);
	void Destroy(GameObject* Object);

	Tekst2d* CreateTekst(glm::vec2 pos, float rot, glm::vec2 scale, float height, float spacing, std::string tekst);
	void DestroyTekst(Tekst2d* tekst);

	void GameInit();
	void Init(float dt);

	void Menu(float dt);
	void MenuInit();

	void Battlezone(float dt);
	void BattlezoneInit();

	void Tempest(float dt);
	void TempestInit();

	void Asteroids(float dt);
	void AsteroidsInit();

	void TEST(float dt);
	void TESTInit();

	bool collisionCircle(glm::vec2 pos1, glm::vec2 pos2, float r1 = 5.0f, float r2 = 5.0f);

	void ChangeState(GameState state);
	void Render(float dt);

	void PlaySound2d(const char file[], bool loop);

	void mouse_callback(GLFWwindow* window, double xpos, double ypos);
};

extern Game* Gra;

#endif
