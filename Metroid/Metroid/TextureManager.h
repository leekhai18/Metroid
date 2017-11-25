#pragma once
#include "Graphics.h"
#include "Constants.h"

class TextureManager
{
	// TextureManager properties
private:
	UINT       width;       // width of texture in pixels
	UINT       height;      // height of texture in pixels
	LP_TEXTURE texture;     // pointer to texture
	const char *file;       // name of file
	Graphics *graphics;     // save pointer to graphics
	bool    initialized;    // true when successfully initialized
	HRESULT hr;             // standard return type

public:
	// Constructor
	TextureManager();

	// Destructor
	virtual ~TextureManager();

	// Returns a pointer to the texture
	LP_TEXTURE getTexture() const { return texture; }

	// Returns the texture width
	UINT getWidth() const { return width; }

	// Return the texture height
	UINT getHeight() const { return height; }

	// Initialize the textureManager
	// Pre: *g points to Graphics object
	//      *file points to name of texture file to load
	// Post: The texture file is loaded
	virtual bool initialize(Graphics *g, const char *file);

	// Release resources
	virtual void onLostDevice();

	// Restore resourses
	virtual void onResetDevice();
};

