/*
 * window.cc
 *
 *  Created on: 17 мар. 2022 г.
 *      Author: unyuu
 */

#include <memory>
#include <SDL2/SDL.h>
#include <chrono>
#include <thread>

#include "window.h"

std::shared_ptr<SDL_Window> w;
std::shared_ptr<SDL_Renderer> r;
std::shared_ptr<WindowLogic> logic;


void panic_sdl(); // реально лежит в main.cc

void create_window()
{
	w = std::shared_ptr<SDL_Window>(
			SDL_CreateWindow("Maze Window", SDL_WINDOWPOS_CENTERED,
					SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0),
			SDL_DestroyWindow);
	if (w == nullptr)
		panic_sdl();
}

void create_renderer()
{
	r = std::shared_ptr<SDL_Renderer>(
			SDL_CreateRenderer(w.get(), -1,
					SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC),
			SDL_DestroyRenderer);
	if (r == nullptr)
		panic_sdl();
}

void logic_loop()
{
	bool want_quit = false;

	std::thread logic_thread([&]() {
		while (not want_quit) {
			logic->update();
			std::this_thread::sleep_for(std::chrono::milliseconds(1000)/60);
		}
	});

	SDL_Event event;
	for (;;) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				want_quit = true;
				logic_thread.join();
				exit(0);
			}
			logic->event(event);
		}
		logic->render(r.get());
		SDL_RenderPresent(r.get());
	}
}

