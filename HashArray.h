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
	arr->data = (struct HashNode*)malloc(size);
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

	arr->size++;
	arr->data[index].data = data;
	arr->data[index].index = index;
}

int HashArrayIndex(struct HashArray* arr, int key)
{
	if (arr->size == arr->capacity)
		HashArrayResize(arr);

	int index = HashArrayHash(arr, key), i = 0;
	while (i++ < arr->capacity && arr->data[index].data != NULL && arr->data[index].index != key)
		index = index == arr->capacity - 1 ? 0 : index + 1; // Solve collisions with linear probing

	if (i == arr->capacity)
		HashArrayResize(arr);

	return index;
}

void HashArrayResize(struct HashArray* arr)
{
	struct HashNode* oldMemory = arr->data;
	int oldCapacity = arr->capacity;

	arr->capacity << 1;
	int newCapacity = sizeof(struct HashNode)* arr->capacity;
	arr->data = (struct HashNode*)malloc(newCapacity);
	memset(arr->data, 0, newCapacity);

	for (int i = 0; i < oldCapacity; i++)
	{
		if (oldMemory[i].data != NULL)
			HashArrayStore(arr, oldMemory[i].index, oldMemory[i].data);
	}
}

void HashArrayFree(struct HashArray* arr)
{
	free(arr->data);
}