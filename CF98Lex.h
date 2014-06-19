#pragma once
#include "HashArray.h"
#include "List.h"
#include "Stack.h"

#define CF_AGGREGATING_STRING 1

#define DIRECTION_NONE -1
#define DIRECTION_UP 0
#define DIRECTION_RIGHT 1
#define DIRECTION_DOWN 2
#define DIRECTION_LEFT 3

#define EOFSY '@'

struct CF98Lex
{
	short flags;
	FILE *fp;
	int x, y;
	int currentDirection;
	struct HashArray* instructions;
	struct Stack* memory;
	struct List* newlines;
	char currentToken;
};

void CF98Init(struct CF98Lex* instance, FILE *fp)
{
	instance->flags = 0;
	instance->fp = fp;
	instance->currentToken = 0;
	instance->x = instance->y = 0;
	instance->currentDirection = DIRECTION_NONE;
	instance->instructions = (struct HashArray*)malloc(sizeof(struct HashArray));
	instance->memory = (struct Stack*)malloc(sizeof(struct Stack));
	instance->newlines = (struct List*)malloc(sizeof(struct List));
	HashArrayInit(instance->instructions);
	StackInit(instance->memory);
	ListInit(instance->newlines);
}

char CharAt(struct CF98Lex* lex, int xPos, int yPos)
{
	if (xPos < 0 || yPos < 0)
		return ' ';

	int yOff = 0;
	if (yPos > 0)
	{
		struct Node* newline = PtrTo(lex->newlines, yPos);
		yOff = *((int*)newline->data) + 2;
	}

	clearerr(lex->fp);
	if (fseek(lex->fp, yOff + xPos, SEEK_SET) == 0)
		return fgetc(lex->fp);

	return EOF;
}

void FindNewLines(struct CF98Lex* lex)
{
	clearerr(lex->fp);
	fseek(lex->fp, 0, SEEK_SET);

	int* zero = (int*)malloc(sizeof(int));
	*zero = 0;
	ListAdd(lex->newlines, zero);

	for (int i = 0; feof(lex->fp) == 0; i++)
	{
		if (fgetc(lex->fp) == '\n')
		{
			int *newInt = (int*)malloc(sizeof(int));
			*newInt = ftell(lex->fp) - 2;
			ListAdd(lex->newlines, newInt);
		}
	}
}

char GetToken(struct CF98Lex* lex)
{
	// Make sure the column exists
	struct HashArray* instructions = (struct HashArray*)lex->instructions->data[HashArrayIndex(lex->instructions, lex->x)].data;
	if (instructions == NULL)
	{
		instructions = malloc(sizeof(struct HashArray));
		HashArrayInit(instructions);
	}

	// Make sure the cell exists
	char* instruction = (char*)instructions->data[HashArrayIndex(lex->instructions, lex->y)].data;
	if (instruction == NULL)
		instruction = (char*)malloc(sizeof(char));
	else
		return *instruction; // Return it if it already does

	// If we didn't find the instruction in the instructions structure, we have to look in the file
	char tmp = CharAt(lex, lex->x, lex->y);
	return *instruction = tmp != EOF ? tmp : ' ';
}

