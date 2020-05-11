#include"board.hh"

Board::Board(size_t size,float refSpeed){
	plate = Plate();
	tileSize = size;
	score=0;
	this->refSpeed = refSpeed;
	std::array<std::array<float,2>,5> pos = {{ { {20.5,14} } , { {14,12} }, { {17,12} } ,{ {17,14} } , { {17,16} } }};
	std::array<std::string,4> names = { {"Shadow","Speedy","Bashful","Pokey"} };
	std::array<sf::Color,5> color =  { {sf::Color(255,255,0),sf::Color(255,0,0),sf::Color(250,197,246),sf::Color(0,255,255),sf::Color(247,187,20)} };

	pacman = Pacman(pos[0],refSpeed);
	pacman.setRayon(0.8);
	pacman.setColor(color[0]);
	std::cout << pacman.getHiddenTime() << std::endl;

	for(int i=0;i<4;i++){
		monsters[i] = Monster(pos[i+1],names[i],0.95*refSpeed);
		monsters[i].setRayon(1.6);
		monsters[i].setColor(color[i+1]);
		monsters[i].setMode("scatter");

	}

}

Plate Board::getPlate(){
	return plate;
}
size_t Board::getScore(){
	return score;
}

size_t Board::getTileSize(){
	return tileSize;
}
void Board::drawBoard(sf::RenderWindow *window){
	plate.drawPlate(window,tileSize);
	pacman.setColor(sf::Color(255,255,0));
	pacman.drawPlayer(window,tileSize,true);

	for( auto i = monsters.cbegin(); i!= monsters.cend();i++)
		i->drawPlayer(window,tileSize,false); 


}
bool Board::monsterMove(){


	for(auto monster = monsters.begin();monster != monsters.end();monster++){

		std::cout << monster->getName() << " " << monster->getMode() << std::endl;

		if(monster->getTime(0) > chaseTime && !monster->getMode().compare("chase"))
			monster->setMode("scatter");
		else
			monster->updateTimes(0);

		if(monster->getTime(1) > scatterTime && !monster->getMode().compare("scatter"))
			monster->setMode("panic");
		else
			monster->updateTimes(1);

		if(monster->getTime(2) > 30 && !monster->getMode().compare("panic"))
			monster->setMode("chase");
		else
			monster->updateTimes(2);

		if(monster->getHiddenTime() > hiddenTime){

			monster->setSpeed(0.95*refSpeed);
			monster->setHide(false);

			if(plate.getTile((size_t)monster->getX(),(size_t)monster->getY()).isTunnel()){
				monster->setSpeed(0.55*monster->getSpeed());
				teleport(*monster);
			}

			if(plate.getTile((size_t)monster->getX(),(size_t)monster->getY()).isFantomHouse())
				monsterOutOfHouse(monster);

			if(!monster->getMode().compare("chase") && !plate.getTile((size_t)monster->getX(),(size_t)monster->getY()).isFantomHouse()){
				monster->chase(plate,plate.getTile((size_t)pacman.getX(),(size_t)pacman.getY()),plate.getTile((size_t)monsters[0].getX(),(size_t)monsters[0].getY()),pacman.getDirection());
			}

			if(!monster->getMode().compare("scatter") && !plate.getTile((size_t)monster->getX(),(size_t)monster->getY()).isFantomHouse())
				monster->scatter(plate);

			if(!monster->getMode().compare("panic") && !plate.getTile((size_t)monster->getX(),(size_t)monster->getY()).isFantomHouse())
				monster->panic(plate);

			if(monster->eat(pacman.getPosition()) && !monster->getHide())
				return true;

			monster->updateHiddenClock();

		}else{
			monster->updateHiddenTime();
		}

	}


	return false;
}
void Board::monsterOutOfHouse(Monster *monster) {
	// Don't care about 3 argument, because it's not needed
	monster->chase(plate,plate.getTile(14,14),plate.getTile(14,14),'o');
}
void Board::teleport(Player &p){

	if((p.getDirection() == 'l' && (size_t) p.getY() == 0) || (p.getDirection() == 'r' && (size_t) p.getY() > plate.getLengthCol()-1)){

		std::array<float,2> tmp;
		tmp[0] = p.getX();
		tmp[1] = abs(p.getY() - plate.getLengthCol());
		p.setPosition(tmp);

		p.setHide(true);
	}
}
void Board::playerMove(){

	if(pacman.getHiddenTime() > hiddenTime){

		pacman.setHide(false);

		std::array<float,2> tmpPos;
		std::array<size_t,2> roundedPos;
		
		tmpPos = pacman.getPosition();
		roundedPos[1] = (size_t)tmpPos[0];
		roundedPos[0] = (size_t)tmpPos[1];

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){
			move('l');
		}
		
		else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			move('r');
		}
		
		else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			move('u');
		}
		
		else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			move('d');
		}
		else
		{
			move(pacman.getDirection());
		}
		if(plate.getTile(roundedPos[1],roundedPos[0]).isTunnel()){
			pacman.setSpeed(0.55*pacman.getSpeed());
			teleport(pacman);
		}else{		pacman.setSpeed(pacman.getSpeed());}

		pacman.updateHiddenClock();
	}else{
		pacman.updateHiddenTime();
	}
}
void Board::printInPrompt(){
	std::array<float,2> tmpPos = pacman.getPosition();
	std::array<float,2> tmpPos2 = monsters[0].getPosition();
	for(size_t i =0;i<plate.getLengthRow();i++){
		for(size_t j =0;j<plate.getLengthCol();j++){
			if(plate.getTile(i,j).isWall())
				std::cout<<" ";
			else if(!(i== round(tmpPos[0]) && j== round(tmpPos[1])) && !(i== round(tmpPos2[0]) && j== round(tmpPos2[1])))
				std::cout<<plate.getTile(i,j).getType();
			else if(!(i== round(tmpPos2[0]) && j== round(tmpPos2[1])))
				std::cout<<"P";
			else 
				std::cout<<"C";
		}
		std::cout<<"\n";
	}
}
Pacman Board::getPacman(){
	return pacman;
}

