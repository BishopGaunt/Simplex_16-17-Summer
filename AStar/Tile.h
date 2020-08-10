/*----------------------------------------------
Programmer: Anthony LaRosa - al8249@rit.edu
Date: 2020/08
----------------------------------------------*/
#ifndef __TILE_H_
#define __TILE_H_

#include "Definitions.h"

namespace Simplex
{

class Tile
{
	/*variables*/
	vector3 position = ZERO_V3;

	bool visited;

	//index
	int row;
	int col;

	//heuristic
	float distance;
	float weight;

	//pathing
	Tile* prev;
	Tile* next;

	/*methods*/
	//init
	//rule of 3
	//display
	//reset
public:
	Tile();

	Tile(vector3 a_position, uint a_row, uint a_col);

	Tile(Tile const& other);

	Tile& operator=(Tile const& other);

	~Tile();

	void Swap(Tile& other);

	void Reset();

#pragma region Accessors
	vector3 GetPosition();

	void SetPosition(vector3 a_position);

	float GetDistance();

	void SetDistance(float a_distance);

	void SetDistance(vector3 a_end);

	float GetWeight();

	void SetWeight(float a_weight);

	int GetRow();

	void SetRow(int a_row);

	int GetCol();

	Tile* GetPrev();

	void SetPrev(Tile* target);

	Tile* GetNext();

	void SetNext(Tile* target);

	void SetCol(int a_col);

	bool CheckVisited();

	void ChangeVisited();

#pragma endregion


private:
	void Release();

	void Init();

};

}
#endif