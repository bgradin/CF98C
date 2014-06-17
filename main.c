#include <string.h>
#include <stdio.h>
#include "CF98Lex.h"

void output(const char* text)
{
	printf("%s", text);
}

int main(int argc, char** argv)
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

	FILE *fp;

	if (fopen_s(&fp, argv[1], "r") != 0)
	{
		output("Error opening source file.\n");
		return 1;
	}

	output("Compiling...\n");

	struct BF98Lex *lex;
	BF98Init(lex, fp);
	BF98Parse(lex);

	if (fclose(fp) == EOF)
	{
		output("Error closing source file.");
		return 1;
	}

	return 0;
}