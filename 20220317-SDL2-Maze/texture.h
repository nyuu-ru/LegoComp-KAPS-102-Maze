/*
 * texture.h
 *
 *  Created on: 12 мая 2022 г.
 *      Author: unyuu
 */

#ifndef TEXTURE_H_
#define TEXTURE_H_

#include <memory>
#include <SDL2/SDL.h>

struct Texture
{
	SDL_Renderer *the_renderer {};
	std::shared_ptr<SDL_Texture> tex;
	int width {}, height {};

	Texture(SDL_Renderer *r, const char *filename);

	void draw(int col, int h, int hs, double s);
};



#endif /* TEXTURE_H_ */
