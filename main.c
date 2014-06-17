#include <string.h>
#include <stdio.h>

void main(int argc, char** argv)
{
	if (argc > 1 && !strcmp(argv[1], "/?"))
	{
		printf("%s", "Usage: bfc [source]\n\n");
		printf("%s", "  [source]: A file containing a program written in Befunge-98\n");
		return 0;
	}

	return 0;
}