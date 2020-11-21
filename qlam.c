/* qlam.c
:w | !gcc % -o bin/%< -Wall
bin/%< -d ~/www/qlam doc
*/

#include "qlam.h"

FILE *qlm, *html;
int debug = 0;

/** concat(dest, "%s[%s...]", s1, [s2, ...]); concatenate strings
returns dest
*/

char *
concat(char *s1,...)
{
    va_list ap;
    va_start(ap, s1);
    char *s;
	static string_t _concat_dest;

    strcpy(_concat_dest, s1);
    while((s = va_arg(ap, char *)) && *s != 0) strcat(_concat_dest, s);

    va_end(ap);
    return _concat_dest;
}

char *
concatf(char *dest, char *fmt, ...)
{
	va_list(ap);
	va_start(ap, fmt);
	vsprintf(dest, fmt, ap);
	va_end(ap);
	return dest;
}

char *
prunenl(char *str)
{
	if(*str) {
		size_t ln = strlen(str) - 1;
		if(str[ln] == '\n') str[ln] = 0;
	}
	return str;
}

char *
strcpyn(char *dest, char *src, size_t n)
{
	*dest = 0;
	return strncat(dest, src, n);
}

char *
insertchr(char *dest, char c, size_t pos)
{
	char *p = dest + pos;
	if(pos < strlen(dest) + 1) {
		memmove(p + 1, p, strlen(p) + 1);
		*p = c;
	}
	return dest;
}

char *
insertstr(char *dest, char *src, size_t pos)
{
	size_t len = strlen(src);
	char *p = dest + pos;
	if(pos < strlen(dest) + 1) {
		memmove(p + len, p, strlen(p) + 1);
		memcpy(p, src, len);
	}
	return dest;
}

char *
replace(char *str, const char *sub, const char *rplc)
{
    string_t cpy;
    strcpy(cpy, str);
    *str = 0;

    char *p, *q;
    size_t lsub = strlen(sub);

    p = cpy;
    while((q = strstr(p, sub)) != NULL) {
	strncat(str, p, q - p);
	strcat(str, rplc);
	p = q + lsub;
    }
    strcat(str, p);
    return str;
}

char *
replacechr(char *str, int c, const char *rplc)
{
    string_t cpy;
    strcpy(cpy, str);
    *str = 0;

    char *p, *q;

    p = cpy;
    while((q = strchr(p, c)) != NULL) {
	strncat(str, p, q - p);
	strcat(str, rplc);
	p = q + 1;
    }
    strcat(str, p);
    return str;
}

char *
mktags(char *str)
{
    static struct {
	int c;
	char r[6];
    } map[] = {{'', "i"}, {'', "b"}, {'', "u"}, {'', "span"} };

    char *p, *p1, *p2;
	string_t cpy; 
    for(int n=0; n<4; n++) {
		strcpy(cpy, str);
		p = cpy;
		*str = 0;
		while((p1 = strchr(p, map[n].c)) != NULL &&
			(p2 = strchr(p1 + 1, map[n].c)) != NULL ) 
		{
			strncat(str, p, p1 - p);
			if(n == 3)
				sprintf(str + strlen(str), "<%s class=\"box\">", map[n].r);
			else
				sprintf(str + strlen(str), "<%s>", map[n].r);
			strncat(str, p1 + 1, p2 - p1 - 1);
			sprintf(str + strlen(str), "</%s>", map[n].r);
			p = p2 + 1;
		}
		strcat(str, p);
		strcpy(cpy, str);
		p = cpy;
		if((p1 = strchr(p, map[n].c)) != NULL) {
			strcpyn(str, p, p1 - p);
			if(n == 3)
				sprintf(str + strlen(str), "<%s class=\"box\">", map[n].r);
			else
				sprintf(str + strlen(str), "<%s>", map[n].r);
			strcat(str, p1 + 1);
			sprintf(str + strlen(str) - 1, "</%s>\n", map[n].r);
		}
    }

    return str;
}

