/*----------------------------------------------
Programmer: Anthony LaRosa - al8249@rit.edu
Date: 2020/08
----------------------------------------------*/
#ifndef __TILEMANAGER_H_
#define __TILEMANAGER_H_

#include "Tile.h"
#include "PriQueue.h"

namespace Simplex
{

class TileManager
{
	/*variables*/
	uint arraySize;
	
	
	std::vector<Tile*> shortPath;

	Tile* start;
	Tile* end;

	PriQueue* priQueue;
	
	/*methods*/
	//A*
	//display
	//create tiles
	//make Path
	//reset
	
public:
	Tile* tiles[10][10];

	TileManager();

	TileManager& operator=(TileManager const& other);

	~TileManager();

	void Swap(TileManager& other);

	void AStar();

	void SetTileDistances();

	void MakePath();

	void ResetTiles();

#pragma region Accessors
	uint GetArraySize();

	Tile* GetStart();

	void SetStart(Tile* a_start);

	Tile* GetEnd();

	void SetEnd(Tile* a_end);

	std::vector<Tile*> GetShortPath();
#pragma endregion

private:
	void Init();

	void Release();

};

}

#endif