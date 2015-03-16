#include "page.h"

#define WAITING_IMG 1
#define WAITING_VID 2
#define WAITING_AUDIO 3
#define WAITING_CODE 4

void handle_pound(FILE *in, FILE *out)
{
	char get = fgetc(in);
	int waiting = 0;
	switch (get)
	{	
		default:
			// User really wanted pound and whichever char followed it
			fputc('#',out);
			fputc(get,out);
			break;
		case 'i':
			fputs("<img src='../res/",out);
			waiting = WAITING_IMG;
			break;
		case 'v':
			fputs("<br /><video controls>\n<source src='../res/",out);
			waiting = WAITING_VID;
			break;
		case 'a':
			fputs("<br /><audio controls>\n<source src='../res/",out);
			waiting = WAITING_AUDIO;
			break;
		case 'm':
			fputs("<br /><audio controls>\n<source src='",out);
			waiting = WAITING_AUDIO;
			break;
		case 'c':
			fputs("<br /><div id='code'>",out);
			waiting = WAITING_CODE;
			break;
	}
	while (get != EOF)
	{
		get = fgetc(in);
		if (get == '#')
		{
			char otherget = fgetc(in);
			if (otherget == '#')
			{
				fputc(otherget,out);
			}
			else
			{
				switch (waiting)
				{
					case WAITING_IMG:
						fputs("' />",out);
						return;
					case WAITING_VID:
						fputs("' type='video/mp4'><br /></video>",out);
						return;
					case WAITING_AUDIO:
						fputs("' type='audio/mpeg'></audio><br />",out);
						return;
					case WAITING_CODE:
						fputs("</div><br />",out);
						return;
				}
			}
		}
		else
		{	
			if (get == '\n')
			{
				fputs("<br />\n",out);
			}
			else
			{
				fputc(get,out);
			}
		}
	}
}

int page_create_article(FILE *in, FILE *out)
{
	if (!in || !out)
	{
		printf("Couldn't open source file.\n");
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
	printf("Parsing \"%s\"\n",title);

	// Print them

	page_header(out,0);
	page_top_bar(out);
	fputs("<div id='wrap'>\n",out);
	fputs("<div id='inner'>\n",out);
	fputs("<br /><h3>\n",out);
	fputs(title,out);
	fputs("</h3>",out);
	char get = fgetc(in);

	fputs("<div id='content'>\n",out);
	fputs("<span id='date'>Written ",out);
	if (date[0] != '#') // Choice to omit the date
	{
		fputs(date,out);
	}
	fputs("</span><br /><br />\n",out);
	// Get the rest of the article. 
	while (get != EOF)
	{
		if (get == '#')
		{
			handle_pound(in,out);
		}
		else
		{
			fputc(get,out);
		}

		// TODO: Check for image macro definitions
		get = fgetc(in);

		// Make linebreaks useful for the writer
		if (get == '\n')
		{
			fputs("<br />\n",out);
		}
	}
	fputs("<br /><a href='../index.html' id='returnlink'>Back to main index</a><br />\n",out);
	fputs("<br /><br /></div></div>\n</div>\n",out);
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

void page_header(FILE *out, int root)
{
	fputs("<!-- Generated by Microlog. -->\n",out);
	fputs("<!DOCTYPE html>\n",out);
	if (root)
	{
		fputs("<link rel=\"stylesheet\" href=\"style.css\">\n",out);
	}
	else
	{
		fputs("<link rel=\"stylesheet\" href=\"../style.css\">\n",out);
	}
	fprintf(out,"<html>\n<head>\n\t<title>%s</title>\n",site_title);
	fprintf(out,"\t<div><h1>");
	if (root)
	{
		fprintf(out,"<a href='index.html'>%s</a></h1>\n",site_title);
	}
	else
	{
		fprintf(out,"<a href='../index.html'>%s</a></h1>\n",site_title);
	}
	fprintf(out,"<h2>%s</h2>\n",site_subtitle);
	files_generate_pages_bar(out, root);
	fprintf(out,"</div></head>\n<body>");
}

void page_footer(FILE *out)
{	
	fprintf(out,"<h4>This website was generated using <a href='https://github.com/Mikejmoffitt/microlog'>MicroLog</a>.</h4>\n");
	fprintf(out,"</body>");
}

void page_top_bar(FILE *out)
{
	
}
