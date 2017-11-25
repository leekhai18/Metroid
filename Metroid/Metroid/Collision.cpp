#include "Collision.h"


Collision* Collision::instance = nullptr;
Collision* Collision::getInstance()
{
	if (instance == nullptr)
	{
		instance = new Collision();
	}
	return instance;
}
bool Collision::checkOnGround(MetroidRect obj)
{
	MetroidRect sweptRect;
	sweptRect.top = obj.top;
	sweptRect.bottom = obj.bottom + 1;
	sweptRect.left = obj.left;
	sweptRect.right = obj.right;
	list<BaseObject*>::iterator begin = ObjectManager::getInstance()->getObjectList()->begin();

	list<BaseObject*>::iterator end = ObjectManager::getInstance()->getObjectList()->end();
	for (list<BaseObject*>::iterator i = begin; i != end; ++i)
	{
		if ((*i)->getBoundCollision().top == sweptRect.bottom)
		{
			if (isCollide((*i)->getBoundCollision(), sweptRect))
			{
				return true;
			}
		}
	}
	return false;
}

bool Collision::isCollide(MetroidRect myRect, MetroidRect otherRect)
{
	if (myRect.right >= otherRect.left&&myRect.left <= otherRect.right&&myRect.bottom >= otherRect.top&&myRect.top <= otherRect.bottom)
	{
		return true;
	}
	return false;
}

bool Collision::checkCollision(BaseObject * myEntity, BaseObject * otherEntity, float frametime)
{
	float _dxEntry, _dyEntry, _dxExit, _dyExit;
	float _txEntry, _tyEntry, _txExit, _tyExit;
	CollisionReturn data;
	CollideDirection direction;
	MetroidRect   myRect = myEntity->getBoundCollision();
	MetroidRect   otherRect = otherEntity->getBoundCollision();
	if (otherRect.top == 3296)
	{
		int test = 0;
	}
	MetroidRect   broadphaseRect = getSweptBroadphaseRect(myEntity, frametime);	// là bound của object được mở rộng ra thêm một phần bằng với vận tốc (dự đoán trước bound)
	if (!isCollide(broadphaseRect, otherRect))				// kiểm tra tính chồng lắp của 2 hcn
	{
		return false;
	}
	//velocity of each frame
	VECTOR2 otherVeloc = VECTOR2(otherEntity->getVelocity().x * frametime, otherEntity->getVelocity().y * frametime);
	VECTOR2 myVelocity = VECTOR2(myEntity->getVelocity().x * frametime, myEntity->getVelocity().y * frametime);

	VECTOR2 velocity = myVelocity;

	if (otherVeloc != VECTOR2(0, 0))
	{
		velocity = otherVeloc - myVelocity;
	}
	// tìm khoảng cách giữa cạnh gần nhất, xa nhất 2 object dx, dy
	// dx
	if (velocity.x > 0)

	{
		_dxEntry = otherRect.left - myRect.right;
		_dxExit = otherRect.right - myRect.left;
	}
	else
	{
		_dxEntry = otherRect.right - myRect.left;
		_dxExit = otherRect.left - myRect.right;
	}

	// dy
	if (velocity.y > 0)
	{
		_dyEntry = otherRect.top - myRect.bottom;
		_dyExit = otherRect.bottom - myRect.top;
	}
	else
	{
		_dyEntry = otherRect.bottom - myRect.top;
		_dyExit = otherRect.top - myRect.bottom;
	}

	// tìm thời gian va chạm 
	if (velocity.x == 0)
	{
		// chia cho 0 ra vô cực
		_txEntry = -std::numeric_limits<float>::infinity();
		_txExit = std::numeric_limits<float>::infinity();
	}

	else
	{
		_txEntry = _dxEntry / velocity.x;
		_txExit = _dxExit / velocity.x;
	}

	if (velocity.y == 0)
	{
		_tyEntry = -std::numeric_limits<float>::infinity();
		_tyExit = std::numeric_limits<float>::infinity();
	}
	else
	{
		_tyEntry = _dyEntry / velocity.y;
		_tyExit = _dyExit / velocity.y;
	}

	// thời gian va chạm
	// va chạm khi x và y CÙNG chạm => thời gian va chạm trễ nhất giữa x và y
	float entryTime = max(_txEntry, _tyEntry);
	// hết va chạm là 1 trong 2 x, y hết va chạm => thời gian sớm nhất để kết thúc va chạm
	float exitTime = min(_txExit, _tyExit);

	// object không va chạm khi:
	// nếu thời gian bắt đầu va chạm hơn thời gian kết thúc va chạm
	// thời gian va chạm x, y nhỏ hơn 0 (chạy qua luôn, 2 thằng đang đi xa ra nhau)
	// thời gian va chạm x, y lớn hơn 1 (còn xa quá chưa thể va chạm)
	if (entryTime > exitTime || _txEntry < 0.0f && _tyEntry < 0.0f || _txEntry > 1.0f || _tyEntry > 1.0f)
	{
		// không va chạm trả về 1 đi tiếp bt
		collideDirection = NONE;
		return false;
	}
	// nếu thời gian va chạm x lơn hơn y
	if (_txEntry > _tyEntry)
	{
		// xét x
		// khoảng cách gần nhất mà nhỏ hơn 0 nghĩa là thằng kia đang nằm bên trái object này => va chạm bên phải nó
		if (_dxEntry < 0.0f)
		{
			direction = CollideDirection::RIGHT;
		}
		else
		{
			direction = CollideDirection::LEFT;
		}
	}
	else
	{
		// xét y
		if (_dyEntry < 0.0f)
		{
			direction = CollideDirection::BOTTOM;

		}
		else
		{
			direction = CollideDirection::TOP;
		}
	}

	if (0<entryTime&& entryTime<1.0f)
	{
		data.direction = direction;
		data.entryTime = entryTime;
		data.idObject = otherEntity->getId();
		switch (direction)
		{
		case TOP:
			data.positionCollision = otherRect.top;
			break;
		case BOTTOM:
			data.positionCollision = otherRect.bottom;
			break;
		case LEFT:
			data.positionCollision = otherRect.left;
			break;
		case RIGHT:
			data.positionCollision = otherRect.right;
			break;
		}
		dataReturn.push_back(data);
		return true;
	}
	return false;
}

