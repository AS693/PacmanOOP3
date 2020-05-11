#ifndef PACMAN_HH
#define PACMAN_HH
#include"player.hh"
#include"monster.hh"
#include"plate.hh"
#include<math.h>

class Pacman : public Player {
public:
	Pacman() = default;
	explicit Pacman(std::array<float,2> initPos,float speed);
	size_t eat(Plate plateau,std::array<Monster,4> monsterArray);
	void setDirection (char dir);
	char getDirection () const;
	bool isSleeping();
	void awake();
private:
	char direction;
	bool sleep;
};
#endif