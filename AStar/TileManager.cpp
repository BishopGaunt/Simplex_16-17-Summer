#include "TileManager.h"
using namespace Simplex;

void TileManager::Init()
{
	arraySize = 10;

	for (int i = 0; i < arraySize; i++)
	{
		for (int j = 0; j < arraySize; j++)
		{
			tiles[i][j] = nullptr;
		}
	}

	start = nullptr;
	end = nullptr;

	priQueue = new PriQueue();
}

TileManager::TileManager()
{
	Init();

	vector3 tempPos = vector3(0.0f);

	for (int i = 0; i < arraySize; i++)
	{
		for (int j = 0; j < arraySize; j++)
		{
			tempPos = vector3(arraySize - i, 0.0f, arraySize - j);
			tiles[i][j] = new Tile(tempPos, i, j);
		}
	}
	
	//remove a few blocks for obstacles
	tiles[5][4] = nullptr;
	tiles[5][5] = nullptr;
	tiles[6][4] = nullptr;
	tiles[6][5] = nullptr;
	tiles[6][6] = nullptr;
	tiles[7][4] = nullptr;
	tiles[7][5] = nullptr;
	tiles[7][6] = nullptr;

	tiles[2][7] = nullptr;
	tiles[3][7] = nullptr;
	tiles[3][8] = nullptr;

	tiles[1][2] = nullptr;
	tiles[1][3] = nullptr;
	tiles[2][3] = nullptr;
	tiles[3][3] = nullptr;

	tiles[4][1] = nullptr;
	tiles[5][1] = nullptr;
	tiles[6][1] = nullptr;
	tiles[7][1] = nullptr;
	tiles[8][1] = nullptr;
	tiles[8][2] = nullptr;

	start = tiles[0][0];
	end = tiles[9][9];
}

TileManager& TileManager::operator=(TileManager const& other)
{
	if (this != &other)
	{
		Release();
		Init();
		TileManager temp(other);
		Swap(temp);
	}
	return *this;
}

TileManager::~TileManager()
{
	Release();
}

void TileManager::Swap(TileManager& other)
{
	std::swap(tiles, other.tiles);

	std::swap(shortPath, other.shortPath);
	
	std::swap(end, other.end);
	std::swap(start, other.start);
}

void Simplex::TileManager::AStar()
{
	ResetTiles();

	SetTileDistances();

	Tile* target = start;
	target->SetWeight(0);
	target->ChangeVisited();
	int row = 0;
	int col = 0;

	while (target != end)
	{
		priQueue->Pop();

		row = target->GetRow();
		col = target->GetCol();

		//calculating the neighbors
		if (row != 0 && tiles[row - 1][col] != nullptr)
		{
			//tiles[row - 1][col]
			if (!tiles[row - 1][col]->CheckVisited() &&
				(target->GetWeight() + 1 + tiles[row - 1][col]->GetDistance() < tiles[row - 1][col]->GetWeight() + tiles[row - 1][col]->GetDistance()))
			{
				//remove from the queue if it was already added
				priQueue->CheckIfAdded(tiles[row - 1][col]);

				//update the tile
				tiles[row - 1][col]->SetWeight(target->GetWeight() + 1);
				tiles[row - 1][col]->SetPrev(target);

				//add to queue
				priQueue->Push(tiles[row - 1][col]);
			}
		}

		if (row != arraySize - 1 && tiles[row + 1][col] != nullptr)
		{
			//tiles[row + 1][col]
			if (!tiles[row + 1][col]->CheckVisited() &&
				(target->GetWeight() + 1 + tiles[row + 1][col]->GetDistance() < tiles[row + 1][col]->GetWeight() + tiles[row + 1][col]->GetDistance()))
			{
				//remove from the queue if it was already added
				priQueue->CheckIfAdded(tiles[row + 1][col]);

				//update the tile
				tiles[row + 1][col]->SetWeight(target->GetWeight() + 1);
				tiles[row + 1][col]->SetPrev(target);

				//add to queue
				priQueue->Push(tiles[row + 1][col]);
			}
		}

		if (col != 0 && tiles[row][col - 1] != nullptr)
		{
			//tiles[row][col - 1]
			if (!tiles[row][col - 1]->CheckVisited() &&
				(target->GetWeight() + 1 + tiles[row][col - 1]->GetDistance() < tiles[row][col - 1]->GetWeight() + tiles[row][col - 1]->GetDistance()))
			{
				//remove from the queue if it was already added
				priQueue->CheckIfAdded(tiles[row][col - 1]);

				//update the tile
				tiles[row][col - 1]->SetWeight(target->GetWeight() + 1);
				tiles[row][col - 1]->SetPrev(target);

				//add to queue
				priQueue->Push(tiles[row][col - 1]);
			}
		}

		if (col != arraySize - 1 && tiles[row][col + 1] != nullptr)
		{
			//tiles[row][col + 1]
			if (!tiles[row][col + 1]->CheckVisited() &&
				(target->GetWeight() + 1 + tiles[row][col + 1]->GetDistance() < tiles[row][col + 1]->GetWeight() + tiles[row][col + 1]->GetDistance()))
			{
				//remove from the queue if it was already added
				priQueue->CheckIfAdded(tiles[row][col + 1]);

				//update the tile
				tiles[row][col + 1]->SetWeight(target->GetWeight() + 1);
				tiles[row][col + 1]->SetPrev(target);

				//add to queue
				priQueue->Push(tiles[row][col + 1]);
			}
		}

		//move target to front of queue
		target = priQueue->GetFront();
		target->ChangeVisited();
	}

	MakePath();
}

void Simplex::TileManager::SetTileDistances()
{
	for (int i = 0; i < arraySize; i++)
	{
		for (int j = 0; j < arraySize; j++)
		{
			if (tiles[i][j] != nullptr)
			{
				tiles[i][j]->SetDistance(end->GetPosition());
			}
		}
	}
}

void Simplex::TileManager::MakePath()
{
	shortPath.clear();
	std::vector<Tile*> temp;

	Tile* target = end;

	do
	{
		temp.push_back(target);
		if (target->GetPrev() == nullptr)
		{
			break;
		}

		target = target->GetPrev();
	} while (target != start);

	for (int i = temp.size() - 1; i >= 0; i--)
	{
		shortPath.push_back(temp[i]);
	}
}

void Simplex::TileManager::ResetTiles()
{
	for (int i = 0; i < arraySize; i++)
	{
		for (int j = 0; j < arraySize; j++)
		{
			if (tiles[i][j] != nullptr)
			{
				tiles[i][j]->Reset();
			}
		}
	}
}

//Accessors
uint Simplex::TileManager::GetArraySize(){ return arraySize; }
Tile* Simplex::TileManager::GetStart(){	return start; }
void Simplex::TileManager::SetStart(Tile* a_start){	start = a_start; }
Tile* Simplex::TileManager::GetEnd(){ return end;}
void Simplex::TileManager::SetEnd(Tile* a_end){	end = a_end; }

std::vector<Tile*> Simplex::TileManager::GetShortPath()
{
	return shortPath;
}

void TileManager::Release()
{
	start = nullptr;
	end = nullptr;

	delete[] tiles;

	shortPath.clear();
}
