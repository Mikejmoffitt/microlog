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
	if (argc > 2)
	{
		site_subtitle = argv[2];
	}
	else
	{
		site_subtitle = (char *)malloc(sizeof(char) * 2);
		sprintf(site_subtitle," ");
	}
	
	files_build_index();
	files_parse_articles();
	files_copy_res();

	return 0;
}
