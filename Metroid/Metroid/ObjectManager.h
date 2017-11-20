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

using namespace rapidjson;
using namespace std;
class ObjectManager
{
private:
	Camera* camera;

	list<BaseObject> object_list;
	static ObjectManager* instance;
public:
	list<BaseObject>& getEntityList() { return object_list; };
	//get instance
	static ObjectManager* getInstance();
	//check collide between entity with another object
	void onCheckCollision(BaseObject* entity, float frametime);
	bool load_list(const char *filename);

	ObjectManager();
	~ObjectManager();

	void release();

	void setCamera(Camera* cam);
};

