#include "ObjectPool.h"

template<class TObject>
ObjectPool<TObject>* ObjectPool<TObject>::instance = nullptr;

template<class TObject>
ObjectPool<TObject>::ObjectPool()
{
	this->list = nullptr;
}

template<class TObject>
ObjectPool<TObject>::ObjectPool(int startNumberElement)
{
	for (int i = 0; i < startNumberElement; i++)
		this->list->push_back(new TObject());
}

template<class TObject>
ObjectPool<TObject>::~ObjectPool()
{
	while (this->list.size())
	{
		TObject *obj = this->list.front();
		this->list.pop_front();
		delete obj;
	}
}

template<class TObject>
ObjectPool<TObject> * ObjectPool<TObject>::getInstance()
{
	if (instance == nullptr)
	{
		instance = new ObjectPool<TObject>();
	}

	return instance;
}

template<class TObject>
TObject * ObjectPool<TObject>::getObject()
{
	if (this->list.empty())
	{
		return new TObject();
	}
	else
	{
		TObject* obj = this->list.front();

		if (obj->isUsing())
			return new TObject();

		this->list.pop_front();
		return obj;
	}
}

template<class TObject>
void ObjectPool<TObject>::returnObject(TObject * object)
{
	object->reset();
	this->list.push_back(object);
}
