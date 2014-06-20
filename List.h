#pragma once
#include <stdlib.h>
#include "Set.h"

struct List
{
	struct Node head;
};

void ListInit(struct List* list)
{
	NodeInit(&list->head, NULL);
}

void ListAdd(struct List* list, void* data)
{
	struct Node* newNode = malloc(sizeof *newNode);
	memset(newNode, 0, sizeof(struct Node));
	NodeInit(newNode, data);

	if (list->head.next == NULL)
	{
		list->head.next = newNode;
		return;
	}

	struct Node *tmpNode = list->head.next;

	while (tmpNode->next != NULL)
		tmpNode = tmpNode->next;

	tmpNode->next = newNode;
}

struct Node* PtrTo(struct List* list, int position)
{
	if (list->head.next == NULL)
		return NULL;

	struct Node* tmpNode = list->head.next;

	for (int i = 0; i < position && tmpNode != NULL; i++)
		tmpNode = tmpNode->next;

	return tmpNode;
}

void ListDelete(struct List* list, int position)
{
	struct Node* prev = position == 0 ? &(list->head) : PtrTo(list, position - 1);
	struct Node* node = PtrTo(list, position);

	if (node != NULL)
	{
		prev->next = node->next;
		free(node);
		node = NULL;
	}
}

void ListFree(struct List* list)
{
	while (list->head.next != NULL)
		ListDelete(list, 0);
}