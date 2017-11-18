#pragma once
#include <windows.h>
#include<fstream>
#include<iostream>
#include "rapidjson-master\include\rapidjson\istreamwrapper.h"
#include "rapidjson-master\include\rapidjson\reader.h"
#include "rapidjson-master\include\rapidjson\document.h"
#include "rapidjson-master\include\rapidjson\filereadstream.h"
#include <vector>
#include "gameError.h"
using namespace rapidjson;
using namespace std;


struct MapCell
{
	// rect of cell
	RECT* rect;
	//visible: true map cell will draw or not 
	bool visible;
};


class MapInfo
{
private:
	//list of rectangle use for mapCell
	vector<RECT*> *listRectOfMapCell;

	//two dimension array : prepresent map
	MapCell** map;

	int mapWidth;
	int mapHeight;

	int tileHeight;
	int tileWidth;

	//file name of json file
	const char* fileJson;

public:
	MapInfo();
	~MapInfo();

	bool initialize(const char* fileJson);

	MapCell** getMap() { return map; }
	int getMapWidth() { return mapWidth; }
	int getMapHeight() { return mapHeight; }
	int getTileHeight() { return tileHeight; }
	int getTileWidth() { return tileWidth; }
};

