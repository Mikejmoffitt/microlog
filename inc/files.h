#ifndef FILES_H
#define FILES_H

#include <dirent.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

void files_process_article(char *d_name);
void files_parse_articles(void);

void files_process_page(char *d_name);
void files_parse_pages(void);

void files_index_entry(FILE *idx, char *d_name);
void files_build_index(void);

void files_copy_res(void);

#endif
