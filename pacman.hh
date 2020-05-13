#ifndef PACMAN_HH
#define PACMAN_HH
#include"player.hh"
#include"monster.hh"
#include"plate.hh"
#include<math.h>

class Pacman : public Player {
public:
	Pacman() = default;
	explicit Pacman(std::array<float,2> initPos,float speed,sf::Color color);
	size_t eat(Plate plateau,std::vector<Monster> monsterArray);
	void setDirection (char dir);
	char getDirection () const;
	bool isSleeping();
	void awake();
private:
	char direction;
	bool sleep;
};
#endif