#include "page.h"

int page_create_article(FILE *in, FILE *out)
{
	printf("-------{ PARSING ARTICLE }-------\n");
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
	page_top_bar(out);
	fputs("<div id='wrap' style='width: 768px; margin: 0 auto; text-align: center;'>\n",out);
	fputs("<div id='inner' style='text-align: left;'>\n",out);
	fputs("<h2>\n",out);
	fputs(title,out);
	fputs("</h2>\n<span id='date'><i>",out);
	fputs(date,out);
	fputs("</i></span><br /><br />\n",out);
	char get = fgetc(in);

	// Get the rest of the article. 
	while (get != EOF)
	{
		fputc(get,out);

		// TODO: Check for image macro definitions
		get = fgetc(in);

		// Make linebreaks useful for the writer
		if (get == '\n')
		{
			fputs("<br />\n",out);

		}
	}
	fputs("</div>\n</div>\n",out);
	// Free
	free(title);
	free(date);

	page_footer(out);

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
	fputs("<html>\n<head>\n\t<title>mikejmoffitt.com</title>\n",out);
	fprintf(out,"\t<h1 style='text-align: center; margin: 0 auto;'>");
	fprintf(out,"%s</h1><br />\n",site_title);
}

void page_footer(FILE *out)
{
	
}

void page_top_bar(FILE *out)
{
	
}
