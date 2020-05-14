#include"monster.hh"
#include <cstdlib>

Monster::Monster(std::array<float,2> initPos,std::string name,float speed,sf::Color color){
	setType('M');
	setPosition(initPos);
	setSpeedInit(speed);
	this->name = name;
	spawn = initPos;
	setLastMove(' ');
	setHide(false);
	setColor(color);
	nominalColor = color;
	hiddenTime = INFINITY;
	hiddenClock = clock();
	modeTimes[0] = 0;
	modeTimes[1] = INFINITY;
	modeTimes[2] = INFINITY;
}

void Monster::setLastMove(const char move){
	lastMove = move;
}

char Monster::getDirection() const{
	return lastMove;
}
bool Monster::eat(const std::array<float,2> pacPos)const{
	//if their centers are the same, collision occurs and pacman gets eaten
	if(abs(getX()-(pacPos[0])) <0.5 && abs(getY()-pacPos[1]) <0.5){
		return true;
	}

	return false;
}

void Monster::chase(const Plate plate,const Tile pacTile,const Tile shadowTile,const char direction){
	std::array<float,2> target;
	bool outOfHouse =false;

	if(direction == 'o')
		outOfHouse = true;

	if(!name.compare("Shadow")){
		//+0.5 because it targets the center of the tile
		target[0] = (size_t)pacTile.getX()+0.5;
		target[1] = (size_t)pacTile.getY()+0.5;
	}

	if(!name.compare("Speedy")){
		switch(direction){
			case 'u':
				target[0] = (size_t)pacTile.getX()+0.5-4;
				target[1] = (size_t)pacTile.getY()+0.5;
				break;
			case 'd':
				target[0] = (size_t)pacTile.getX()+0.5+4;
				target[1] = (size_t)pacTile.getY()+0.5;
				break;
			case 'l':
				target[0] = (size_t)pacTile.getX()+0.5;
				target[1] = (size_t)pacTile.getY()+0.5-4;
				break;
			case 'r':
				target[0] = (size_t)pacTile.getX()+0.5;
				target[1] = (size_t)pacTile.getY()+0.5+4;
				break;
			default:
				target[0] = (size_t)pacTile.getX()+0.5;
				target[1] = (size_t)pacTile.getY()+0.5;
				break;
		}
	}
	if(!name.compare("Bashful")){
		std::array<float,2> aheadPac;
		switch(direction){
			case 'u':
				aheadPac[0] = (size_t)pacTile.getX()+0.5-2;
				aheadPac[1] = (size_t)pacTile.getY()+0.5;
				break;
			case 'd':
				aheadPac[0] = (size_t)pacTile.getX()+0.5+2;
				aheadPac[1] = (size_t)pacTile.getY()+0.5;
				break;
			case 'l':
				aheadPac[0] = (size_t)pacTile.getX()+0.5;
				aheadPac[1] = (size_t)pacTile.getY()+0.5-2;
			case 'r':
				aheadPac[0] = (size_t)pacTile.getX()+0.5;
				aheadPac[1] = (size_t)pacTile.getY()+0.5+2;
				break;
			default:
				aheadPac[0] = (size_t)pacTile.getX()+0.5;
				aheadPac[1] = (size_t)pacTile.getY()+0.5;
				break;
		}
		std::array<float,2> sha;
		sha[0] = shadowTile.getX() +0.5;
		sha[1] = shadowTile.getY() +0.5;
		target = differentialArray(sha,aheadPac,2);
;
	}
	if(!name.compare("Pokey")){
		target[0] = (size_t)pacTile.getX()+0.5;
		target[1] = (size_t)pacTile.getY()+0.5;

		if(euclidianDistance(plate.getTile(getX(),getY())) <= 8 && !plate.getTile(getX(),getY()).isFantomHouse()){
			setTarget(spawn);
			outOfHouse =true;
		}
	}
	setTarget(target);
	char c = getDisplacement(plate,outOfHouse,false);
	move(c);
	alignInAisle(c);
	setLastMove(c);

	if(!(plate.getTile((size_t)getX(),(size_t)getY()).isPlayable() || plate.getTile((size_t)getX(),(size_t)getY()).isFantomHouse()))
		move(opposite(c));
	
}

bool Monster::isAtSpawn() const{

	if(getPosition() == spawn)
		return true;

	return false;
}