Tile Board::getTileNext(size_t y, size_t x, char dir)
{
	if(dir == 'u')
		return plate.getTile(y-1, x);
	else if(dir == 'd')
		return plate.getTile(y+1, x);
	else if(dir == 'r')
		return plate.getTile(y, x+1);
	else if(dir == 'l')
		return plate.getTile(y, x -1 );

	return Tile(0, true, (std::array<size_t,2>) {0,0});
}

bool Board::isCloseEnough(std::array<float,2> p, Tile tile, char dir)
{
	if(dir == 'u' || dir == 'd')
	{
		float dy = abs(p[0] - (size_t)p[0]);
		return dy < 0.55 && dy > 0.45;
	}else{
		float dx = abs(p[1] - (size_t)p[1]);
		return dx < 0.55 && dx > 0.45;
	}
}

void Board::move(char dir)
{
	/* Index inversion of roundedPos because of the true representation of the grid
	In the memory of the program */
	std::array<float,2> tmpPos;
	std::array<size_t,2> roundedPos;

	tmpPos = pacman.getPosition();
	roundedPos[1] = (size_t)tmpPos[0];
	roundedPos[0] = (size_t)tmpPos[1];

	size_t value = 0;
	bool aboveCenter = false;

	Tile next = getTileNext(roundedPos[1], roundedPos[0], dir);
	Tile usualNext = getTileNext(roundedPos[1], roundedPos[0], pacman.getDirection());
	
	std::cout << tmpPos[0] << " " << tmpPos[1] << " | " << usualNext.getX() << " " << usualNext.getY() << std::endl;

	if(isPerpendicular(pacman.getDirection(), dir)){
		if(pacman.getDirection() == 'u')
			aboveCenter = usualNext.getY() - tmpPos[0] > 0.5;
		else if(pacman.getDirection() == 'd')
			aboveCenter = usualNext.getY() - tmpPos[0] < 0.5;
		else if(pacman.getDirection() == 'r')
			aboveCenter = usualNext.getX() - tmpPos[1] < 0.5;
		else if(pacman.getDirection() == 'l')
			aboveCenter = usualNext.getX() - tmpPos[1] > 0.5;
	}
	//Tile nextSec = getTileNext(roundedPos[1], roundedPos[0], pacmanSecondMove); // à commenter pour enlever le rappel du tournant

 /*if(!nextSec.isWall() && !isCloseEnough(tmpPos, usualNext, pacman.getDirection()) ){ // à enlever pour enlever le rappelle du tournant

		pacman.move(pacmanSecondMove);
		pacman.setDirection(pacmanSecondMove);
		pacmanSecondMove = 'e';
		recenterPacman();
	
	}else */if(next.isPlayable() && !aboveCenter){

		pacman.move(dir);
		if(pacman.getDirection() != dir){
			pacman.setDirection(dir);
			//pacmanSecondMove = 'e';
			recenterPacman();
	 	}

	}else{
		/*
		if(dir != pacman.getDirection())	
			pacmanSecondMove = dir;*/
		if(usualNext.isPlayable() || !isCloseEnough(tmpPos, usualNext, pacman.getDirection()))
			pacman.move(pacman.getDirection());
	}



	value = pacman.eat(plate,monsters);
	if(value){
		score+= value;
		if(value <= 50){
			Tile tile(0,false,roundedPos);
			plate.setTile(tile);
			plate.decountFood();
		}		
	}
}

void Board::recenterPacman(){

	std::array<float,2> pacPos = pacman.getPosition();

	if(pacman.getDirection() == 'u' || pacman.getDirection() == 'd')
		pacPos[1] = (float) ( (size_t) pacPos[1]) + 0.5;
	else
		pacPos[0] = (float) ((size_t) pacPos[0]) + 0.5;

	pacman.setPosition(pacPos);
}

bool Board::isPerpendicular(char x, char y){
	if(x == 'u' || 'd')
		return y == 'r' || y == 'l';
	else
		return y == 'u' || y == 'r';
}