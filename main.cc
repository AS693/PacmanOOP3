#include<SFML/Window.hpp>
#include<SFML/Graphics.hpp>

#include<string>
#include<iostream>
#include <cstdlib>
#include"board.hh"
#include"plate.hh"
#include "constants.hpp"

int main(){
//	Plate plate;
	Board board(constants::CELL_SIZE, 0.16);
	sf::RenderWindow window(sf::VideoMode(board.getPlate().getLengthCol()*board.getTileSize(),board.getPlate().getLengthRow()*board.getTileSize()),"Pacorona");
	size_t k=0;
	srand(time(NULL));

	while(window.isOpen()){
		sf::Event event;

		window.clear();
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::R))
			board = Board(constants::CELL_SIZE, 0.16);

		board.drawBoard(&window);
	
		if(!board.isEnded()){
			board.playerMove();
			board.monsterMove();
		}
		
		k++;
		window.display();
		while(window.pollEvent(event)){
			if(event.type == sf::Event::Closed)
				window.close();
		}
	}

	return 0;

}