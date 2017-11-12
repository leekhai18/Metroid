#include "Text.h"



Text::Text()
{
}

Text::Text(std::string text, eFont font, int fontHeight, VECTOR2 position, D3DCOLOR color, bool isItalic, bool isSingle)
{
	this->graphics = nullptr;

	this->text = text;
	
	switch (font)
	{
	case body:
		this->font = "Press Start 2P";
		break;
	case header:
		this->font = "North Face";
		break;
	case header1:
		this->font = "Sign Painter's Gothic Shaded JL";
		break;
	default:
		this->font = "Arial";
		break;
	}

	this->fontHeight = fontHeight;
	this->color = color;
	this->isItalic = isItalic;

	if (isSingle)
		this->format = DT_LEFT | DT_NOCLIP;
	else
		this->format = DT_LEFT | DT_WORDBREAK;

	this->position = position;
}


Text::~Text()
{
}

bool Text::initialize(Graphics * graphics)
{
	try {
		this->graphics = graphics;      
	}
	catch (...) { return false; }

	this->DXFont = this->graphics->loadDirectXFont(this->font, this->fontHeight, this->isItalic);

	return true;
}

void Text::draw()
{
	if (this->graphics == nullptr)
		return;

	this->graphics->drawText(this->DXFont, this->text, this->position, this->format, this->color);
}

void Text::setText(std::string text)
{
	this->text = text;
}

std::string Text::getText()
{
	return this->text;
}

void Text::setColor(D3DCOLOR color)
{
	this->color = color;
}

D3DCOLOR Text::getColor()
{
	return this->color;
}
