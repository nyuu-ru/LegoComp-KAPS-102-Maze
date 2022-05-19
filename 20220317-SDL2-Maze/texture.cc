/*
 * texture.cc
 *
 *  Created on: 12 мая 2022 г.
 *      Author: unyuu
 */

#include <stdexcept>
#include <SDL2/SDL_image.h>
#include <cmath>
#include "texture.h"

void panic_sdl();

Texture::Texture(SDL_Renderer *r, const char *filename)
{
	tex = std::shared_ptr<SDL_Texture>(
			IMG_LoadTexture(r, filename),
			SDL_DestroyTexture);
	if (tex == nullptr)
		panic_sdl();

	the_renderer = r;
	SDL_QueryTexture(tex.get(), nullptr, nullptr, &width, &height);
}

void Texture::draw(int col, int h, int hs, double s)
{
	SDL_Rect sr { int(floor(s * width)), 0, 1, height };
	SDL_Rect dr { col, hs/2 - h/2, 1, h };
	SDL_RenderCopy(the_renderer, tex.get(), &sr, &dr);
}
