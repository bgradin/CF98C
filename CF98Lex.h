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

struct ExistenceReturn
{
	int existed;
	char* location;
};

struct ExistenceReturn CF98InstructionVerifyExistence(struct CF98Lex* lex, int xPos, int yPos)
{
	// Make sure the column exists
	int instX = HashArrayIndex(lex->instructions, xPos);
	struct HashArray* instructions = lex->instructions->data[instX].data;
	if (instructions == NULL)
	{
		instructions = malloc(sizeof(struct HashArray));
		HashArrayInit(instructions);
		HashArrayStore(lex->instructions, instX, instructions);
	}

	// Make sure the cell exists
	int instY = HashArrayIndex(lex->instructions, yPos);
	char* instruction = instructions->data[instY].data;
	struct ExistenceReturn returnValue;
	if (instruction == NULL)
	{
		returnValue.location = instruction = (char*)malloc(sizeof(char));
		*instruction = ' ';
		HashArrayStore(instructions, instY, instruction);
		returnValue.existed = 1;
	}
	else
	{
		returnValue.location = instruction;
		returnValue.existed = 0;
	}

	return returnValue;
}

void CF98InstructionStore(struct CF98Lex* lex, char value, int xPos, int yPos)
{
	struct ExistenceReturn existence = CF98InstructionVerifyExistence(lex, xPos, yPos);
	*(existence.location) = value;
}

char GetToken(struct CF98Lex* lex, int xPos, int yPos)
{
	struct ExistenceReturn existence = CF98InstructionVerifyExistence(lex, xPos, yPos);
	
	if (existence.existed != 0)
		*(existence.location) = CharAt(lex, xPos, yPos);

	return *(existence.location);
}

void CF98Move(struct CF98Lex* lex)
{
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
			int* num1 = (int*)StackPop(lex->memory, sizeof(int));
			int* num2 = (int*)StackPop(lex->memory, sizeof(int));
			int* result = (int*)malloc(sizeof(int));
			*result = *num2 + *num1;
			free(num1);
			free(num2);
			StackPush(lex->memory, result);
		}
			break;
		case '-':
		{
			int* num1 = (int*)StackPop(lex->memory, sizeof(int));
			int* num2 = (int*)StackPop(lex->memory, sizeof(int));
			int* result = (int*)malloc(sizeof(int));
			*result = *num2 - *num1;
			free(num1);
			free(num2);
			StackPush(lex->memory, result);
		}
			break;
		case '*':
		{
			int* num1 = (int*)StackPop(lex->memory, sizeof(int));
			int* num2 = (int*)StackPop(lex->memory, sizeof(int));
			int* result = (int*)malloc(sizeof(int));
			*result = *num2 * *num1;
			free(num1);
			free(num2);
			StackPush(lex->memory, result);
		}
			break;
		case '/':
		{
			int* num1 = (int*)StackPop(lex->memory, sizeof(int));
			int* num2 = (int*)StackPop(lex->memory, sizeof(int));
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
			int* num1 = (int*)StackPop(lex->memory, sizeof(int));
			int* num2 = (int*)StackPop(lex->memory, sizeof(int));
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
			int* num1 = (int*)StackPop(lex->memory, sizeof(int));
			int* num2 = (int*)StackPop(lex->memory, sizeof(int));
			int* result = (int*)malloc(sizeof(int));
			*result = *num2 > *num1 ? 1 : 0;
			free(num1);
			free(num2);
			StackPush(lex->memory, result);
		}
			break;
		case '!':
		{
			int* num1 = (int*)StackPop(lex->memory, sizeof(int));
			int* result = (int*)malloc(sizeof(int));
			*result = *num1 == 0 ? 1 : 0;
			free(num1);
			StackPush(lex->memory, result);
		}
			break;
		case '_':
		{
			int* num1 = (int*)StackPop(lex->memory, sizeof(int));
			lex->currentDirection = *num1 == 0 ? DIRECTION_RIGHT : DIRECTION_LEFT;
			free(num1);
		}
			break;
		case '|':
		{
			int* num1 = (int*)StackPop(lex->memory, sizeof(int));
			lex->currentDirection = *num1 == 0 ? DIRECTION_DOWN : DIRECTION_UP;
			free(num1);
		}
			break;
		case ':':
		{
			int* num = (int*)StackPop(lex->memory, sizeof(int)), *copy = (int*)malloc(sizeof(int));
			*copy = *num;
			StackPush(lex->memory, num);
			StackPush(lex->memory, copy);
		}
			break;
		case '.':
		{
			int* num = (int*)StackPop(lex->memory, sizeof(int));
			printf("%i", *num);
			free(num);
		}
			break;
		case ',':
		{
			int* num = (int*)StackPop(lex->memory, sizeof(int));
			printf("%c", *num);
			free(num);
		}
			break;
		case '\\':
		{
			int* num1 = (int*)StackPop(lex->memory, sizeof(int));
			int* num2 = (int*)StackPop(lex->memory, sizeof(int));
			StackPush(lex->memory, num1);
			StackPush(lex->memory, num2);
		}
			break;
		case '$':
		{
			int* num1 = (int*)StackPop(lex->memory, sizeof(int));
			free(num1);
		}
			break;
		case '#':
			CF98Move(lex);
			break;
		case 'p':
		{
			int* num1 = (int*)StackPop(lex->memory, sizeof(int));
			int* num2 = (int*)StackPop(lex->memory, sizeof(int));
			int* num3 = (int*)StackPop(lex->memory, sizeof(int));
			CF98InstructionStore(lex, *num3, *num2, *num1);
			free(num1);
			free(num2);
			free(num3);
		}
			break;
		case 'g':
		{
			int* num1 = (int*)StackPop(lex->memory, sizeof(int));
			int* num2 = (int*)StackPop(lex->memory, sizeof(int));
			int* value = malloc(sizeof(int));
			*value = GetToken(lex, *num1, *num2);
			StackPush(lex->memory, value);
			free(num1);
			free(num2);
		}
			break;
		case '&':
		{
			int* input = malloc(sizeof(int));
			scanf_s("%i", input);
			StackPush(lex->memory, input);
		}
			break;
		case '~':
		{
			int* input = malloc(sizeof(int));
			char tmp = 0;
			scanf_s("%c", &tmp);
			*input = tmp;
			StackPush(lex->memory, input);
		}
			break;
		default:
			if (lex->currentToken >= '0' && lex->currentToken <= '9')
			{
				int* newMemory = (int*)malloc(sizeof(int));
				*newMemory = lex->currentToken - '0';
				StackPush(lex->memory, newMemory);
			}
			if (lex->currentToken >= 'a' && lex->currentToken <= 'f')
			{
				int* newMemory = (int*)malloc(sizeof(int));
				*newMemory = lex->currentToken - 82;
				StackPush(lex->memory, newMemory);
			}
			break;
		}
	}

	CF98Move(lex);
}

void CF98Parse(struct CF98Lex* lex)
{
	// Generate "skeleton" of file
	// File streams are 1D, but by knowing where the newline characters are,
	//   we can calculate where in the stream (x, y) is.
	FindNewLines(lex);

	while (lex->currentToken != EOFSY)
	{
		lex->currentToken = GetToken(lex, lex->x, lex->y);
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
	StackFree(lex->memory, sizeof(int));
	HashArrayFree(lex->instructions);

	free(lex->newlines);
	free(lex->memory);
	free(lex->instructions);
}