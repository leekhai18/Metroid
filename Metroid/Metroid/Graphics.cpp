#include "Graphics.h"
#include "Metroid.h"
//=============================================================================
// Constructor
//=============================================================================
Graphics::Graphics()
{
	direct3d = NULL;
	device3d = NULL;
	sprite = NULL;
	fullscreen = false;
	width = GAME_WIDTH;    // width & height are replaced in initialize()
	height = GAME_HEIGHT;
	backColor = GraphicsNS::BACK_COLOR;
}

//=============================================================================
// Destructor
//=============================================================================
Graphics::~Graphics()
{
	releaseAll();
}



//=============================================================================
// Release all
//=============================================================================
void Graphics::releaseAll()
{
	SAFE_RELEASE(sprite);
	SAFE_RELEASE(device3d);
	SAFE_RELEASE(direct3d);
}

//=============================================================================
// Initialize DirectX graphics
// throws GameError on error
//=============================================================================
void Graphics::initialize(HWND hw, int w, int h, bool full)
{
	hwnd = hw;
	width = w;
	height = h;
	fullscreen = full;

	//initialize Direct3D
	direct3d = Direct3DCreate9(D3D_SDK_VERSION);
	if (direct3d == NULL)
		throw(GameError(GameErrorNS::FATAL_ERROR, "Error initializing Direct3D"));

	initD3Dpp();        // init D3D presentation parameters
	if (fullscreen)      // if full-screen mode
	{
		if (isAdapterCompatible())   // is the adapter compatible
									 // set the refresh rate with a compatible one
			d3dpp.FullScreen_RefreshRateInHz = pMode.RefreshRate;
		else
			throw(GameError(GameErrorNS::FATAL_ERROR,
				"The graphics device does not support the specified resolution and/or format."));
	}

	// determine if graphics card supports harware texturing and lighting and vertex shaders
	D3DCAPS9 caps;
	DWORD behavior;
	result = direct3d->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);
	// If device doesn't support HW T&L or doesn't support 1.1 vertex 
	// shaders in hardware, then switch to software vertex processing.
	if ((caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) == 0 ||
		caps.VertexShaderVersion < D3DVS_VERSION(1, 1))
		behavior = D3DCREATE_SOFTWARE_VERTEXPROCESSING;  // use software only processing
	else
		behavior = D3DCREATE_HARDWARE_VERTEXPROCESSING;  // use hardware only processing

														 //create Direct3D device
	result = direct3d->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hwnd,
		behavior,
		&d3dpp,
		&device3d);

	if (FAILED(result))
		throw(GameError(GameErrorNS::FATAL_ERROR, "Error creating Direct3D device"));

	result = D3DXCreateSprite(device3d, &sprite);
	if (FAILED(result))
		throw(GameError(GameErrorNS::FATAL_ERROR, "Error creating Direct3D sprite"));

}

//=============================================================================
// Initialize D3D presentation parameters
//=============================================================================
void Graphics::initD3Dpp()
{
	try {
		ZeroMemory(&d3dpp, sizeof(d3dpp));              // fill the structure with 0
														// fill in the parameters we need
		d3dpp.BackBufferWidth = width;
		d3dpp.BackBufferHeight = height;
		if (fullscreen)                                  // if fullscreen
			d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;  // 24 bit color
		else
			d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;   // use desktop setting
		d3dpp.BackBufferCount = 1;
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		d3dpp.hDeviceWindow = hwnd;
		d3dpp.Windowed = (!fullscreen);
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	}
	catch (...)
	{
		throw(GameError(GameErrorNS::FATAL_ERROR,
			"Error initializing D3D presentation parameters"));
	}
}



