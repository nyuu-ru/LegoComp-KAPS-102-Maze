/*
 * main.cc
 *
 *  Created on: 17 мар. 2022 г.
 *      Author: unyuu
 */

/*
 * Библиотеки:
 * 		Для MinGW:
 *
 * 			mingw32
 * 			SDL2main
 * 			SDL2
 * 			SDL2_image
 */

#include <iostream>
#include <memory>
#include <chrono>
#include <thread>

#include <SDL2/SDL.h>
#include <SDL2/SDL_main.h>

#include "window.h"
#include "maze_window_logic.h"

void panic_sdl()
{
	std::cerr << "Произошла ошибка: " << SDL_GetError() << std::endl;
	exit(1);
}


int main(int, char **)
{
	SDL_Init(SDL_INIT_EVERYTHING);

	create_window();
	create_renderer();

	logic = std::make_shared<MazeWindowLogic>();

	logic_loop();

	return 0;
}


