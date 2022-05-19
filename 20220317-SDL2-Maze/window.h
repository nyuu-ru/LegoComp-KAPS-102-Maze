/*
 * window.h
 *
 *  Created on: 17 мар. 2022 г.
 *      Author: unyuu
 */

#ifndef WINDOW_H_
#define WINDOW_H_

#include <memory>
#include <SDL2/SDL.h>

#include "window_logic.h"

constexpr int WINDOW_WIDTH  = 1920;
constexpr int WINDOW_HEIGHT = 1080;

extern std::shared_ptr<SDL_Window> w;
extern std::shared_ptr<SDL_Renderer> r;
extern std::shared_ptr<WindowLogic> logic;

void create_window();
void create_renderer();
void logic_loop();


#endif /* WINDOW_H_ */
