#include <SDL_image.h>
#include <SDL_ttf.h>
#include "SDL2/SDL_mixer.h"
#include "Game.h"
#include "Object.h"
#include "Camera.h"
#include "screenobject.h"
#include "Attractor.h"
#include <cmath>
#ifndef M_PI
#define M_PI 3.14159265359
#endif
#include <cassert>

std::vector<Attractor*> attractors;

Attractor planet(1);
Attractor blackhole(1);
Camera camera;
ScreenObject vviring;
ScreenObject vvifin;
ScreenObject vvilwing;
ScreenObject vvirwing;

SDL_Texture* fonttexture1, * fonttexture2, * fonttexture3, * fonttexture4, * fonttexture5, * fonttexture6, * fonttexture7, * fonttexture8;
SDL_Rect fontrect1, fontrect2, fontrect3, fontrect4, fontrect5, fontrect6, fontrect7, fontrect8;
TTF_Font* font;
Mix_Music* mp3music;
bool showstatustext = true;

const double camerarotspeed = (5.0 / 180.0)*M_PI;
const double robotturnspeed = (0.3 / 180.0)*M_PI;
bool gamerunning = true;
double alpha = ((2.0/180.0)*M_PI);
double robottimer = 0;
bool showtitlescreen = false;
double titlescreentimer = 6.1; // Break in music happens exactly then

// TODO: Move to own class obviously
void DrawCircle(SDL_Renderer * renderer, int32_t centreX, int32_t centreY, int32_t radius)
{
	const int32_t diameter = (radius * 2);

	int32_t x = (radius - 1);
	int32_t y = 0;
	int32_t tx = 1;
	int32_t ty = 1;
	int32_t error = (tx - diameter);

	while (x >= y)
	{
		//  Each of the following renders an octant of the circle
		SDL_RenderDrawPoint(renderer, centreX + x, centreY - y);
		SDL_RenderDrawPoint(renderer, centreX + x, centreY + y);
		SDL_RenderDrawPoint(renderer, centreX - x, centreY - y);
		SDL_RenderDrawPoint(renderer, centreX - x, centreY + y);
		SDL_RenderDrawPoint(renderer, centreX + y, centreY - x);
		SDL_RenderDrawPoint(renderer, centreX + y, centreY + x);
		SDL_RenderDrawPoint(renderer, centreX - y, centreY - x);
		SDL_RenderDrawPoint(renderer, centreX - y, centreY + x);

		if (error <= 0)
		{
			++y;
			error += ty;
			ty += 2;
		}

		if (error > 0)
		{
			--x;
			tx += 2;
			error += (tx - diameter);
		}
	}
}

// TODO: Move
void get_text_and_rect(SDL_Renderer* renderer, int x, int y, char* text,
	TTF_Font* font, SDL_Texture** texture, SDL_Rect* rect) {
	if (*texture)
	{
		SDL_DestroyTexture(*texture);
	}
	int text_width;
	int text_height;
	SDL_Surface* surface;
	SDL_Color textColor = { 255, 255, 255, 0 };

	surface = TTF_RenderText_Solid(font, text, textColor);
	*texture = SDL_CreateTextureFromSurface(renderer, surface);
	text_width = surface->w;
	text_height = surface->h;
	SDL_FreeSurface(surface);
	rect->x = x;
	rect->y = y;
	rect->w = text_width;
	rect->h = text_height;
}

//SDL_Texture * robot_tex;
//SDL_Rect robot_srcRect, robot_destRect;
Game::Game():window(nullptr),renderer(nullptr)
{
    // Inte sa snygg design det har...
    attractors.push_back(&blackhole);
    attractors.push_back(&planet);
    blackhole.pattractors = &attractors;
    planet.pattractors = &attractors;
}

Game::~Game()
{
	// Never gets called...
}

