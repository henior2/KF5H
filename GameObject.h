#ifndef GameObject_H
#define GameObject_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

struct Transformations {
	glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 orientation = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
};

struct Rendering
{
	unsigned int VAO;
	float lineWidth = 1.0f;
	bool onTop = false;
	float opacity = 1;
	int pointsNum = 0;
	int lines = 0;

};

struct VertexData {
	float *vertecies;
	unsigned int *indecies;
	int vNum;
	int iNum;
};

class GameObject
{
public:
	Transformations Transform;
	//Rendering View;

	int index = 0;

	bool DifferentColor;
	glm::vec3 color;

	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;

	int activeStage;
	std::vector<Rendering> Stage;

	GameObject(glm::vec3 pos, glm::vec3 rot, glm::vec3 sc, std::string object, int i);
	GameObject(glm::vec3 pos, glm::vec3 rot, glm::vec3 sc, std::vector<float> vertecies, std::vector<unsigned int> indecies, int i);
	~GameObject();

	void Move(glm::vec3 pos);
	void MoveGlobal(glm::vec3 pos);
	void MoveTo(glm::vec3 pos);

	void Rotate(glm::vec3 rot, float degries);
	void Rotate(glm::vec3 degries);
	void RotateTo(glm::vec3 rot);

	void Scale(glm::vec3 scale);
	void ScaleTo(glm::vec3 scale);

	void SetColor(glm::vec3 color);
	void UnColor();

	int AddStage(std::string file);
	int AddStage(std::vector<float> vertecies, std::vector<unsigned int> indecies);
private:
	void UpdateVectors();

	void AddVao(int& vNum, int& iNum, float vertecies[], unsigned int indecies[]);

	VertexData ReadVertexFile(std::string file);
};

#endif // MY_HEADER_H

