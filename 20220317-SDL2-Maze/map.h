/*
 * map.h
 *
 *  Created on: 7 апр. 2022 г.
 *      Author: unyuu
 */

#ifndef MAP_H_
#define MAP_H_

#include <vector>
#include <string>

struct Entity
{
	double x, y;
	int type;
	Entity(double ex, double ey, int et): x(ex), y(ey), type(et) {}
};

struct Map
{
	int width {}, height {};
	double start_x {}, start_y {}, start_dir {};
	std::vector<std::string> data;
	std::vector<Entity> entities;

	Map(const char *file); // Это конструктор. Он будет загружать карту.
	bool is_wall(int x, int y);
	bool is_wall(double x, double y);

	char what(int x, int y);
	char what(double x, double y);
};



#endif /* MAP_H_ */
