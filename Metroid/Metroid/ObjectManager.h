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
#include "Camera.h"
#include "Quadtree.h"
#define QUADTREE_W 7680
#define QUADTREE_H 7680
using namespace rapidjson;
using namespace std;
class ObjectManager
{
private:
	Camera* camera;
	TextureManager* textureManager;
	Graphics* graphics;

	static ObjectManager* instance;
	Quadtree* quadtree;
	list<BaseObject*>* object_list;
public:
	list<BaseObject*>* getEntityList() { return object_list; };
	void init(TextureManager* textureM, Graphics* graphics);
	bool load_list(const char *filename);
	void release();

	//get instance
	static ObjectManager* getInstance();

	//check collide between entity with another object
	void onCheckCollision(BaseObject* entity, float dt);


	ObjectManager();
	~ObjectManager();

	void setCamera(Camera* cam);
};

