#pragma once
#include <iostream>
#include <list>
using namespace std;

template <class TObject>
class ObjectPool
{
private:
	list<TObject*> list;

	static ObjectPool* instance;
public:
	ObjectPool();
	ObjectPool(int startNumberElement);
	~ObjectPool();

	static ObjectPool* getInstance();

	TObject* getObject();
	void returnObject(TObject* object);
};
