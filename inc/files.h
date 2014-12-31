#ifndef FILES_H
#define FILES_H

#include <dirent.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#define IN_ARTICLES_DIR "source/articles/"
#define OUT_ARTICLES_DIR "site/articles/"
#define IN_PAGES_DIR "source/pages/"
#define OUT_PAGES_DIR "site/pages/"
#define OUT_INDEX_DIR "site/"
#define OUT_APPEND ".html"
#define ARTICLES_PER_PAGE 15

// Filename to parse, directory to look in, directory to put resulting HTML file 
void files_process_article(char *d_name, char *in_dir, char *out_dir);

// Directory to pull pages from, directory to dump resulting HTML files
void files_parse_dir(char *dir_path, char *out_path);

// Slap on a line for the article d_name in articles_dir onto idx_file
void files_index_entry(FILE *idx_file, char *articles_dir, char *d_name);

// Go through the articles dir and run files_index_entry for each one
void files_build_index(void);

// Look through pages_dir and print top bar links to page
void files_generate_pages_bar(FILE *page, int root);

// TODO: Take out this garbage. Builds the site directory and copies to res/
void files_make_structure(void);
void files_copy_res(void);

#endif
