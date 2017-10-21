#pragma once
#include "graphics.h"
#include "constants.h"
#include<fstream>
#include<iostream>
#include "rapidjson-master\include\rapidjson\istreamwrapper.h"
#include "rapidjson-master\include\rapidjson\reader.h"
#include "rapidjson-master\include\rapidjson\document.h"
#include "rapidjson-master\include\rapidjson\filereadstream.h"
using namespace rapidjson;
using namespace std;

class SpriteManager
{
private:
	//list of sprites data
	SpriteData* spritesData;
	//filename to read
	const char* filename;
public:
	bool initialize(const char* filename);
	void releaseAll();
	const SpriteData* getSpritesData();
	SpriteManager();
	~SpriteManager();
};
