#include "files.h"

#include "page.h"

#define IN_ARTICLES_DIR "source/articles/"
#define OUT_ARTICLES_DIR "site/articles/"
#define OUT_APPEND ".html"

#define IN_PAGES_DIR "source/pages/"
#define OUT_PAGES_DIR "site/pages/"

#define OUT_INDEX_DIR "site/"

#define ARTICLES_PER_PAGE 10

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

void files_parse_articles()
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

void files_process_page(char *d_name)
{

}

void files_parse_pages()
{

}

void files_index_entry(FILE *idx_file, char *d_name)
{
	int in_name_len = strnlen(IN_ARTICLES_DIR,255);
	in_name_len += strnlen(d_name,255);
	in_name_len++; // For null terminator
	char *in_name = (char *)malloc(sizeof(char) * in_name_len);
	char *ptr = in_name;
	strcpy(ptr, IN_ARTICLES_DIR);
	ptr += strnlen(IN_ARTICLES_DIR,255);
	strcpy(ptr, d_name);
	printf("Opening %s for reading...\n", in_name);
	FILE *art_file = fopen(in_name, "r");	
	if (!art_file)
	{
		printf("Couldn't open %s for reading.\nError %d\n",in_name,errno);
		free(in_name);
		return;
	}
	char *title = NULL;
	char *date = NULL;

	size_t len = 0;

	getline(&title, &len, art_file);
	chomp_newline(title);

	getline(&date, &len, art_file);
	chomp_newline(date);

	fclose(art_file);
	printf("Putting in line about article\n");
	fprintf(idx_file,"<a href=\"articles/\%s.html\">",d_name);
	fprintf(idx_file,"%s: %s<br /><br />",date, title);
	fprintf(idx_file,"</a>\n");
}

void files_build_index()
{
	int article_rollover = 0;
	int article_num = 0;
	DIR *d;
	struct dirent *dir;
	
	char *fname = (char *)malloc(sizeof(char) * 64);
	FILE *idx_file = NULL;

	// Put normal header here
	d = opendir(IN_ARTICLES_DIR);
	if (d)
	{
		while((dir = readdir(d)) != NULL)
		{
			// Be sure a page exists
			if (dir->d_type == DT_REG)
			{
				printf("Building index #%d,%d\n",article_num,article_rollover);
				// Create a new index page
				if (article_rollover == 0)
				{
					if (article_num == 0)
					{
						sprintf(fname,"site/index.html");
					}
					else
					{
						sprintf(fname,"site/index%d.html",article_num);
					}
					idx_file = fopen(fname,"w");
					if (!idx_file)
					{
						printf("Couldn't open %s for writing.\nError %d\n",
							fname,errno);
						free(fname);
						return;
					}
					// Write top-of-page stuff
					page_header(idx_file);
					page_top_bar(idx_file);
					fputs("<div id='wrap' style='width: 768px; margin: 0 auto; text-align: center'>\n",idx_file);
					fputs("<div id='inner' style='text-align: left;'>\n",idx_file);
					fprintf(idx_file,"<ul>\n");
					// TODO: Title for index listing itself
				}

				// Write a line to the index page for the article
				// open dir->d_name

				files_index_entry(idx_file, dir->d_name);

				article_rollover = article_rollover + 1;
				// Index has grown large enough, close this one up.
				if (article_rollover == ARTICLES_PER_PAGE)
				{
					fprintf(idx_file,"</div></div></ul>\n");
					page_footer(idx_file);
					fclose(idx_file);
					idx_file = NULL;
					article_rollover = 0;
					article_num = article_num + 1;
				}
			}
		}
	}
	if (idx_file != NULL)
	{
		fprintf(idx_file,"</ul>\n");
		page_footer(idx_file);
		fclose(idx_file);
	}

	free(fname);
}