char *
mkstrings(char *str)
{
	char *p, *p1, *p2;
	string_t cpy;
	strcpy(cpy, str);
	p = cpy;
	*str = 0;
	while((p1 = strchr(p, '\"')) && (p2 = strchr(p1 + 1, '\"'))) {
		strncat(str, p, p1 - p);
		strcat(str, "\"<i>");
		strncat(str, p1 + 1, p2 - p1 - 1);
		strcat(str, "</i>\"");
		p = p2 + 1;
	}
	strcat(str, p);
	return str;
}
char *
mkcode(char *str)
{
    char *p, *p1, *p2;
	string_t cpy, inner; 
	strcpy(cpy, str);
	p = cpy;
	*str = 0;
	while((p1 = strchr(p, '')) != NULL &&
		(p2 = strchr(p1 + 1, '')) != NULL ) 
	{
		strncat(str, p, p1 - p);
		sprintf(str + strlen(str), "<code>");
		strcpyn(inner, p1 + 1, p2 - p1 - 1);
		strcat(str, mkhtmlentities(inner));
		sprintf(str + strlen(str), "</code>");
		p = p2 + 1;
	}
	strcat(str, p);
	strcpy(cpy, str);
	p = cpy;
	if((p1 = strchr(p, '')) != NULL) {
		strcpyn(str, p, p1 - p);
		sprintf(str + strlen(str), "<code>");
		strcpy(inner, p1 + 1);
		strcat(str, mkhtmlentities(inner));
		sprintf(str + strlen(str), "</code>");
	}

    return str;

}

char *
mklinks(char *str)
{
    char *p, *p1, *p2, *p3;
    string_t cpy, href;
    strcpy(cpy, str);
    *str = 0;

    p = cpy;
    while( (p1 = strchr(p, '[')) != NULL &&
	    (p2 = strstr(p1 + 1, "->")) != NULL &&
	    (p3 = strchr(p2 + 2, ']')) != NULL )
    {
	strncat(str, p, p1 - p);
	strcat(str, "<a href=\"");
	strcpyn(href, p2 + 2, p3 - p2 - 2);
	strcat(str, urlencode(href));
	strcat(str, "\">");
	strncat(str, p1 + 1, p2 - p1 - 1);
	strcat(str, "</a>");
	p = p3 + 1;
    }
    strcat(str, p);
    return str;
}

char *
mknotes(char *str)
{
	char *p, *p1, *p2;
	string_t cpy;
	strcpy(cpy, str);
	*str = 0;
	char num[4];

	p = cpy;
	while(  (p1 = strstr(p, "#[")) != NULL &&
			(p2 = strchr(p1, ']')) != NULL )
	{
		strncat(str, p, p1 - p);
		strcpyn(num, p1 + 2, p2 - p1 - 2);
		sprintf(str + strlen(str), "<a class=\"nte\" href=\"#%s\">(%s)</a>", 
			num, num);
		p = p2 + 1;
	}
	strcat(str, p);
	return str;
}

char *
mkhtmlentities(char *str)
{
	struct {
		char c;
		char s[12];
	} map[] = { 
		{'&', "&amp;"},
		{'<', "&lt;"}, 
		{'>', "&gt;"}, 
		{'[', "&lsqb;"}, 
		{']', "&rsqb;"},
		{'#', "&num;"},
	};

	for(int i=0; i<LENGTH(map); i++)
		replacechr(str, map[i].c, map[i].s);
	return str;
}

