//
// Created by Christoffer Wärnbring on 2020-05-02.
//

#ifndef SDLTEST_TEXTUREMANAGER_H
#define SDLTEST_TEXTUREMANAGER_H

#include <SDL.h>

class TextureManager {
public:
	static SDL_Texture * LoadTexture(const char * filepath, SDL_Renderer * renderer);
};


#endif //SDLTEST_TEXTUREMANAGER_H
