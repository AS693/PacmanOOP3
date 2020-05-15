#include"plate.hh"
#include<iostream>
#include "constants.hpp"

Plate::Plate(){
	Loader loader;
	plat = loader.loadPlate();
	lengthCol = loader.getNbrRow();
	lengthRow = loader.getNbrCol();
	nbrFood = loader.getCountedFood();
}
size_t Plate::getNbrFood() const{
	return nbrFood;
}
size_t Plate::getLengthRow() const{
	return lengthRow;
}
size_t Plate::getLengthCol()const{
	return lengthCol;
}
Tile Plate::getTile(size_t x,size_t y) const{
	
	return plat[x][y];
}

Tile* Plate::getTilePointer(size_t x,size_t y){

	return &plat[x][y];
}

void Plate::decountFood(){
	nbrFood--;
}

void Plate::setTile(Tile t){
	plat[t.getX()][t.getY()] = t;
}
void Plate::drawPlate(sf::RenderWindow *window,size_t tileSize){
// /!\ utilisation de k et p comme itérateur et pas de la position de Tile
	// Parce que celle-ci est corrompue par endroit (à vérifier)
		for(size_t i=0;i<lengthRow;i++){
			for(size_t j=0; j<lengthCol;j++){
				auto t = plat[i][j];

				if(t.isFood()){
					sf::CircleShape cercle(tileSize/6);
					cercle.setPosition(j*tileSize+tileSize/3,i*tileSize+tileSize/3);

					if(t.isContaminated() && constants::DEBUG)
						cercle.setFillColor(sf::Color(255,0,255));
					else	
						cercle.setFillColor(sf::Color(247,192,158));

					window->draw(cercle);
				}
				if(t.isWall()){
					refineWall(window,&t,tileSize);

				}

				if(t.isEmpty() || t.isTunnel()){
					sf::RectangleShape rectangle(sf::Vector2f(tileSize,tileSize));
					rectangle.setPosition(j*tileSize,i*tileSize);
					rectangle.setFillColor(sf::Color(0,0,0));
					window->draw(rectangle);
				}
				if(t.isOut()){
					sf::RectangleShape rectangle(sf::Vector2f(tileSize,tileSize));
					rectangle.setPosition(j*tileSize,i*tileSize);
					rectangle.setFillColor(sf::Color(0,0,0));
					window->draw(rectangle);
				}
				if(t.isPilz()){
					sf::CircleShape cercle(tileSize/2);
					cercle.setPosition(j*tileSize,i*tileSize);

					if(t.isContaminated() && constants::DEBUG)
						cercle.setFillColor(sf::Color(255,0,255));
					else	
						cercle.setFillColor(sf::Color(247,192,158));

					window->draw(cercle);
				}
				if(t.isFantomHouse()){
					sf::RectangleShape rectangle(sf::Vector2f(tileSize,tileSize));
					rectangle.setPosition(j*tileSize,(i+0.5)*tileSize);
					rectangle.setFillColor(sf::Color(0,0,0));
					window->draw(rectangle);
				}
				if(t.getY() == 15 && (t.getX() == 13 || t.getX() == 14)){
					sf::RectangleShape rectangle2(sf::Vector2f(tileSize,tileSize/2));
					rectangle2.setPosition(j*tileSize,(i+0.5)*tileSize);
					rectangle2.setFillColor(sf::Color(255,255,255));
					window->draw(rectangle2);
				}
				//Trace les lignes blanches entre les cases à supprimer plus tard
				if(constants::DEBUG){
					sf::VertexArray line(sf::LineStrip,2);
					line[0].position = sf::Vector2f(j*tileSize,i*tileSize);
					line[1].position = sf::Vector2f((j+1)*tileSize,i*tileSize);
					window->draw(line);
					line[0].position = sf::Vector2f(j*tileSize,i*tileSize);
					line[1].position = sf::Vector2f(j*tileSize,(i+1)*tileSize);
					window->draw(line);
					line[0].position = sf::Vector2f((j+1)*tileSize,i*tileSize);
					line[1].position = sf::Vector2f((j+1)*tileSize,(i+1)*tileSize);
					window->draw(line);
					line[0].position = sf::Vector2f(j*tileSize,(i+1)*tileSize);
					line[1].position = sf::Vector2f((j+1)*tileSize,(i+1)*tileSize);
					window->draw(line);
				}
			}


		}

}
void Plate::drawAngle(sf::RenderWindow *window,size_t type,const Tile *t,size_t tileSize){
	//Grand rectangle
	size_t row = t->getX();
	size_t col = t->getY();
	sf::RectangleShape rectangle(sf::Vector2f(tileSize,tileSize));
	rectangle.setPosition(row*tileSize,col*tileSize);
	rectangle.setFillColor(sf::Color(160,160,160));
	//Petit rectangle noir
	sf::RectangleShape rectangleSmall(sf::Vector2f(tileSize/2,tileSize/2));
	rectangleSmall.setFillColor(sf::Color(0,0,0));
	switch(type){
		case 1:
			rectangleSmall.setPosition(row*tileSize,col*tileSize);
		break;
		case 2:
			rectangleSmall.setPosition((row+0.5)*tileSize,(col+0.5)*tileSize);
		break;
		case 3:
			rectangleSmall.setPosition((row+0.5)*tileSize,col*tileSize);
		break;
		case 4:
			rectangleSmall.setPosition(row*tileSize,(col+0.5)*tileSize);
		break;
	}
	window->draw(rectangle);
	window->draw(rectangleSmall);

}
void Plate::drawLine(sf::RenderWindow *window,size_t type,const Tile *t,size_t tileSize){
	//Grand rectangle
	size_t row = t->getX();
	size_t col = t->getY();
	sf::RectangleShape rectangle(sf::Vector2f(tileSize,tileSize));
	rectangle.setPosition(row*tileSize,col*tileSize);
	rectangle.setFillColor(sf::Color(160,160,160));
	sf::RectangleShape rectangleSmall(sf::Vector2f(tileSize,tileSize/2));
	rectangleSmall.setFillColor(sf::Color(0,0,0));

	switch(type){

		case 0:
			window->draw(rectangle);
			return;
		break;

		case 1:
			rectangleSmall.setPosition(row*tileSize,(col+0.5)*tileSize);
		break;

		case 2:
			rectangleSmall.setSize(sf::Vector2f(tileSize,tileSize/2));
			rectangleSmall.setPosition(row*tileSize,col*tileSize);
		break;

		case 3:
			rectangleSmall.setSize(sf::Vector2f(tileSize/2,tileSize));
			rectangleSmall.setPosition((row+0.5)*tileSize,col*tileSize);
		break;

		case 4:
			rectangleSmall.setSize(sf::Vector2f(tileSize/2,tileSize));
			rectangleSmall.setPosition(row*tileSize,col*tileSize);
		break;
	}

	window->draw(rectangle);
	window->draw(rectangleSmall);

}
void Plate::drawCorner(sf::RenderWindow *window,const Tile *t,size_t tileSize){
	size_t col = t->getX();
	size_t row = t->getY();
	if(row <=0 || row>= lengthRow-1 || col<=0 || col>= lengthCol-1 || isAngle(t))
		return;

	sf::RectangleShape rectangle(sf::Vector2f(tileSize,tileSize));
	rectangle.setPosition(col*tileSize,row*tileSize);
	rectangle.setFillColor(sf::Color(0,0,0));
	sf::RectangleShape rectangleSmall(sf::Vector2f(tileSize/2,tileSize/2));
	rectangleSmall.setFillColor(sf::Color(160,160,160));

	Tile tu = plat[row-1][col];
	Tile td = plat[row+1][col];
	Tile tl = plat[row][col-1];;
	Tile tr = plat[row][col+1];

	if(tu.isWall() && tr.isWall()){
		rectangleSmall.setPosition((col+0.5)*tileSize,row*tileSize);
	}
	
	if(tu.isWall() & tl.isWall())
		rectangleSmall.setPosition(col*tileSize,row*tileSize);

	if(td.isWall() & tr.isWall())
		rectangleSmall.setPosition((col+0.5)*tileSize,(row+0.5)*tileSize);

	if(td.isWall() & tl.isWall())
		rectangleSmall.setPosition(col*tileSize,(row+0.5)*tileSize);
	
	//rempli les cases entourées de murs
	if(tr.isWall() & tl.isWall() & td.isWall() & tu.isWall()){
		rectangle.setFillColor(sf::Color(160,160,160));
		window->draw(rectangle);
		return;
	}
	window->draw(rectangle);
window->draw(rectangleSmall);
}
size_t Plate::isAngle(const Tile *t){
	size_t col = t->getX();
	size_t row = t->getY();
	Tile tu = plat[row-1][col];
	Tile td = plat[row+1][col];
	Tile tl;
	Tile tr;

	if(col == lengthCol-1){
		tl = plat[row][col-1];
		if(td.isWall() && tl.isWall() && plat[row+1][col-1].isPlayable())
			return 4;
		if(tu.isWall() && tl.isWall() && plat[row-1][col-1].isPlayable())
			return 1;
	}

	else if(col == 0){
		tr = plat[row][col+1];
		if(td.isWall() && tr.isWall() && plat[row+1][col+1].isPlayable())
			return 2;
		if(tu.isWall() && tr.isWall() && plat[row-1][col+1].isPlayable())
			return 3;
	}

	else{
		tl = plat[row][col-1];
		tr = plat[row][col+1];
		if(tu.isWall() && tl.isWall() && (plat[row-1][col-1].isPlayable() && !td.isPlayable()))
			return 1;
		if(td.isWall() && tr.isWall() && (plat[row+1][col+1].isPlayable() && !tu.isPlayable()))
			return 2;
		if(td.isWall() && tl.isWall() && (plat[row+1][col-1].isPlayable() && !tu.isPlayable()))
			return 4;
		if(tu.isWall() && tr.isWall() && (plat[row-1][col+1].isPlayable() && !td.isPlayable()))
			return 3;
	}
	//not a corner
	return 0;
}
size_t Plate::isLine(const Tile *t){
	size_t col = t->getX();
	size_t row = t->getY();
	//Cas de base
	if(row <=0 || row>= lengthRow-1 || col<0 || col> lengthCol-1)
		return 0;

	Tile tu = plat[row-1][col];
	Tile td = plat[row+1][col];
	Tile tl;
	Tile tr;

	if(col == lengthCol-1){
		tl = plat[row][col-1];
		if(tl.isWall() && td.isPlayable())
			return 1;
		if(tl.isWall() && tu.isPlayable())
			return 2;
		if(tu.isWall() && td.isWall() && tl.isPlayable())
			return 4;
	}

	else if(col == 0){
		tr = plat[row][col+1];
		if(tr.isWall() && td.isPlayable())
			return 1;
		if(tr.isWall() && tu.isPlayable())
			return 2;
		if(tu.isWall() && td.isWall() && tr.isPlayable())
			return 3;
	}

	else{
		tl = plat[row][col-1];
		tr = plat[row][col+1];

		//pour la maison de fantôme
		
		if((tr.isWall() && tl.isWall() && tu.isPlayable()) || plat[row+1][col].isFantomHouse())
			return 2;
		
		if((tr.isWall() && tl.isWall() && td.isPlayable()) || plat[row-1][col].isFantomHouse())
			return 1;
		
		if(tu.isWall() && td.isWall() && tl.isPlayable() && !plat[row][col-1].isOut() && !plat[row][col-1].isFantomHouse())
			return 4;

		if((tu.isWall() && td.isWall() && tr.isPlayable()) || plat[row][col-1].isFantomHouse())
			return 3;
		
		

	}

	//not a line
	return 0;
}
void Plate::refineWall(sf::RenderWindow *window,const Tile *t,size_t tileSize){

	size_t cornerType = isAngle(t);
	size_t lineType;

	if(cornerType){
		drawAngle(window,cornerType,t,tileSize);
	}
	lineType = isLine(t);
	if(lineType)
		drawLine(window,lineType,t,tileSize);
	else
		drawCorner(window,t,tileSize);

}

void Plate::Contaminate(Tile* t){
	t->getContaminated();
	listCont.push_back(t);
}

void Plate::updateSickness(){

	for(size_t i = 0; i < listCont.size(); i++){

		float time = ( std::clock() - listCont[i]->getConTime() ) / (float) CLOCKS_PER_SEC;

		if(time > sickTime){
			listCont[i]->unContaminate();
			listCont.erase(listCont.begin() + i);
			i--;
		}
	}
}