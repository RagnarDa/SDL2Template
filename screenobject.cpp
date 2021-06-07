#include "screenobject.h"
#include "Object.h"
#include "Camera.h"

bool ScreenObject::render(SDL_Renderer* renderer, Camera camera)
{
	destrect.w = screenRelativeWidthPixels;
	destrect.h = screenRelativeHeightPixels;
	const double screenCenterX = 0.5 + (SCREENWIDTH / 2.0) - (screenRelativeWidthPixels / 2.0);
	const double screenCenterY = 0.5 + (SCREENHEIGHT / 2.0) - (screenRelativeHeightPixels / 2.0);
	destrect.x = posX;
	destrect.y = posY;
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