char * 
mkescapes(char *str)
{
    struct {
       char s1[4];
       char s2[12];
    } map[] = {
    	{"\\<",  "&lt;" },
      	{"\\>",  "&gt;" },
       	{"\\[",  "&lsqb;" },
       	{"\\]",  "&rsqb;" },
       	{"\\/",  "&sol;" },
       	{"\\#",  "&num;" },
       	{"\\*",  "&ast;" },
       	{"\\_",  "&lowbar;" },
       	{"\\\n", "&bsol;n" },
       	{"\\'",  "&apos;" },
       	{"\\\"", "&quot"},
       	{"\\`",  "&agrave;"},
		{"\\&",  "&amp;"}
    };

    for(int i=0; i<LENGTH(map); i++)
		replace(str, map[i].s1, map[i].s2);
    return str;
}

char *
mkhtml(char *str)
{
	mkescapes(str);
	mkcode(str);
	mktags(str);
	mklinks(str);
	return str;
}

/**
Qlam
*/

void
fields_set(line_t *line, char *string)
{
	size_t nl = strlen(string) - 1;
	if(string[nl] == '\n') string[nl] = 0;
	strcpy(line->str, string);
    char *p = line->str;
	// count leading tabs
    line->tabs = 0;
    while(*p == '\t') {
		line->tabs++;
		p++;
    }
	// check for key and set it
    if(strchr(KEYS, *p) != NULL) 
		line->key = *p++;
    else 
		line->key = 0;
	// kwd
	*line->kwd = 0;
	if(line->key == '#') {
		char *q = strchr(p, ' ');
		if(q) {
			strcpyn(line->kwd, p, q - p);
			p = q + 1;
		} else {
			strcpy(line->kwd, p);
			p = p + strlen(p);
		}
	} 
	// prune white spaces after kwd if any
	while(*p == ' ' || *p == '\t') p++;
	line->args = p;
}

char * 
line_get(line_t *line, FILE *fp)
{
	string_t string;
    char *ret = fgets(string, SSIZE, fp);
	fields_set(line, string);
    return ret;
}


void 
abstract(char *class)
{
	fprintf(html, "<!--abstract-->\n");
	// write "<div class="<class>">
	if(*class) fprintf(html, "<div class=\"%s\">\n", class);
	else fprintf(html, "<div class=\"substance\">\n");
	// write contents
	string_t buffer;
	while(fgets(buffer, SSIZE, qlm) && *buffer != '\n') {
		fprintf(html, mkhtml(buffer));
	}
	fprintf(html, "</div>\n");
	fprintf(html, "<!--abstract-->\n");
}

void
anchor(char *args)
{
	char *target, *name;
	target = strtok(args, " ");
	name = strtok(NULL, " ");
	if(name != NULL)
		fprintf(html, "<a id=\"%s\">%s</a>\n", target, name);
	else
		fprintf(html, "<a id=\"%s\"></a>\n", target);
}

char *
urlencode(char *url)
{
	replacechr(url, ' ', "%20"); 
	replace(url, "é", "e");
	replace(url, "è", "e");
	replace(url, "ê", "e");
	replace(url, "à", "a");
	replace(url, "â", "a");
	replace(url, "ù", "u");
	replace(url, "î", "i");
	return url;
}

void 
article(char *name) 
{
	strcat(name, ".html");

	string_t buffer;
	char title[128] = {0};
	FILE *fp = fopen(name, "r");
	if(fp == NULL) {
		printf("Ignore %s !\n", name);
		return;
	} 
	// skip lines until <title>
	while(fgets(buffer, SSIZE, fp) && strncmp(buffer, "\t<title>", 8));
	// write title
	char *p = buffer + 8;
	strncat(title, p, strstr(p, "</title>") - p);
	fprintf(html, "<h4><a href=\"%s\">%s</a></h4>\n", name, title);
	
	// find begin of abstract text
	while(fgets(buffer, SSIZE, fp) && strcmp(buffer, "<!--abstract-->\n"));
	// copy
	while(fgets(buffer, SSIZE, fp)) {
		if(!strcmp(buffer, "<!--abstract-->\n")) break;
		fprintf(html, buffer);
	}
	fclose(fp);
}


int
iskwd(line_t line, char *kwd)
{
    return !strcmp(line.kwd, kwd);
}

