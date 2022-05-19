/*
 * player.h
 *
 *  Created on: 14 апр. 2022 г.
 *      Author: unyuu
 */

#ifndef PLAYER_H_
#define PLAYER_H_

#include "map.h"

struct Player
{
	Map * the_map { nullptr };
	double x { 0.0 }, y { 0.0 }, dir { 0.0 };

	void spawn(Map * where);

	void turn_left(double angle);
	void turn_right(double angle);
	void walk_fwd(double dist);
	void walk_back(double dist);
	void walk_left(double dist);
	void walk_right(double dist);
};


#endif /* PLAYER_H_ */
