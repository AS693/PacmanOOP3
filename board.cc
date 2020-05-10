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
		monsters[i].setMode("chase");

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
void Board::monsterMove(){


	for(auto monster = monsters.begin();monster != monsters.end();monster++){

		if(monster->getHiddenTime() > hiddenTime){

			monster->setSpeed(0.95*refSpeed);
			monster->setHide(false);

			if(plate.getTile((size_t)monster->getX(),(size_t)monster->getY()).isTunnel()){
				monster->setSpeed(0.55*monster->getSpeed());

				teleport(*monster);
			}
			if(plate.getTile((size_t)monster->getX(),(size_t)monster->getY()).isFantomHouse())
				monsterOutOfHouse(monster);

			if(!monster->getMode().compare("chase") && !plate.getTile((size_t)monster->getX(),(size_t)monster->getY()).isFantomHouse())
				monster->chase(plate,plate.getTile((size_t)pacman.getX(),(size_t)pacman.getY()),pacman.getDirection());

			monster->updateHiddenClock();

		}else{
			monster->updateHiddenTime();
		}
	}
}
void Board::monsterOutOfHouse(Monster *monster) {
	monster->chase(plate,plate.getTile(15,12),'o');
}
void Board::teleport(Player &p){

	if((p.getDirection() == 'l' && (size_t) p.getY() == 0) || (p.getDirection() == 'r' && (size_t) p.getY() > plate.getLengthCol()-1)){//(size_t)p.getY() == plate.getLengthCol() || (size_t)p.getY() == 0){
		std::cout << " TELEPORT "<< std::endl;
		std::array<float,2> tmp;
		tmp[0] = p.getX();
		tmp[1] = abs(p.getY() - plate.getLengthCol());
		p.setPosition(tmp);

		p.setHide(true);
	}
}
void Board::playerMove(){

	std::cout << pacman.getHiddenTime() << std::endl;
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
			teleport(pacman);
		}

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

void Board::move(char dir)
{
	/* Index inversion of roundedPos because of the true representation of the grid
	In the memory of the program */
	std::array<float,2> tmpPos;
	std::array<size_t,2> roundedPos;

	tmpPos = pacman.getPosition();
	roundedPos[1] = (size_t)tmpPos[0];
	roundedPos[0] = (size_t)tmpPos[1];

	//std::cout << tmpPos[1] << " " << tmpPos[0] << " " << tileSize << std::endl;

	size_t value =0;

	Tile next = getTileNext(roundedPos[1], roundedPos[0], dir);
	Tile usualNext = getTileNext(roundedPos[1], roundedPos[0], pacman.getDirection());
	Tile nextSec = getTileNext(roundedPos[1], roundedPos[0], secondMove);


	if(!nextSec.isWall() && ((abs(tmpPos[1]-(roundedPos[0])) < 0.5 || abs(tmpPos[0]-(roundedPos[1])) < 0.5))){

		pacman.move(secondMove);
		pacman.setDirection(secondMove);
		secondMove = 'e';
	}	

	else if(!next.isWall() || ((abs(tmpPos[1]-(roundedPos[0])) < 0.5 || abs(tmpPos[0]-(roundedPos[1])) < 0.5))){

		pacman.move(dir);
		if(pacman.getDirection() != dir){
			pacman.setDirection(dir);
			secondMove = 'e';
		}
		
	}else{
		
		if(dir != pacman.getDirection())	
			secondMove = dir;
		if(!usualNext.isWall() || ((abs(tmpPos[1]-(roundedPos[0])) < 0.5 || abs(tmpPos[0]-(roundedPos[1])) < 0.5)))
			pacman.move(pacman.getDirection());

	}


	tmpPos = pacman.getPosition();

	std::cout << "Y = " << tmpPos[0] << " X = " << tmpPos[1] << std::endl;

	if(pacman.getDirection() == 'u' || pacman.getDirection() == 'd')
		tmpPos[1] = (float) ( (size_t) tmpPos[1]) + 0.5;
	else
		tmpPos[0] = (float) ((size_t) tmpPos[0]) + 0.5;

	pacman.setPosition(tmpPos);

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