MetroidRect Collision::CalculateCollisionRect(MetroidRect rect1, MetroidRect rect2)
{
	MetroidRect  result;
	//chon max Left
	result.left = rect1.left > rect2.left ? rect1.left : rect2.left;
	//chon max right
	result.right = rect1.right < rect2.right ? rect1.right : rect2.right;
	//chon min bottom
	result.bottom = rect1.bottom < rect2.bottom ? rect1.bottom : rect2.bottom;
	//chon max top
	result.top = rect1.top > rect2.top ? rect1.top : rect2.top;

	return result;
}

MetroidRect Collision::getSweptBroadphaseRect(BaseObject * obj, float frametime)
{
	VECTOR2 velocity = VECTOR2(obj->getVelocity().x * frametime, obj->getVelocity().y * frametime);
	MetroidRect myRect = obj->getBoundCollision();

	MetroidRect   rect;
	rect.top = velocity.y > 0 ? myRect.top : myRect.top + velocity.y;
	rect.bottom = velocity.y > 0 ? myRect.bottom + velocity.y : myRect.bottom;
	rect.left = velocity.x > 0 ? myRect.left : myRect.left + velocity.x;
	rect.right = velocity.x > 0 ? myRect.right + velocity.x : myRect.right;

	return rect;
}

list<CollisionReturn>& Collision::getDataReturn()
{
	// TO DO: insert return statement here
	return this->dataReturn;
}

void Collision::clearDataReturn()
{
	this->dataReturn.clear();
}

Collision::Collision()
{
}
void Collision::release()
{
	delete instance;
}

Collision::~Collision()
{

}
