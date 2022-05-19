/*
 * map.cc
 *
 *  Created on: 7 апр. 2022 г.
 *      Author: unyuu
 */

#include "map.h"
#include <fstream>
#include <cmath>
#include <sstream>
#include <stdexcept>

Map::Map(const char *file)
{
	std::ifstream in { file };
	if (not in.good())
		throw std::runtime_error("Не могу открыть карту!");

	std::string line;
	std::getline(in, line);
	std::stringstream(line) >> width >> height >>
			start_x >> start_y >> start_dir;

	data.resize(height);
	for (auto &&s: data) std::getline(in, s);

	int count;
	in >> count;
	while (count--) {
		double ex, ey;
		int et;
		in >> ex >> ey >> et;
		entities.push_back(Entity(ex, ey, et));
	}
}

bool Map::is_wall(int x, int y)
{
	if (x < 0 or x >= width or y < 0 or y >= height) return true;
	return data[y][x] != ' ';
}

bool Map::is_wall(double x, double y)
{
	return is_wall(int(floor(x)), int(floor(y)));
}

char Map::what(int x, int y)
{
	if (x < 0 or x >= width or y < 0 or y >= height) return '#';
	return data[y][x];
}

char Map::what(double x, double y)
{
	return what(int(floor(x)), int(floor(y)));
}
