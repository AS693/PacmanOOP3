#include<SFML/Window.hpp>
#include<SFML/Graphics.hpp>

#include<string>
#include<iostream>
#include <cstdlib>
#include"board.hh"
#include"plate.hh"


int main(){
	Plate plate;
	Board board(24,0.05);
	sf::RenderWindow window(sf::VideoMode(board.getPlate().getLengthCol()*board.getTileSize(),board.getPlate().getLengthRow()*board.getTileSize()),"Pacorona");
	size_t k=0;
	srand(time(NULL));

	while(window.isOpen()){
		sf::Event event;

		window.clear();
		board.drawBoard(&window);
		board.playerMove();

		if(board.monsterMove()){
			std::cout << "END OF THE GAME "<< std::endl;
			//end game
			return 1;
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