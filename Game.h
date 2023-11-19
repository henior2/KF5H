#ifndef GAME_H
#define GAME_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

//stan gry
enum GameState {
	Menu,
	Battlezone,
	Tempest,
	Asteroids
};

class Game
{
public:
	GameState State;
	bool Keys[1024];
	
	Game();
	~Game();
};

#endif
