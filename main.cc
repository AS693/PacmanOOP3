#include<SFML/Window.hpp>
#include<SFML/Graphics.hpp>

#include<string>
#include<iostream>
#include"board.hh"
#include"plate.hh"


int main(){
	Plate plate;
	Board board(20,0.2);
	sf::RenderWindow window(sf::VideoMode(board.getPlate().getLengthCol()*board.getTileSize(),board.getPlate().getLengthRow()*board.getTileSize()),"Pacorona");
	size_t k=0;

	while(window.isOpen()){
		sf::Event event;

		window.clear();
		board.drawBoard(&window);
		board.playerMove();
		board.monsterMove();

		k++;
		window.display();
		while(window.pollEvent(event)){
			if(event.type == sf::Event::Closed)
				window.close();
		}
	}

	return 0;

}