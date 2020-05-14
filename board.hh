#ifndef BOARD_HH
#define BOARD_HH
#include<vector>
#include<math.h>
#include"plate.hh"
#include"monster.hh"
#include"pacman.hh"
#include"tile.hh"
#include <ctime>
#include<iostream>
#include <ctime>

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
	void monsterOutOfHouse(Monster &monster);
	void teleport(Player &p);
	void drawBoard(sf::RenderWindow *window);
	void incIngameMonsters();
	bool isEnded() const;

private:
	Pacman pacman;
	std::vector<Monster> ingameMonsters;
	std::array<Monster,4> monsters;
	Plate plate;
	size_t score;
	size_t tileSize;

	float refSpeed;
	char lastShortcut = 'e';

	float hiddenTime = 30.0;
	float scatterTime = 9.0;
	float chaseTime = 30.0;
	float panicTime = 7.0;

	float timePlayed = 0.0;
	std::clock_t start;

	float contTime = 20.0;
	float sickTime = 10.0;
	size_t rdmTime;
	bool plague = false;
	short int gameState = 2;

	void move(char);
	Tile getTileNext(size_t x, size_t y, char dir);
	bool isCloseEnough(std::array<float,2>, Tile, char);
	bool isPerpendicular(char x, char y);
	bool isBelowCenter(std::array<float,2> p, char direction);
	void updateSickness();
	void activate();
	void drawText(sf::RenderWindow *window);
};
#endif