#pragma once
#include "Graphics.h"


class Text
{
private:
	Graphics *graphics;
	ID3DXFont* DXFont;

	std::string text;
	std::string font;
	int fontHeight;
	VECTOR2 position;
	D3DCOLOR color;
	bool isItalic;
	DWORD format;

public:
	Text();
	Text(std::string text, eFont font, int fontHeight, VECTOR2 position, D3DCOLOR color, bool isItalic, bool isSingle);
	~Text();

	bool initialize(Graphics* graphics);
	void draw();

	void setText(std::string text);
	std::string getText();

	void setColor(D3DCOLOR color);
	D3DCOLOR getColor();
};

