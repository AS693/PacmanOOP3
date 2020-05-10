#ifndef MONSTER_HH
#define MONSTER_HH
#include"player.hh"
#include"plate.hh"
#include"tile.hh"
#include<string>
#include<math.h>

class Monster : public Player {
public:
	Monster() = default;
	explicit Monster(std::array<float,2> initPos,std::string name,float speed);
	void chase(const Plate plate,const Tile,const char direction);
	std::string getName() const;
	void setMode(std::string mo);
	std::string getMode();
	void setTarget(std::array<float,2> target);
	char getDisplacement(const Plate plate, const bool outOfHouse) const ;
	float euclidianDistance(const Tile t) const;
	void setLastMove(const char move);
	std::array<float,2> getSpawn();

	char getDirection() const;
private:
	std::vector<Tile> availableNextTile(const Plate plate,const bool outOfHouse) const ;
	std::array<float,2> spawn;
	std::array<float,2> target;

	std::string name;
	std::string mode;
	char lastMove;

};
char opposite(char c);
#endif