#pragma once
#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "Constants.h"
#include "GameError.h"

// DirectX pointer types
#define LP_TEXTURE  LPDIRECT3DTEXTURE9
#define LP_SPRITE   LPD3DXSPRITE
#define LP_3DDEVICE LPDIRECT3DDEVICE9
#define LP_3D       LPDIRECT3D9
#define VECTOR2     D3DXVECTOR2
#define VECTOR3		D3DXVECTOR3
#define VECTOR2ZERO VECTOR2(0.0f, 0.0f)
#define VECTOR2ONE  VECTOR2(1.0f, 1.0f)
#define LP_LINE		LPD3DXLINE

// Color defines
#define COLOR_ARGB DWORD
#define SETCOLOR_ARGB(a,r,g,b) \
    ((COLOR_ARGB)((((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)))

namespace GraphicsNS
{
	const COLOR_ARGB WHITE = D3DCOLOR_ARGB(255, 255, 255, 255);
	const COLOR_ARGB BLACK = D3DCOLOR_ARGB(255, 0, 0, 0);
	const COLOR_ARGB RED = D3DCOLOR_ARGB(255, 255, 0, 0);
}

// SpriteData: The properties required by Graphics::drawSprite to draw a sprite
struct SpriteData
{
	int         width;      // width of sprite in pixels
	int         height;     // height of sprite in pixels
	VECTOR2		position;
	VECTOR2		scale;
	VECTOR2		origin;
	float		rotate;
	RECT        rect;       // used to select an image from a larger texture
	LP_TEXTURE  texture;    // pointer to texture
	bool        flipHorizontal; // true to flip sprite horizontally (mirror)
	bool        flipVertical;   // true to flip sprite vertically
};

	

class Graphics
{
private:
	// DirectX pointers and stuff
	LP_3D       direct3d;
	LP_3DDEVICE device3d;
	LP_SPRITE   sprite;
	LP_LINE line;
	D3DPRESENT_PARAMETERS d3dpp;
	D3DDISPLAYMODE pMode;

	// other variables
	HRESULT     result;         // standard Windows return codes
	HWND        hwnd;
	bool        fullscreen;
	int         width;
	int         height;
	COLOR_ARGB  backColor;      // background color

								// (For internal engine use only. No user serviceable parts inside.)
								// Initialize D3D presentation parameters
	void    initD3Dpp();

public:
	Graphics();
	virtual ~Graphics();

	// Releases direct3d and device3d.
	void    releaseAll();

	void    initialize(HWND hw, int width, int height, bool fullscreen);

	HRESULT loadTexture(const char * filename, COLOR_ARGB transcolor, UINT &width, UINT &height, LP_TEXTURE &texture);

	ID3DXFont* Graphics::loadDirectXFont(std::string fontName, int fontHeight, bool isItalic);

	void drawText(ID3DXFont* font, std::string text, VECTOR2 position, DWORD format, D3DCOLOR color);

	// Display the offscreen backbuffer to the screen.
	HRESULT showBackbuffer();

	void    drawSprite(const SpriteData &spriteData,           // sprite to draw
		COLOR_ARGB color = GraphicsNS::WHITE);      // default to white color filter (no change)

	void drawLine(const VECTOR3 * vertices, int count, COLOR_ARGB color = GraphicsNS::RED);
	
	// Reset the graphics device.
	HRESULT reset();

	// Return device3d.
	LP_3DDEVICE get3Ddevice() { return device3d; }

	// Return sprite
	LP_SPRITE   getSprite() { return sprite; }

	// Test for lost device
	HRESULT getDeviceState();

	// Checks the adapter to see if it is compatible with the BackBuffer height,
	// width and refresh rate specified in d3dpp. Fills in the pMode structure with
	// the format of the compatible mode, if found.
	// Pre: d3dpp is initialized.
	// Post: Returns true if compatible mode found and pMode structure is filled.
	//       Returns false if no compatible mode found.
	bool    isAdapterCompatible();


	// Set color used to clear screen
	void setBackColor(COLOR_ARGB c) { backColor = c; }

	void clear(COLOR_ARGB C);

	HRESULT beginScene()
	{
		result = E_FAIL;
		if (device3d == NULL)
			return result;
		// clear backbuffer to backColor
		device3d->Clear(0, NULL, D3DCLEAR_TARGET, backColor, 1.0F, 0);
		result = device3d->BeginScene();          // begin scene for drawing
		return result;
	}

	HRESULT endScene()
	{
		result = E_FAIL;
		if (device3d)
			result = device3d->EndScene();
		return result;
	}

	void spriteBegin()
	{
		sprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_DONOTSAVESTATE);
	}

	void spriteEnd()
	{
		sprite->End();
	}
};

