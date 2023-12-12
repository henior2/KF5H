#include "Game.h"

Tekst2d* KF5H[4];

float Time = 0;

bool _esc = false;

using namespace glm;

float f(float x) {
	return 10.0f - (powf(x, 4.0f) * 1.125f) / 2.0f;
}

void Game::GameInit() {
	Time = 0;
	KF5H[0] = CreateTekst(vec2(-0.275f, 0.0f), 0, vec2(1), 1, 0, "K");
	KF5H[1] = CreateTekst(vec2(-0.125f, 0.0f), 0, vec2(1), 1, 0, "F");
	KF5H[2] = CreateTekst(vec2(0.025f, 0.0f), 0, vec2(1), 1, 0, "5");
	KF5H[3] = CreateTekst(vec2(0.175f, 0.0f), 0, vec2(1), 1, 0, "H");

	for (Tekst2d* tekst : KF5H) {
		tekst->properties.opacity = 0;
	}

	camera->perspective = false;
}

void Game::Init(float dt) {
	Time += dt;
	if (Time <= 2.0f) {
		KF5H[0]->properties.opacity = 1;
		float v = f(Time);
		KF5H[0]->ScaleTo(vec2(v / 10.0f));
	}

	if (Time > 1 && Time < 3) {
		KF5H[3]->properties.opacity = 1;
		float v = f(Time - 1);
		KF5H[3]->ScaleTo(vec2(v / 10.0f));
	}

	if (Time > 2 && Time < 4) {
		KF5H[1]->properties.opacity = 1;
		float v = f(Time - 2);
		KF5H[1]->ScaleTo(vec2(v / 10.0f));
	}

	if (Time > 3 && Time < 5) {
		KF5H[2]->properties.opacity = 1;
		float v = f(Time - 3);
		KF5H[2]->ScaleTo(vec2(v / 10.0f));
	}

	if (Time >= 6 && Time <= 10) {
		for (Tekst2d* tekst : KF5H) {
			tekst->properties.opacity -= dt / 4.0f;
		}
	}

	if(_esc)
		ChangeState(Game_Menu);

	if (Time > 12 || glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
		for (Tekst2d* tekst : KF5H) {
			tekst->properties.opacity = 0;
		}
		_esc = true;
	}
}