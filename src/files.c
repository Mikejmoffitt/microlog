#include "files.h"

#include "page.h"

#define IN_ARTICLES_DIR "source/articles/"
#define OUT_ARTICLES_DIR "site/articles/"
#define OUT_APPEND ".html"

#define IN_PAGES_DIR "source/pages/"
#define OUT_PAGES_DIR "site/pages/"

#define OUT_INDEX_DIR "site/"

#define ARTICLES_PER_PAGE 100

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
			if (dir->d_type == DT_REG && dir->d_name[0] != '.')
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
	printf("   Opening %s for reading...\n", in_name);
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
	printf("   Making index entry... ");
	fprintf(idx_file,"<a href=\"articles/\%s.html\">",d_name);
	fprintf(idx_file,"%s: %s<br /><br />",date, title);
	fprintf(idx_file,"</a>\n");
	printf("ok.\n");
}

void files_build_index()
{
	printf("-------{ CONSTRUCTING INDECES }-------\n");
	int index_rollover = 0;
	int index_num = 0;
	struct dirent **list;
	
	char *fname = (char *)malloc(sizeof(char) * 64);
	FILE *idx_file = NULL;

	// Test print the list
	int n = scandir(IN_ARTICLES_DIR,&list,0,alphasort);
	if (n < 0)
	{
		printf("Articles directory is empty. The index will not be built.\n");
		free(list);
		return;
	}
	while (n--)
	{
		if (list[n]->d_type == DT_REG && list[n]->d_name[0] != '.')
		{
			// Be sure a page exists
			printf("Building page %d, listing %d\n",index_num+1,index_rollover+1);
			printf("   Article #%s\n",list[n]->d_name);
			// Create a new index page
			if (index_rollover == 0)
			{
				if (index_num == 0)
				{
					sprintf(fname,"site/index.html");
				}
				else
				{
					sprintf(fname,"site/index%d.html",index_num);
				}
				idx_file = fopen(fname,"w");
				if (!idx_file)
				{
					printf("Couldn't open %s for writing.\nError %d\n",fname,errno);
					free(fname);
					free(list[n]);
					while(n--) { free(list[n]); }
					free(list);
					return;
				}
				// Write top-of-page stuff
				page_header(idx_file,1);
				page_top_bar(idx_file);
				fputs("<div id='wrap'>\n",idx_file);
				fputs("<div id='inner'>\n",idx_file);
				fputs("<br /><h3>Articles</h3><br />\n",idx_file);
				fprintf(idx_file,"<div id='listing'>\n");
				// TODO: Title for index listing itself
			}
	
			// Write a line to the index page for the article
			// open dir->d_name
	
			files_index_entry(idx_file, list[n]->d_name);
	
			index_rollover = index_rollover + 1;
			// Index has grown large enough, close this one up.
			if (index_rollover == ARTICLES_PER_PAGE)
			{
				fprintf(idx_file,"</div></div></div><br />\n");
				page_footer(idx_file);
				fclose(idx_file);
				idx_file = NULL;
				index_rollover = 0;
				index_num = index_num + 1;
			}
		}
		free(list[n]);
	}
	free(list);
	fprintf(idx_file,"<br /></div></div></div><br />\n");
	page_footer(idx_file);
	fclose(idx_file);
	free(fname);
}

void files_make_structure()
{
	// TODO: system(char *) seriously take this out soon
	// and don't just rely on mkdir silently failing
	system("mkdir -p ./site/articles");
	system("mkdir -p ./site/pages");
	system("mkdir -p ./site/res");
}

void files_copy_res()
{
	// TODO: Don't use system(char *)!
	system("cp ./source/style.css ./site/style.css");
	system("cp -n -r ./source/resources/* ./site/res/");
}
