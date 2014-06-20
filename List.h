#pragma once
#include <stdlib.h>
#include "Set.h"

struct List
{
	struct Node* head;
};

void ListInit(struct List* list)
{
	list->head = NULL;
}

void ListAdd(struct List* list, void* data)
{
	struct Node **newNodePointer = &list->head;

	while (*newNodePointer)
		newNodePointer = &(*newNodePointer)->next;

	(*newNodePointer) = malloc(sizeof **newNodePointer);
	(*newNodePointer)->data = data;
	(*newNodePointer)->next = NULL;
}

struct Node* PtrTo(struct List* list, int position)
{
	struct Node **returnNodePointer = &list->head;

	while (*returnNodePointer && position--)
		returnNodePointer = &(*returnNodePointer)->next;

	return *returnNodePointer;
}

void ListDelete(struct List* list, int position)
{
	struct Node **deleteNodePointer = &list->head;

	while (*deleteNodePointer && position--)
		deleteNodePointer = &(*deleteNodePointer)->next;

	if (*deleteNodePointer)
	{
		struct Node *tmp = *deleteNodePointer;
		*deleteNodePointer = tmp->next;
		free(tmp);
	}
}

void ListFree(struct List* list)
{
	while (list->head)
		ListDelete(list, 0);
}