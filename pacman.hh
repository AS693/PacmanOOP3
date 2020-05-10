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
	size_t eat(Plate plateau,std::array<Monster,4> monsterArray) const;
	void setDirection (char dir);
	char getDirection () const;
private:
	char direction;
};
#endif