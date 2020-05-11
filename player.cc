#include"player.hh"

Player::Player(char ty,std::array<float,2> initPos){
	type=ty;
	rayon = 0;
	position = initPos;
	hidden = false;
}

void Player::move(char direction){

	if(direction=='u')
		position[0] -= currentSpeed;
	if(direction=='d')
		position[0] += currentSpeed;
	if(direction=='r')
		position[1] += currentSpeed;
	if(direction=='l')
		position[1] -= currentSpeed;
}

std::array<float,2> Player::getPosition() const{
	return position;
}
void Player::setSpeed(float sp){
	currentSpeed = sp;
}
void Player::setHide(bool state){
	if(state)
		hiddenTime = 0;
	hidden = state;
}
void Player::setSpeedInit(float speed){
	nominalSpeed = speed;
	currentSpeed = speed;
}
float Player::getSpeed() const{
	return nominalSpeed;
}
float Player::getX() const{
	return position[0];
}

float Player::getY() const{
	return position[1];
}

char Player::getType() const{
	return type;
	}

void Player::setType(char t){
	type = t;
}

void Player::setPosition(std::array<float,2> pos){
	position = pos;
}
float Player::getRayon() const{
	return rayon;
}
void Player::setRayon(float ray){
	rayon = ray;
}
void Player::setColor(sf::Color color){
	this->color = color;
}

void Player::drawPlayer(sf::RenderWindow *window,size_t tileSize,bool sha) const{

	if(!hidden){
		if(sha){
		sf::CircleShape shape(tileSize*rayon);
		shape.setOrigin(tileSize*getRayon(),tileSize*getRayon());
		shape.setPosition(position[1]*tileSize,position[0]*tileSize);
		shape.setFillColor(color);
		window->draw(shape);

		}
	else{
		sf::RectangleShape shape(sf::Vector2f(tileSize*rayon,tileSize*rayon));
		shape.setOrigin(tileSize*getRayon()/2,tileSize*getRayon()/2);
		shape.setPosition(position[1]*tileSize,position[0]*tileSize);
		shape.setFillColor(color);
		window->draw(shape);
	}
}
}
bool Player::getHide() const {
	return hidden;
}

void Player::updateHiddenClock(){
	hiddenClock = std::clock();
}

void Player::updateHiddenTime(){
	hiddenTime += ( std::clock() - hiddenClock ) / (float) CLOCKS_PER_SEC;
}

float Player::getHiddenTime(){
	return hiddenTime;
}

float Player::getCurrSpeed()
{
	return currentSpeed;
}