#include"board.hh"

Board::Board(size_t size,float refSpeed){
	plate = Plate();
	tileSize = size;
	score=0;
	this->refSpeed = refSpeed;
	std::array<std::array<float,2>,5> pos = {{ { {20.5,14} } , { {14,12} }, { {17,12} } ,{ {17,14} } , { {17,16} } }};
	std::array<std::string,4> names = { {"Shadow","Speedy","Bashful","Pokey"} };
	std::array<sf::Color,5> color =  { {sf::Color(255,255,0),sf::Color(255,0,0),sf::Color(250,197,246),sf::Color(0,255,255),sf::Color(247,187,20)} };
	std::array<std::string,4> mode = { {"chase","scatter","scatter","scatter"} };
	std::array<float, 4> timer = {chaseTime + scatterTime, scatterTime, scatterTime, scatterTime};

	pacman = Pacman(pos[0],refSpeed);
	pacman.setRayon(0.8);
	pacman.setColor(color[0]);
	std::cout << pacman.getHiddenTime() << std::endl;

	for(int i=0;i<4;i++){
		monsters[i] = Monster(pos[i+1],names[i],0.95*refSpeed);
		monsters[i].setRayon(1.6);
		monsters[i].setColor(color[i+1]);
		monsters[i].setMode(mode[i]);
		monsters[i].setTimer(timer[i]);
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

		if(monster->getTimer(0) < 0 && !monster->getMode().compare("chase")){
			monster->setMode("scatter");
			monster->setTimer(scatterTime);
		}
		else
			monster->updateTimer(0);

		if(monster->getTimer(1) < 0 && !monster->getMode().compare("scatter")){
			monster->setMode("chase");
			monster->setTimer(chaseTime);
		}
		else
			monster->updateTimer(1);

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

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && lastShortcut == 'e'){
			move('l');
		}
		
		else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && lastShortcut == 'e')
		{
			move('r');
		}
		
		else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && lastShortcut == 'e')
		{
			move('u');
		}
		
		else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && lastShortcut == 'e')
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
		return plate.getTile(y - 1, x);
	else if(dir == 'd')
		return plate.getTile(y+1, x);
	else if(dir == 'r')
		return plate.getTile(y, x+1);
	else if(dir == 'l')
		return plate.getTile(y, x - 1);

	return Tile(0, true, (std::array<size_t,2>) {0,0});
}

bool Board::isCloseEnough(std::array<float,2> p, Tile tile, char dir){
	if(dir == 'u'){
		float d = p[0] - tile.getY();
		return d <= 1.5;
	}else if(dir == 'd'){
		float d = p[0] - tile.getY();
		return d >= -0.5;
	}else if(dir == 'l'){
		float d = p[1] - tile.getX();
		return d <= 1.5;
	}else if(dir == 'r'){
		float d = p[1] - tile.getX();
		return d >= -0.5;
	}else
		return true;
}

void Board::move(char dir)
{
	if(pacman.isSleeping()){
		pacman.awake();
		return ;
	}
	/* Index inversion of roundedPos because of the true representation of the grid
	In the memory of the program */
	std::array<float,2> tmpPos;
	std::array<size_t,2> roundedPos;

	tmpPos = pacman.getPosition();
	roundedPos[1] = (size_t)tmpPos[0];
	roundedPos[0] = (size_t)tmpPos[1];

	size_t value = 0;
	bool aboveCenter = false;
	bool tooClose = false;
	float defaultv = pacman.getCurrSpeed();

	Tile next = getTileNext(roundedPos[1], roundedPos[0], dir);
	Tile usualNext = getTileNext(roundedPos[1], roundedPos[0], pacman.getDirection());

	if(next.isPlayable() && !aboveCenter){

		/* shortcut */
		if((isBelowCenter(tmpPos, pacman.getDirection()) && isPerpendicular(pacman.getDirection(), dir)) || lastShortcut != 'e'){

			if(lastShortcut == 'e'){
				lastShortcut = pacman.getDirection();
				pacman.setDirection(dir);
			}

			pacman.setSpeed(defaultv / sqrt(2));
			pacman.move(lastShortcut);
			pacman.move(pacman.getDirection());

			pacman.setSpeed(defaultv);

			Tile newNext = getTileNext(roundedPos[1], roundedPos[0], lastShortcut);

			if(isCloseEnough(pacman.getPosition(), newNext, lastShortcut))
				lastShortcut = 'e';
			
		}else{
			pacman.move(dir);
			if(pacman.getDirection() != dir)
				pacman.setDirection(dir);
		}

	}else{

		if(usualNext.isPlayable() || !isCloseEnough(tmpPos, usualNext, pacman.getDirection())){
			
			/* assure that pacman won't go through a wall if he has a too high speed */

	 		float delta;
			
			if(pacman.getDirection() == 'd'){

				delta = (pacman.getX() - usualNext.getY());
				tooClose = delta > 0.5 && delta > 0.5 + 2*defaultv;
			
			}else if(pacman.getDirection() == 'u'){
			
				delta = (pacman.getX() - usualNext.getY());
				tooClose = delta < -0.5 && delta < -0.5 - 2*defaultv;
				delta *= -1;	
			
			}else if(pacman.getDirection() == 'l'){
			
				delta = (pacman.getY() - usualNext.getX());

				tooClose = delta < -0.5 && delta < -0.5 - 2*defaultv;
				delta *= -1;

			}else if(pacman.getDirection() == 'r'){
			
				delta = abs(pacman.getY() - usualNext.getX());
				tooClose = delta > 0.5 && delta < 0.5 + 2*defaultv;

			}
			if(tooClose){
				pacman.setSpeed(delta-0.5);
			}

			pacman.move(pacman.getDirection());
			pacman.setSpeed(defaultv);
		}
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

bool Board::isPerpendicular(char x, char y){

	if(x == 'u' || x == 'd')
		return y == 'r' || y == 'l';
	else if(x == 'r' || x == 'l')
		return y == 'u' || y == 'd';

	return false;
}

bool Board::isBelowCenter(std::array<float,2> p, char direction){

	if(direction == 'u')
		return p[0] - (size_t) p[0] > 0.55;
	else if(direction == 'd')
		return p[0] - (size_t) p[0] < 0.45;
	else if(direction == 'l')
		return p[1] - (size_t) p[1] > 0.55;
	else if(direction == 'r')
		return p[1] - (size_t) p[1] < 0.45;
	return false;
}