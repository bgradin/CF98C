#pragma once
#include <stdlib.h>
#include <string.h>
#include "Set.h"

struct Stack
{
	struct Node* head;
};

void StackInit(struct Stack* stack)
{
	stack->head = NULL;
}

void StackPush(struct Stack* stack, void* data)
{
	struct Node** headNode = &stack->head;
	struct Node* newNode = malloc(sizeof *newNode);
	NodeInit(newNode, data);

	newNode->next = stack->head;
	*headNode = newNode;
}

void* StackPop(struct Stack* stack, size_t size)
{
	if (stack->head == NULL)
	{
		void* newMemory = malloc(size);
		memset(newMemory, 0, size);
		return newMemory;
	}

	void* returnValue = stack->head->data;
	struct Node* nextNode = stack->head->next;
	struct Node** headNode = &stack->head;
	free(stack->head);
	*headNode = nextNode;

	return returnValue;
}

void StackFree(struct Stack* stack, size_t size)
{
	while (stack->head != NULL)
		StackPop(stack, size);
}