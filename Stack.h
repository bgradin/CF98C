#pragma once
#include <stdlib.h>
#include <string.h>
#include "Set.h"

struct Stack
{
	struct Node head;
};

void StackInit(struct Stack* stack)
{
	NodeInit(&stack->head, NULL);
}

void StackPush(struct Stack* stack, void* data)
{
	struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
	NodeInit(newNode, data);

	newNode->next = stack->head.next;
	stack->head.next = newNode;
}

void* StackPop(struct Stack* stack, size_t size)
{
	if (stack->head.next == NULL)
	{
		void* newMemory = malloc(size);
		memset(newMemory, 0, size);
		return newMemory;
	}

	void* returnValue = stack->head.next->data;
	struct Node* nextNode = stack->head.next->next;
	free(stack->head.next);
	stack->head.next = nextNode;

	return returnValue;
}

void StackFree(struct Stack* stack, size_t size)
{
	while (stack->head.next != NULL)
		StackPop(stack, size);
}