#include "DynamicMapGenerator.h"

#include <iostream>
#include <stack>
#include <map>
#include <ctime>
#include <algorithm>

/**
Constructeur :
First attribute r: the regions representing the game map
nbR: number of filled regions,
nbRV = number of empty areas
nbL = number of lines
nbC = number of columns of the 2d game grid.
*/
DynamicMap::DynamicMap(std::vector<Region>& r, unsigned int nbR, unsigned int nbRV, unsigned int nbL, unsigned int nbC) : regions(r), NbRegions(nbR), NbRegionsEmpty(nbRV), NbLines(nbL), NbColumns(nbC)
{
	regions.clear();
	// creation of a nb rows and nb columns matrix in parameter, representing the game grid:
	Grid = new unsigned int* [NbLines];
	for (unsigned int i = 0; i < NbLines; i++) {
		Grid[i] = new unsigned int[NbColumns];
	}

	// initialize cells to 0 == no cell belongs to a region yet
	for (unsigned int i = 0; i < NbLines; i++) {
		for (unsigned int j = 0; j < NbColumns; j++) {
			Grid[i][j] = 0;
		}
	}
}

DynamicMap::~DynamicMap()
{
	for (unsigned int i = 0; i < NbLines; i++) {
		delete[] Grid[i];
	}
	delete[] Grid;
}

/**
Loads the map into the "regions" attribute from
of the different criteria initialized in the constructor.
*/
void DynamicMap::LoadMap()
{
	// we try to put the index (0 to n-1) in each cell of the table, to associate a region with several boxes
	for (unsigned int i = 0; i < NbRegions + NbRegionsEmpty; i++) {
		Region v;
		regions.push_back(v);
	}
	
	// real random
	srand(static_cast<unsigned int>(time(NULL)));

	// setting the first cell of each region
	unsigned int currRegion = 0, randI, randJ;
	while (currRegion < (NbRegions + NbRegionsEmpty))
	{
		// we are looking for a random cell that is not a region
		randI = rand() % NbLines;
		randJ = rand() % NbColumns;
		if (Grid[randI][randJ] == 0) // if zero then is not a region (region represented by an id from 1 to NbRegions)
		{
			regions[currRegion].push_back({ randI, randJ });
			Grid[randI][randJ] = currRegion + 1;
			currRegion++;
		}
	}

	// to loop as long as we haven't filled the table: there is already the first box of each region, so NbRegions(+empty) elements
	unsigned int nbAddedElements = NbRegions + NbRegionsEmpty;

	while (nbAddedElements < NbLines * NbColumns) {
		// for each region
		for (unsigned int i = 0; nbAddedElements < (NbLines * NbColumns) && i < (NbRegions + NbRegionsEmpty); i++) {

			//we are looking for a neighbor of the last cell added to the region
			Cell* voisin = nullptr;
			Region::iterator it = regions[i].begin();

			// we iterate until we find a cell with a neighbor
			while (voisin == nullptr && it != regions[i].end()) {
				voisin = GetFreeCell(it->first, it->second);
				++it;
			}

			if (voisin != nullptr) {
				Grid[voisin->x][voisin->y] = i + 1;
				regions[i].push_back({ voisin->x, voisin->y }); // added cell for region

				nbAddedElements++; // incrementing the number of elements in the array

				delete voisin; // cell memory release
			}
		}


	}
	// using a dictionary to store regions and removing empty regions in the "regions regions" attribute
	for (unsigned int i = 0; i < NbRegions + NbRegionsEmpty; i++) {
		RegionsMap[i] = regions[i];
	}

	// We remove NbRegionsEmpty (given as a parameter) from the created regions:
	unsigned int regionNumberToDelete = -1;
	std::set<unsigned int> regionsToDelete;
	unsigned int tentative;
	for (unsigned int i = 0; i < NbRegionsEmpty; i++) {
		regionNumberToDelete = 1 + rand() % (NbRegions + NbRegionsEmpty);
		tentative = 0;
		while ((!IsErasable(regionNumberToDelete) || regionsToDelete.find(regionNumberToDelete) != regionsToDelete.end()) && tentative < (NbRegions + NbRegionsEmpty)) {
			regionNumberToDelete = 1 + rand() % (NbRegions + NbRegionsEmpty);
			tentative++;
		}
		if (tentative >= (NbRegions + NbRegionsEmpty)) {
			std::cout << "Trop de tentatives pour trouver une région permettant de garder une seule composante connexe." << std::endl;
			std::cout << "Seules " << i << " régions vides seront dans la génération de la carte au lieu de " << NbRegionsEmpty << "." << std::endl;
			break;
		}
		else {
			regionsToDelete.insert(regionNumberToDelete);
			std::vector<Region>::iterator position = std::find(regions.begin(), regions.end(), RegionsMap[regionNumberToDelete - 1]);
			if (position != regions.end()) // if we find it
				regions.erase(position); //delete this region
			for (unsigned int line = 0; line < NbLines; line++) {
				for (unsigned int col = 0; col < NbColumns; col++) {
					if (Grid[line][col] == regionNumberToDelete) {
						// we update the grid
						Grid[line][col] = 0; //this region becomes an empty zone
					}
				}
			}
			std::cout << std::endl;
		}
	}

	PrintGrid();
}

