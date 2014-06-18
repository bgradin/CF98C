#pragma once
#include <stdlib.h>

struct Node
{
	void* data;
	struct Node* next;
};

void NodeInit(struct Node* node, void* data)
{
	node->data = data;
	node->next = NULL;
}