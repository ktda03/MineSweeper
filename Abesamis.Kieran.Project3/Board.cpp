#include "Board.h"


Board::Board(int height, int width, int mines) {
	this->width = width;
	this->height = height;
	this->mines = mines;
	this->flags = 0;
}


void Board::NewBoard() {
	//Resetting Board
	this->tiles.clear();
	this->gameOver = false;
	this->victory = false;
	//Creating board of empty tiles
	Tile newTile;
	for (int i = 0; i < this->height; i++) {
		for (int j = 0; j < this->width; j++) {
			sf::Vector2f coordinates = sf::Vector2f((float)(j) * 32, float(i) * 32);
			//Creating a single tile at designated xy coordinates, empty tile for now
			newTile = Tile(coordinates, "tile_hidden", 0);
			this->tiles.push_back(newTile);
		}
	}

	//Generating list of mine locations
	vector<int> mineLocations;
	mineLocations.push_back(Random::Int(0, (this->height * this->width) - 1));
	while (mineLocations.size() < this->mines) {
		int newMine = Random::Int(0, (this->height * this->width) - 1);
		bool duplicateMine = false;
		for (int k = 0; k < mineLocations.size(); k++) {
			if (newMine == mineLocations[k]) {
				duplicateMine = true;
				break;
			}
		}
		if (duplicateMine == false) {
			mineLocations.push_back(newMine);
		}
	}

	//Setting mineNumber as 9, isMine to true
	for (int l = 0; l < mineLocations.size(); l++) {
		this->tiles[mineLocations[l]].mineNumber = 9;
		this->tiles[mineLocations[l]].isMine = true;
		if (this->inDebug) {
			this->tiles[mineLocations[l]].SetRevealedTile("hidden");
		}
	}

	TileNeighbors();
}

void Board::TestBoard(vector<char> testTiles) {
	//Resetting Board
	this->tiles.clear();
	this->gameOver = false;
	this->victory = false;
	this->mines = 0;
	
	Tile newTile;
	for (int i = 0; i < this->height; i++) {
		for (int j = 0; j < this->width; j++) {
			sf::Vector2f newCoordinates = sf::Vector2f((float)(j) * 32, (float)(i) * 32);
			int index = (this->width * i) + j;
			if (testTiles[index] == '1') {
				if (this->inDebug) {
					newTile = Tile(newCoordinates, "mine", 9);
					newTile.SetRevealedTile("tile_hidden");
				}
				else {
					newTile = Tile(newCoordinates, "tile_hidden", 9);
				}
				this->mines += 1;
			}
			else {
				newTile = Tile(newCoordinates, "tile_hidden", 0);
			}
			tiles.push_back(newTile);
		}
	}
	TileNeighbors();
}

Tile& Board::GetTile(unsigned int tileIndex) {
	return this->tiles[(tileIndex)];
}

int Board::GetWidth() {
	return this->width;
}

int Board::GetHeight() {
	return this->height;
}

int Board::GetMines() {
	return this->mines;
}
int Board::GetFlags() {
	int flagCount = 0;
	for (int i = 0; i < height * width; i++) {
		if (tiles.at(i).isFlag == true) {
			flagCount += 1;
		}
	}
	this->flags = flagCount;
	return this->flags;
}

void Board::CheckGameOver() {
	if (this->gameOver == true) {
		this->victory = false;
		for (int i = 0; i < this->tiles.size(); i++) {
			this->tiles[i].canClick = false;
			this->tiles[i].isRevealed = true;
			if (this->tiles[i].isMine == true) {
				this->tiles[i].SetImage("mine");
				if(this->tiles[i].isFlag){
					this->tiles[i].SetDebugFlag("flag");
				}
			}
		}
	}
	else {
		this->gameOver == false;
	}
}

void Board::CheckVictory() {
	int revealedTiles = 0;
	//checking and counting if all tiles that are not mines are revealed
	for (int i = 0; i < tiles.size(); i++) {
		if (this->tiles[i].isRevealed == true) {
			revealedTiles += 1;
		}
	}
		//If all tiles are revealed besides mines which must be flagged, the player has won
	if (this->tiles.size() - revealedTiles - GetFlags() == 0) {
		for (int i = 0; i < this->tiles.size(); i++) {
			if (this->tiles[i].isMine == true && this->tiles[i].isFlag == false) {
				this->victory = false;
				break;
			}
			else if (this->tiles[i].isMine == false && this->tiles[i].isFlag == true) {
				this->victory = false;
				break;
			}
			else {
				this->victory = true;
			}
		}
	}
	else {
		this->victory = false;
	}
	if (this->victory == true) {
		for (int i = 0; i < this->tiles.size(); i++) {
			tiles[i].canClick = false;
			tiles[i].isRevealed = true;
		}
	}
}


void Board::DebugMode() {
	if (inDebug == false) {
		inDebug = true;
		for (int i = 0; i < tiles.size(); i++) {
			if (tiles[i].isMine == true && tiles[i].isFlag == true) {
				tiles[i].SetDebugFlag("flag");
				tiles[i].SetImage("mine");
			}
			else if(tiles[i].isMine == true && tiles[i].isFlag == false) {
				tiles[i].SetDebugFlag("tile_hidden");
				tiles[i].SetImage("mine");
			}
		}
	}
	else {
		inDebug = false;
		for (int i = 0; i < tiles.size(); i++) {
			if (tiles[i].isMine == true && tiles[i].isFlag == true) {
				tiles[i].SetDebugFlag("tile_hidden");
				tiles[i].SetImage("flag");
			}
			else if (tiles[i].isMine == true && tiles[i].isFlag == false) {
				tiles[i].SetRevealedTile("tile_hidden");
				tiles[i].SetImage("tile_hidden");
			}
		}
	}
}

