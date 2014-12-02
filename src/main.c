#include <stdlib.h>
#include <stdio.h>

#include "page.h"
#include "files.h"

int main(int argc, char **argv)
{
	printf("Microlog Generator\n");

	if (argc > 1)
	{
		site_title = argv[1];
	}
	else
	{
		site_title = (char *)malloc(sizeof(char) * 18);
		sprintf(site_title,"UNTITLED WEBSITE");
	}

	files_build_index();
	files_parse_articles();

	return 0;
}
