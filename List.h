#pragma once
#include <stdlib.h>
#include <stdio.h>

struct ListNode
{
	void* data;
	struct ListNode* next;
};

void ListNodeInit(struct ListNode* node, void* data)
{
	node->data = data;
	node->next = NULL;
}

void ListNodeFree(struct ListNode* node)
{
	free(node->data);
}

struct List
{
	struct ListNode head;
};

void ListInit(struct List* list)
{
	ListNodeInit(&list->head, NULL);
}

void ListAdd(struct List* list, void* data)
{
	struct ListNode* newNode = (struct ListNode*)malloc(sizeof(struct ListNode));
	ListNodeInit(newNode, data);

	if (list->head.next == NULL)
	{
		list->head.next = newNode;
		return;
	}

	struct ListNode *tmpNode = list->head.next;

	while (tmpNode->next != NULL)
		tmpNode = tmpNode->next;

	tmpNode->next = newNode;
}

void ListDelete(struct List* list, int position)
{
	if (list->head.next == NULL)
		return;

	struct ListNode* prev = &list->head, * tmpNode = list->head.next;

	int i = 0;
	while (tmpNode->next != NULL && i++ < position)
		tmpNode = tmpNode->next, prev = prev->next;

	if (i == position + 1)
		ListNodeFree(tmpNode), prev->next = NULL;
}

void ListFree(struct List* list)
{
	while (list->head.next != NULL)
		ListDelete(list, 0);
}

void PrintList(struct List* list)
{
	for (struct ListNode* cur = list->head.next; cur != NULL; cur = cur->next)
		printf("%d\n", *((int*)cur->data));
}