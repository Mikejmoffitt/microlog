#include "files.h"
#include "page.h"
 
void files_process_article(char *d_name, char *in_dir, char *out_dir)
{
	printf("--------------------------------------------------\n");

	// Determine the input filename
	int in_name_len = strnlen(in_dir,255);
	in_name_len += strnlen(d_name,255);
	in_name_len++; // For null terminator

	char *in_name = (char *)malloc(sizeof(char) * in_name_len);
	char *ptr = in_name;
	strcpy(ptr, in_dir);
	ptr += strnlen(in_dir,255);
	strcpy(ptr, d_name);


	// Do the same for output
	int out_name_len = strnlen(out_dir,255);
	out_name_len += strnlen(OUT_APPEND,255);
	out_name_len += strnlen(d_name,255);
	out_name_len++; // For null terminator

	char *out_name = (char *)malloc(sizeof(char) * out_name_len);
	ptr = out_name;
	strcpy(ptr, out_dir);
	ptr += strnlen(out_dir,255);
	strcpy(ptr, d_name);
	ptr += strnlen(d_name,255);
	strcpy(ptr,OUT_APPEND);

	printf("Opening %s for reading...\n", in_name);
	FILE *in_file = fopen(in_name,"r");
	if (!in_file)
	{
		printf("Couldn't open %s for reading.\nError %d\n",in_name,errno);
		free(in_name);
		free(out_name);
		return;
	}	
	printf("Opening %s for writing...\n", out_name);
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

void files_parse_dir(char *dir_path, char *out_path)
{
	DIR *d;
	struct dirent *dir;
	d = opendir(dir_path);
	if (d)
	{
		while ((dir = readdir(d)) != NULL)
		{	
			// Be sure this is not a symlink, directory, etc
			// Generate an article based on the file present
			if (dir->d_type == DT_REG && dir->d_name[0] != '.')
			{
				files_process_article(dir->d_name, dir_path, out_path);
			}
		}
		closedir(d);
	}
}

void files_index_entry(FILE *idx_file, char *articles_dir, char *d_name)
{
	int in_name_len = strnlen(articles_dir,255);
	in_name_len += strnlen(d_name,255);
	in_name_len++; // For null terminator
	char *in_name = (char *)malloc(sizeof(char) * in_name_len);
	char *ptr = in_name;
	strcpy(ptr, articles_dir);
	ptr += strnlen(articles_dir,255);
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
	int num_pages = n / ARTICLES_PER_PAGE;
	num_pages++;
	while (n--)
	{
		if (list[n] && list[n]->d_type == DT_REG && list[n]->d_name[0] != '.')
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
				fputs("<div id='wrap'>\n",idx_file);
				fputs("<div id='inner'>\n",idx_file);
				if (num_pages > 1)
				{
					fprintf(idx_file,"<br /><h3>Articles (Page %d / %d)</h3><br />\n",index_num+1,num_pages);
				}
				else
				{
					fputs("<br /><h3>Articles</h3><br />\n",idx_file);
				}
				fprintf(idx_file,"<div id='listing'>\n");
				// TODO: Title for index listing itself
			}
	
			// Write a line to the index page for the article
			// open dir->d_name
	
			files_index_entry(idx_file, IN_ARTICLES_DIR, list[n]->d_name);
	
			index_rollover = index_rollover + 1;
			// Index has grown large enough, close this one up.
			if (index_rollover == ARTICLES_PER_PAGE || n == 0)
			{
				fprintf(idx_file,"<br /></div></div></div>\n");
				fprintf(idx_file,"<h6>");
				if (index_num != 0)
				{
					if (index_num != 1)
					{
						fprintf(idx_file,"<a href='index%d.html'><< Previous </a>",index_num - 1);
					}
					else
					{
						fprintf(idx_file,"<a href='index.html'><< Previous </a>");
					}	
				}
				else
				{
					fprintf(idx_file,"<span id='nav-seperator'><< Previous </span>");
				}
				fprintf(idx_file,"<span id='nav-seperator'> | </span>");
				// Next link
				if (n != 0)
				{
					fprintf(idx_file,"<a href='index%d.html'>Next >></a>\n",index_num + 1);
				}
				else
				{
					fprintf(idx_file,"<span class='disabled'>Next >></span>\n");
				}
				fprintf(idx_file,"</h6>");
				page_footer(idx_file);
				fclose(idx_file);
				idx_file = NULL;
				index_rollover = 0;
				index_num = index_num + 1;
			}
		}
		// Make dummy entries to maintain page length
		if (n == 0)
		{
			while (index_rollover < ARTICLES_PER_PAGE)
			{
				fprintf(idx_file," <br /><br />\n");
				index_rollover++;
			}
		}
		printf("Trying to free the list entry\n");
		if (list[n])
		{
			printf("REALLY DOING IT \n");
			free(list[n]);
			printf("Doneskies\n");
		}
	}
	if (list)
	{
		free(list);
	}
	if (!fname || !idx_file)
	{
		return;
	}
	fprintf(idx_file,"<br /></div></div></div>\n");
	fprintf(idx_file,"<h6>");
	if (index_num != 0)
	{
		if (index_num != 1)
		{
			fprintf(idx_file,"<a href='index%d.html'><< Previous </a>",index_num - 1);
		}
		else
		{
			fprintf(idx_file,"<a href='index.html'><< Previous </a>");
		}	
	}
	else
	{
		fprintf(idx_file,"<span id='nav-seperator'><< Previous </span>");
	}
	fprintf(idx_file,"<span id='nav-seperator'> | </span>");
	fprintf(idx_file,"<span id='nav-seperator'>Next >></span>\n");
	fprintf(idx_file,"</h6>");
	page_footer(idx_file);
	fclose(idx_file);
	free(fname);
}

