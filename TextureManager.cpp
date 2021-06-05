//
// Created by Christoffer Wärnbring on 2020-05-02.
//

#include <SDL_image.h>
#include <iostream>
#include "TextureManager.h"
//#include <experimental/filesystem>
SDL_Texture *TextureManager::LoadTexture(const char *filepath, SDL_Renderer *renderer) {
	std::cout << "Loading " << filepath << std::endl;

		SDL_Surface *tmp_surface;
		tmp_surface = IMG_Load(filepath);
		if (!tmp_surface)
		{
			std::string str;
			str.append("Couldnt load texture file: ");
//			str.append(std::filesystem::current_path());
			str.append(filepath);
			throw std::invalid_argument(str);
		}
		SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, tmp_surface);
		SDL_FreeSurface(tmp_surface);

		return texture;
}
