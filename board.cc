#include"board.hh"
#include "constants.hh"
Board::Board(size_t size,float refSpeed){
	plate = Plate();
	tileSize = size;
	score=0;
	this->refSpeed = refSpeed;
	std::array<std::array<float,2>,5> pos = {{ { {26.5,14} } , { {14,14} }, { {17,12} } ,{ {17,14} } , { {17,16} } }};
	std::array<std::string,4> names = { {"Shadow","Speedy","Bashful","Pokey"} };
	std::array<sf::Color,5> color =  { {sf::Color(255,255,0),sf::Color(255,0,0),sf::Color(250,197,246),sf::Color(0,255,255),sf::Color(247,187,20)} };
	
	pacman = Pacman(pos[0],refSpeed,color[0]);
	pacman.setRayon(0.8);
	std::cout << pacman.getHiddenTime() << std::endl;

	rdmTime = std::rand() % constants::LATEST_VIRUS;

	for(int i=0;i<4;i++){
		monsters[i] = Monster(pos[i+1],names[i],0.95*refSpeed,color[i+1]);
		monsters[i].setRayon(1.6);
		//monsters[i].getContagious();
	}

}
void Board::activate(){

	start = std::clock();
	std::array<std::string,4> mode = { {"chase","scatter","scatter","scatter"} };
	std::array<float, 4> timer = {chaseTime + scatterTime, scatterTime, scatterTime, scatterTime};

	for(int i = 0; i < 4; i++){

		monsters[i].setMode(mode[i]);
		monsters[i].setTimer(timer[i]);
	}

	//Launch of shadow && Speedy
	incIngameMonsters();
	incIngameMonsters();

	return ;
}

bool Board::isEnded() const{
	return gameState == 1 || gameState == -1;
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
	size_t k=0;

	plate.drawPlate(window,tileSize);
	pacman.setColor(sf::Color(255,255,0));
	pacman.drawPlayer(window,tileSize,true);

	//Draw players that are in game
	for( auto i = ingameMonsters.cbegin(); i!= ingameMonsters.cend();i++){
		i->drawPlayer(window,tileSize,false);
		k++;
	}
	//Draw out of the game players.
	while(k<monsters.size()){
		monsters[k].drawPlayer(window,tileSize,false);
		k++;
	}

	drawText(window);
}