void ExecuteToken(struct CF98Lex* lex)
{
	if (lex->currentToken != '"' && lex->flags & CF_AGGREGATING_STRING == CF_AGGREGATING_STRING)
	{
		int* newMemory = (int*)malloc(sizeof(int));
		*newMemory = lex->currentToken;
		StackPush(lex->memory, newMemory);
	}
	else
	{
		switch (lex->currentToken)
		{
		case '>':
			lex->currentDirection = DIRECTION_RIGHT;
			break;
		case '<':
			lex->currentDirection = DIRECTION_LEFT;
			break;
		case '^':
			lex->currentDirection = DIRECTION_UP;
			break;
		case 'v':
			lex->currentDirection = DIRECTION_DOWN;
			break;
		case '?':
			lex->currentDirection = rand() % 4;
			break;
		case '"':
			lex->flags ^= CF_AGGREGATING_STRING;
			break;
		case '+':
		{
			int* num1 = (int*)StackPop(lex->memory);
			int* num2 = (int*)StackPop(lex->memory);
			int* result = (int*)malloc(sizeof(int));
			*result = *num2 + *num1;
			free(num1);
			free(num2);
			StackPush(lex->memory, result);
		}
			break;
		case '-':
		{
			int* num1 = (int*)StackPop(lex->memory);
			int* num2 = (int*)StackPop(lex->memory);
			int* result = (int*)malloc(sizeof(int));
			*result = *num2 - *num1;
			free(num1);
			free(num2);
			StackPush(lex->memory, result);
		}
			break;
		case '*':
		{
			int* num1 = (int*)StackPop(lex->memory);
			int* num2 = (int*)StackPop(lex->memory);
			int* result = (int*)malloc(sizeof(int));
			*result = *num2 * *num1;
			free(num1);
			free(num2);
			StackPush(lex->memory, result);
		}
			break;
		case '/':
		{
			int* num1 = (int*)StackPop(lex->memory);
			int* num2 = (int*)StackPop(lex->memory);
			int* result = (int*)malloc(sizeof(int));
			if (*num2 != 0)
				*result = *num2 / *num1;
			free(num1);
			free(num2);
			StackPush(lex->memory, result);
		}
			break;
		case '%':
		{
			int* num1 = (int*)StackPop(lex->memory);
			int* num2 = (int*)StackPop(lex->memory);
			int* result = (int*)malloc(sizeof(int));
			if (*num2 != 0)
				*result = *num2 % *num1;
			free(num1);
			free(num2);
			StackPush(lex->memory, result);
		}
			break;
		case '`':
		{
			int* num1 = (int*)StackPop(lex->memory);
			int* num2 = (int*)StackPop(lex->memory);
			int* result = (int*)malloc(sizeof(int));
			*result = *num2 > *num1 ? 1 : 0;
			free(num1);
			free(num2);
			StackPush(lex->memory, result);
		}
			break;
		case '!':
		{
			int* num1 = (int*)StackPop(lex->memory);
			int* result = (int*)malloc(sizeof(int));
			*result = *num1 == 0 ? 1 : 0;
			free(num1);
			StackPush(lex->memory, result);
		}
			break;
		case '_':
		{
			int* num1 = (int*)StackPop(lex->memory);
			lex->currentDirection = *num1 == 0 ? DIRECTION_RIGHT : DIRECTION_LEFT;
			free(num1);
		}
			break;
		case '|':
		{
			int* num1 = (int*)StackPop(lex->memory);
			lex->currentDirection = *num1 == 0 ? DIRECTION_DOWN : DIRECTION_UP;
			free(num1);
		}
			break;
		case ':':
		{
			int* num = (int*)StackPop(lex->memory), *copy = (int*)malloc(sizeof(int));
			*copy = *num;
			StackPush(lex->memory, num);
			StackPush(lex->memory, copy);
		}
			break;
		case '.':
		{
			int* num = (int*)StackPop(lex->memory);
			printf("%i", *num);
			free(num);
		}
			break;
		case ',':
		{
			int* num = (int*)StackPop(lex->memory);
			printf("%c", *num);
			free(num);
		}
			break;
		default:
			if (lex->currentToken >= '0' && lex->currentToken <= '9')
			{
				int* newMemory = (int*)malloc(sizeof(int));
				*newMemory = lex->currentToken - '0';
				StackPush(lex->memory, newMemory);
			}
			break;
		}
	}

	// Keep us moving
	switch (lex->currentDirection)
	{
	case DIRECTION_UP:
		lex->y--;
		break;
	case DIRECTION_DOWN:
		lex->y++;
		break;
	case DIRECTION_LEFT:
		lex->x--;
		break;
	case DIRECTION_NONE:
	case DIRECTION_RIGHT:
		lex->x++;
		break;
	}
}

void CF98Parse(struct CF98Lex* lex)
{
	// Generate "skeleton" of file
	// File streams are 1D, but by knowing where the newline characters are,
	//   we can calculate where in the stream (x, y) is.
	FindNewLines(lex);

	while (lex->currentToken != EOFSY)
	{
		lex->currentToken = GetToken(lex);
		ExecuteToken(lex);
	}
}

void CF98Close(struct CF98Lex* lex)
{
	free(lex->newlines->head.data);

	for (int i = 0; i < lex->instructions->capacity; i++)
	{
		if (lex->instructions->data[i].data != NULL)
			HashArrayFree((struct HashArray*)lex->instructions->data[i].data);
	}

	ListFree(lex->newlines);
	StackFree(lex->memory);
	HashArrayFree(lex->instructions);

	free(lex->newlines);
	free(lex->memory);
	free(lex->instructions);
}