void Monster::scatter(const Plate plate){

	std::array<float,2> target;

	if(!name.compare("Shadow")){
		//+0.5 because it targets the center of the tile
		target[0] = 0;
		target[1] = 33;
	}
	if(!name.compare("Speedy")){
		target[0] = 0;
		target[1] = 2;
	}

	if(!name.compare("Bashful")){
		target[0] = 27;
		target[1] = 36;
	}
	if(!name.compare("Pokey")){
		target[0] = 0;
		target[1] = 36;
	}

	char c = getDisplacement(plate,false,false);
	move(c);
	alignInAisle(c);
	setLastMove(c);

	if(!(plate.getTile((size_t)getX(),(size_t)getY()).isPlayable() || plate.getTile((size_t)getX(),(size_t)getY()).isFantomHouse()))
		move(opposite(c));
	
}

void Monster::returnHouse(const Plate plate){
	std::array<float,2> target;

	target[0] =12;
	target[1] =17;
	setTarget(target);
	char c = getDisplacement(plate,true,true);
	move(c);
	alignInAisle(c);
	setLastMove(c);

	if(!(plate.getTile((size_t)getX(),(size_t)getY()).isPlayable() || plate.getTile((size_t)getX(),(size_t)getY()).isFantomHouse()))
		move(opposite(c));
	
}