void Game::init(const char* title, int width, int height, bool fullscreen)
{
	int flags = 0;
	
	if (fullscreen)
	{
		flags = SDL_WINDOW_FULLSCREEN;
	}

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer)
		{
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		}

		isRunning = true;
	}

	/* Inint TTF. */
	TTF_Init();
	font = TTF_OpenFont("../JosefinSans-Regular.ttf", 24);
	if (font == NULL) {
		fprintf(stderr, "error: font not found\n");
		exit(EXIT_FAILURE);
	}

	gamerunning = true; // Start paused


	// Music
	bool usemp3 = true;

	int result;

	auto mixinitflags = /*MIX_INIT_OGG | */MIX_INIT_MP3;
	if (mixinitflags != (result = Mix_Init(mixinitflags))) {
		printf("Could not initialize mixer (result: %d).\n", result);
		printf("Mix_Init: %s\n", Mix_GetError());
		//exit(1);
	}
	assert(0 == Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 512));
	mp3music = Mix_LoadMUS("res/music.mp3");
	if (!mp3music) {
		std::cerr << "Failed to load music. " << Mix_GetError() << " Using fallback wav" << std::endl;
		usemp3 = false;
//			exit(1);
	}
	if (usemp3 && Mix_PlayMusic(mp3music, -1) == -1) {
		printf("Mix_PlayMusic: %s\n", Mix_GetError());
		usemp3 = false;
		// well, there's no music, but most games don't break without music...
	}

	IMG_Init(IMG_INIT_PNG) ;
//	SDL_Surface * tmprobot_surface = IMG_Load("planet.png");
//	robot_tex = SDL_CreateTextureFromSurface(renderer,tmprobot_surface);
//	SDL_FreeSurface(tmprobot_surface);
	
	ResetGame();

	
}

int level = 1;
int lives = 3;
void Game::ResetGame()
{
	showtitlescreen = false;
	showstatustext = false;
	char buff[100];
	std::sprintf(buff, "Level %i", level);
	get_text_and_rect(renderer, 0, 0, buff, font, &fonttexture1, &fontrect1);
	std::sprintf(buff, "Lives %i", lives);
	get_text_and_rect(renderer, 0, fontrect1.h, buff, font, &fonttexture2, &fontrect2);
	robottimer = 0.0;
	double planealtitude = 10 * level;
	std::sprintf(buff, "ALT: %.0f", planealtitude);
	get_text_and_rect(renderer, (SCREENWIDTH / 2) - (fontrect1.w/2), SCREENHEIGHT - fontrect1.h, buff, font, &fonttexture4, &fontrect4);
    const int goblinwidth = 704/11;
    const int goblinheight = 320/5;
    int goblinanimx = 0;
    const int goblinanimy = 0;
    // Place black hole in middle of world
    blackhole.srcrect.x = 0;
    blackhole.srcrect.y = 0;
    blackhole.srcrect.h = goblinheight;
    blackhole.srcrect.w = goblinwidth;
	blackhole.init("../goblin.png", renderer);
    blackhole.movementZ = 0.0;
    blackhole.sizeX = 5;
    blackhole.sizeY = 5;
    blackhole.sizeZ = 5;
    blackhole.posX = 0;
    blackhole.posY = 0.0;
    blackhole.posZ = 0.0;

    planet.srcrect.x = goblinwidth;
    planet.srcrect.y = goblinheight;
    planet.srcrect.h = goblinheight;
    planet.srcrect.w = goblinwidth;
    planet.init(blackhole.texture);
    planet.sizeX = 5;
    planet.sizeY = 5;
    planet.sizeZ = 5;
    planet.posX = 0.0;
    planet.posY = 1.0;
    planet.posZ = 1.0;
    planet.movementZ = 1.0;

    // Camera position
	camera.posX = -100.0; // Zoomed out
	camera.posY = 0.0;
	camera.posZ = 0.0;

	camera.movementX = 0.0;
	camera.movementY = -0.0;
	camera.movementZ = 0.0;

	isRunning = true;
	gamerunning = true;
}

