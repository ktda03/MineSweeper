#include "Tile.h"
Tile::Tile() {
	this->coordinates.x = 0;
	this->coordinates.y = 0;
	this->image = sf::Sprite(TextureManager::GetTexture("tile_hidden"));
	this->image.setPosition(coordinates.x, coordinates.y);
	this->revealedTile = sf::Sprite(TextureManager::GetTexture("tile_revealed"));
	this->revealedTile.setPosition(coordinates.x, coordinates.y);
	this->mineNumber = 0;
	this->isFlag = false;
	this->canClick = true;
	this->isRevealed = false;
}

Tile::Tile(sf::Vector2f xyCoordinates, const char* texture, int mineNumber) {
	this->coordinates = xyCoordinates;
	this->image = sf::Sprite(TextureManager::GetTexture(texture));
	this->image.setPosition(xyCoordinates.x, xyCoordinates.y);
	this->revealedTile = sf::Sprite(TextureManager::GetTexture("tile_revealed"));
	this->revealedTile.setPosition(xyCoordinates.x, xyCoordinates.y);
	//Checking if the tile is a mine to initialize isMine
	this->mineNumber = mineNumber;
	if (mineNumber == 9) {
		this->isMine = true;
	}
	else {
		this->isMine = false;
	}
	this->isFlag = false;
	this->canClick = true;
	this->isRevealed = false;
}

void Tile::SetImage(const char* texture) {
	this->image.setTexture(TextureManager::GetTexture(texture));
	this->image.setPosition(coordinates);
}

void Tile::SetRevealedTile(const char* texture) {
	this->revealedTile.setTexture(TextureManager::GetTexture(texture));
	this->revealedTile.setPosition(coordinates);
}

void Tile::SetDebugFlag(const char* texture) {
	this->debugFlag.setTexture(TextureManager::GetTexture(texture));
	this->debugFlag.setPosition(coordinates);
}

sf::Sprite& Tile::GetImage(){
	return this->image;
}

sf::Sprite& Tile::GetRevealedTile() {
	return this->revealedTile;
}

sf::Sprite& Tile::GetDebugFlag() {
	return this->debugFlag;
}

void Tile::newNeighbor(Tile* neighborTile){
	this->neighborTiles.push_back(neighborTile);
}

bool Tile::RevealTile() {
	bool clickedMine = false;
	if (this->canClick) {
		if (this->isRevealed == false) {
			this->isRevealed = true;
			this->canClick = false;
			switch (this->mineNumber) {
				case 0:
					SetImage("tile_revealed");
					clickedMine = false;
					//Recursive if the tile is empty
					for (int i = 0; i < neighborTiles.size(); i++) {
						this->neighborTiles[i]->RevealTile();
					}
					break;
				case 1:
					SetImage("number_1");
					clickedMine = false;
					break;
				case 2:
					SetImage("number_2");
					clickedMine = false;
					break;
				case 3:
					SetImage("number_3");
					clickedMine = false;
					break;
				case 4:
					SetImage("number_4");
					clickedMine = false;
					break;
				case 5:
					SetImage("number_5");
					clickedMine = false;
					break;
				case 6:
					SetImage("number_6");
					clickedMine = false;
					break;
				case 7:
					SetImage("number_7");
					clickedMine = false;
					break;
				case 8:
					SetImage("number_8");
					clickedMine = false;
					break;
				case 9:
					SetImage("mine");
					clickedMine = true;
					break;
			}
			return clickedMine;
		}
	}
	return clickedMine;
}

void Tile::PlaceFlag() {
	if (this->isFlag == false && this->canClick == true) {
		if (this->isRevealed == false) {
			this->canClick = false;
			this->isFlag = true;
			SetRevealedTile("tile_hidden");
			SetImage("flag");
		}
		else {
			cout << "Cannot place flag here" << endl;
		}
	}
	else {
		if (this->isRevealed == false) {
			this->canClick = true;
			this->isFlag = false;
			SetRevealedTile("tile_revealed");
			SetImage("tile_hidden");
		}
		else {
			cout << "Cannot place flag here" << endl;
		}
	}
}