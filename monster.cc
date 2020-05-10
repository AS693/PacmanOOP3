#include"monster.hh"

Monster::Monster(std::array<float,2> initPos,std::string name,float speed){
	setType('M');
	setPosition(initPos);
	setSpeed(speed);
	this->name = name;
	spawn = initPos;
	setLastMove(' ');
	setHide(false);

	hiddenTime = INFINITY;
	hiddenClock = clock();
}

void Monster::setLastMove(const char move){
	lastMove = move;
}

char Monster::getDirection() const{
	return lastMove;
}

void Monster::chase(const Plate plate,const Tile tile,const char direction){
	std::array<float,2> target;
	bool outOfHouse =false;

	if(direction == 'o')
		outOfHouse = true;

	if(!name.compare("Shadow")){
		target[0] = (size_t)tile.getX();
		target[1] = (size_t)tile.getY();
		setTarget(target);
		char c = getDisplacement(plate,outOfHouse);
		move(c);
		setLastMove(c);

		if(!(plate.getTile((size_t)getX(),(size_t)getY()).isPlayable() || plate.getTile((size_t)getX(),(size_t)getY()).isFantomHouse()))
			move(opposite(c));
	}

	if(!name.compare("Speedy")){
		switch(direction){
			case 'u':
				target[0] = (size_t)tile.getX()-4;
				target[1] = (size_t)tile.getY();
				break;
			case 'd':
				target[0] = (size_t)tile.getX()+4;
				target[1] = (size_t)tile.getY();
				break;
			case 'l':
				target[0] = (size_t)tile.getX();
				target[1] = (size_t)tile.getY()-4;
				break;
			case 'r':
				target[0] = (size_t)tile.getX();
				target[1] = (size_t)tile.getY()+4;
				break;
			default:
				target[0] = (size_t)tile.getX();
				target[1] = (size_t)tile.getY();
				break;
		}
		setTarget(target);
		char c = getDisplacement(plate,outOfHouse);
		move(c);
		setLastMove(c);
		if(!(plate.getTile((size_t)getX(),(size_t)getY()).isPlayable() || plate.getTile((size_t)getX(),(size_t)getY()).isFantomHouse()))
			move(opposite(c));
	}
	if(!name.compare("Bashful")){

	}
	if(!name.compare("Pokey")){
		target[0] = (size_t)tile.getX();
		target[1] = (size_t)tile.getY();
		setTarget(target);

		if(euclidianDistance(plate.getTile(getX(),getY())) <= 8 && !plate.getTile(getX(),getY()).isFantomHouse())
			setTarget(spawn);

		char c = getDisplacement(plate,outOfHouse);
		move(c);
		setLastMove(c);

		if(!(plate.getTile((size_t)getX(),(size_t)getY()).isPlayable() || plate.getTile((size_t)getX(),(size_t)getY()).isFantomHouse()))
			move(opposite(c));
	}
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
	mode = mo;
}
std::string Monster::getMode(){
	return mode;
}
void Monster::setTarget(std::array<float,2> target){
	this->target = target;
}
char Monster::getDisplacement(const Plate plate,const bool outOfHouse) const{
	//Displacement by default
	char displacement =' ';
	float minDistance = std::numeric_limits<float>::infinity();
	std::vector<Tile> nextTile =  availableNextTile(plate,outOfHouse);


	for(auto i = nextTile.cbegin();i!= nextTile.cend();i++){
	
		if(minDistance>euclidianDistance(*i)){

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
std::vector<Tile> Monster::availableNextTile(const Plate plate,const bool outOfHouse) const{
	std::vector<Tile> vect;
	if(outOfHouse){
		if((size_t)getX() < plate.getLengthCol() && (size_t)getY() < plate.getLengthRow()){

			if((plate.getTile((size_t)getX()+1,(size_t)getY()).isPlayable() || plate.getTile((size_t)getX()+1,(size_t)getY()).isFantomHouse()) 
				&& lastMove != 'u'){

				vect.push_back(plate.getTile((size_t)getX()+1,(size_t)getY()));
			}

			if((plate.getTile((size_t)getX(),(size_t)getY()+1).isPlayable() || plate.getTile((size_t)getX(),(size_t)getY()+1).isFantomHouse()) 
				&& lastMove != 'l'){

				vect.push_back(plate.getTile((size_t)getX(),(size_t)getY()+1));
			}

		}
		if(getY()-1>=0 && getX()-1>=0){

			if((plate.getTile((size_t)getX()-1,(size_t)getY()).isPlayable() || plate.getTile((size_t)getX()-1,(size_t)getY()).isFantomHouse())
				&& lastMove != 'd'){
								vect.push_back(plate.getTile((size_t)getX()-1,(size_t)getY()));
				}
			
			if((plate.getTile((size_t)getX(),(size_t)getY()-1).isPlayable() ||plate.getTile((size_t)getX(),(size_t)getY()-1).isFantomHouse())
				&& lastMove != 'r'){

				vect.push_back(plate.getTile((size_t)getX(),(size_t)getY()-1));
			}
		
		}
	}
	else{
		if((size_t)getX() < plate.getLengthCol() && (size_t)getY() < plate.getLengthRow()){

			if(plate.getTile((size_t)getX()+1,(size_t)getY()).isPlayable() && lastMove != 'u')
				vect.push_back(plate.getTile((size_t)getX()+1,(size_t)getY()));


			if(plate.getTile((size_t)getX(),(size_t)getY()+1).isPlayable() && lastMove != 'l')
				vect.push_back(plate.getTile((size_t)getX(),(size_t)getY()+1));


		}
		if(getY()-1>=0 && getX()-1>=0){
			if(plate.getTile((size_t)getX()-1,(size_t)getY()).isPlayable() && lastMove != 'd')
				vect.push_back(plate.getTile((size_t)getX()-1,(size_t)getY()));

			
			if(plate.getTile((size_t)getX(),(size_t)getY()-1).isPlayable() && lastMove != 'r')
				vect.push_back(plate.getTile((size_t)getX(),(size_t)getY()-1));

		
		}
	}

	return vect;
}
float Monster::euclidianDistance(const Tile t) const{

	size_t deltaX = (target[0]- t.getX());
	size_t deltaY = (target[1]- t.getY());

	return sqrt((deltaX*deltaX)+(deltaY*deltaY));
}
std::array<float,2> Monster::getSpawn(){
	return spawn;
}