void Game::handleEvents()
{
	SDL_Event event;

	SDL_PollEvent(&event);

	switch (event.type)
	{
	case SDL_QUIT :
		isRunning = false;
		//clean();
		break;
		case SDL_KEYDOWN :
			switch (event.key.keysym.scancode)
			{
				case SDL_SCANCODE_Q:
					isRunning = false;
					break;
				case SDL_SCANCODE_R:
					// FIXME: When using this way to pull key state there will sometimes be a delay
					// between when user presses key and something happens.
					ResetGame();
					break;
				case SDL_SCANCODE_D:
//					robot_destRect.x++;
//					planet.TurnRollStabilized(robotturnspeed,0.0);
					break;
				case SDL_SCANCODE_A:
//					robot_destRect.x--;
					//planet.movementX--;
//					planet.TurnRollStabilized(-robotturnspeed,0.0);
					break;
				case SDL_SCANCODE_W:
//					robot_destRect.y--;
					//planet.movementY++;
//					planet.TurnRollStabilized(0.0*M_PI,robotturnspeed);
					break;
				case SDL_SCANCODE_S:
//					robot_destRect.y++;
					//planet.movementY--;
//					planet.TurnRollStabilized(0.0 * M_PI, -robotturnspeed);
					break;
			/*	case SDL_SCANCODE_KP_8:
					if (event.key.repeat == 0)
					{
						camera.pitchSpeed = camerarotspeed;
					}
					break;
				case SDL_SCANCODE_KP_2:
					if (event.key.repeat == 0)
					{
						camera.pitchSpeed = -camerarotspeed;
					}
					break;
				case SDL_SCANCODE_KP_4:
					if (event.key.repeat == 0)
					{
						camera.yawSpeed = -camerarotspeed;
						std::cout << "LEFTGO";
					}
					break;
				case SDL_SCANCODE_KP_6:
					if (event.key.repeat == 0)
					{
						camera.yawSpeed = camerarotspeed;
					}
					break;*/
			}
			break;
		//case SDL_KEYUP:
		//{
		//	switch (event.key.keysym.scancode)
		//	{
		//	case SDL_SCANCODE_KP_8:
		//		camera.pitchSpeed = 0.0;
		//		break;
		//	case SDL_SCANCODE_KP_2:
		//		camera.pitchSpeed = 0.0;
		//		break;
		//	case SDL_SCANCODE_KP_4:
		//		camera.yawSpeed = 0.0;
		//		std::cout << "LEFTSTOP";
		//		break;
		//	case SDL_SCANCODE_KP_6:
		//		camera.yawSpeed = 0.0;
		//		break;
		//	}
		//}
		//break;
	default:
		break;
	}
}

void Game::update(double deltatime)
{
    showtitlescreen = false;
	if (gamerunning) {

	        camera.update(deltatime);
	        blackhole.update(deltatime);
	        planet.update(deltatime);
	} else if(showtitlescreen)
	{
		titlescreentimer -= deltatime;
		if (titlescreentimer <= 0)
		{
			// Show instructions
			get_text_and_rect(renderer, 0, 0, "Level 1", font, &fonttexture1, &fontrect1);
			get_text_and_rect(renderer, 0, fontrect1.y + fontrect1.h, "Lives 3", font, &fonttexture2, &fontrect2);
			get_text_and_rect(renderer, 0, SCREENHEIGHT - fontrect1.h, "IAS: km/h", font, &fonttexture3, &fontrect3);
			get_text_and_rect(renderer, (SCREENWIDTH / 2) - fontrect1.w, SCREENHEIGHT - fontrect1.h, "ALT: m", font, &fonttexture4, &fontrect4);
			get_text_and_rect(renderer, (SCREENWIDTH - fontrect1.w), SCREENHEIGHT - fontrect1.h, "DIST: m", font, &fonttexture5, &fontrect5);
			get_text_and_rect(renderer, (SCREENWIDTH / 2) - fontrect1.w, (SCREENHEIGHT / 2) - fontrect1.h, "Press R to start", font, &fonttexture6, &fontrect6);
			get_text_and_rect(renderer, (SCREENWIDTH / 2) - (fontrect1.w / 1.0), (SCREENHEIGHT / 2) - (fontrect1.h * 2.0), "Press Q to quit", font, &fonttexture7, &fontrect7);
			showtitlescreen = false;
		}
		else {
			// Show title screen
			get_text_and_rect(renderer, (SCREENWIDTH* (titlescreentimer / (5.0 - 1.0)))-(fontrect8.w), (SCREENHEIGHT / 2)-fontrect8.h, "RB05SIM", font, &fonttexture8, &fontrect8);
		}
	}
}

