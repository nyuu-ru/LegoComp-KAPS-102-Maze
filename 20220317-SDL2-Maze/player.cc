/*
 * player.cc
 *
 *  Created on: 14 апр. 2022 г.
 *      Author: unyuu
 */

#include "player.h"
#include <cmath>
#include <iostream>

static constexpr double Pi = acos(-1.0);

void Player::spawn(Map *where)
{
	the_map = where;
	x   = where->start_x;
	y   = where->start_y;
	dir = where->start_dir;
}

void Player::turn_left(double angle)
{
	dir -= angle;
	if (dir < 0)
		dir += 2*Pi;
}

void Player::turn_right(double angle)
{
	dir += angle;
	if (dir >= 2*Pi)
		dir -= 2*Pi;
}

void Player::walk_fwd(double dist)
{
	double nx, ny, dx, dy;
	dx = dist * cos(dir);
	dy = dist * sin(dir);
	nx = x + dx;
	ny = y + dy;
	if (the_map != nullptr and
			not the_map->is_wall(nx, ny)) {
		x = nx;
		y = ny;
	}
}

void Player::walk_back(double dist)
{
	walk_fwd(-dist);
}

void Player::walk_left(double dist)
{
	walk_right(-dist);
}

void Player::walk_right(double dist)
{
	double nx, ny, dx, dy;
	dx = dist * cos(dir + Pi/2);
	dy = dist * sin(dir + Pi/2);
	nx = x + dx;
	ny = y + dy;
	if (the_map != nullptr and
			not the_map->is_wall(nx, ny)) {
		x = nx;
		y = ny;
	}
}
