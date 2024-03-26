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
	bool UseUniversalUnits = false;
};

struct ColisionMesh {
	float farthestVertex = 0;
	unsigned int edgeSidesNumber = 0;
	unsigned int* Sides = nullptr;

	~ColisionMesh() {
		if(Sides != nullptr)
			delete[] Sides;
	}

	ColisionMesh() = default;

	ColisionMesh(const ColisionMesh& sec)
		:farthestVertex(sec.farthestVertex), edgeSidesNumber(sec.edgeSidesNumber)
	{
		Sides = new unsigned int[edgeSidesNumber * 2];

		for (int i = 0; i < edgeSidesNumber * 2; i++) {
			Sides[i] = sec.Sides[i];
			Sides[i + 1] = sec.Sides[i + 1];
		}
	}

	void operator=(const ColisionMesh& sec)
	{
		farthestVertex = sec.farthestVertex;
		edgeSidesNumber = sec.edgeSidesNumber;
		Sides = new unsigned int[edgeSidesNumber * 2];

		for (int i = 0; i < edgeSidesNumber * 2; i++) {
			Sides[i] = sec.Sides[i];
		}
	}
};

struct VertexData {
	ColisionMesh Colision;
	float* vertecies = nullptr;
	unsigned int* indecies = nullptr;
	int vNum = 0;
	int iNum = 0;

	VertexData(int vNum2, int iNum2) {
		vertecies = new float[vNum2];
		indecies = new unsigned int[iNum2 * 2];
		vNum = vNum2;
		iNum = iNum2;
	};

	VertexData() {};

	~VertexData() {
		if (vertecies != nullptr) {
			delete[] vertecies;
			delete[] indecies;
		}
	}

	VertexData(const VertexData& sec) {
		vertecies = new float[sec.vNum];
		indecies = new unsigned int[sec.iNum * 2];
		iNum = sec.iNum;
		vNum = sec.vNum;
		for (int i = 0; i < vNum; i++) {
			vertecies[i] = sec.vertecies[i];
		}
		for (int i = 0; i < iNum * 2; i++) {
			indecies[i] = sec.indecies[i];
		}

		if(sec.Colision.edgeSidesNumber > 0)
			Colision = sec.Colision;
	};

	void operator=(const VertexData& sec) {
		if (sec.Colision.edgeSidesNumber > 0)
			Colision = sec.Colision;

		if (sec.vNum <= 0 && sec.iNum <= 0) {
			return;
		}
		vertecies = new float[sec.vNum];
		indecies = new unsigned int[sec.iNum * 2];
		iNum = sec.iNum;
		vNum = sec.vNum;
		for (int i = 0; i < vNum; i++) {
			vertecies[i] = sec.vertecies[i];
		}
		for (int i = 0; i < iNum * 2; i++) {
			indecies[i] = sec.indecies[i];
		}
	}

	void CreateCollision() {
		for (int i = 0; i < vNum - 6; i += 6) {
			float length = sqrt(pow(vertecies[i], 2) + pow(vertecies[i + 1],2) + pow(vertecies[i + 2], 2));
			if (Colision.farthestVertex < length) {
				Colision.farthestVertex = length;
			}
		}
	}
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
	bool DifferentColor;
	vec color = vec(1, 3);

	Rendering() {};

	Rendering(const Rendering& sec): doVerex(sec.doVerex), name(sec.name), lineWidth(sec.lineWidth), onTop(sec.onTop), opacity(sec.opacity), pointsNum(sec.pointsNum), lines(sec.lines), DifferentColor(sec.DifferentColor)
	{
		color = sec.color;
		if (doVerex) {
			
			verticies = VertexData(sec.verticies);
		}
	}
};


class GameObject
{
public:
	Transformations Transform;
	//Rendering View;

	int index = 0;

	vec Front = vec(0, 3);
	vec Up = vec(0, 3);
	vec Right = vec(0, 3);

	Rendering& Object = *new Rendering;
	int activeStage;
	std::vector<Rendering> Stage;

	GameObject(const vec& pos, const vec& rot, const vec& sc, std::wstring object, int i);
	GameObject(const vec& pos3, const vec& rot3, const vec& sc3, std::vector<float> vertecies, std::vector<unsigned int> indecies, int i, bool CreateCollisionMesh = false, std::vector<unsigned int> CollisionMesh = {});
	GameObject(const GameObject* second);
	~GameObject();

	void Move(const vec& pos3);
	void MoveGlobal(const vec& pos3);
	void MoveTo(const vec& pos3);

	void Rotate(const vec& rot3, float degries);
	void Rotate(const vec& degries3);
	void RotateTo(const vec& rot3);

	void Scale(const vec& scale3);
	void ScaleTo(const vec& scale3);

	void SetColor(const vec& color3);
	void UnColor();

	int AddStage(std::wstring file);
	int AddStage(std::vector<float> vertecies, std::vector<unsigned int> indecies, bool AddCollision, std::vector<unsigned int> CollisionMesh);
private:
	void UpdateVectors();
};

#endif // MY_HEADER_H

