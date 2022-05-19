/*
 * maze_window_logic.cc
 *
 *  Created on: 31 мар. 2022 г.
 *      Author: unyuu
 */

#include "maze_window_logic.h"
#include <cmath>
#include <iostream>
#include <algorithm>

static constexpr double Pi = acos(-1.0);

MazeWindowLogic::MazeWindowLogic()
{
	the_player.spawn(&the_map);
}

void MazeWindowLogic::draw_minimap(SDL_Renderer *r)
{
	SDL_SetRenderDrawColor(r, 255, 255, 255, 255);
	for (int x = 0; x < the_map.width; ++x)
		for (int y = 0; y < the_map.height; ++y) {
			SDL_Rect wr { x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE };
			if (the_map.is_wall(x, y))
				SDL_RenderFillRect(r, &wr);
		}
	SDL_Rect player_rect {
		int(the_player.x * CELL_SIZE) - PLAYER_SIZE/2,
		int(the_player.y * CELL_SIZE) - PLAYER_SIZE/2,
		PLAYER_SIZE, PLAYER_SIZE };
	SDL_SetRenderDrawColor(r, 255, 0, 0, 255);
	SDL_RenderFillRect(r, &player_rect);
	int x1 = int(the_player.x * CELL_SIZE);
	int y1 = int(the_player.y * CELL_SIZE);
	int dx = int(cos(the_player.dir) * CELL_SIZE);
	int dy = int(sin(the_player.dir) * CELL_SIZE);
	int x2 = x1 + dx;
	int y2 = y1 + dy;
	SDL_SetRenderDrawColor(r, 255, 255, 128, 255);
	SDL_RenderDrawLine(r, x1, y1, x2, y2);
}

void MazeWindowLogic::render(SDL_Renderer *r)
{
	// FIXME: Пока так, хотя и не очень хорошее решение
	if (wall == nullptr) {
		wall  = std::make_shared<Texture>(r, "wall.png");
		wall2 = std::make_shared<Texture>(r, "wall2.png");
		plant = std::make_shared<Texture>(r, "plant.png");
	}

	int width, height;
	SDL_GetRendererOutputSize(r, &width, &height);

	SDL_Rect floor_rect { 0, height/2, width, height/2 };
	SDL_Rect sky_rect   { 0, 0,        width, height/2 };

	SDL_SetRenderDrawColor(r, 29, 220, 255, 255);
	SDL_RenderFillRect(r, &sky_rect);

	SDL_SetRenderDrawColor(r, 70, 123, 62, 255);
	SDL_RenderFillRect(r, &floor_rect);

	SDL_SetRenderDrawColor(r, 0, 0, 255, 255);

	double eps = 0.0001;
	double phi = Pi / 2.0;						// Угол обзора
	double ds = width / 2.0 / tan(phi / 2.0);	// Расстояние до экрана
	double px = the_player.x;					// Координаты
	double py = the_player.y;					//     игрока
	double alpha = the_player.dir;				// Направление

	std::vector<double> zbuffer;
	zbuffer.resize(width);

	for (int col = 0; col < width; ++col) {
		double gamma = atan2(col - width/2, ds);// Направление на колонку
		double beta  = alpha + gamma;			// Куда полетит луч
		double rx, ry, dx, dy;					// Координаты точки на луче
		double dv, dh, d; 						// Расстояние до стены
		double sv, sh, s;						// Текстурная координата s
		char   tv, th, t;						// Какая текстура?
		int h; 						// Видимая высота стены в текущей колонке

		// Бросание луча в горизонтальную стену
		if (sin(beta) > eps) {
			dy = 1.0;
			dx = 1.0 / tan(beta);
			ry = floor(py) + eps;
			rx = px - (py - ry) * dx;
			cast_ray(rx, ry, dx, dy);
			dh = hypot(rx - px, ry - py);
			sh = 1.0 - rx + floor(rx);
			th = the_map.what(rx, ry);
		} else if (sin(beta) < -eps) {
			dy = -1.0;
			dx = 1.0 / tan(-beta);
			ry = ceil(py) - eps;
			rx = px - (ry - py) * dx;
			cast_ray(rx, ry, dx, dy);
			dh = hypot(rx - px, ry - py);
			sh = rx - floor(rx);
			th = the_map.what(rx, ry);
		} else {
			dh = INFINITY;
			sh = 0.0;
			th = '#';
		}

		// Бросание луча в вертикальную стену
		if (cos(beta) > eps) {
			dx = 1.0;
			dy = tan(beta);
			rx = floor(px) + eps;
			ry = py - (px - rx) * dy;
			cast_ray(rx, ry, dx, dy);
			dv = hypot(rx - px, ry - py);
			sv = ry - floor(ry);
			tv = the_map.what(rx, ry);
		} else if (cos(beta) < -eps) {
			dx = -1.0;
			dy = tan(-beta);
			rx = ceil(px) - eps;
			ry = py - (rx - px) * dy;
			cast_ray(rx, ry, dx, dy);
			dv = hypot(rx - px, ry - py);
			sv = 1.0 - ry + floor(ry);
			tv = the_map.what(rx, ry);
		} else {
			dv = INFINITY;
			sv = 0.0;
			tv = '#';
		}

		if (dv < dh) {
			d = dv;
			s = sv;
			t = tv;
		} else {
			d = dh;
			s = sh;
			t = th;
		}

		zbuffer[col] = d;

		h = ds * 0.5 / d / cos(gamma);

		if (t == '@')
			wall2->draw(col, h, height, s);
		else
			wall->draw(col, h, height, s);
	}

	struct EntIdx {
		int index;
		double dist;
	};

	std::vector<EntIdx> indices(the_map.entities.size());
	for (unsigned k = 0; k < the_map.entities.size(); ++k) {
		double ex = the_map.entities[k].x;
		double ey = the_map.entities[k].y;
		indices[k].dist = hypot(ey-py, ex-px);
		indices[k].index = k;
	}
	std::sort(std::begin(indices), std::end(indices),
			[&](const EntIdx& a, const EntIdx& b) -> int {
		return a.dist > b.dist;
	});

	for (unsigned k = 0; k < the_map.entities.size(); ++k) {
		int kk = indices[k].index;
		double ex = the_map.entities[kk].x;
		double ey = the_map.entities[kk].y;
		double omega = atan2(ey - py, ex - px);
		double lambda = omega - alpha;
		if (cos(lambda) < eps) continue;

		int col1 = int(width/2 + ds * tan(lambda));
		double d = hypot(ey-py, ex-px);
		double gamma = atan2(col1 - width/2, ds);// Направление на колонку
		int h = ds * 0.5 / d / cos(gamma);
		double aspect = double(plant->width) / double(plant->height);
		int w = h * aspect;
		for (int col2 = col1 - w/2; col2 < col1 + w/2; ++col2) {
			if (col2 < 0 or col2 >= width) continue;
			if (d >= zbuffer[col2]) continue;
			double s = double(col2 - (col1 - w/2)) / w;
			plant->draw(col2, h, height, s);
		}
	}


	draw_minimap(r);
}