void files_generate_pages_bar(FILE *page, int root)
{
	fprintf(page,"<br /><h5 id='navbar'>");
	DIR *d;
	struct dirent *dir;
	d = opendir(IN_PAGES_DIR);
	int first = 1;
	if (d)
	{
		while ((dir = readdir(d)) != NULL)
		{	
			// Be sure this is not a symlink, directory, etc
			// Generate an article based on the file present
			if (dir->d_type == DT_REG && dir->d_name[0] != '.')
			{
				if (!first)
				{
					fprintf(page, " <span id='nav-seperator'>|</span> ");
				}
				// Determine the input filename
				int in_name_len = strnlen(IN_PAGES_DIR,255);
				in_name_len += strnlen(dir->d_name,255);
				in_name_len++; // For null terminator

				char *in_name = (char *)malloc(sizeof(char) * in_name_len);
				char *ptr = in_name;
				strcpy(ptr, IN_PAGES_DIR);
				ptr += strnlen(IN_PAGES_DIR,255);
				strcpy(ptr, dir->d_name);
				printf("Opening %s for reading...\n", in_name);
				FILE *in_file = fopen(in_name,"r");
				if (!in_file)
				{
					printf("Couldn't open %s for reading.\nError %d\n",in_name,errno);
					free(in_name);
					return;
				}	
				
				// Start of <a> tag
				if (root)
				{
					fprintf(page,"<a href='pages/%s.html'>",dir->d_name);
				}
				else
				{
					fprintf(page,"<a href='../pages/%s.html'>",dir->d_name);
				}

				// Just pull the first line of the page
				char get = fgetc(in_file);
				while (get != EOF && get != '\n' && get != '\r')
				{
					fputc(get,page);
					get = fgetc(in_file);
				}
				fclose(in_file);
				free(in_name);
				fprintf(page,"</a>\n");
				first = 0;
			}
		}
		closedir(d);
	}

	fprintf(page,"</h5><br />");
}

void files_make_structure()
{
	// TODO: system(char *) seriously take this out soon
	// and don't just rely on mkdir silently failing
	system("mkdir -p ./source/");
	system("mkdir -p ./source/articles");
	system("mkdir -p ./source/resources");
	system("mkdir -p ./source/pages");
	system("mkdir -p ./site/articles");
	system("mkdir -p ./site/pages");
	system("mkdir -p ./site/res");
}

void files_copy_res()
{
	// TODO: Don't use system(char *)! This is terrible code!
	system("cp ./source/style.css ./site/style.css");
	system("cp -n -r ./source/resources/* ./site/res/");
}
