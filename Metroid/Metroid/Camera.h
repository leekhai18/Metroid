#pragma once
#include <d3dx9.h>
#include <d3d9.h>


class Camera
{
private:
	int             width;
	int             height;

	D3DXVECTOR3     position;


public:
    Camera(int width, int height);
	~Camera();

    void setPosition(float x, float y);
    void setPosition(D3DXVECTOR3 pos);
	void setPosition(D3DXVECTOR2 pos);

    int getWidth();
    int getHeight();

	D3DXVECTOR3 getPosition();
    RECT getBound();
};