void Game::render()
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);


	DrawDistanceLines();
	if (blackhole.texture) // Texture needs to be loaded first
	{
		blackhole.render(renderer, camera);
		planet.render(renderer, camera);
		//	vviring.render(renderer, camera);
	}


	/* Use TTF textures. */
	if (showtitlescreen)
	{
		SDL_RenderCopy(renderer, fonttexture8, NULL, &fontrect8);
	} else {
	}


	SDL_RenderPresent(renderer);
}

void Game::DrawDistanceLines()
{
	if (camera.posY == 0)
	{
		// Don't draw during titlescreen etc.
		return;
	}
	// Draw horizon
	SDL_RenderDrawLine(renderer, 0.0, SCREENHEIGHT / 2, SCREENWIDTH, SCREENHEIGHT / 2);

	for (size_t d = 0; d < 100; d++)
	{
		// Draw every kilometer?
		double distance = d * 1000.0;
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
		double diffX = distance - camera.posX; // NOTE: X is actually depth! Aircraft friendly orientation system!
		if (diffX > 0 && diffX < 10000)
		{
			double diffY = camera.posY;
			//double diffZ = posZ - camera.posZ;

			// SOHCAHTOA
			// Here we convert our orientation so X is right and Y is up
			//double diffXrad = std::atan(diffZ / diffX) - camera.yaw;
			double diffYrad = std::atan(diffY / diffX) - camera.pitch;
			//double screenX = (diffXrad / (SCREENFOV / 1.0)) * 1.0;
			double screenY = (diffYrad / (SCREENFOV / 1.0)) * 1.0;
			//double screenXpixels = screenX * SCREENWIDTH;
			double screenYpixels = screenY * SCREENWIDTH;
//			double distancetocamera = std::sqrt((diffY * diffY) + (diffX * diffX));
			//auto screensize = GetScreenSize(this->sizeX, this->sizeY, distancetocamera);
			//double screenRelativeWidthPixels = screensize.first * SCREENWIDTH;
			double screenRelativeHeightPixels = 1.0;// screensize.second* SCREENWIDTH;
			// Dont allow texture to go below 4 pixel in size
			//const double minimumscreensize = 4.0;
			//if (screenRelativeWidthPixels < minimumscreensize)
			//{
			//	screenRelativeWidthPixels = minimumscreensize;
			//}
			//if (screenRelativeHeightPixels < minimumscreensize)
			//{
			//	screenRelativeHeightPixels = minimumscreensize;
			//}
		/*	destrect.w = screenRelativeWidthPixels;
			destrect.h = screenRelativeHeightPixels;
			const double screenCenterX = 0.5 + (SCREENWIDTH / 2.0) - (screenRelativeWidthPixels / 2.0);*/
			const double screenCenterY = 0.5 + (SCREENHEIGHT / 2.0) - (screenRelativeHeightPixels / 2.0);
			int pixelY = screenCenterY + screenYpixels;
			if (pixelY < SCREENHEIGHT && pixelY >= 0)
			{
				Uint8 greenfade = 255 - ((diffX / 10000)*255);
//				Uint8 bluefade = 40 - ((diffX / 10000) * 40);
				SDL_SetRenderDrawColor(renderer, 0, greenfade, 0, greenfade);
				SDL_RenderDrawLine(renderer, 0.0, pixelY, SCREENWIDTH, pixelY);
			}
		}
	/*	destrect.x = screenCenterX + screenXpixels;
		destrect.y = screenCenterY - screenYpixels;*/
	}

}

void Game::clean()
{
	// Needs to destroy these so texture is destroyed before renderer... :|
	blackhole.~Attractor();
	planet.~Attractor();
	
	if (renderer) {
		SDL_DestroyRenderer(renderer);
	}
	SDL_DestroyWindow(window);
//	Mix_Quit();
	SDL_Quit();
}