void MazeWindowLogic::event(const SDL_Event &event)
{
	switch (event.type) {
	case SDL_KEYDOWN:
		if (event.key.keysym.scancode == SDL_SCANCODE_Q)
			controls.turn_left = true;
		if (event.key.keysym.scancode == SDL_SCANCODE_E)
			controls.turn_right = true;
		if (event.key.keysym.scancode == SDL_SCANCODE_W)
			controls.walk_fwd = true;
		if (event.key.keysym.scancode == SDL_SCANCODE_S)
			controls.walk_back = true;
		if (event.key.keysym.scancode == SDL_SCANCODE_A)
			controls.shift_left = true;
		if (event.key.keysym.scancode == SDL_SCANCODE_D)
			controls.shift_right = true;
		break;
	case SDL_KEYUP:
		if (event.key.keysym.scancode == SDL_SCANCODE_Q)
			controls.turn_left = false;
		if (event.key.keysym.scancode == SDL_SCANCODE_E)
			controls.turn_right = false;
		if (event.key.keysym.scancode == SDL_SCANCODE_W)
			controls.walk_fwd = false;
		if (event.key.keysym.scancode == SDL_SCANCODE_S)
			controls.walk_back = false;
		if (event.key.keysym.scancode == SDL_SCANCODE_A)
			controls.shift_left = false;
		if (event.key.keysym.scancode == SDL_SCANCODE_D)
			controls.shift_right = false;
		break;
	default:
		;
	}
}

void MazeWindowLogic::update()
{
	if (controls.turn_left)   the_player.turn_left(0.05);
	if (controls.turn_right)  the_player.turn_right(0.05);
	if (controls.walk_fwd)    the_player.walk_fwd(0.03);
	if (controls.walk_back)   the_player.walk_back(0.03);
	if (controls.shift_left)  the_player.walk_left(0.03);
	if (controls.shift_right) the_player.walk_right(0.03);
}

void MazeWindowLogic::cast_ray(double &rx, double &ry, double dx, double dy)
{
	do {
		rx += dx;
		ry += dy;
	} while (not the_map.is_wall(rx, ry));
}
