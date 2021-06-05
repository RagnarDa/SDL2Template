//
// Created by Christoffer Wärnbring on 2020-05-02.
//

#ifndef SDLTEST_OBJECT_H
#define SDLTEST_OBJECT_H


#include <string>
#include <SDL.h>
#include "rotmat.h"
//#include "Camera.h"

class Camera;
class Object {
public:
	Object():texture(nullptr),srcrect(),destrect(),orientation(0.0,0.0,0.0){};
	void init(const char *filepath, SDL_Renderer *renderer);
	void update(double dt);
	void render(SDL_Renderer *renderer, Camera camera);
	~Object();

	SDL_Texture* texture;
	bool ownstexture = false;
	SDL_Rect srcrect, destrect;
	double posX=0.0,posY=0.0,posZ=0.0;
	double sizeX=3.0,sizeY=3.0,sizeZ=1.0;
	double movementX=0.0,movementY=0.0,movementZ=0.0;
	//double xrotX=0.0,xrotY=0.0,xrotZ=0.0; // We only care about X axis of rotation right now
	double yaw = 0.0, pitch = 0.0, roll = 0.0; // Radians TODO delete
	rotmat orientation;
	double yawSpeed = 0.0, pitchSpeed = 0.0, rollSpeed = 0.0; // Radians per second
	void TurnRelative(double right, double noseup, double rightwingdown); // Is roll-stabilized
	void TurnRollStabilized(double clockwise, double climb); 

	void init(SDL_Texture *tex);
};


#endif //SDLTEST_OBJECT_H
