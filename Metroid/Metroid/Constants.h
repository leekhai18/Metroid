#pragma once
#include <Windows.h>
#include <d3dx9math.h>

//-----------------------------------------------
// Useful macros
//-----------------------------------------------
// Safely delete pointer referenced item
#define SAFE_DELETE(ptr)       { if (ptr) { delete (ptr); (ptr)=NULL; } }
// Safely release pointer referenced item
#define SAFE_RELEASE(ptr)      { if(ptr) { (ptr)->Release(); (ptr)=NULL; } }
// Safely delete pointer referenced array
#define SAFE_DELETE_ARRAY(ptr) { if(ptr) { delete [](ptr); (ptr)=NULL; } }
// Safely call onLostDevice
#define SAFE_ON_LOST_DEVICE(ptr)    { if(ptr) { ptr->onLostDevice(); } }
// Safely call onResetDevice
#define SAFE_ON_RESET_DEVICE(ptr)   { if(ptr) { ptr->onResetDevice(); } }

#define TRANSCOLOR  SETCOLOR_ARGB(0,255,0,255)  // transparent color (magenta)

// Window
const char CLASS_NAME[] = "Metroid";	
const char GAME_TITLE[] = "Metroid";
const bool FULLSCREEN = false;              // windowed or fullscreen
const UINT VIEWPORT_WIDTH = 256;               // width of game in pixels
const UINT VIEWPORT_HEIGHT = 224;               // height of game in pixels
const UINT MAP_WIDTH = 7680;
const UINT MAP_HEIGHT = 4560;

// Game Manger
const double PI = 3.14159265;
const float FRAME_RATE = 61.0f;                // the target frame rate (frames/sec)
const float MIN_FRAME_RATE = 50.0f;             // the minimum frame rate
const float MIN_FRAME_TIME = 1.0f / FRAME_RATE;   // minimum desired time for 1 frame
const float MAX_FRAME_TIME = 1.0f / MIN_FRAME_RATE; // maximum time used in calculations
const float GRAVITY = 6.67428e-11f;             // gravitational constant


// key mappings
// In this game simple constants are used for key mappings. If variables were used
// it would be possible to save and restore key mappings from a data file.
const UCHAR ESC_KEY = VK_ESCAPE;       // escape key
const UCHAR ALT_KEY = VK_MENU;         // Alt key
const UCHAR ENTER_KEY = VK_RETURN;       // Enter key
#define VK_X 0x58
#define VK_Z 0x5A


// Source
const char SOURCE_IMAGE[] = "Resources\\SpriteSheets\\MetroidTexture.png";//character of game
const char SOURCE_JSON[] = "json\\MetroidTexture.json"; //json file of metroid sprite
const char MAP_BRINSTAR_JSON[] = "json\\MapBrinstar.json"; //json file of map brinstar
const char TILESET_IMAGE[] = "Resources\\Tileset\\Tileset.png"; //tileset of map brinstar
const char OBJECT_LAYER_BRINSTAR_JSON[] = "json\\ObjectLayerBrinstar.json"; //object layer json of Brinstar

																			//Rectangle of metroid
struct MetroidRect
{
	float top, left, right, bottom;
	MetroidRect()
	{
	};
	MetroidRect(float top, float bottom, float left, float right)
	{
		this->top = top;
		this->bottom = bottom;
		this->left = left;
		this->right = right;
	};
};


// Element ID
enum eID {
	SAMUS,
	SKREE,
	ZEB,
	WAVER,
	ZOMMER,
	RIO,
	RIPPER,
	ALIENBIG,
	ALIENSMALL,
	BOSSDRAGON,
	BOSSKRAID,
	MARUMARI,
	VARIA,
	MISSILEROCKET,
	LONGBEAM,
	ICEBEAM,
	ENERGYTANK,
	BOMB,
	WALL,
	PORT,
	FIRE,
	GATEBLUE,
	GATERED,
	BRICK,
};

// collision direction
enum CollideDirection
{
	TOP,
	LEFT,
	RIGHT,
	BOTTOM,
	NONE
};

// Element's status
enum eStatus {
	START,
	JUMPING,
	RUNNING,
	ROLLING,
	ENDING,
	STANDING,
	FALLING,
	FOLLOW,
	HITTING,
};

// direction
enum eDirection {
	left = -1,
	right = 1,
};

// Text
enum eFont {
	body,
	header,
	header1,
};

// Brick style
enum BrickStyle
{
	BrickSecretBlue,
	BrickSerectGreen,
	BrickGreen,
	BrickBlue,
	BrickRed
};

// Enemy colors
enum EnemyColors
{
	Yellow,
	Brown,
	Red
};


struct CollisionReturn
{
	// top, left ,right ,bottom
	CollideDirection direction;
	//id of object that main object collide with
	eID idObject;
	//time two object collide
	float entryTime;
	//position of side that object collide. For example, when direction = TOP, it means positionCollision = otherBound.top
	float positionCollision;
};