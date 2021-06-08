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
    double frictionmod = 0.999999; // Let's pretend there is some friction in vacuum
	posX += movementselfX * dt * orientation.getX()[0] * frictionmod;
	posY += movementselfX * dt * orientation.getX()[1] * frictionmod;
	posZ += movementselfX * dt * orientation.getX()[2] * frictionmod;
	posX += movementselfY * dt * orientation.getY()[0] * frictionmod;
	posY += movementselfY * dt * orientation.getY()[1] * frictionmod;
	posZ += movementselfY * dt * orientation.getY()[2] * frictionmod;
	posX += movementselfZ * dt * orientation.getZ()[0] * frictionmod;
	posY += movementselfZ * dt * orientation.getZ()[1] * frictionmod;
	posZ += movementselfZ * dt * orientation.getZ()[2] * frictionmod;
	posX += movementworldX * dt * frictionmod;
	posY += movementworldY * dt * frictionmod;
	posZ += movementworldZ * dt * frictionmod;
	yaw += yawSpeed * dt;
	pitch += pitchSpeed * dt;
	roll += rollSpeed * dt;
//    rotation += movementrotation * dt;
    orientation = orientation.turnAroundX(movementrotation * dt);
    orientation.CalculateEulerAngles();
    rotation = orientation.getRoll();
    if (rotation > M_PI*2.0)
        rotation -= M_PI*2.0;
//	const double deltaZ = SCREENWIDTH * dt;
//	destrect.w -= deltaZ;
//	destrect.h -= deltaZ;
//	destrect.x += (deltaZ/1.0);
//	destrect.y += (deltaZ/1.0);
}

// Returns false if sprite is outside screen
Object::objectonscreen Object::render(SDL_Renderer *renderer, Camera camera) {
    if (!draw)
        return Object::objectonscreen::outofscreen;
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
	objectonscreen visible = objectonscreen::onscreen;
	double closeedge = SCREENWIDTH/10.0;
	double nearedge = SCREENWIDTH/8.0;
	if (destrect.x < closeedge || destrect.y < closeedge
        || destrect.x > SCREENWIDTH - closeedge || destrect.y > SCREENHEIGHT - closeedge)
	    visible = objectonscreen::outofscreen;
	else if (destrect.x < nearedge || destrect.y < nearedge
	    || destrect.x > SCREENWIDTH - nearedge || destrect.y > SCREENHEIGHT - nearedge)
	    visible = objectonscreen::closetoedge;
//	SDL_RenderCopy(renderer, texture, &srcrect, &destrect);
	// What is the center point relative to? Original textue, source rectange or dest rect???
	// I should just set it to destrect.h/2 and destrect.w/2 = NULL
	SDL_RenderCopyEx(renderer,
	                     texture,
	                     &srcrect,
	                     &destrect,
                     (rotation/M_PI)*180.0, // Degrees
	                     NULL, // NULL=destrect/2
	                     SDL_RendererFlip::SDL_FLIP_NONE // Mirroring
	                     );
	return visible;
}

Object::~Object() {
	if (texture && ownstexture) // Only destroy textures loaded by this object
	{
		SDL_DestroyTexture(texture);
	}
	texture = nullptr;
}

Object::Object() :texture(nullptr),srcrect(),destrect(),orientation(0.0,0.0,0.0){
}

