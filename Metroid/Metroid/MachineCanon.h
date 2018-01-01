#pragma once
#include "BaseObject.h"
#include "Samus.h"
class MachineCanon:public BaseObject
{
private:
	Samus* samus;
public:
	MachineCanon();
	MachineCanon(Graphics* graphics,TextureManager* textureM,Samus* samus);
	~MachineCanon();
};

