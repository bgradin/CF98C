#include <string.h>
#include <stdio.h>
#include "BF98Lex.h"

void output(const char* text)
{
	printf("%s", text);
}

void main(int argc, char** argv)
{
	if (argc != 2)
	{
		output("Invalid number of command-line arguments.\n");
		return 1;
	}

	if (!strcmp(argv[1], "/?"))
	{
		output("Usage: bfc [source]\n\n");
		output("  [source]: A file containing a program written in Befunge-98\n");
		return 0;
	}

	FILE *fp = fopen(argv[1], "r");

	if (fp == NULL)
	{
		output("Error opening source file.\n");
		return 1;
	}

	BF98Parse(fp);

	if (fclose(fp) == EOF)
	{
		output("Error closing source file.");
		return 1;
	}

	return 0;
}