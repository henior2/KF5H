#pragma once
#include "vec.h"
#include "GameObject.h"
#include <vector>
#include <map>

struct Positions {
	float Left;
	float Right;
	float Top;
	float Bottom;
};

class TextBox
{
public:

	Positions Position;
	std::vector<GameObject*> Letters;
	float Spacing;
	unsigned int Boldicity = 1;
	unsigned int Index;
	bool AlignH;
	vec Color = vec(1, 3);
	TextBox(const float& Left, const float Right, const float& Top, const float& Bottom, const float& spacing, const bool& AlignCenterHorizontaly, const unsigned int& index);
	~TextBox();

	void Write(std::string Word);
	void ChangeText(std::string Word);
private:
	std::map<char, std::wstring> models{
		{'.', L"dot"},
		{',', L"comma"},
		{';', L"semi-colon"},
		{':', L"colon"},
		{'!', L"exlamation-mark"},
		{'?', L"question-mark"},
		{'/', L"slash"},
		{'#', L"hash"},
		{'-', L"dash"},
		{'%', L"percent"},
		{'*', L"asterisk"},
		{'+', L"plus"},
		{'=', L"equals"},
		{'_', L"underscore"},
		{'<', L"less-than"},
		{'>', L"more-than"},
		{'(', L"left-bracket"},
		{')', L"right-bracket"},
		{'[', L"left-bracket-square"},
		{']', L"right-bracket-square"},
		{'{', L"left-bracekt-brace"},
		{'}', L"right-bracket-brace"},
		{'\'', L"apostrophe"}
	};

	unsigned int Spaces = 0;
};