//=============================================================================
// Load the texture into default D3D memory (normal texture use)
// For internal engine use only. Use the TextureManager class to load game textures.
// Pre: filename is name of texture file.
//      transcolor is transparent color
// Post: width and height = size of texture
//       texture points to texture
// Returns HRESULT
//=============================================================================
HRESULT Graphics::loadTexture(const char *filename, COLOR_ARGB transcolor,
	UINT &width, UINT &height, LP_TEXTURE &texture)
{
	// The struct for reading file info
	D3DXIMAGE_INFO info;
	result = E_FAIL;

	try {
		if (filename == NULL)
		{
			texture = NULL;
			return D3DERR_INVALIDCALL;
		}
		// Get width and height from file
		result = D3DXGetImageInfoFromFile(filename, &info);
		if (result != D3D_OK)
			return result;
		if (width == 0)
			width = info.Width;
		if (height == 0)
			height = info.Height;

		// Create the new texture by loading from file
		result = D3DXCreateTextureFromFileEx(
			device3d,           //3D device
			filename,           //image filename
			width,         //texture width
			height,        //texture height
			1,                  //mip-map levels (1 for no chain)
			0,                  //usage
			D3DFMT_UNKNOWN,     //surface format (default)
			D3DPOOL_DEFAULT,    //memory class for the texture
			D3DX_DEFAULT,       //image filter
			D3DX_DEFAULT,       //mip filter
			transcolor,         //color key for transparency
			&info,              //bitmap file info (from loaded file)
			NULL,               //color palette
			&texture);         //destination texture

	}
	catch (...)
	{
		throw(GameError(GameErrorNS::FATAL_ERROR, "Error in Graphics::loadTexture"));
	}
	return result;
}

ID3DXFont * Graphics::loadDirectXFont(std::string fontName, int fontHeight, bool isItalic)
{
	ID3DXFont* font = nullptr;

	D3DXCreateFont(
		device3d,		
		fontHeight,									
		0,												
		FW_NORMAL,									
		1,												
		isItalic,										
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		ANTIALIASED_QUALITY,
		FF_DONTCARE,
		fontName.c_str(),										
		&font										
	);

	return font;
}

void Graphics::drawText(ID3DXFont* font, std::string text, VECTOR2 position, DWORD format, D3DCOLOR color)
{
	RECT rect;
	SetRect(&rect, (int) position.x, (int) position.y, (int) position.x, (int) position.y);

	font->DrawText(NULL,        //pSprite

		text.c_str(),  //pString

		-1,          //Count

		&rect,  //pRect

		format, //Format single line

		color); //Color
}

void Graphics::drawLine(const VECTOR2* vertices, int count, COLOR_ARGB color)
{
	// default to fail, replace on success
	result = E_FAIL;
	//D3DXCreateLine

}
//=============================================================================
// Display the backbuffer
//=============================================================================
HRESULT Graphics::showBackbuffer()
{
	result = E_FAIL;    // default to fail, replace on success
						// Display backbuffer to screen
	result = device3d->Present(NULL, NULL, NULL, NULL);
	return result;
}

//=============================================================================
// Checks the adapter to see if it is compatible with the BackBuffer height,
// width and refresh rate specified in d3dpp. Fills in the pMode structure with
// the format of the compatible mode, if found.
// Pre: d3dpp is initialized.
// Post: Returns true if compatible mode found and pMode structure is filled.
//       Returns false if no compatible mode found.
//=============================================================================
bool Graphics::isAdapterCompatible()
{
	UINT modes = direct3d->GetAdapterModeCount(D3DADAPTER_DEFAULT,
		d3dpp.BackBufferFormat);
	for (UINT i = 0; i<modes; i++)
	{
		result = direct3d->EnumAdapterModes(D3DADAPTER_DEFAULT,
			d3dpp.BackBufferFormat,
			i, &pMode);
		if (pMode.Height == d3dpp.BackBufferHeight &&
			pMode.Width == d3dpp.BackBufferWidth &&
			pMode.RefreshRate >= d3dpp.FullScreen_RefreshRateInHz)
			return true;
	}
	return false;
}
void Graphics::clear(COLOR_ARGB c)
{
	device3d->Clear(0, NULL, D3DCLEAR_TARGET, c, 1.0F, 0);
}
//=============================================================================
// Draw the sprite described in SpriteData structure
// Color is optional, it is applied like a filter, WHITE is default (no change)
// Pre : sprite->Begin() is called
// Post: sprite->End() is called
// spriteData.rect defines the portion of spriteData.texture to draw
//   spriteData.rect.right must be right edge + 1
//   spriteData.rect.bottom must be bottom edge + 1
//=============================================================================
void Graphics::drawSprite(const SpriteData &spriteData, COLOR_ARGB color)
{
	if (spriteData.texture == NULL)      // if no texture
		return;

	D3DXMATRIX matTransformed;
	D3DXMATRIX matOld;

	VECTOR3 center = VECTOR3(spriteData.width * spriteData.origin.x, spriteData.height * spriteData.origin.y, 0);

	sprite->GetTransform(&matOld);

	D3DXMatrixTransformation2D(
		&matTransformed,
		&spriteData.position,
		0.0f,
		&spriteData.scale,
		&spriteData.position,
		D3DXToRadian(spriteData.rotate),
		0
	);

	//set matrix transformed
	sprite->SetTransform(&matTransformed);	

	// BEGIN
	sprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_DONOTSAVESTATE);
	
	// Draw the sprite
	sprite->Draw(spriteData.texture, &spriteData.rect, &center, &VECTOR3(spriteData.position.x, spriteData.position.y, 0), color);

	// Magic... TO use only this sprite or you can know as to refesh to old
	sprite->SetTransform(&matOld);

	// END
	sprite->End();
}

