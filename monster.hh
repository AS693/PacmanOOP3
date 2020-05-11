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
	void chase(const Plate plate,const Tile,const Tile shadowTile,const char direction);
	void scatter(const Plate plate);
	void panic(const Plate plate);
	std::string getName() const;
	void setMode(std::string mo);
	std::string getMode();
	bool eat(const std::array<float,2> pacPos) const;
	void setTarget(std::array<float,2> target);
	char getDisplacement(const Plate plate, const bool outOfHouse) const ;
	float euclidianDistance(const Tile t) const;
	void setLastMove(const char move);
	std::array<float,2> getSpawn();
	void updateTimes(size_t nbr);
	float getTime(const size_t nbr)const;

	char getDirection() const;
private:
	void alignInAisle(const char displacement);
	std::vector<Tile> availableNextTile(const Plate plate,const bool outOfHouse) const ;
	std::array<float,2> spawn;
	std::array<float,2> target;
	std::array<float,3> modeTimes;

	std::string name;
	std::string mode;
	char lastMove;


};
char opposite(char c);
std::array<float,2> differentialArray(std::array<float,2> a1,std::array<float,2> a2,float coefficient);
#endif