#pragma once
#include "MapLoader.h"

#include <map>

struct Cell {
    Cell(unsigned int x2, unsigned int y2) : x(x2), y(y2) {};
    unsigned int x;
    unsigned int y;
    
};

class DynamicMap {

public:
	DynamicMap(std::vector<Region>& r, unsigned int nbR, unsigned int nbRV, unsigned int nbL, unsigned int nbC);
	virtual ~DynamicMap();
	void LoadMap();
	void PrintGrid();
	bool IsErasable(unsigned int numRegion);
	std::set<unsigned int> GetNeighbours(unsigned int** copieGrille, unsigned int numRegion);
	int GetMinNeighbours(unsigned int** copieGrille, unsigned int numRegion);
	Cell* GetFreeCell(unsigned int x, unsigned int y);

private:
	unsigned int** Grid;
	Regions& regions;
	unsigned int NbRegions;
	unsigned int NbRegionsEmpty;
	unsigned int NbLines;
	unsigned int NbColumns;
	std::map<unsigned int, Region> RegionsMap;

};