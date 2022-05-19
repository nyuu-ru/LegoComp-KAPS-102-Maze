/*
 * maze_window_logic.h
 *
 *  Created on: 31 мар. 2022 г.
 *      Author: unyuu
 */

#ifndef MAZE_WINDOW_LOGIC_H_
#define MAZE_WINDOW_LOGIC_H_

#include "window.h"
#include "window_logic.h"
#include "map.h"
#include "player.h"
#include "texture.h"

struct MazeWindowLogic : WindowLogic
{
	Map the_map { "map1.txt" };
	Player the_player;
	struct {
		bool turn_left   { false };
		bool turn_right  { false };
		bool walk_fwd    { false };
		bool walk_back   { false };
		bool shift_left  { false };
		bool shift_right { false };
	} controls;
	std::shared_ptr<Texture> wall;
	std::shared_ptr<Texture> wall2;
	std::shared_ptr<Texture> plant;

	MazeWindowLogic();

	virtual void render(SDL_Renderer *r);


	static constexpr int CELL_SIZE = 30;
	static constexpr int PLAYER_SIZE = 9;

	virtual void event(const SDL_Event &event);
	virtual void update();

private:
	void draw_minimap(SDL_Renderer *r);
	void cast_ray(double & rx, double & ry, double dx, double dy);
};




#endif /* MAZE_WINDOW_LOGIC_H_ */
