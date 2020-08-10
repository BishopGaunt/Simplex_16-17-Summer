#include "Tile.h"
using namespace Simplex;

void Tile::Init()
{
	position = vector3(0.0f);

	visited = false;

	//index
	row = 0;
	col = 0;

	//heuristic
	distance = 999;
	weight = 999;

	//pathing
	prev = nullptr;
	next = nullptr;
}

Tile::Tile()
{
	Init();
}

Tile::Tile(vector3 a_position, uint a_row, uint a_col)
{
	Init();

	position = a_position;

	row = a_row;
	col = a_col;
}

Tile::Tile(Tile const& other)
{
	position = other.position;

	visited = other.visited;

	row = other.row;
	col = other.col;

	distance = other.distance;
	weight = other.weight;

	prev = other.prev;
	next = other.next;
}

Tile& Tile::operator=(Tile const& other)
{
	if (this != & other)
	{
		Release();
		Init();
		Tile temp(other);
		Swap(temp);
	}
	return *this;
}

Tile::~Tile()
{
	Release();
}

void Tile::Swap(Tile& other)
{
	std::swap(position, other.position);

	std::swap(visited, other.visited);

	std::swap(row, other.row);
	std::swap(col, other.col);

	std::swap(distance, other.distance);
	std::swap(weight, other.weight);

	std::swap(prev, other.prev);
	std::swap(next, other.next);
}

void Tile::Reset()
{
	visited = false;

	//heuristic
	distance = 999;
	weight = 999;

	//pathing
	prev = nullptr;
	next = nullptr;
}

//Accessors
vector3 Tile::GetPosition(){ return position; }
void Tile::SetPosition(vector3 a_position)
{ 
	position = a_position;
	matrix4 mBlock = glm::translate(position);
	//block->SetModelMatrix(mBlock);
}
float Tile::GetDistance(){ return distance; }
void Tile::SetDistance(float a_distance) { distance = a_distance; }
void Tile::SetDistance(vector3 a_end){ distance = glm::distance(position, a_end); }
float Tile::GetWeight(){ return weight; }
void Tile::SetWeight(float a_weight){ weight = a_weight; }
int Tile::GetRow(){ return row; }
void Tile::SetRow(int a_row){ row = a_row; }
int Tile::GetCol(){ return col; }
Tile* Simplex::Tile::GetPrev(){ return prev; }
void Simplex::Tile::SetPrev(Tile* target){ prev = target; }
Tile* Simplex::Tile::GetNext(){	return next; }
void Simplex::Tile::SetNext(Tile* target){ next = target; }
void Tile::SetCol(int a_col){ col = a_col; }
bool Tile::CheckVisited(){ return visited; }
void Tile::ChangeVisited(){ visited = !visited;}

void Tile::Release()
{
	prev = nullptr;
	next = nullptr;
}
