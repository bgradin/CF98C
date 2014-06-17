#pragma once
#include <stdlib.h>

struct Node
{
	void* data;
	struct Node* next;
};

void ListNodeInit(struct Node* node, void* data)
{
	node->data = data;
	node->next = NULL;
}

void ListNodeFree(struct Node* node)
{
	free(node->data);
}