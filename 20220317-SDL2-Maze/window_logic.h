/*
 * window_logic.h
 *
 *  Created on: 31 мар. 2022 г.
 *      Author: unyuu
 */

#ifndef WINDOW_LOGIC_H_
#define WINDOW_LOGIC_H_

#include <SDL2/SDL.h>

struct WindowLogic
{
	WindowLogic() { }
	virtual ~WindowLogic() { }

	virtual void update()                      { }
	virtual void event(const SDL_Event &event) { }
	virtual void render(SDL_Renderer * r)      { }
};


#endif /* WINDOW_LOGIC_H_ */
