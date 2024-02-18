#include "Tekst2d.h"
#include <map>
#include <cctype>
#include <cwctype>
#include <algorithm>

namespace Tekst2d_ {
	const wchar_t polish[] = { L'•', L'∆', L' ', L'£', L'—', L'”', L'å', L'è', L'Ø', L'\0' };
	const float smallLetterMultiplier = .8f;

	// prawdopodobnie nie trzeba az tyle, dodalem na wszelki wypadek - niepotrzebne usunac
	std::map<char, std::string> models{
		{'.', "dot"},
		{',', "comma"},
		{';', "semi-colon"},
		{':', "colon"},
		{'!', "exlamation-mark"},
		{'?', "question-mark"},
		{'/', "slash"},
		{'#', "hash"},
		{'-', "dash"},
		{'%', "percent"},
		{'*', "asterisk"},
		{'+', "plus"},
		{'=', "equals"},
		{'_', "underscore"},
		{'<', "less-than"},
		{'>', "more-than"},
		{'(', "left-bracket"},
		{')', "right-bracket"},
		{'[', "left-bracket-square"},
		{']', "right-bracket-square"},
		{'{', "left-bracekt-brace"},
		{'}', "right-bracket-brace"},
		{'\'', "apostrophe"},
		{' ', "space"}
	};
}
using namespace Tekst2d_;


Tekst2d::Tekst2d(glm::vec2 pos, float rot, glm::vec2 sc, std::string object, float height, float spacing, int i)
	: index(i)
{
	this->properties.height = height;
	this->properties.spacing = spacing;
	this->Transform.position = pos;
	this->Transform.orientation = rot;
	this->Transform.scale = sc;

	for (int i = 0; i < object.size(); i++)
	{
		bool upper = true;
		std::string lett;
		wchar_t check = object[i]; 

		if (std::iswalpha(check) || std::iswalnum(check) || std::find(std::begin(polish), std::end(polish), std::towupper(check)) != std::end(polish)) {
			if (std::iswlower(check)) upper = false;
			lett = check;
		}
		else if (check == ' ') lett = "space";
		else {
			lett = models[check];
		}

		if (!upper) {
			// todo: wirte scaling letters code ig
		}

		AddLetter(lett, i);
	}

	AddVao();
}

Tekst2d::~Tekst2d() {
	glDeleteVertexArrays(1, &Letters.VAO);
}

void Tekst2d::Move(glm::vec2 pos) {
	this->Transform.position += pos;
}

void Tekst2d::MoveTo(glm::vec2 pos) {
	this->Transform.position = pos;
}

void Tekst2d::Rotate(float degries) {
	this->Transform.orientation += degries;
	this->Transform.orientation = std::fmod(this->Transform.orientation, 360.0f);
}

void Tekst2d::RotateTo(float rot) {
	this->Transform.orientation = std::fmod(rot, 360.0f);
}

void Tekst2d::Scale(glm::vec2 scale) {
	this->Transform.scale = glm::vec2(this->Transform.scale.x * scale.x, this->Transform.scale.y * scale.y);
}

void Tekst2d::ScaleBy(glm::vec2 scale) {
	this->Transform.scale += scale;
}

void Tekst2d::ScaleTo(glm::vec2 scale) {
	this->Transform.scale = scale;
}


void Tekst2d::SetColor(glm::vec3(color)) {
	this->color = color;
}

void Tekst2d::AddLetter(std::string file, int letter) {
	if (file == " ") {
		return;
	}
	VertexData data = ReadVertexFile(file);

	for (int i = 0; i < data.vNum; i+=6) {
		vertecies.push_back(data.vertecies[i] + (1 + properties.spacing) * letter);
		for(int j = 1; j < 6; j++)
			vertecies.push_back(data.vertecies[i + j]);
	}
	for (int i = 0; i < data.iNum; i++) {
		indecies.push_back(data.indecies[i] + Alreadyletters);
	}

	Letters.pointsNum += data.vNum / 6;
	Letters.lines += data.iNum;
	Alreadyletters += data.vNum / 6;
}

void Tekst2d::AddVao() {
	int vNum = this->vertecies.size();
	int iNum = this->indecies.size();
	float* vertecies = new float[vNum];
	unsigned int* indecies = new unsigned int[iNum];
	unsigned int vao, vbo, ebo;

	for (int i = 0; i < this->vertecies.size(); i++) {
		vertecies[i] = this->vertecies[i];
	}

	for (int i = 0; i < this->indecies.size(); i++) {
		indecies[i] = this->indecies[i];
	}

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	//bindowanie   
	glBindVertexArray(vao);

	//bindowanie VBO
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vNum * sizeof(float), vertecies, GL_STATIC_DRAW);

	//bindowanie EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, iNum * sizeof(unsigned int), indecies, GL_STATIC_DRAW);

	//informacja o verteksach dla VAO
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//informacja o kolorach dla VAO
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	Letters.VAO = vao;

	glBindVertexArray(0);
	glDeleteBuffers(1, &ebo);
	glDeleteBuffers(1, &vbo);
}

VertexData Tekst2d::ReadVertexFile(std::string file) {
	std::string vPath = "textFiles/alphabet/" + file + ".vx.txt";
	std::string iPath = "textFiles/alphabet/" + file + ".ind.txt";
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
		else if (vCode[i] == '/')
		{
			i++;
			while (vCode[i] != '/')
				i++;
			i++;
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
		else if (iCode[i] == '/') {
			i++;
			while (iCode[i] != '/')
				i++;
			i++;
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

	VertexData r;
	r.indecies = indicies2;
	r.vertecies = verecies2;
	r.iNum = iNum;
	r.vNum = vNum;

	return r;
}