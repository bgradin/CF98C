#pragma once
#include <stdlib.h>
#include <string.h>

#define DEFAULT_MAP_SIZE 50

struct HashNode
{
	void* data;
	int index;
};

struct HashArray
{
	int capacity;
	int size;
	struct HashNode* data;
};

void HashArrayInit(struct HashArray* arr)
{
	arr->size = 0;
	arr->capacity = DEFAULT_MAP_SIZE;
	int size = sizeof(struct HashNode) * arr->capacity;
	arr->data = malloc(size);
	memset(arr->data, 0, size);
}

int HashArrayHash(struct HashArray* arr, int key)
{
	return key % arr->capacity;
}

void HashArrayResize(struct HashArray* arr);

void HashArrayStore(struct HashArray* arr, int key, void* data)
{
	int index = HashArrayIndex(arr, key);

	if (arr->data == NULL)
		arr->size++;

	arr->data[key].data = data;
	arr->data[key].index = index;
}

int HashArrayIndex(struct HashArray* arr, int key)
{
	if (arr->size == arr->capacity)
		HashArrayResize(arr);

	int index = HashArrayHash(arr, key);
	for (int i = 0; i < arr->capacity && arr->data[index].data != NULL && arr->data[index].index != key; i++)
		index = index == arr->capacity - 1 ? 0 : index + 1; // Solve collisions with linear probing

	return index;
}

void HashArrayResize(struct HashArray* arr)
{
	struct HashNode* oldMemory = arr->data;
	int oldCapacity = arr->capacity;

	arr->capacity <<= 1;
	int newCapacity = sizeof(struct HashNode) * arr->capacity;
	arr->data = malloc(newCapacity);
	memset(arr->data, 0, newCapacity);
	arr->size = 0;

	for (int i = 0; i < oldCapacity; i++)
	{
		if (oldMemory[i].data != NULL)
		{
			int index = HashArrayHash(arr, oldMemory[i].index);
			HashArrayStore(arr, index, oldMemory[i].data);
		}
	}

	free(oldMemory);
}

void HashArrayFree(struct HashArray* arr)
{
	free(arr->data);
}