void Monster::panic(const Plate plate){

	bool atCenter = (getX() - (size_t) getX()) < 0.6 && (getX() - (size_t) getX()) > 0.4;
	atCenter = atCenter && (getY() - (size_t) getY()) < 0.6 && (getY() - (size_t) getY()) > 0.4;
	
	char c;

	if(atCenter){
		bool outOfHouse = true;
		std::vector<Tile> vt = availableNextTile(plate, outOfHouse,false);

		if(vt.size() != 0){
			int randnbr = rand() % vt.size();
			std::array<float,2> target;
			target[0] = vt[randnbr].getX();
			target[1] = vt[randnbr].getY();
			setTarget(target);

			c = getDisplacement(plate,outOfHouse,false);
		}else
			c = lastMove;
	}else
	c = lastMove;

	move(c);
	alignInAisle(c);
	setLastMove(c);
	
	if(!(plate.getTile((size_t)getX(),(size_t)getY()).isPlayable() || plate.getTile((size_t)getX(),(size_t)getY()).isFantomHouse()))
		move(opposite(c));
}
std::array<float,2> differentialArray(std::array<float,2> a1,std::array<float,2> a2,float coefficient){
	std::array<float,2> tmp;
	tmp[0] = coefficient*(a2[0] - a1[0]);
	tmp[1] = coefficient*(a2[1] - a1[1]);

	return tmp;
}
char opposite(char c){
	if( c == 'd')
		return 'u';
	if( c == 'u')
		return 'd';
	if( c == 'l')
		return 'r';
	if( c == 'r')
		return 'l';

	return ' ';
}
std::string Monster::getName() const{
	return name;
}
void Monster::setMode(std::string mo){
	if(!mo.compare("scatter"))
		modeTimes[1] = 0.0;
	if(!mo.compare("chase"))
		modeTimes[0] = 0.0;
	if(!mo.compare("panic"))
		modeTimes[2] = 0.0;

	mode = mo;
}
std::string Monster::getMode() const{
	return mode;
}
void Monster::setTarget(std::array<float,2> target){
	this->target = target;
}
char Monster::getDisplacement(const Plate plate,const bool outOfHouse,const bool turnU) const{
	//Displacement by default
	char displacement =' ';
	float minDistance = std::numeric_limits<float>::infinity();
	std::vector<Tile> nextTile =  availableNextTile(plate,outOfHouse,turnU);

	for(auto i = nextTile.cbegin();i!= nextTile.cend();i++){

		if(minDistance>=euclidianDistance(*i)){

			minDistance = euclidianDistance(*i);

			if((size_t)getY()-1 == i->getX() )
				displacement ='l';

			if((size_t)getY()+1 == i->getX() )
				displacement ='r';

			if((size_t)getX()-1 == i->getY() )
				displacement ='u';

			if((size_t)getX()+1 == i->getY() )
				displacement ='d';
			
		}
	}

	return displacement;
}
std::vector<Tile> Monster::availableNextTile(const Plate plate,const bool outOfHouse,const bool turnU) const{
	std::vector<Tile> vect;
	
	if(outOfHouse){



			if((plate.getTile((size_t)getX(),(size_t)getY()-1).isPlayable() ||plate.getTile((size_t)getX(),(size_t)getY()-1).isFantomHouse())
				&& lastMove != 'r')
				vect.push_back(plate.getTile((size_t)getX(),(size_t)getY()-1));

			if((plate.getTile((size_t)getX()+1,(size_t)getY()).isPlayable() || plate.getTile((size_t)getX()+1,(size_t)getY()).isFantomHouse()) 
				&& lastMove != 'u' )
				vect.push_back(plate.getTile((size_t)getX()+1,(size_t)getY()));

			if((plate.getTile((size_t)getX(),(size_t)getY()+1).isPlayable() || plate.getTile((size_t)getX(),(size_t)getY()+1).isFantomHouse()) 
				&& lastMove != 'l' )
				vect.push_back(plate.getTile((size_t)getX(),(size_t)getY()+1));

			if((plate.getTile((size_t)getX()-1,(size_t)getY()).isPlayable() || plate.getTile((size_t)getX()-1,(size_t)getY()).isFantomHouse())
				&& lastMove != 'd' )
				vect.push_back(plate.getTile((size_t)getX()-1,(size_t)getY()));
		
		}

	else if(turnU){

			if(plate.getTile((size_t)getX(),(size_t)getY()-1).isPlayable() && lastMove != 'r' )
				vect.push_back(plate.getTile((size_t)getX(),(size_t)getY()-1));

			if(plate.getTile((size_t)getX()+1,(size_t)getY()).isPlayable() && lastMove != 'u')
				vect.push_back(plate.getTile((size_t)getX()+1,(size_t)getY()));

			if(plate.getTile((size_t)getX(),(size_t)getY()+1).isPlayable() && lastMove != 'l')
				vect.push_back(plate.getTile((size_t)getX(),(size_t)getY()+1));

			
			if(plate.getTile((size_t)getX()-1,(size_t)getY()).isPlayable() && lastMove != 'd' )
				vect.push_back(plate.getTile((size_t)getX()-1,(size_t)getY()));

		}
	else{
		if(plate.getTile((size_t)getX(),(size_t)getY()-1).isPlayable())
				vect.push_back(plate.getTile((size_t)getX(),(size_t)getY()-1));

			if(plate.getTile((size_t)getX()+1,(size_t)getY()).isPlayable())
				vect.push_back(plate.getTile((size_t)getX()+1,(size_t)getY()));

			if(plate.getTile((size_t)getX(),(size_t)getY()+1).isPlayable())
				vect.push_back(plate.getTile((size_t)getX(),(size_t)getY()+1));

			
			if(plate.getTile((size_t)getX()-1,(size_t)getY()).isPlayable())
				vect.push_back(plate.getTile((size_t)getX()-1,(size_t)getY()));

		}


	return vect;
}
void Monster::alignInAisle(const char displacement){
	std::array<float,2> correctedPos;
	//if vertical aisle
	if(displacement == 'd' || displacement == 'u'){
		correctedPos[0]= getX();
		correctedPos[1] = (size_t)getY()+0.5;
		setPosition(correctedPos);
	}
	//horizontal aisle
	if(displacement == 'l' || displacement == 'r'){
		correctedPos[0]= (size_t)getX()+0.5;
		correctedPos[1] = getY();
		setPosition(correctedPos);
	}
}
float Monster::euclidianDistance(const Tile t) const{

	float deltaX = (target[0]- t.getX());
	float deltaY = (target[1]- t.getY());

	return sqrt((deltaX*deltaX)+(deltaY*deltaY));
}
std::array<float,2> Monster::getSpawn(){
	return spawn;
}
void Monster::updateTimer(size_t nbr){

	switch(nbr){
		case 0:
			modeTimes[0] -= ( std::clock() - hiddenClock ) / (float) CLOCKS_PER_SEC;
			break;
		case 1:
			modeTimes[1] -= ( std::clock() - hiddenClock ) / (float) CLOCKS_PER_SEC;
			break;
		case 2:
			modeTimes[2] -= ( std::clock() - hiddenClock ) / (float) CLOCKS_PER_SEC;
			break;
	}
}
float Monster::getTimer(const size_t nbr) const{
	if(nbr < modeTimes.size())
		return modeTimes[nbr];

	return -1;
}

void Monster::setTimer(float value){
	if(!mode.compare("chase"))
		modeTimes[0] = value;
	else if(!mode.compare("scatter"))
		modeTimes[1] = value;
	else if(!mode.compare("panic"))
		modeTimes[2] = value;
	return ;
}