void
body(int bare)
{
    line_t line;
    
    while(line_get(&line, qlm)) {
		begin:

		if(debug) 
			printf("key: '%c', kwd: \"%s\", args: \"%s\"\n", 
				line.key, line.kwd, line.args);
		
		if(!*line.str) continue;

		switch(line.key) {
			case '#' :
				if(!*line.kwd) 
					fprintf(html, "<!-- %s -->\n", line.args);

				if('1' <= *line.kwd && *line.kwd <= '6') 
					subtitle(line.kwd, line.args);
				
				if(iskwd(line, "substance")) abstract(line.args);
				if(iskwd(line, "article")) article(line.args);
				if(iskwd(line, "code")) code(line.args);
				if(iskwd(line, "js")) js(line.args);
				if(iskwd(line, "note")) note(line.args);
				if(iskwd(line, "table")) table(line.args);
				if(iskwd(line, "inchtml")) inchtml(line.args);
				if(iskwd(line, ":")) anchor(line.args);
				break;
			case '+' :
			case '-' :
				//if(line.tabs) {
					line = list(line);
					goto begin;
				//}
				break;
			case '?' :
				dl(line);
				break;
			//TODO <div>

			default :
				mknotes(line.str);
				mkhtml(line.str);
				fprintf(html, "<p>%s</p>\n", line.str);
				break;
		}
    }
	fprintf(html, "<a id=\"end\"></a>\n");
	if(!bare) {
	fprintf(html, "<br><br><br><hr>\n");
	fprintf(html, "Réalisé avec <a href=\"http://arad.free.fr/qlam/\">");
	fprintf(html, "Qlam</a> - LGPL");
	}
	fprintf(html, "\n</body>\n</html>");
}

void 
hikeywords(char *str, char *key_words)
{
	char *p, *word;
	size_t plen, wlen;
	string_t kwds;
	p = str;
	while(*p) {
		strcpy(kwds, key_words);
		p += strcspn(p, alpha); // at first alpha char
		plen = strspn(p, alpha); // word length
		if(!*(p + plen)) break; // next chars are not alpha
		word = strtok(kwds, " ");
		do {
			wlen = strlen(word);
			if(!strncmp(word, p, plen < wlen ? wlen : plen)) {
				insertchr(p, '', 0);
				insertchr(p, '', plen + 1);
				p += 2;
				break;
			}
			word = strtok(NULL, " ");
		} while(word);
		
		p += plen + 1;
	}
}

void
code(char *lang)
{
	string_t key_words;
	char comment[4], begin[4], end[4];
	*key_words = *comment = *begin = *end = 0;
	char *p;
	if(*lang) {
		string_t fname;
		concatf(fname, "%s%s%s", "include/", lang, ".kwds");
		FILE *fp = fopen(fname, "r");
		if(fp != NULL) {
			fgets(key_words, SSIZE, fp);
			if(!feof(fp)) fgets(begin, 4, fp);
			prunenl(begin);
			if(!feof(fp)) fgets(end, 4, fp);
			prunenl(end);
			if(!feof(fp)) fgets(comment, 4, fp);
			prunenl(comment);
		}
		fclose(fp);
	}
	
    string_t line;
    fprintf(html, "<pre>\n");
	
    while(fgets(line, SSIZE, qlm)) {
		if(*line == '\n') break;
		mkhtmlentities(line);
		if(*lang && *key_words) {
			hikeywords(line, key_words);
			if(*comment && (p = strstr(line, comment))) 
				insertchr(line, '', p - line);
			
			if(*begin) {
				if((p = strstr(line, begin))) 
					insertstr(line, "<i>", p - line);
				if((p = strstr(line, end))) 
					insertstr(line, "</i>", p - line);
			} 
			mkstrings(line);
		}
		mktags(line);
		fprintf(html, "%s", line);
    }
    fprintf(html, "</pre>\n");
}

