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
	float lineWidth = 1.0f;
	bool onTop = false;
	int pointsNum = 0;
	int lines = 0;

};

class GameObject
{
public:
	Transformations Transform;
	Rendering View;

	//VBO, VAO, EBO
	unsigned int VBO, VAO, EBO;

	int index = 0;

	GameObject(glm::vec3 pos, glm::vec3 rot, glm::vec3 sc, std::string object, int i);
	~GameObject();

	void Move(glm::vec3 pos);
	void MoveTo(glm::vec3 pos);

	void Rotate(glm::vec3 rot, float degries);
	void Rotate(glm::vec3 rot, glm::vec3 degries);
	void RotateTo(glm::vec3 rot);

	void Scale(glm::vec3 scale);
	void ScaleTo(glm::vec3 scale);
};

#endif // MY_HEADER_H

