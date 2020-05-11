#include"pacman.hh"

Pacman::Pacman(std::array<float,2> initPos,float speed){
	setType('P');
	setPosition(initPos);
	setSpeedInit(speed);
	setHide(false);
	hiddenTime = INFINITY;
	hiddenClock = clock();
}
size_t Pacman::eat(Plate plateau,std::array<Monster,4> monsterArray) const{

	//place une tolérancesur le centre de la case car vu qu'on est en point flotant,
	//il peut exister une petite différence dans les positions.
	//bouffe

	if(abs(getX()-((size_t)getX() +0.5)) <0.1 || abs(getY()-((size_t)getY()+0.5)) <0.1){
			
		if(plateau.getTile((size_t)getX(),(size_t)getY()).isFood()){
			return 10;
		}

		else if(plateau.getTile((size_t)getX(),(size_t)getY()).isPilz())
			return 50;

	}
	//Monstres
	for(auto i=monsterArray.cbegin();i!=monsterArray.cend();i++){
		if(abs(getX()-i->getX()) <0.1 || abs(getY()-i->getY()) <0.1)
			return 100;
	}

	return 0;
}
void Pacman::setDirection(char dir){
	direction = dir;

	std::array<float,2> pos = {getY(), getX()};

	if(dir == 'u' || 'd')
		pos[0] = (float) ((size_t) pos[0] + 0.1);
	else
		pos[1] = (float) ((size_t) pos[1] + 0.1);

	//setPosition(pos);
}
char Pacman::getDirection () const{
	return direction;
}