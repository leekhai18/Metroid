#pragma once

template <class EP>
class ElementPool
{
private:
	EP ep;
	bool inUse;

public:
	ElementPool() : inUse(false) {}
	~ElementPool() {}

	void setUse(bool flag);
	bool isUsing();

	void setValue(EP ep);
	EP getValue();
};