void Board::drawText(sf::RenderWindow *window){
	sf::Font font;
	font.loadFromFile("press-start-2p-v8-latin-regular.ttf");

	std::string textScore = "";
	for(float i = score + 0.2; i < 1000; i *= 10)
		textScore += "0";
	if(score != 0)
		textScore += std::to_string(score);

	sf::Text scoreDisplay;
	scoreDisplay.setString(textScore);
	scoreDisplay.setFont(font);
	scoreDisplay.setCharacterSize(tileSize);
	scoreDisplay.setFillColor(sf::Color(247,192,158));

	float xposition = plate.getLengthCol()/2*tileSize - 2*tileSize;
 	scoreDisplay.setPosition(xposition, tileSize);

 	if(gameState == 1){

		sf::Text win;
		win.setString("You win!");
		win.setFont(font);
		win.setCharacterSize(tileSize);
		win.setFillColor(sf::Color(247,192,158));

		float x = plate.getLengthCol()/2*tileSize - 4*tileSize;
		float y = plate.getLengthRow()/2*tileSize + 2*tileSize;
		win.setPosition(x, y);
		window->draw(win);

 	}else if(gameState == -1){

		sf::Text loose;
		loose.setString("Too bad!");
		loose.setFont(font);
		loose.setCharacterSize(tileSize);
		loose.setFillColor(sf::Color(247,192,158));

		float x = plate.getLengthCol()/2*tileSize - 4*tileSize;
		float y = plate.getLengthRow()/2*tileSize + 2*tileSize;
		loose.setPosition(x, y);
		window->draw(loose);

 	}else if(gameState == 2){

		sf::Text ready;
		ready.setString("Ready?!");
		ready.setFont(font);
		ready.setCharacterSize(tileSize);
		ready.setFillColor(sf::Color(247,192,158));

		float x = plate.getLengthCol()/2*tileSize - 3.5*tileSize;
		float y = plate.getLengthRow()/2*tileSize + 2*tileSize;
		ready.setPosition(x, y);
		window->draw(ready);
 	}


	window->draw(scoreDisplay);
}
void Board::incIngameMonsters(){

	if(ingameMonsters.size()< monsters.size())
		ingameMonsters.push_back(monsters[ingameMonsters.size()]);
	
}
void Board::monsterMove(){

	if(gameState != 0)
		return ;

	timePlayed = ( std::clock() - start ) / (float) CLOCKS_PER_SEC;

	if((score == 30 || timePlayed== 15.0) || (score == plate.getNbrFood()/3) || (timePlayed == 60))
		incIngameMonsters();

	for(auto monster = ingameMonsters.begin();monster != ingameMonsters.end();monster++){

		Tile* monsterTile = plate.getTilePointer((size_t)monster->getX(),(size_t)monster->getY());

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

		if(monster->getTimer(2) < 0 && !monster->getMode().compare("panic")){
			monster->setMode("panic");

			if(monster->getTimer(2)<=0.5)
				monster->setColor(sf::Color(255,255,255));

			if(monster->getTimer(2)==0)
				monster->setMode("chase");
		}
		else
			monster->updateTimer(2);

		if(monster->getHiddenTime() > hiddenTime){

			monster->setHide(false);

			if(monster->isSick())
				monster->setSpeed(0.2*refSpeed);

			else if(monsterTile->isTunnel()){
				monster->setSpeed(0.55*monster->getSpeed());
				teleport(*monster);
			}
			else{
				monster->setSpeed(0.95*refSpeed);
			}

			if(monsterTile->isFantomHouse())
				monsterOutOfHouse(*monster);

			if(!monster->getMode().compare("chase") && !monsterTile->isFantomHouse()){
				monster->chase(plate,plate.getTile((size_t)pacman.getX(),(size_t)pacman.getY()),*monsterTile,pacman.getDirection());
			}

			if(!monster->getMode().compare("scatter") && !monsterTile->isFantomHouse())
				monster->scatter(plate);

			if(!monster->getMode().compare("eaten")){
				monster->returnHouse(plate);
				monster->setSpeed(1.3*refSpeed);

				if(monster->isAtSpawn()){
					monster->getWeak();
					monster->setMode("chase");
				}
			}

			if(!monster->getMode().compare("panic") && !monsterTile->isFantomHouse())
				monster->panic(plate);

			if(!monster->getMode().compare("scatter") || !monster->getMode().compare("chase"))
				monster->setColor(monster->getNominalColor());

			if(monster->eat(pacman.getPosition()) && !monster->getHide() && (!monster->getMode().compare("scatter") || !monster->getMode().compare("chase"))){
				gameState = -1;
				return ;
			}

			if(monster->isContagious())
				plate.Contaminate(monsterTile);

			monster->updateHiddenClock();

		}else{
			monster->updateHiddenTime();
		}

	}

	updateSickness();

	return ;
}
void Board::monsterOutOfHouse(Monster &monster) {
	// Don't care about 3 argument, because it's not needed
	monster.chase(plate,plate.getTile(14,14),plate.getTile(14,14),'o');
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
			
			if(gameState == 2){
				gameState = 0;
				activate();
			}
			
			move('l');
		}
		
		else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && lastShortcut == 'e')
		{
			if(gameState == 2){
				gameState = 0;
				activate();
			}

			move('r');
		}
		
		else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && lastShortcut == 'e')
		{
			if(gameState == 2){
				gameState = 0;
				activate();
			}
			move('u');
		}
		
		else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && lastShortcut == 'e')
		{
			if(gameState == 2){
				gameState = 0;
				activate();
			}
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
	if(gameState != 0)
		return ;

	if(pacman.isSleeping()){
		pacman.awake();
		return ;
	}

	if(pacman.isSick())
		pacman.setSpeed(0.2*pacman.getSpeed());
	else 
		pacman.setSpeed(pacman.getSpeed());
	/* Index inversion of roundedPos because of the true representation of the grid
	In the memory of the program */
	std::array<float,2> tmpPos;
	std::array<size_t,2> roundedPos;

	tmpPos = pacman.getPosition();
	roundedPos[1] = (size_t)tmpPos[0];
	roundedPos[0] = (size_t)tmpPos[1];

	size_t value = 0;
	bool tooClose = false;
	float defaultv = pacman.getCurrSpeed();

	Tile next = getTileNext(roundedPos[1], roundedPos[0], dir);
	Tile usualNext = getTileNext(roundedPos[1], roundedPos[0], pacman.getDirection());

	if(next.isPlayable()){

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

	value = pacman.eat(plate,ingameMonsters);

	std::array<size_t,2> newRoundedPos;

	newRoundedPos[0] = (size_t) pacman.getY();
	newRoundedPos[1] = (size_t) pacman.getX();

	if(value){
		score+= value;
		if(value <= 50){
			Tile tile(0,false,newRoundedPos);
			plate.setTile(tile);
			plate.decountFood();

			if(value == 50){
				for(auto i=ingameMonsters.begin();i!=ingameMonsters.end();i++){
						i->setMode("panic");
						i->setColor(sf::Color(0,0,255));
						i->setTimer(panicTime);
				}
			}
		}	
		if(value == 100){

			for(auto i=ingameMonsters.begin();i!=ingameMonsters.end();i++){
				if((abs(pacman.getX()-i->getX()) <0.5 && abs(pacman.getY()-i->getY()) <0.5) && !i->getMode().compare("panic")){
					i->setMode("eaten");
					i->getImmune();
					i->setColor(sf::Color(0,0,255,127));
					i->setSpeed(1.3*refSpeed);
				}
			}
		}
	}

	if(plate.getNbrFood() == 0)
		gameState = 1;
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

void Board::updateSickness(){

	if(!plague && rdmTime == (size_t) timePlayed){

		unsigned int i = std::rand() % 4;
		if(i > ingameMonsters.size())
			monsters[i].getContagious();
		else
			ingameMonsters[i].getContagious();
		plague = true;
	}

	plate.updateSickness();

	for(auto monster = monsters.begin(); monster < monsters.end(); monster++){

		Tile monsterTile = plate.getTile((size_t)monster->getX(),(size_t)monster->getY());
		
		if(monster->isContagious() && !monster->isSick()){
		
			if(monster->illTime() > contTime)
				monster->getContaminated();
		
		}else if(monster->isSick() && monster->illTime() > sickTime)
			monster->getImmune();

		else if(monster->sickable() && monsterTile.isContaminated())
			monster->getContagious();
	}

	for(auto monster = ingameMonsters.begin(); monster < ingameMonsters.end(); monster++){

		Tile monsterTile = plate.getTile((size_t)monster->getX(),(size_t)monster->getY());
		
		if(monster->isContagious() && !monster->isSick()){
		
			if(monster->illTime() > contTime)
				monster->getContaminated();
		
		}else if(monster->isSick() && monster->illTime() > sickTime)
			monster->getImmune();

		else if(monster->sickable() && monsterTile.isContaminated())
			monster->getContagious();
	}

	Tile pacTile = plate.getTile((size_t)pacman.getX(),(size_t)pacman.getY());
	
	if(pacman.isContagious() && !pacman.isSick()){
	
		if(pacman.illTime() > contTime)
			pacman.getContaminated();
	
	}else if(pacman.isSick() && pacman.illTime() > sickTime)
		pacman.getImmune();

	else if(pacman.sickable() && pacTile.isContaminated())
		pacman.getContagious();
}
