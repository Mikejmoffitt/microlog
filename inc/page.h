#ifndef PAGE_H
#define PAGE_H

#include <stdio.h>
#include <stdlib.h>

// Takes the source from in and generates an article page, putting it in
// file out. Returns a 1 on success, 0 on failure for any reason.
int page_create_article(FILE *in, FILE *out);
void chomp_newline(char *s);

// Puts a general page header up, including site title and navigation bar.
// It ends on the opening of the content. The content should follow.
void page_header(FILE *out);
void page_footer(FILE *out);
void page_top_bar(FILE *out);

// Generates listing of articles
void page_make_index(FILE *out);
#endif
