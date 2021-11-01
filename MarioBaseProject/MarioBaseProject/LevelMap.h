#pragma once
#ifndef _LEVELMAP_H
#define _LEVELMAP_H

#include "constants.h"
#include <iostream>
#include <vector>

class LevelMap
{
public:
	LevelMap(std::string path);
	~LevelMap();

	int GetTileAt(unsigned int h, unsigned int w);
	void ChangeTileAt(unsigned int row, unsigned int column, unsigned int new_value);

private:
	std::vector<std::vector<int>> ReadFile(std::string path);
	int** m_map;
};

#endif