void
dl(line_t line) 
{
	fprintf(html, "<dl>\n");
	 do {
		fprintf(html, "\t<dt>%s</dt>\n", mkhtml(line.args));
		while(line_get(&line, qlm) && line.tabs == 1) {
			fprintf(html, "\t<dd>%s</dd>\n", mkhtml(line.args));
		}
	} while(line.key == '?');
	fprintf(html, "</dl>\n");
}

void 
head(int bare)
{
    line_t line;
    string_t title;

    line_get(&line, qlm);
    if(line.key == '#') line_get(&line, qlm); // skip first comment
    strcpy(title, line.str);

    fprintf(html, "%s\n", "<!DOCTYPE html>");
    fprintf(html, "<html lang=\"fr\">\n<head>\n");
    fprintf(html, "\t%s\n", "<meta charset=\"utf-8\">");
    meta("generator:qlam");
	meta("viewport:width=device-width, initial-scale=1");
	hlink("icon:favicon.png");
    if(access("css/style.css", F_OK) !=-1) style("style");
	if(!bare && access("css/header.css", F_OK) !=-1) style("header");
    while(line_get(&line, qlm) && !iskwd(line, "date")) {
		if(line.key == '#') {
			if(iskwd(line, "meta")) meta(line.args);
			if(iskwd(line, "style")) style(line.args);
			if(iskwd(line, "js")) js(line.args);
		} else
			fprintf(html, "\t%s\n", line.str);
	}

    fprintf(html, "\t<title>%s</title>\n", title);
    fprintf(html, "</head>\n<body>\n");
    if(!bare && access("include/header.html", F_OK) !=-1)
		inchtml("include/header");
	if(!bare)
		fprintf(html, "<p class=\"date\">%s</p>\n", line.str + 5);
    fprintf(html, "<h1>%s</h1>\n", title);
	fprintf(html, "<a id=\"top\"></a>\n");
}

void 
hlink(char *s)
{
	char rel[64], href[256];
	char *p = strchr(s, ':');
	if(p && *p) {
		//*rel = 0;
		strcpyn(rel, s, p - s);
		strcpy(href, p + 1);
		fprintf(html, "\t<link rel=\"%s\" href=\"%s\">\n", rel, href);
	} 
}

void
inchtml(const char *s)
{
	string_t fname;
	strcpy(fname, s);
	strcat(fname,  ".html");
    FILE *fp = fopen(fname, "r");
	if(fp == NULL) 
		printf("%s ? !!! \nIgnore %s\n", fname, fname);
	else {
		string_t str;
		while(fgets(str, SSIZE, fp))
		fprintf(html, str);
		fclose(fp);
	}
}

void 
js(char *src)
{
	if(!*src) {
		string_t buffer;
		fprintf(html, "<script>\n");
		while(fgets(buffer, SSIZE, qlm) && *buffer != '\n')
			fprintf(html, "\t%s", buffer);
		fprintf(html, "</script>\n");
	} else {
		fprintf(html, "\t<script src=\"script/%s.js\"></script>\n", src); 
	}
}

line_t
list(line_t line)
{
    char key = line.key;
    int tabs = line.tabs;
	char stabs[4] = {0};
	for(int t=0; t<tabs; t++) strcat(stabs, "\t");
    fprintf(html, "%s<%s>\n", stabs, key == '+' ? "ol" : "ul");
    
    do {
		fprintf(html, "%s<li>%s</li>\n", stabs, mkhtml(line.args));
		line_get(&line, qlm);
		if(line.tabs > tabs) 
			line = list(line);
		
		if(line.key != key || line.tabs < tabs) 
			break;
		
    } while(!feof(qlm));
    fprintf(html, "%s</%s>\n", stabs, key == '+' ? "ol" : "ul");
    
    return line;
}

