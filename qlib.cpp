/*
project: qlam
file:		qstr.cpp
*/
#define TEST
#include "qlib.h"

#include <string>
#include <iostream>
#include <map>
#include <regex>

using namespace std;

// constants --------------------------------------------------------------
// escape chars

// replaceAll(txt, s, r): replace in txt all occurences of s by r
// return txt
string replaceAll(string& txt, const string& s, const string& r) {
	size_t p = 0;
	while ((p = txt.find(s, p)) != string::npos) {
		txt.replace(p, s.length(), r);
		p += r.length();
	}
	return txt;
} // replaceAll

string replaceStyles(string& s) {

	map <string,string> map_styles = {
		{"'*", "<B>"}, {"*'", "</B>"},
		{"'/", "<I>"}, {"/'", "</I>"},
		{"'_", "<U>"}, {"_'", "</U>"}
	}; // map€scChars
	
	for (auto& x: map_styles) {
		s = replaceAll(s, x.first, x.second);
	} // for 

	return s;
} // replaceEscChars

/*
string replaceLinks(string& s) {
// work greedly
	map <string,string> map_links = {
		{"\\[(.+?)->([^.]+?)\\]",	"<A href=\"#$2\">$1</A>"}
		,{"\\[(.+?)->(.+?\\..+?)\\]", "<A href=\"$2\">$1</A>"}
		, {"# (.*)","<!-- $1 -->"}
	}; // map_links

	regex re;
	for (auto& x: map_links) {
		re = x.first;
		s = regex_replace(s, re, x.second);
	} // for 
	
	return s;
} // replaceLinks
*/

typedef struct {
	string key;
	string val;
} Map;

string replaceLinks(string& s) {

	Map map_links[] = {
		{"\\[(.+?)->([^.]+?)\\]",	"<A href=\"#$2\">$1</A>"}
		,{"\\[(.+?)->(.+?\\..+?)\\]", "<A href=\"$2\">$1</A>"}
		, {"# (.*)","<!-- $1 -->"}
	}; // map_links

	regex re;
	for (auto& x: map_links) {
		re = x.key;
		s = regex_replace(s, re, x.val);
	} // for 
	
	return s;
} // replaceLinks


string _replaceLinks(string& s) {

	regex e;
	e = "\\[(.+?)->([^.]+?)\\]";
	s = regex_replace(s, e, "<A href=\"#$2\">$1</A>");
	e = "\\[(.+?)->(.+?\\..+?)\\]";
	s = regex_replace(s, e, "<A href=\"$2\">$1</A>");
	
	return s;
} // replaceLinks



#ifdef TEST
int main() {
	BEGIN

	string text = "normal '/italic/'\n";
	text += " [link->target] or [zob->http://zob.cul] end";
	cout << text << endl;
	text = replaceStyles(text);
	text = replaceLinks(text);
	/*
	regex e("\\[(.+?)->([^.]+?)\\]");
	text = regex_replace(text, e, "<A href=\"#$2\">$1</A>");
	e = "\\[(.+?)->(.+?\\..+?)\\]";
	text = regex_replace(text, e, "<A href=\"$2\">$1</A>");
	*/
	cout << text << endl;

	END
	return 0;
}
#endif
