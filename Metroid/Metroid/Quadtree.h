#define MAX_LEVEL                6
#define MAX_OBJECT_IN_REGION    5
#include "BaseObject.h"
#include <list>
#include "Constants.h"

class Quadtree
{
private:
	int            level;
	MetroidRect   region;
	list<BaseObject*>   objects_list;
	Quadtree**     nodes;
	bool           isContain(BaseObject* obj);
	void           split();
public:
	Quadtree();
	Quadtree(int level, MetroidRect region);
	~Quadtree();
	void clear();
	void insert(BaseObject* entity);
	void retrieve(list<BaseObject*>* objects_list, BaseObject* obj);
	static Quadtree*  createQuadTree(MetroidRect rect , list<BaseObject*>* object_list);

};