/**
Returns an available & neighbor cell of Grille[r][c]
Else if no free cell: returns nullptr
*/
Cell* DynamicMap::GetFreeCell(unsigned int r, unsigned int c)
{
	if (r % 2 == 0) // r pair
	{
		if (r > 0) {
			if (Grid[r - 1][c] == 0)
				return new Cell(r - 1, c);
			if (c > 0) {
				if (Grid[r - 1][c - 1] == 0)
					return new Cell(r - 1, c - 1);
			}
		}

		if (c > 0) {
			if (Grid[r][c - 1] == 0)
				return new Cell(r, c - 1);
		}
		if (c < (NbColumns - 1)) {
			if (Grid[r][c + 1] == 0)
				return new Cell(r, c + 1);
		}
		if (r < (NbLines - 1)) {
			if (Grid[r + 1][c] == 0)
				return new Cell(r + 1, c);
			if (c > 0) {
				if (Grid[r + 1][c - 1] == 0)
					return new Cell(r + 1, c - 1);
			}
		}
	}
	else // r odd
	{
		if (r > 0) {
			if (Grid[r - 1][c] == 0)
				return new Cell(r - 1, c);
			if (c < (NbColumns - 1)) {
				if (Grid[r - 1][c + 1] == 0)
					return new Cell(r - 1, c + 1);
			}
		}
		if (c > 0) {
			if (Grid[r][c - 1] == 0)
				return new Cell(r, c - 1);
		}
		if (c < (NbColumns - 1)) {
			if (Grid[r][c + 1] == 0)
				return new Cell(r, c + 1);
		}
		if (r < (NbLines - 1)) {
			if (Grid[r + 1][c] == 0)
				return new Cell(r + 1, c);
			if (c < (NbColumns - 1)) {
				if (Grid[r + 1][c + 1] == 0)
					return new Cell(r + 1, c + 1);
			}
		}
	}
	return nullptr; // no neighbor found
}

/**
Display of the game grid representing the different regions
*/
void DynamicMap::PrintGrid()
{
	std::cout << "Affichage de la Grille du jeu : " << std::endl;
	for (unsigned int i = 0; i < NbLines; i++) {
		for (unsigned int j = 0; j < NbColumns; j++) {
			std::cout << Grid[i][j] << " ";
		}
		std::cout << std::endl;
	}
}

/**
For a region, the function returns whether the region is erasable:
  i.e., if once this region is removed all of the
  remaining regions always form a connected component
  then the function returns true, returns false otherwise.
*/
bool DynamicMap::IsErasable(unsigned int numRegion)
{
	// we copy the game grid
	unsigned int** copyGrid = new unsigned int* [NbLines];
	for (unsigned int i = 0; i < NbLines; i++) {
		copyGrid[i] = new unsigned int[NbColumns];
	}
	// and we replace the "numRegion" region with zeros so as not to take it into account in the algo to find the régions voisines
	bool exists = false;
	for (unsigned int i = 0; i < NbLines; i++) {
		for (unsigned int j = 0; j < NbColumns; j++) {
			copyGrid[i][j] = Grid[i][j];
			if (copyGrid[i][j] == numRegion) {
				copyGrid[i][j] = 0;
				exists = true;
			}
		}
	}
	if (!exists) {
		return false;
	}
	int regionsVoisines = GetMinNeighbours(copyGrid, numRegion);

	// freeing the memory of the copy of the grid:
	for (unsigned int i = 0; i < NbLines; i++) {
		delete[] copyGrid[i];
	}
	delete[] copyGrid;

	// a region is deletable if the regions form a single connected component
	// so if there is the same number of regions-1: because the region numRegion will be deleted and not taken into account:
	return int(regions.size() - 1) == regionsVoisines;
}

