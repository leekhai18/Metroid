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
#include "Samus.h"
#include <map>
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
	int tempOK;
	Samus* samus;
	//list<BaseObject*>* object_list;

	int totalObjectsPerFrame;
	map<int,BaseObject*>* listNotWallCanCollideSamus;
	map<int, BaseObject*>* listObjectNotWallOnViewPort;
	map<int, BaseObject*>* listWallCanCollideSamus;
	map<int, BaseObject*>* listWallEmnermy;
	map<int, BaseObject*> map_object;
	map<int, BaseObject*>* bullet_object;
	float timer;

	void insertQuadTreeNode(const Value& value,Quadtree* quadtree);

	bool isEnd;
public:
	void setEnd(bool active);
	bool getEnd();

	static ObjectManager* getInstance();

	map<int, BaseObject*>* getListWallOnViewPort() { return listWallCanCollideSamus; };

	void init(TextureManager* textureM, Graphics* graphics, Samus* samus);
	bool load_list(const char *filename);

	bool load_quatree(const char *filename);
	void release();

	void handleVelocity(float dt);

	void onCheckCollision(float frametime);
	void onCheckCollision(BaseObject* obj, float frametime);

	int getTotalObjectsPerFrame();
	void setTotalObjectPerFrame(int num);

	void update(float dt);
	void draw();

	ObjectManager();
	~ObjectManager();
};

