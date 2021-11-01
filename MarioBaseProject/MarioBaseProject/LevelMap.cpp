#include "LevelMap.h"
#include <fstream>

/*
 * when making a map, remember:
 * 0 = empty space,	1 = blocked/occupied space
 */

LevelMap::LevelMap(std::string path)
{
	std::vector<std::vector<int>> map = ReadFile(path);


	//Allocate memory for the level map
	m_map = new int* [map.size()];
	for (unsigned int i = 0; i < map.size(); i++)
	{
		m_map[i] = new int[map[0].size()];
	}

	//populate the array
	for (unsigned int i = 0; i < map.size(); i++)
	{
		for (unsigned int j = 0; j < map[0].size(); j++)
		{
			m_map[i][j] = map[i][j];
		}
	}
}

std::vector<std::vector<int>> LevelMap::ReadFile(std::string path)
{
	// Open File
	std::ifstream inFile;

	inFile.open(path);

	if (!inFile.good())
	{
		std::cerr << "Can't open level map" << std::endl;
	}

	// Set up 2D Vec
	std::vector<std::vector<int>> vecMap;
	std::vector<int> line;

	int newLine = 0;
	int currentNumber = 0;
	int tileWidth = 16;

	// Push back all numbers to a line, once its hit the tileWidth, push line to vecMap and continue
	while (!inFile.eof())
	{
		inFile >> currentNumber;

		line.push_back(currentNumber);
		newLine++;
		if (newLine == tileWidth)
		{
			vecMap.push_back(line);
			line.clear();
			newLine = 0;
		}
	}
	inFile.close();

	return vecMap;
}


int LevelMap::GetTileAt(unsigned int h, unsigned int w)
{
	if (h < MAP_HEIGHT && w < MAP_WIDTH)
	{
		return m_map[h][w];
	}

	return 0;

}

void LevelMap::ChangeTileAt(unsigned int row, unsigned int column, unsigned int new_value)
{
	m_map[row][column] = new_value;
}

LevelMap::~LevelMap()
{
	//delete all elements of the array
	for (unsigned int i = 0; i < MAP_HEIGHT; i++)
	{
		delete[] m_map[i];
	}
	delete[]m_map;
}