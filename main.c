#include <string.h>
#include <stdio.h>
#include <sys/stat.h>
#include "CF98Lex.h"

#define output(text) printf("%s", text)

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
		output("  [source]: A file containing a program written in Concurrent Funge-98\n");
		return 0;
	}

	struct stat st;
	stat(argv[1], &st);
	long size = st.st_size;

	if (size > ((unsigned int)-1))
	{
		output("Source file is too large.\n");
		return 1;
	}

	FILE *fp;

	if (fopen_s(&fp, argv[1], "r") != 0)
	{
		output("Error opening source file.\n");
		return 1;
	}

	output("Compiling...\n");

	struct CF98Lex* lex = malloc(sizeof(struct CF98Lex));
	CF98Init(lex, fp);
	CF98Parse(lex);
	CF98Close(lex);
	free(lex);

	if (fclose(fp) == EOF)
	{
		output("Error closing source file.");
		return 1;
	}
	return 0;
}