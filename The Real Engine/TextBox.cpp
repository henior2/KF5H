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

		float posx = left + (Letters.size() * ((Position.Top - Position.Bottom) + Spacing)) / 2.0f + (Spaces * (Position.Top - Position.Bottom)) / 2.0f;
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
void TextBox::ChangeText(std::string Word) {
	for (int i = 0; i < Letters.size(); i++) {
		Game::Destroy(Letters[i]);
	}
	Letters.clear();
	Spaces = 0;

	Write(Word);
}