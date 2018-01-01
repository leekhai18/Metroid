#pragma once
#include "BaseObject.h"
#include "Samus.h"
class MaChineGun:public BaseObject
{
private:
	Samus* samus;
public:
	MaChineGun();
	MaChineGun(Graphics* graphics,TextureManager* textureM,Samus* samus);
	~MaChineGun();
};