/**
For a region, the function returns all of its neighboring regions (in number).
*/
std::set<unsigned int> DynamicMap::GetNeighbours(unsigned int** copieGrille, unsigned int numRegion)
{
	Region& region = RegionsMap[numRegion - 1];
	std::set<unsigned int> numRegionsVoisines;
	unsigned int r, c;
	for (auto& celluleR1 : region) {
		r = celluleR1.first;
		c = celluleR1.second;

		if (r % 2 == 0) // r pair
		{
			if (r > 0) {
				if (copieGrille[r - 1][c] != 0 && copieGrille[r - 1][c] != numRegion)
					numRegionsVoisines.insert(copieGrille[r - 1][c]);
				if (c > 0) {
					if (copieGrille[r - 1][c - 1] != 0 && copieGrille[r - 1][c - 1] != numRegion)
						numRegionsVoisines.insert(copieGrille[r - 1][c - 1]);
				}
			}

			if (c > 0) {
				if (copieGrille[r][c - 1] != 0 && copieGrille[r][c - 1] != numRegion)
					numRegionsVoisines.insert(copieGrille[r][c - 1]);
			}
			if (c < (NbColumns - 1)) {
				if (copieGrille[r][c + 1] != 0 && copieGrille[r][c + 1] != numRegion)
					numRegionsVoisines.insert(copieGrille[r][c + 1]);
			}
			if (r < (NbLines - 1)) {
				if (copieGrille[r + 1][c] != 0 && copieGrille[r + 1][c] != numRegion)
					numRegionsVoisines.insert(copieGrille[r + 1][c]);
				if (c > 0) {
					if (copieGrille[r + 1][c - 1] != 0 && copieGrille[r + 1][c - 1] != numRegion)
						numRegionsVoisines.insert(copieGrille[r + 1][c - 1]);
				}
			}
		}
		else // r odd
		{
			if (r > 0) {
				if (copieGrille[r - 1][c] != 0 && copieGrille[r - 1][c] != numRegion)
					numRegionsVoisines.insert(copieGrille[r - 1][c]);
				if (c < (NbColumns - 1)) {
					if (copieGrille[r - 1][c + 1] != 0 && copieGrille[r - 1][c + 1] != numRegion)
						numRegionsVoisines.insert(copieGrille[r - 1][c + 1]);
				}
			}
			if (c > 0) {
				if (copieGrille[r][c - 1] != 0 && copieGrille[r][c - 1] != numRegion)
					numRegionsVoisines.insert(copieGrille[r][c - 1]);
			}
			if (c < (NbColumns - 1)) {
				if (copieGrille[r][c + 1] != 0 && copieGrille[r][c + 1] != numRegion)
					numRegionsVoisines.insert(copieGrille[r][c + 1]);
			}
			if (r < (NbLines - 1)) {
				if (copieGrille[r + 1][c] != 0 && copieGrille[r + 1][c] != numRegion)
					numRegionsVoisines.insert(copieGrille[r + 1][c]);
				if (c < (NbColumns - 1)) {
					if (copieGrille[r + 1][c + 1] != 0 && copieGrille[r + 1][c + 1] != numRegion)
						numRegionsVoisines.insert(copieGrille[r + 1][c + 1]);
				}
			}
		}

	}
	return numRegionsVoisines;
}

/**
For a region, the function looks for each of its neighboring regions:
- the number of regions accessible from this neighboring region
  Among these numbers, the function returns the smallest
  to confirm that each region allows access to each of the others (related)
*/
int DynamicMap::GetMinNeighbours(unsigned int** copieGrille, unsigned int numRegion)
{
	std::set<unsigned int> voisinsTotaux = GetNeighbours(copieGrille, numRegion);
	int minVoisinsDuVoisin = -1;
	for (auto& voisin : voisinsTotaux) {
		std::stack<unsigned int> pile;
		pile.push(voisin);
		std::set<unsigned int> voisinsDuVoisinCourant;
		voisinsDuVoisinCourant.insert(voisin);

		std::set<unsigned int> alreadySeen;
		while (!pile.empty()) {
			unsigned int numRegionDepilated = pile.top();
			pile.pop();
			if (alreadySeen.find(numRegionDepilated) != alreadySeen.end()) { // the neighbors of this region have already been added.

			}
			else { // we add it and we get its neighbors
				alreadySeen.insert(numRegionDepilated);
				std::set<unsigned int> voisinsRegionDepilee = GetNeighbours(copieGrille, numRegionDepilated);

				voisinsDuVoisinCourant.insert(voisinsRegionDepilee.begin(), voisinsRegionDepilee.end());
				for (auto& numRegion : voisinsRegionDepilee) {
					pile.push(numRegion);
				}
			}
		}

		if (minVoisinsDuVoisin == -1 || int(voisinsDuVoisinCourant.size()) < minVoisinsDuVoisin) {
			minVoisinsDuVoisin = static_cast<int>(voisinsDuVoisinCourant.size());
		}
	}

	return minVoisinsDuVoisin;
}
