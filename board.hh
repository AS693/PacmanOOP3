#ifndef BOARD_HH
#define BOARD_HH
#include<vector>
#include<math.h>
#include"plate.hh"
#include"monster.hh"
#include"pacman.hh"
#include"tile.hh"
#include<iostream>

class Board{
public:
	Board(size_t size,float refSpeed);
	Plate getPlate();
	size_t getTileSize();
	size_t getScore();
	Pacman getPacman();
	void printInPrompt();
	void playerMove();
	void monsterMove();
	void monsterOutOfHouse(Monster *monster);
	void teleport(Player &p);
	void drawBoard(sf::RenderWindow *window);
private:
	Pacman pacman;
	std::array<Monster,4> monsters;
	Plate plate;
	size_t score;
	size_t tileSize;
	float refSpeed;
	const float hiddenTime = 100.0;
	char secondMove;

	void move(char);
	Tile getTileNext(size_t x, size_t y, char dir);
};
#endif