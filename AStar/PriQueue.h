/*----------------------------------------------
Programmer: Anthony LaRosa - al8249@rit.edu
Date: 2020/08
----------------------------------------------*/
#ifndef __PRIQUEUE_H_
#define __PRIQUEUE_H_

#include "Definitions.h"
#include "Tile.h"

namespace Simplex
{

class PriQueue
{
	std::vector<Tile*> storage;
	int index;

public:
	PriQueue();

	PriQueue(PriQueue const& other);

	PriQueue& operator=(PriQueue const& other);

	~PriQueue();

	void Swap(PriQueue& other);

	void Push(Tile* tile);

	Tile* Pop();

	void CheckIfAdded(Tile* target);

	int GetSize();

	Tile* GetFront();

private:
	void Init();

	void Release();

};

}
#endif