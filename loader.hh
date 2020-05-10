#ifndef LOADER_HH
#define LOADER_HH
#include<fstream>
#include<vector>
#include"tile.hh"

class Loader{
public:
	Loader();
	std::vector<std::vector<Tile>> loadPlate();
	size_t getCountedFood() const;
	size_t getNbrRow() const ;
	size_t getNbrCol()const;
private:
	std::ifstream iFile;
	size_t countedFood;
	size_t nbrRow;
	size_t nbrCol;
};
#endif