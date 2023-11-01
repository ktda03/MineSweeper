#pragma once
#include "TextureManager.h"
#include "Tile.h"
#include "Random.h"
#include <vector>
using std::vector;

class Board
{
	int height;
	int width;
	vector<Tile> tiles;
	int mines;
	int flags;
public:
	bool gameOver = false;
	bool victory = false;
	bool inDebug = false;

	Board(int height, int width, int mines);
	void NewBoard();
	void TestBoard(vector<char> testTiles);

	Tile& GetTile(unsigned int tileIndex);
	int GetWidth();
	int GetHeight();
	int GetMines();
	int GetFlags();

	void CheckGameOver();
	void CheckVictory();
	void DebugMode();
	void TileNeighbors();
};

