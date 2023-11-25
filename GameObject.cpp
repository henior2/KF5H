#include "GameObject.h"
#include <math.h>

GameObject::GameObject(glm::vec3 pos, glm::vec3 rot, glm::vec3 sc, std::string file, int i) 
	:index(i)
{
	this->Transform.position = pos;
	this->Transform.orientation = rot;
	this->Transform.scale = sc;

	std::string vPath = file + ".vx.txt";
	std::string iPath = file + ".ind.txt";
	std::string vCode;
	std::string iCode;
	std::ifstream iFile;
	std::ifstream vFile;
	//pozwolenie na wyrzucanie blendow
	vFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	iFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	//zmiana pliku txt na string(kod)
	try {
		//otworzenie plikow z modelem
		vFile.open(vPath);
		iFile.open(iPath);
		std::stringstream vStream, iStream;

		//zczytanie plikow do zmiennych stringstream
		vStream << vFile.rdbuf();
		iStream << iFile.rdbuf();

		//zamkniecie plikow
		vFile.close();
		iFile.close();

		//zmiana stringstream na string z kodem shaderow
		vCode = vStream.str();
		iCode = iStream.str();
	}
	//w razie blendu z plikiem, wypisanie blendu
	catch (std::ifstream::failure& e) {
		std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
	}

	std::vector<float> vertecies;
	std::vector<unsigned int> indicies;

	float vertex = 0.0f;
	unsigned int index = 0;
	bool minus = false;
	int przecinek = 0;
	bool czyPrzecinek = false;
	for (int i = 0; i < vCode.length(); i++) {
		if (vCode[i] == ' ' || vCode[i] == '\n' || vCode[i] == '\0') {
			if (minus == true) {
				vertex = -vertex;
			}
			vertecies.push_back(vertex / (float)(pow(10, przecinek)));
			minus = false;
			vertex = 0.0f;
			czyPrzecinek = false;
			przecinek = 0;
		}
		else if (vCode[i] == '-') {
			minus = true;
		}
		else if (vCode[i] == '.') {
			czyPrzecinek = true;
		}
		else
		{
			vertex *= 10.0f;
			vertex += vCode[i] - '0';
			if (czyPrzecinek == true) {
				przecinek++;
			}
		}
	}
	if (minus == true) {
		vertex = -vertex;
	}
	vertecies.push_back(vertex / (float)(pow(10, przecinek)));
	minus = false;
	vertex = 0.0f;
	czyPrzecinek = false;
	przecinek = 0;

	for (int i = 0; i < iCode.length(); i++) {
		if (iCode[i] == ' ' || iCode[i] == '\n' || iCode[i] == '\0') {
			indicies.push_back(index);
			minus = false;
			index = 0;
		}
		else
		{
			index *= 10;
			index += iCode[i] - '0';
		}
	}
	indicies.push_back(index);
	minus = false;
	index = 0;

	float* verecies2 = new float[vertecies.size()];
	unsigned int* indicies2 = new unsigned int[indicies.size()];

	int vNum = 0;
	int iNum = 0;

	for (int i = 0; i < vertecies.size(); i++) {
		verecies2[i] = vertecies[i];
		vNum++;
	}

	for (int i = 0; i < indicies.size(); i++) {
		indicies2[i] = indicies[i];
		iNum++;
	}

	this->View.pointsNum = vNum / 6;
	this->View.lines = iNum;

	unsigned int vao, vbo, ebo;

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	//bindowanie   
	glBindVertexArray(vao);

	//bindowanie VBO
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vNum * sizeof(float), verecies2, GL_STATIC_DRAW);

	//bindowanie EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, iNum * sizeof(unsigned int), indicies2, GL_STATIC_DRAW);

	//informacja o verteksach dla VAO
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//informacja o kolorach dla VAO
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	this->VAO = vao;
	this->VBO = vbo;
	this->EBO = ebo;
}

GameObject::~GameObject() {
	glDeleteVertexArrays(1, &this->VAO);
	glDeleteBuffers(1, &this->VBO);
	glDeleteBuffers(1, &this->EBO);
}

void GameObject::Move(glm::vec3 pos) {
	this->Transform.position += glm::vec3(pos.x * Right);
	this->Transform.position += glm::vec3(pos.y * Up);
	this->Transform.position += glm::vec3(pos.z * Front);
}

void GameObject::MoveTo(glm::vec3 pos) {
	this->Transform.position = glm::vec3(pos.x * Right);
	this->Transform.position = glm::vec3(pos.y * Up);
	this->Transform.position = glm::vec3(pos.z * Front);
}

void GameObject::Rotate(glm::vec3 rot, float degries) {
	this->Transform.orientation += rot * degries;
	this->Transform.orientation = glm::vec3(std::fmod(this->Transform.orientation.x, 360.0f), std::fmod(this->Transform.orientation.y, 360.0f), std::fmod(this->Transform.orientation.z, 360.0f));
	UpdateVectors();
}

void GameObject::Rotate(glm::vec3 degries) {
	this->Transform.orientation += degries;
	this->Transform.orientation = glm::vec3(std::fmod(this->Transform.orientation.x, 360.0f), std::fmod(this->Transform.orientation.y, 360.0f), std::fmod(this->Transform.orientation.z, 360.0f));
	UpdateVectors();
}

void GameObject::RotateTo(glm::vec3 rot) {
	this->Transform.orientation = glm::vec3(std::fmod(rot.x, 360.0f), std::fmod(rot.y, 360.0f), std::fmod(rot.z, 360.0f));
	UpdateVectors();
}

void GameObject::Scale(glm::vec3 scale) {
	this->Transform.scale = glm::vec3(this->Transform.scale.x * scale.x, this->Transform.scale.y * scale.y, this->Transform.scale.z * scale.z);
}

void GameObject::ScaleTo(glm::vec3 scale) {
	this->Transform.scale = scale;
}

void GameObject::UpdateVectors() {
	glm::vec3 front;
	float x, y, z;
	front.x = cos(glm::radians(this->Transform.orientation.y - 90.0f)) * cos(glm::radians(this->Transform.orientation.x));
	front.y = sin(glm::radians(this->Transform.orientation.x));
	front.z = sin(glm::radians(this->Transform.orientation.y - 90.0f)) * cos(glm::radians(this->Transform.orientation.x));
	Front = glm::normalize(front);
	
	Right.x = cos(glm::radians(this->Transform.orientation.z));
	Right.y = sin(glm::radians(this->Transform.orientation.z));
	Right.z = 0.0f;
	Right = glm::normalize(Right);

	Up = -glm::normalize(glm::cross(Front, Right));
}