void
meta(char *str)
{
	char name[64], content[1024];
	char *p = strchr(str, ':');
	if(p) {
		//*name = 0;
		strcpyn(name, str, p - str);
		strcpy(content, p + 1);
		fprintf(html, "\t<meta name=\"%s\" content=\"%s\">\n", name, content);
	} else
		fprintf(html, "<meta %s>\n", str);
}

void 
note(char *num)
{
	while(*num == ' ') num++;
	fprintf(html, "<a id=\"%s\">(%s)</a>\n", num, num);
	fprintf(html, "<div class=\"note\">\n");
	char buffer[SSIZE];
	
	while(fgets(buffer, SSIZE, qlm) && *buffer != '\n') {
		mkhtml(buffer);
		fprintf(html, "<p>%s</p>\n", buffer);
	}
	fprintf(html, "</div>\n");
}

void
subtitle(char *kwd, char *str)
{
	string_t href;
	strcpy(href, str);
	urlencode(href);
    fprintf(html, "<a id=\"%s\"></a>\n", href);
    fprintf(html, "<h%c>%s</h%c>\n", *kwd, str, *kwd);
}

void
style(char *s)
{
	char *p = strchr(s, ' ');
	if(p)
		fprintf(html, "\t<link rel=\"stylesheet\" href=\"css/%s.css\">\n", p + 1);
	else
		fprintf(html, "\t<link rel=\"stylesheet\" href=\"css/%s.css\">\n", s);
}

void
table(char *class)
{
	line_t line;
	// write "<table>" or <table class="<class>"
	if(*class) fprintf(html, "<table class=\"%s\">\n", class);
	else  fprintf(html, "<table>\n");
	// headers
	line_get(&line, qlm);
	mkhtml(line.str);
	replacechr(line.str, '|', "</th><th>");
	fprintf(html,"\t<tr>\n\t<th>%s</th>\n\t</tr>\n", line.str);
	// rows
	while(line_get(&line, qlm)) {
		if(!*line.str) break;
		mkhtml(line.str);
		replacechr(line.str, '|', "</td><td>");
		fprintf(html,"\t<tr>\n\t<td>%s</td>\n\t</tr>\n", line.str);
	}

	fprintf(html, "</table>");
}

#ifndef LIB_QLAM

void
usage(int i)
{
    printf("Usage: qlam [-a][-b][-h] [-d directory] FILE [FILE ...]\n");
    if(i) exit(EXIT_FAILURE);
    exit(EXIT_SUCCESS);
}

int
main(int argc, char *argv[]) 
{ HEADER;
    
    if(argc < 2) usage(1);

    char opt;
    int index, bare = 0;
    char filename[512];

    while((opt = getopt(argc, argv, "abghd:")) != -1) {
		switch (opt) {
			case 'a':
				break;
			case 'b':
				bare = 1;
				break;
			case 'g':
				debug = 1;
				break;
			case 'h':
				usage(0);
				break;
			case 'd':
				chdir(optarg);
				break;
			default:
				usage(1);
		}
    }

    for(index = optind; index < argc; index++) {

		// cut suffix .html if any, to allow bash completion in cmd line
		string_t name;
		strcpy(name, argv[index]);
		char *p = strstr(name, ".html");
		if(p != NULL && p - name == strlen(name) - 5) *p = 0;
		if((p = strchr(name, '/'))) memmove(name, p + 1, strlen(p));
		// make FILE qlm
		concatf(filename, "%s%s%s", "qlm/", name, ".qlm");
		printf("%s ", filename);
		qlm = fopen(filename, "r");
		if(qlm == NULL) {
			printf("?  ignore %s. Stop !\n", filename);
			exit(EXIT_FAILURE);
		}
		// make FILE html
		concatf(filename, "%s%s", name, ".html");
		html = fopen(filename, "w");
		assert(html != NULL);
		printf("-->  %s\n", filename);

		head(bare);
		body(bare);
		
		fclose(qlm);
		fclose(html);
    }
    return 0;
}

#endif
