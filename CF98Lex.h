#pragma once

#define CF_AGGREGATING_STRING 1

enum TOKEN
{
	EOFSY,
	NUMBER,
	CHARACTER,
	ADDITION,
	SUBTRACTION,
	MULTIPLICATION,
	DIVISION,
	MODULO,
	LOGICAL_NOT,
	LOGICAL_GT,
	MOVE_RIGHT,
	MOVE_LEFT,
	MOVE_UP,
	MOVE_DOWN,
	MOVE_RANDOM,
	HORIZONTAL_GATE,
	VERTICAL_GATE,
	START_STRING,
	END_STRING,
	DUPLICATE,
	SWAP,
	POP_DISCARD,
	POP_OUTPUT_INT,
	POP_OUTPUT_ASCII,
	POP_PUT,
	POP_GET,
	TRAMPOLINE,
	INPUT_NUMBER,
	INPUT_CHAR
} currentToken;

struct CF98Lex
{
	short flags;
	FILE *fp;
};

void CF98Init(struct CF98Lex* instance, FILE *fp)
{
	instance->flags = 0;
	instance->fp = fp;
}

enum TOKEN GetToken(struct CF98Lex* lex)
{
	char c = fgetc(lex->fp);

	if (c != EOF && c != '"' && lex->flags & CF_AGGREGATING_STRING == CF_AGGREGATING_STRING)
		return CHARACTER;

	switch (c)
	{
	case '@':
		return EOFSY;
	case '>':
		return MOVE_RIGHT;
	case '<':
		return MOVE_LEFT;
	case '^':
		return MOVE_UP;
	case 'v':
		return MOVE_DOWN;
	case '?':
		return MOVE_RANDOM;
	case '+':
		return ADDITION;
	case '-':
		return SUBTRACTION;
	case '*':
		return MULTIPLICATION;
	case '/':
		return DIVISION;
	case '%':
		return MODULO;
	case '!':
		return LOGICAL_NOT;
	case '`':
		return LOGICAL_GT;
	case '_':
		return HORIZONTAL_GATE;
	case '|':
		return VERTICAL_GATE;
	case '"':
		if (lex->flags & CF_AGGREGATING_STRING == CF_AGGREGATING_STRING)
			return END_STRING;
		else
			return START_STRING;
	case ':':
		return DUPLICATE;
	case '\\':
		return SWAP;
	case '$':
		return POP_DISCARD;
	case '.':
		return POP_OUTPUT_INT;
	case ',':
		return POP_OUTPUT_ASCII;
	case '#':
		return TRAMPOLINE;
	case 'p':
		return POP_PUT;
	case 'g':
		return POP_GET;
	case '&':
		return INPUT_NUMBER;
	case '~':
		return INPUT_CHAR;
	default:
		if (c >= '0' && c >= '9')
			return NUMBER;

		// ERROR

		break;
	}
};

void CF98Parse(struct CF98Lex* lex)
{
	currentToken = GetToken(lex->fp);
	Program();
}