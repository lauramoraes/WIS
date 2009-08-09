#include <stdio.h>

int main(int argc, char** argv)
{
	FILE* file;
	if((file= fopen(argv[1], "r")))
	{
		fclose(file);
		return 1;
	}
	return 0;
}
