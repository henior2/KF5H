#ifndef GameObject_H
#define GameObject_H

#include "mat.h"
#include "vec.h"
#include "Kmath.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

struct Transformations {
	vec position = vec(0, 3);
	vec orientation = vec(0, 3);
	vec scale = vec(1, 3);
};

struct VertexData {
	float* vertecies;
	unsigned int* indecies;
	int vNum;
	int iNum;
};

struct Rendering
{
	VertexData verticies;
	bool doVerex = false;
	std::wstring name;
	float lineWidth = 1.0;
	bool onTop = false;
	float opacity = 1.0;
	int pointsNum = 0;
	int lines = 0;

};


class GameObject
{
public:
	Transformations Transform;
	//Rendering View;

	int index = 0;

	bool DifferentColor;
	vec color = vec(1, 3);

	vec Front = vec(0, 3);
	vec Up = vec(0, 3);
	vec Right = vec(0, 3);

	Rendering& Object = *new Rendering;
	int activeStage;
	std::vector<Rendering> Stage;

	GameObject(vec pos3, vec rot3, vec sc3, std::wstring object, int i);
	GameObject(vec pos3, vec rot3, vec sc3, std::vector<float> vertecies, std::vector<unsigned int> indecies, int i);
	~GameObject();

	void Move(vec pos3);
	void MoveGlobal(vec pos3);
	void MoveTo(vec pos3);

	void Rotate(vec rot3, float degries);
	void Rotate(vec degries3);
	void RotateTo(vec rot3);

	void Scale(vec scale3);
	void ScaleTo(vec scale3);

	void SetColor(vec color3);
	void UnColor();

	int AddStage(std::wstring file);
	int AddStage(std::vector<float> vertecies, std::vector<unsigned int> indecies);
private:
	void UpdateVectors();
};

#endif // MY_HEADER_H

