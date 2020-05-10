#ifndef TILE_HH
#define TILE_HH
#include<cstddef>
#include<array>

class Tile{
public:
	explicit Tile(size_t ty,bool wa,const std::array<size_t,2> pos);
	Tile() = default;
	//Récupère le type d'élément sur la case
	//Les types ont différentes significations si mur ou cellule
	//Pour une cellule, défini si elle est vide ou quelle type de nourriture s'y situe
	void setType(size_t ty);
	void setWall(bool wa);
	size_t getType() const;
	bool isPlayable() const;
	bool isEmpty()const;
	bool isTunnel() const;
	bool isFood()const;
	bool isPilz()const;
	bool isWall() const;
	bool isOut() const;
	bool isFantomHouse() const;
	void operator =(Tile const &tile);
	bool operator !=(Tile const &tile);
	bool operator ==(Tile const &tile);
	size_t getX() const;
	size_t getY() const;
private:
	std::array<size_t,2> position;
	size_t type;
	bool wall;
};
#endif