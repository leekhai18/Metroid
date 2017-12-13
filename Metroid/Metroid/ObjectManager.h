#pragma once
#include<fstream>
#include <list>
#include "rapidjson-master\include\rapidjson\istreamwrapper.h"
#include "rapidjson-master\include\rapidjson\reader.h"
#include "rapidjson-master\include\rapidjson\document.h"
#include "rapidjson-master\include\rapidjson\filereadstream.h"
#include "BaseObject.h"
#include "Graphics.h"
#include "Constants.h"
#include "Quadtree.h"
#include "Zommer.h"
#include "Samus.h"

#define QUADTREE_W 7680
#define QUADTREE_H 7680

using namespace rapidjson;
using namespace std;


class ObjectManager
{
private:
	static ObjectManager* instance;

	TextureManager* textureManager;
	Graphics* graphics;

	Quadtree* quadtree;

	Samus* samus;
	Zommer* zommer;

	list<BaseObject*>* object_list;

	int totalObjectsPerFrame;
	list<BaseObject*>* listCanCollideSamus;
	list<BaseObject*>* listObjectNotWallOnViewPort;
	list<BaseObject*>* listWallCanCollideSamus;

	float timer;

public:
	static ObjectManager* getInstance();

	list<BaseObject*> *getListWallOnViewPort() { return listWallCanCollideSamus; };

	void init(TextureManager* textureM, Graphics* graphics, Samus* samus);
	bool load_list(const char *filename);
	void release();

	void onCheckCollision(float frametime);
	void onCheckCollision(BaseObject* obj, float frametime);

	int getTotalObjectsPerFrame();
	void setTotalObjectPerFrame(int num);

	void update(float dt);
	void draw();

	ObjectManager();
	~ObjectManager();
};

