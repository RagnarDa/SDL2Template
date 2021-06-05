//
// Created by Christoffer Wärnbring on 2020-05-02.
//

#include <SDL_image.h>
#include <cmath>
#include <iostream>
#include <cassert>
#include "Object.h"
#include "TextureManager.h"
#include "Camera.h"
#ifndef M_PI
#define M_PI 3.14159265359
#endif


std::pair<double,double> GetScreenSize(double sizeX, double sizeY, double distance)
{
	// SOHCAHTOA where opposite is half size of obj
	// and adjacent is distance
	double radX = std::atan((sizeX/1.0)/distance);
	double radY = std::atan((sizeY/1.0)/distance);
	double screenX = (radX/(SCREENFOV/1.0))*1.0;
	double screenY = (radY/(SCREENFOV/1.0))*1.0;
	assert(!std::isnan(screenX));
	assert(!std::isnan(screenY));
	return std::pair<double,double>(screenX,screenY);
}

void Object::TurnRollStabilized(double clockwise, double climb)
{
	// Assume zero roll for now
	//assert(orientation.getRoll() == 0.0);
	orientation = orientation.turnAroundY(clockwise);
	orientation = orientation.turnPitch(climb);
	//	orientation.turn
}
void Object::init(SDL_Texture * tex) {
	texture = tex;
	ownstexture = false;
	// TODO: How to handle double to int conversion? Add 0.5 so it rounds correctly?
	//destrect.w = sizeX;
	//destrect.h = sizeY;
}
void Object::init(const char *filepath, SDL_Renderer *renderer) {
	if (texture)
	{
		SDL_DestroyTexture(texture);
		texture = nullptr;
	}
	init(TextureManager::LoadTexture(filepath,renderer));
	ownstexture = true;
}

void Object::update(double dt) {
	posX += movementX * dt * orientation.getX()[0];
	posY += movementX*dt * orientation.getX()[1];
	posZ += movementX*dt * orientation.getX()[2];
	posX += movementY * dt * orientation.getY()[0];
	posY += movementY * dt * orientation.getY()[1];
	posZ += movementY * dt * orientation.getY()[2];
	posX += movementZ * dt * orientation.getZ()[0];
	posY += movementZ * dt * orientation.getZ()[1];
	posZ += movementZ * dt * orientation.getZ()[2];
	yaw += yawSpeed * dt;
	pitch += pitchSpeed * dt;
	roll += rollSpeed * dt;
//	const double deltaZ = SCREENWIDTH * dt;
//	destrect.w -= deltaZ;
//	destrect.h -= deltaZ;
//	destrect.x += (deltaZ/1.0);
//	destrect.y += (deltaZ/1.0);
}

void Object::render(SDL_Renderer *renderer, Camera camera) {
	// Assume camera is pointed straight ahead right now
	//    o
	// o-----x
	//  \    |
	//   \   |
	//  h \  | a
	//     \ |
	//      \|
	//       c
	// So we calculate radian offset in X and Y
	double diffX = posX-camera.posX; // NOTE: X is actually depth! Aircraft friendly orientation system!
	double diffY = posY-camera.posY;
	double diffZ = posZ-camera.posZ;
	
	// SOHCAHTOA
	// Here we convert our orientation so X is right and Y is up
	double diffXrad = std::atan(diffZ/diffX)-camera.yaw;
	double diffYrad = std::atan(diffY/diffX)-camera.pitch;
	double screenX = (diffXrad/(SCREENFOV/1.0))*1.0;
	double screenY = (diffYrad/(SCREENFOV/1.0))*1.0;
	double screenXpixels = screenX*SCREENWIDTH;
	double screenYpixels = screenY*SCREENWIDTH;
	double distancetocamera = std::sqrt((diffZ*diffZ)+(diffY*diffY)+(diffX*diffX));
	auto screensize = GetScreenSize(this->sizeX,this->sizeY,distancetocamera);
	double screenRelativeWidthPixels = screensize.first*SCREENWIDTH;
	double screenRelativeHeightPixels = screensize.second*SCREENWIDTH;
	// Dont allow texture to go below 4 pixel in size
	const double minimumscreensize = 4.0;
	if (screenRelativeWidthPixels < minimumscreensize)
	{
		screenRelativeWidthPixels = minimumscreensize;
	}
	if (screenRelativeHeightPixels < minimumscreensize)
	{
		screenRelativeHeightPixels = minimumscreensize;
	}
	destrect.w = screenRelativeWidthPixels;
	destrect.h = screenRelativeHeightPixels;
	const double screenCenterX = 0.5 + (SCREENWIDTH / 2.0) - (screenRelativeWidthPixels / 2.0);
	const double screenCenterY = 0.5 + (SCREENHEIGHT / 2.0) - (screenRelativeHeightPixels / 2.0);
	destrect.x = screenCenterX + screenXpixels;
	destrect.y = screenCenterY - screenYpixels;
//	SDL_RenderCopy(renderer, texture, &srcrect, &destrect);
	SDL_Point center;
	// What is the center point relative to? Original textue, source rectange or dest rect???
	// I should just set it to destrect.h/2 and destrect.w/2 = NULL
	center.x = -5;
	center.y = 0;
	SDL_RenderCopyEx(renderer,
	                     texture,
	                     &srcrect,
	                     &destrect,
							0, // Degrees
	                     NULL, // NULL=destrect/2
	                     SDL_RendererFlip::SDL_FLIP_NONE // Mirroring
	                     );
}

Object::~Object() {
	if (texture && ownstexture) // Only destroy textures loaded by this object
	{
		SDL_DestroyTexture(texture);
	}
	texture = nullptr;
}

