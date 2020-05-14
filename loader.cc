#include"loader.hh"

Loader::Loader(){
	countedFood=0;
	nbrRow=0;
	nbrCol=0;
}
std::vector<std::vector<Tile>> Loader::loadPlate(){
	iFile.open("plate.saved");
	std::string buffer;
	std::string tok;
	std::vector<std::vector<Tile>> table;
	nbrCol=0;

	while(getline(iFile,buffer)){
			table.push_back(std::vector<Tile>());
			nbrRow=0;
		while(nbrRow<buffer.length()){
			//Simple nourriture
			std::array<size_t,2> pos;
			pos[0]=nbrCol;
			pos[1]=nbrRow;
			//Tile vide par défaut
			Tile tile(0,false,pos);
			//Nourriture
			if(buffer[nbrRow] == '.'){
				tile.setType(1);
				countedFood++;
			}
			if(buffer[nbrRow] == 'T'){
				tile.setType(3);
			}
			//Hors du maze
			if(buffer[nbrRow] == 'o'){
				tile.setType(2);
				tile.setWall(true);
			}
			//Pilz
			if(buffer[nbrRow] == '0'){
				tile.setType(2);
				countedFood++;
			}
			//Mur
			if(buffer[nbrRow] =='%'){
				tile.setType(0);
				tile.setWall(true);
			}
			//maison fantome
			if(buffer[nbrRow] =='/'){
				tile.setType(1);
				tile.setWall(true);
			}
			table[nbrCol].push_back(tile);
			nbrRow++;
		}

	nbrCol++;
	}
	iFile.close();

	return table;
}
size_t Loader::getCountedFood() const{
	return countedFood;
}
size_t Loader::getNbrRow() const{
	return nbrRow;
}
size_t Loader::getNbrCol() const{
	return nbrCol;
}