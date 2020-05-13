#ifndef PLATE_HH
#define PLATE_HH
#include<vector>
#include"tile.hh"
#include"loader.hh"
#include<SFML/Window.hpp>
#include<SFML/Graphics.hpp>

class Plate{
public:
	Plate();
	Tile getTile(size_t x,size_t y) const;
	Tile* getTilePointer(size_t x,size_t y);

	void setTile(Tile t);
	size_t getNbrFood() const;
	void decountFood();
	size_t getLengthRow() const ;
	size_t getLengthCol()const;
	void drawPlate(sf::RenderWindow *window,size_t tileSize);
	void drawAngle(sf::RenderWindow *window,size_t type,const Tile *t,size_t tileSize);
	void drawLine(sf::RenderWindow *window,size_t type,const Tile *t,size_t tileSize);
	void drawCorner(sf::RenderWindow *window,const Tile *t,size_t tileSize);
	void refineWall(sf::RenderWindow *window,const Tile *t,size_t tileSize);
	size_t isAngle(const Tile *t);
	size_t isLine(const Tile *t);

	void Contaminate(Tile*);
	void updateSickness();
private:
	std::size_t lengthRow;
	std::size_t lengthCol;
	std::size_t nbrFood;
	std::vector<std::vector<Tile>> plat;
	std::vector<Tile*> listCont;

	float sickTime = 6.0;

};
#endif