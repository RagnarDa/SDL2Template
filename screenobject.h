#ifndef SCREENOBJECT_H
#define SCREENOBJECT_H
#include <SDL.h>
#include "TextureManager.h"
#include "Object.h"

class Camera;

// 2D representation (ie HUD symbol) on screen
class ScreenObject : public Object{
public:
	ScreenObject() :Object() {};
	void update(double dt);
	bool render(SDL_Renderer* renderer, Camera camera);
	double posX = 0.0, posY = 0.0, posZ = 0.0;
	double sizeX = 3.0, sizeY = 3.0, sizeZ = 1.0;
	int screenRelativeWidthPixels, screenRelativeHeightPixels;


};

#endif // SCREENOBJECT_H