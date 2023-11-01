#pragma once
#include <iostream>
#include <vector>
#include "TextureManager.h"
using namespace std;

class Tile
{
private:
	sf::Vector2f coordinates;
	sf::Sprite image;
	sf::Sprite revealedTile;
	sf::Sprite debugFlag;

public:
	//1-8 is the number, empty = 0, mine = 9
	bool isMine;
	bool isFlag;
	bool canClick;
	bool isRevealed;
	int mineNumber;
	vector<Tile*> neighborTiles;

	//Default Tile Constructor
	Tile();
	//Tile Constructor
	Tile(sf::Vector2f xyCoordinates, const char* texture, int _mineNumber);
	sf::Sprite& GetImage();
	sf::Sprite& GetRevealedTile();
	sf::Sprite& GetDebugFlag();
	void SetImage(const char* texture);
	void SetRevealedTile(const char* texture);
	void SetDebugFlag(const char* texture);

	void newNeighbor(Tile* neighborTile);
	bool RevealTile();
	void PlaceFlag();
};