//=============================================================================
// Test for lost device
//=============================================================================
HRESULT Graphics::getDeviceState()
{
	result = E_FAIL;    // default to fail, replace on success
	if (device3d == NULL)
		return  result;
	result = device3d->TestCooperativeLevel();
	return result;
}

//=============================================================================
// Reset the graphics device
//=============================================================================
HRESULT Graphics::reset()
{
	result = E_FAIL;    // default to fail, replace on success
	initD3Dpp();                        // init D3D presentation parameters
	sprite->OnLostDevice();
	result = device3d->Reset(&d3dpp);   // attempt to reset graphics device

	sprite->OnResetDevice();
	return result;
}

//=============================================================================
// Toggle window or fullscreen mode
// Pre: All user created D3DPOOL_DEFAULT surfaces are freed.
// Post: All user surfaces are recreated.
//=============================================================================
void Graphics::changeDisplayMode(GraphicsNS::DISPLAY_MODE mode)
{
	try {
		switch (mode)
		{
		case GraphicsNS::FULLSCREEN:
			if (fullscreen)      // if already in fullscreen mode
				return;
			fullscreen = true; break;
		case GraphicsNS::WINDOW:
			if (fullscreen == false) // if already in window mode
				return;
			fullscreen = false; break;
		default:        // default to toggle window/fullscreen
			fullscreen = !fullscreen;
		}
		reset();
		if (fullscreen)  // fullscreen
		{
			SetWindowLong(hwnd, GWL_STYLE, WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP);
		}
		else            // windowed
		{
			SetWindowLong(hwnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);
			SetWindowPos(hwnd, HWND_TOP, 0, 0, GAME_WIDTH, GAME_HEIGHT,
				SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);

			// Adjust window size so client area is GAME_WIDTH x GAME_HEIGHT
			RECT clientRect;
			GetClientRect(hwnd, &clientRect);   // get size of client area of window
			MoveWindow(hwnd,
				0,                                           // Left
				0,                                           // Top
				GAME_WIDTH + (GAME_WIDTH - clientRect.right),    // Right
				GAME_HEIGHT + (GAME_HEIGHT - clientRect.bottom), // Bottom
				TRUE);                                       // Repaint the window
		}

	}
	catch (...)
	{
		// An error occured, try windowed mode 
		SetWindowLong(hwnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);
		SetWindowPos(hwnd, HWND_TOP, 0, 0, GAME_WIDTH, GAME_HEIGHT,
			SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);

		// Adjust window size so client area is GAME_WIDTH x GAME_HEIGHT
		RECT clientRect;
		GetClientRect(hwnd, &clientRect);   // get size of client area of window
		MoveWindow(hwnd,
			0,                                           // Left
			0,                                           // Top
			GAME_WIDTH + (GAME_WIDTH - clientRect.right),    // Right
			GAME_HEIGHT + (GAME_HEIGHT - clientRect.bottom), // Bottom
			TRUE);                                       // Repaint the window
	}
}
