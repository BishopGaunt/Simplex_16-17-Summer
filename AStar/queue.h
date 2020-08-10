#ifndef _QUEUE_H
#define _QUEUE_H

#include <iostream>
using namespace std;

template <class Data>
class Queue
{
public:
	//members
	Data* storage;
	int arraySize;
	int index; //keeps track of the end of the data in the array which is also the lowest value

	//constructor
	Queue()
	{
		arraySize = 5;
		storage = new Data[arraySize];
		index = -1;
	}

	//copy constructor
	Queue(const Queue& other)
	{
		arraySize = other.arraySize;
		storage = new Data[arraySize];
		index = other.index;

		for (int i = 0; i <= index; i++)
		{
			storage[i] = other.storage[i];
		}
	}

	//copy assigntment operator
	Queue& operator = (const Queue& other)
	{
		if (this == &other)
		{
			return *this;
		}

		if (storage != nullptr)
		{
			delete[] storage;
			storage = nullptr;
		}

		arraySize = other.arraySize;
		storage = new Data[arraySize];
		index = other.index;

		for (int i = 0; i <= index; i++)
		{
			storage[i] = other.storage[i];
		}
	}

	//destructor
	~Queue()
	{
		delete[] storage;
		storage = nullptr;
	}

	//push
	void Push(Data data)
	{
		//add in the new data
		index++;

		Data* hold;

		//check if the array needs to be bigger
		if (index > arraySize)
		{
			arraySize += 5;
			hold = new Data[arraySize]; //create temp array

			for (int i = 0; i <= index; i++)
			{
				hold[i] = storage[i];
			}

			delete[] storage; //remove old array
			storage = hold; //set storage to the new array
			hold = nullptr;
		}

		storage[index] = data;

		//sort the storage array
		hold = new Data[arraySize]; //new array which will become storage's target
		int minValue; //keeps track of the index for the lowest value in storage

		//check each value and place into correct index
		for (int i = 0; i <= index; i++)
		{
			minValue = i;
			for (int j = i + 1; j <= index; j++) //check each value against the other values after it in the array
			{
				if (storage[j] < storage[minValue])
				{
					minValue = j; //set the new lowest value at this point
				}
			}
			hold[i] = storage[minValue]; //add to the new array in order of lowest to highest
		}

		//remove old storage array and set to new array
		delete[] storage;
		storage = hold;
	}

	//pop
	void Pop()
	{
		if (!IsEmpty()) //just to check if there is anything to pop
		{
			index--;

			for (int i = 0; i <= index; i++) //move down each value in the array
			{
				storage[i] = storage[i + 1];
			}
		}
		else
			cout << "Queue is empty" << endl;
	}

	//print
	void Print()
	{
		cout << "Printing Queue" << endl;

		for (int i = 0; i <= index; i++)
		{
			cout << storage[i] << endl;
		}
	}

	//getsize
	int GetSize()
	{
		return index + 1;
	}

	//isempty
	bool IsEmpty()
	{
		if (index == -1)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
};

#endif //_QUEUE_H
