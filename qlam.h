/* qlam.h
:w | !gcc qlam.c -o qlam -Wall
(C) 2020 by marnout à free pt fr  Released under the GPL
*/
#ifndef _MRD_QLAM_H_
#define _MRD_QLAM_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <assert.h>

#define VERSION "2.0"
#define DIR_QLM "qlm/"
#define HEADER \
    (printf("\n\e[1mQlam-%s -- marnout à free _ fr\e[0m\n" , VERSION))

#define SSIZE 4096
#define KEYS "#+-?="
#define WSIZE 32
#define alpha "abcdefghijklmnopqrstuvwxyz"

#define LENGTH(x)  (sizeof(x) / sizeof(x[0]))

//const char KEYS[]= "#+-?=";

typedef char string_t[SSIZE + 1];

// all the six following functions return str

// concatenates strings in static variable _concat_dest and returns it
// the last parameter must be ""
char *concat(char *s, ...);
// concatenate strings in str, using format *fmt as in printf
char *concatf(char *dest, char *fmt,...);

// like strncpy but put '\0' at the end
char *strcpyn(char *dest, char *src, size_t n);

char * insertchr(char *str, char c, size_t pos);

// replace in string str, sub string sub by string rplc
char *replace(char *str, const char *sub, const char *rplc);
// replace in string str, char c, by string tag
char *replacechr(char *str, int c, const char *tag);

// make html tags : i.e. .. -> <i>...</i>, ... -> <b>...</b>,
// ... -> <code>...</code>, ... -> <u>...</u>
char *mktags(char *str);

// make links, i.e. [link->address] become <a href="address">link</a>
char * mklinks(char *str);

// replace in str "\\<" by "&lt;", "\\>" by "&gt;", ... "\\`" by "&agrave;" 
char *mkescapes(char *str);

char *mkhtmlentities(char *str);

void hikeywords(char *str, char *lang);

typedef struct line_t {
	string_t str;
    int tabs; 
    char key;  //
	char kwd[16];
    char *args;
    //size_t len;  // strlen(line)
} line_t;


void fields_set(line_t *line, char *string);
// read line->str from file *fp, prune the '\n' and set fields top and len
char *line_get(line_t *line, FILE *fp);

// return 1 if line attributes key and str are # and kwd
int matchkwd(line_t line, char *kwd);

char *urlencode(char *url);
void abstract(char *class);
void article(char *str); 
void body(int bare);
void code();
void dl(line_t line);
void head(int bare);
void hlink(char *s);
void inchtml(const char *fname);
void js(char *);
line_t list(line_t line);
void note(char *num);
void meta(char *args);
void subtitle(char *kwd, char *str);
void style(char *str);
void table(char *str);

#endif

