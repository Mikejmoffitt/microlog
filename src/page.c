#include "page.h"

int page_create_article(FILE *in, FILE *out)
{
	if (!in || !out)
	{
		printf("Couldn't create article.\n");
		return 0;
	}
	char *title = NULL; // 80 characters of whatever
	char *date = NULL; // Format mm/dd/yyyy

	size_t len = 0;

	// Get the article title
	getline(&title, &len, in);
	chomp_newline(title);
	// And the date
	getline(&date, &len, in);
	chomp_newline(date);
	printf("Title: %s\n%s\n",title,date);

	// Print them

	page_header(out);
	fputs(title,out);
	fputs("\n",out);
	fputs(date,out);
	fputs("\n",out);
	char get = fgetc(in);

	// Get the rest of the article. 
	while (get != EOF)
	{
		fputc(get,out);

		// TODO: Check for image macro definitions
		get = fgetc(in);
	}

	// Free
	free(title);
	free(date);

	return 1;
}

void chomp_newline(char *s)
{
	while(*s && *s != '\n' && *s != '\r') { s++; }
	*s = 0; 
}

void page_header(FILE *out)
{
	fputs("<!-- Warning: Ugly generated HTML. I'm not a web designer! -->\n",out);
	fputs("<!DOCTYPE html>\n",out);
	fputs("<link rel=\"stylesheet\" href=\"style.css\">\n",out);
	fputs("<html>\n<head>\n\t<title>mikejmoffitt.com</title>\n</head>\n",out);
}

void page_footer(FILE *out)
{
	
}

void page_top_bar(FILE *out)
{
	
}
