#include "files.h"

#include "page.h"

#define IN_ARTICLES_DIR "source/articles/"
#define OUT_ARTICLES_DIR "site/articles/"
#define OUT_APPEND ".html"

#define IN_PAGES_DIR "source/pages/"
#define OUT_PAGES_DIR "site/pages/"

#define OUT_INDEX_DIR "site/"

void files_process_article(char *d_name)
{
	printf("--------------------------------------------------\n");

	// Determine the input filename
	int in_name_len = strnlen(IN_ARTICLES_DIR,255);
	in_name_len += strnlen(d_name,255);
	in_name_len++; // For null terminator
	char *in_name = (char *)malloc(sizeof(char) * in_name_len);
	char *ptr = in_name;
	strcpy(ptr, IN_ARTICLES_DIR);
	ptr += strnlen(IN_ARTICLES_DIR,255);
	strcpy(ptr, d_name);
	printf("Opening %s for reading...\n", in_name);

	// Do the same for output
	int out_name_len = strnlen(OUT_ARTICLES_DIR,255);
	out_name_len += strnlen(OUT_APPEND,255);
	out_name_len += strnlen(d_name,255);
	out_name_len++; // For null terminator
	char *out_name = (char *)malloc(sizeof(char) * out_name_len);
	ptr = out_name;
	strcpy(ptr, OUT_ARTICLES_DIR);
	ptr += strnlen(OUT_ARTICLES_DIR,255);
	strcpy(ptr, d_name);
	ptr += strnlen(d_name,255);
	strcpy(ptr,OUT_APPEND);
	printf("Opening %s for writing...\n", out_name);
	FILE *in_file = fopen(in_name,"r");
	if (!in_file)
	{
		printf("Couldn't open %s for reading.\nError %d\n",in_name,errno);
		free(in_name);
		free(out_name);
		return;
	}	
	FILE *out_file = fopen(out_name,"w");
	if (!out_file)
	{
		printf("Couldn't open %s for writing.\nError %d\n",out_name,errno);
		free(in_name);
		free(out_name);
		return;
	}
	// Time to make the article

	page_create_article(in_file,out_file);

	free(in_name);
	free(out_name);
}

void files_parse_articles(void)
{
	DIR *d;
	struct dirent *dir;
	d = opendir("./source/articles/");
	if (d)
	{
		while ((dir = readdir(d)) != NULL)
		{	
			// Be sure this is not a symlink, directory, etc
			// Generate an article based on the file present
			if (dir->d_type == DT_REG)
			{
				files_process_article(dir->d_name);
			}
		}
	}
	closedir(d);
}
