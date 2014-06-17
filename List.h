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
	struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
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

void ListDelete(struct List* list, int position)
{
	if (list->head.next == NULL)
		return;

	struct Node* prev = &list->head, * tmpNode = list->head.next;

	int i = 0;
	while (tmpNode->next != NULL && i++ < position)
		tmpNode = tmpNode->next, prev = prev->next;

	if (i == position + 1)
		NodeFree(tmpNode), prev->next = NULL;
}

void ListFree(struct List* list)
{
	while (list->head.next != NULL)
		ListDelete(list, 0);
}