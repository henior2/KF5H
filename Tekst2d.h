#ifndef Tekst2d_H
#define Tekst2d_H

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

#include "GameObject.h" // tylko po vertexData, todo: stworzyc header ze strukturami

struct Transformations2d {
	glm::vec2 position = glm::vec2(0.0f, 0.0f);
	float orientation = 0;
	glm::vec2 scale = glm::vec2(1.0f, 1.0f);
};

struct RenderingTekst
{
	unsigned int VAO;
	int pointsNum = 0;
	int lines = 0;

};

/*struct VertexData {
	float* vertecies;
	unsigned int* indecies;
	int vNum;
	int iNum;
};*/

struct TekstProperties {
	float spacing = 0.01;
	float space = 1;
	float height;
	float lineWidth = 1.0f;
	float opacity = 1;
};

class Tekst2d
{
public:
	Transformations2d Transform;
	
	TekstProperties properties;

	std::vector<float> vertecies;
	std::vector<unsigned int> indecies;

	int index = 0;

	glm::vec3 color = glm::vec3(1.0f);

	RenderingTekst Letters;

	Tekst2d(glm::vec2 pos, float rot, glm::vec2 sc, std::string object, float height, float spacing, int i);
	~Tekst2d();

	void Move(glm::vec2 pos);
	void MoveTo(glm::vec2 pos);

	void Rotate(float degries);
	void RotateTo(float rot);

	void Scale(glm::vec2 scale);
	void ScaleBy(glm::vec2 scale);
	void ScaleTo(glm::vec2 scale);

	void SetColor(glm::vec3 color);

private:
	void AddLetter(std::string file, int letter);

	void AddVao();

	VertexData ReadVertexFile(std::string file);
};

#endif // MY_HEADER_H


