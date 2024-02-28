#include "TextBox.h"
#include "Game.h"


TextBox::TextBox(const float& Left, const float Right, const float& Top, const float& Bottom, const float& spacing, const bool& AlignCenterHorizontaly, const unsigned int& index)
	:AlignH(AlignCenterHorizontaly), Spacing(spacing), Index(index)
{
	Position.Left = Left;
	Position.Right = Right;
	Position.Top = Top;
	Position.Bottom = Bottom;
}
TextBox::~TextBox() {
	for (int i = 0; i < Letters.size(); i++) {
		Game::Destroy(Letters[i]);
	}
}

void TextBox::Write(std::string Word) {
	this->Word += Word;
	float left = Position.Left;
	if (AlignH) {
		left = (Position.Right + Position.Left) / 2.0f - (float(Word.length()) * ((Position.Top - Position.Bottom) + Spacing) / 2.0f - Spacing) / 2.0f;
	}
	for (char i : Word) {
		std::wstring Model;
		if (models.find(i) != models.end())
			Model = models[i];
		else if (i == ' ') {
			Spaces++;
			continue;
		}
		else
		{
			Model = wchar_t(std::toupper(i));
		}

		float posx = left + ((Letters.size() + Spaces) * ((Position.Top - Position.Bottom) + Spacing)) / 2.0f;
		float posy = Position.Bottom;
		vec pos = vec(posx, posy, 0);

		vec scale = vec((Position.Top - Position.Bottom) / 2.0f, 3);

		GameObject* Letter = Game::Create(pos, vec(0, 3), scale, Model);
		Letter->Stage[Letter->activeStage].onTop = true;
		Letter->SetColor(Color);
		Letter->Stage[Letter->activeStage].lineWidth = Boldicity;
		Letters.push_back(Letter);
	}
}
void TextBox::ChangeText(std::string Word, const unsigned int& BoldicityChange, const float& sizeChange) {
	for (int i = 0; i < Letters.size(); i++) {
		Game::Destroy(Letters[i]);
	}
	Letters.clear();
	this->Word = "";
	Spaces = 0;

	if(BoldicityChange != 0 && sizeChange != 0)
		ChangeSize(BoldicityChange, sizeChange);
	else
		Write(Word);
}

void TextBox::ChangeSize(const unsigned int& BoldicityChange, const float& sizeChange) {
	this->Boldicity += BoldicityChange;
	this->Position.Bottom -= sizeChange / 2.0f;
	this->Position.Top += sizeChange / 2.0f;

	ChangeText(Word);
}

bool TextBox::Hovered(const float& FreeSpace) const {
	float MouseX = float(Game::MousePosition.x) / float(Game::ScreenSize.x);
	MouseX -= 0.5f;
	MouseX *= 2.0f;

	float MouseY = float(Game::MousePosition.y) / float(Game::ScreenSize.y);
	MouseY -= 0.5f;
	MouseY *= 2.0f;
	MouseY = MouseY * -1.0f;

	if (MouseY >= Position.Bottom - FreeSpace && MouseY <= Position.Top + FreeSpace && MouseX >= Position.Left - FreeSpace && MouseX <= Position.Right + FreeSpace) return true;
	else return false;
}