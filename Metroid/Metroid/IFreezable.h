#pragma once
class IFreezable
{
protected:
	bool isCold;
	const int* frameID;
	float timeReturnNormal;
public:
	bool getCold();
	void setCold(bool cold);
	IFreezable();
	IFreezable(const int* frameID);
	~IFreezable();
};

