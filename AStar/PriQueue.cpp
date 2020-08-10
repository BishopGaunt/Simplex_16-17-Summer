#include "PriQueue.h"
using namespace Simplex;

void PriQueue::Init()
{
}


PriQueue::PriQueue()
{
	Init();
}

PriQueue::PriQueue(PriQueue const& other)
{
	storage = other.storage;
	index = other.index;
}

PriQueue& PriQueue::operator=(PriQueue const& other)
{
	if (this != &other)
	{
		Release();
		Init();
		PriQueue temp(other);
		Swap(temp);
	}
	return *this;
}

PriQueue::~PriQueue()
{
	Release();
}

void PriQueue::Swap(PriQueue& other)
{
	std::swap(storage, other.storage);
	std::swap(index, other.index);
}

void PriQueue::Push(Tile* tile)
{
	storage.push_back(tile);

	std::vector<Tile*> hold;
	Tile* minTile;

	for (int i = 0; i < storage.size(); i++)
	{
		minTile = storage[i];
		for (int j = i + 1; j < storage.size(); j++)
		{
			if (minTile->GetDistance() + minTile->GetWeight() > storage[j]->GetDistance() + storage[j]->GetWeight())
			{
				minTile = storage[j];
			}
		}
		hold.push_back(minTile);
	}

	storage = hold;
}

Tile* PriQueue::Pop()
{
	if (!storage.empty())
	{
		Tile* hold = storage[0];
		
		for (int i = 0; i < storage.size() - 1; i++) //move down each value in the array
		{
			storage[i] = storage[i + 1];
		}

		storage.pop_back();

		return hold;
	}
	else
		return nullptr;
}

void PriQueue::CheckIfAdded(Tile* target)
{
	for (int i = 0; i < storage.size(); i++)
	{
		if (target == storage[i])
		{
			storage.erase(storage.begin() + i);
			break;
		}
	}
}

int PriQueue::GetSize()
{
	return storage.size();
}

Tile* Simplex::PriQueue::GetFront()
{
	return storage[0];
}


void PriQueue::Release()
{
	storage.clear();
}
