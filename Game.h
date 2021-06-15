//https://www.youtube.com/watch?v=YrWQsuDT3NE&list=PLhfAbcv9cehhkG7ZQK0nfIGJC_C-wSLrx&index=4
#pragma once

// Video says that this should be <SDL2/SDL.h> when not on windows...
// https://www.youtube.com/watch?v=lBCVC7ubBzs
#include <SDL2/SDL.h>
#include <iostream>

class Game
{
public:
	Game();
	~Game();

	void init(const char* title, int width, int height, bool fullscreen);

	void ResetGame();

	void handleEvents();
	void update(double deltatime);
	bool running() { return isRunning; }
	void render();
	void DrawDistanceLines();
	void clean();

private:
	bool isRunning = false;
	int cnt = 0;
	SDL_Window *window;
	SDL_Renderer *renderer;

    double OrbitalV(double Gconstant, double orbitdist, double centermass) const;
};