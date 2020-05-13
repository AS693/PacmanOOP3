#include"tile.hh"
#include<iostream>
Tile::Tile(size_t ty,bool wa,const std::array<size_t,2> pos){
	position[0]= pos[0];
	position[1]=pos[1];
	type=ty;
	wall=wa;
	contaminated = false;
}
 void Tile::operator =(const Tile &tile){
 	std::array<size_t,2> pos;
 	pos[0] = tile.getX();
 	pos[1] = tile.getY();
	position = pos;
	type = tile.getType();
	wall = tile.isWall();
	contaminated = tile.contaminated;
}
bool Tile::operator !=(Tile const &tile){
 	return (position[1] != tile.getX() || position[0] != tile.getY() || type != tile.getType() || wall != tile.isWall());
}
bool Tile::operator ==(Tile const &tile){

 	return (position[1] == tile.getX() && position[0] == tile.getY() && type == tile.getType() && wall == tile.isWall());
}
size_t Tile::getType() const{
	return type;
}
bool Tile::isWall() const{
	return (wall==true) && (type==0);
}
bool Tile::isTunnel() const{
	return (wall ==false) && (type ==3);
}
bool Tile::isFantomHouse() const{
	return (wall==true) && (type==1);
}
bool Tile::isEmpty()const{
	return (type ==0) && (wall==false);
}
bool Tile::isFood()const{
	return (type ==1) && (wall == false);
}
bool Tile::isPlayable()const{
	return isFood()||isEmpty()||isPilz()||isTunnel();
}
bool Tile::isPilz()const{
	return (type ==2) && (wall==false);
}
bool Tile::isOut() const{
	return (type==2) && (wall==true);
}
void Tile::setType(size_t ty){
	type = ty;
}
void Tile::setWall(bool wa){
	wall=wa;
}
size_t Tile::getY() const{
	return position[0];
}
size_t Tile::getX() const{
	return position[1];
}

void Tile::getContaminated(){

	if(isFood() || isPilz()){
		contaminated = true;
		contaminatedTime = clock();
	}
}

void Tile::unContaminate(){

	contaminated = false;
}

bool Tile::isContaminated() const{
	return contaminated;
}

clock_t Tile::getConTime() const{
	return contaminatedTime;
}