void Board::TileNeighbors() {
	//Going thru all tiles and adding their neighbors
	for (int i = 0; i < tiles.size(); i++) {
		//Setting top left corner tile's neighbors
		if (i == 0) {
			this->tiles[i].newNeighbor(&tiles[1]);
			this->tiles[i].newNeighbor(&tiles[width]);
			this->tiles[i].newNeighbor(&tiles[width + 1]);
		}
		//Setting bottom left corner tile's neighbors
		else if (i == (width * height) - width) {
			this->tiles[i].newNeighbor(&tiles[i - width]);
			this->tiles[i].newNeighbor(&tiles[i - width + 1]);
			this->tiles[i].newNeighbor(&tiles[i + 1]);
		}
		//Setting top right corner tile's neighbors
		else if (i == width - 1) {
			this->tiles[i].newNeighbor(&tiles[i - 2]);
			this->tiles[i].newNeighbor(&tiles[i * 2]);
			this->tiles[i].newNeighbor(&tiles[(i * 2) + 1]);
		}
		//Setting bottom right corner tile's neighbors
		else if (i == (width * height) - 1) {
			this->tiles[i].newNeighbor(&tiles[i - width]);
			this->tiles[i].newNeighbor(&tiles[i - width - 1]);
			this->tiles[i].newNeighbor(&tiles[i - 1]);
		}
		//Setting left edge tiles
		else if (i % width == 0) {
			this->tiles[i].newNeighbor(&tiles[i + 1]);
			this->tiles[i].newNeighbor(&tiles[i + width]);
			this->tiles[i].newNeighbor(&tiles[i - width]);
			this->tiles[i].newNeighbor(&tiles[i - width + 1]);
			this->tiles[i].newNeighbor(&tiles[i + width + 1]);
		}
		//Setting top edge tiles
		else if (i > 0 && i < width - 1) {
			this->tiles[i].newNeighbor(&tiles[i - 1]);
			this->tiles[i].newNeighbor(&tiles[i + 1]);
			this->tiles[i].newNeighbor(&tiles[i + width]);
			this->tiles[i].newNeighbor(&tiles[i + width + 1]);
			this->tiles[i].newNeighbor(&tiles[i + width - 1]);
		}
		//Setting right edge tiles
		else if ((i + 1) % width == 0) {
			this->tiles[i].newNeighbor(&tiles[i - 1]);
			this->tiles[i].newNeighbor(&tiles[i + width]);
			this->tiles[i].newNeighbor(&tiles[i + width - 1]);
			this->tiles[i].newNeighbor(&tiles[i - width]);
			this->tiles[i].newNeighbor(&tiles[i - width - 1]);
		}
		//Setting bottom edge tiles
		else if (i > (width * height) - width && i < (width * height) - 1) {
			this->tiles[i].newNeighbor(&tiles[i + 1]);
			this->tiles[i].newNeighbor(&tiles[i - 1]);
			this->tiles[i].newNeighbor(&tiles[i - width]);
			this->tiles[i].newNeighbor(&tiles[i - width + 1]);
			this->tiles[i].newNeighbor(&tiles[i - width - 1]);
		}
		//Setting "middle" tiles
		else {
			this->tiles[i].newNeighbor(&tiles[i + 1]);
			this->tiles[i].newNeighbor(&tiles[i - 1]);
			this->tiles[i].newNeighbor(&tiles[i + width]);
			this->tiles[i].newNeighbor(&tiles[i - width]);
			this->tiles[i].newNeighbor(&tiles[i + width + 1]);
			this->tiles[i].newNeighbor(&tiles[i - width + 1]);
			this->tiles[i].newNeighbor(&tiles[i + width - 1]);
			this->tiles[i].newNeighbor(&tiles[i - width - 1]);
		}
	}
	//Setting numbers for neighbor tiles if necessary
	int nearbyMines = 0;
	for (int j = 0; j < this->tiles.size(); j++) {
		//Checking if mine
		if (this->tiles[j].isMine == false) {
			nearbyMines = 0;
			for (int k = 0; k < this->tiles[j].neighborTiles.size(); k++) {
				Tile* currentTile = this->tiles[j].neighborTiles[k];
				if (currentTile->isMine == true) {
					nearbyMines += 1;
				}
			}
			switch (nearbyMines) {
			case 0:
				this->tiles[j].mineNumber = 0;
				break;
			case 1:
				this->tiles[j].mineNumber = 1;
				break;
			case 2:
				this->tiles[j].mineNumber = 2;
				break;
			case 3:
				this->tiles[j].mineNumber = 3;
				break;
			case 4:
				this->tiles[j].mineNumber = 4;
				break;
			case 5:
				this->tiles[j].mineNumber = 5;
				break;
			case 6:
				this->tiles[j].mineNumber = 6;
				break;
			case 7:
				this->tiles[j].mineNumber = 7;
				break;
			case 8:
				this->tiles[j].mineNumber = 8;
				break;
			}
		}
	}
}
