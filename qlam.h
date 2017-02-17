/*
project: Qlam
file:		qlam.h
version: 0.1.5
sam. juin  4 08:13:55 CEST 2016
*/

#ifndef QLAM_H
#define QLAM_H

#define PROJECT "Qlam"
#define VERSION "0.1.5"
#define AUTHOR "2016 marnout"
#define HEADER cout << endl <<PROJECT << "-"  << VERSION << " -- " << AUTHOR << "\n\n";


#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <regex>
#include <unistd.h>
#include <dirent.h>

using namespace std;

#define SITE "site/"
#define MSG_OPEN "\tOuverture de "
#define MSG_CLOSE "\tFermeture de "
#define MSG_FILE "Conversion de "

// maps ---------------------------------------------------------------------


map <string, string> map_esc = {
	{"\\<", "&lt;"}, {"\\>", "&gt;"},
	{"\\\\[", "&#91;"}, {"\\\\]", "&#93;"},
	{"\\/", "&#47;"}, {"\\#", "&num;"},
	{"\\\\*", "&#42;"}, {"\\_", "&#95;"}
}; // map_entities

map <string, string> map_entities = {
	{"<", "&lt;"}, {">", "&gt;"},
	{"\\[", "&#91;"}, {"\\]", "&#93;"},
	{"/", "&#47;"}, {"#", "&num;"},
	{"\\*", "&#42;"}, {"_", "&#95;"}
}; // map_entities


map <string, string> map_styles = {
	{"'\\*", "<b>"}, {"\\*'", "</b>"},
	{"'/", "<i>"}, {"/'", "</i>"},
	{"'_", "<u>"}, {"_'", "</u>"},
	{"\\[(.+?)->(.+?)\\]", "<a href=\"$2\">$1</a>"},
	{"#\\[(.+?)\\]", "<a href=\"#$1\">($1)</a>"},
	{"\\\\<", "&lt;"}, {"\\\\>", "&gt;"}
	//{"\\\\[", "&#91;"}, {"\\\\]", "&#93;"},
	//{"\\/", "&#47;"}, {"\\#", "&num;"},
	//{"\\\\*", "&#42;"}, {"\\_", "&#95;"}
}; // map_styles

map <string, string> map_head = {
	{"^# (.*)", "<!-- $1 -->"},
	{"^#meta (.*)", "<meta $1>"},
	{"^#style (.*)", 
		"<link rel=\"stylesheet\" type=\"text/css\" href=\"css/$1\">"},
	{"^#js (.*)", "<script src=\"js/$1.js\"></script>"},
	{"^#link (.*)", "<link $1>"},
	{"^#title (.*)", "<title>$1</title>"}
}; // map_htags

map <string, string> map_body = {
	{"^# (.*)", "<!-- $1 -->"},
	{"^#:(.+?) (.*)", "<a id=\"$1\">$2</a>"},
	{"^#([1-6]) (.*)", 
		"<a id=\"$2\"></a>\n<h$1>$2</h$1>"},
	{"^#date (.*)$", "<p class=\"date\">$1</p>"},
	{"^#inc_js (.*)", "<script src=\"$1\"></script>"},
}; // map_htags

string re_cmd[] = {
	"^#(article) ?(.*)", "^#(code) ?(.*)", 
	"^#(inchtml) (.*)", "^#(editorial) (.*)",	"^#(js) *(.*)", 
	"^#(menu) (.*)", "^#(substance) ?(.*)", "^#(table) ?(.*)"
};

// list ---------------------------------------------------------------------
map <char, string> map_list = {
	{'-', "ul"},  {'+', "ol"}, 
	{'?', "dl"}, {'=', "dd"}
}; // map_list

string raw(string s)
{
	regex re;
	for(auto& x: map_entities) {
		re = x.first;
		s = regex_replace(s, re, x.second);
	}
	return s;
}

string styles(string s)
{

	smatch m;
	while(regex_search(s, m, regex("`([^`]+)?`"))) {
		s = m.prefix().str() + "<code>" 
			+ raw(m[1].str()) + "</code>" + m.suffix().str();
	}

	regex re;
	for(auto& x: map_styles) {
		re = x.first;
		s = regex_replace(s, re, x.second);
	}

	return s;
}

/* Qlam ====================================================================



*/

class Qlam 
{
	public:
		string name;
		bool verbose;
		bool footer;

		Qlam();
		Qlam(const string&, bool);
		~Qlam();

		void article(string);
		void body();
		void code(string);
		void div(string);
		void dl(string);
		void head();
		void inchtml(string);
		void editorial(string);
		void js(string);
		void list(int, char, string);
		void menu(string);
		void substance(string);
		void table(string);
		void to_html();
	private:
		ifstream fqlm;
		ofstream fhtml;
		string line;
		smatch m;
		regex re;
	
}; // class Qlam

#endif
