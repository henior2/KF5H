#ifndef Renderer_H
#define Renderer_H
#include "GameObject.h"
#include "ModelMenager.h"

class Renderer
{
public:
	static void DrawGame(std::vector<GameObject*>& Objects, HDC& hdc, const mat& ProjectionMatrix, const mat& ViewMatrix, const float& width, const float& height);
	static void DrawObject(VertexData data, HDC& hdc, Transformations Model, /*const mat& ProjectionMatrix, const mat& ViewMatrix*/const mat& Pro, const float& width, const float& height, const vec& color, const int& LineWidth, const bool& UseVertexColor, const bool& onTop);
};

#endif Renderer_H