#include <stdlib.h>
#include <stdio.h>

#include "page.h"
#include "files.h"

int main(int argc, char **argv)
{
	printf("Microlog generator v0.12\n");

	files_parse_articles();

	return 0;
}
