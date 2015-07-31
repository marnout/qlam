/*
project: qlam
file:		qstr.cpp
*/
#include "qlib.h"
/*
#include <string>
#include <iostream>
#include <map>
#include <regex>
*/

#define TEST
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

// replace styles '* *' '/ /' '_ _' by <b> </b> <i> </i> <u> </u>
string replaceStyles(string& s) {

	map <string,string> map_styles = {
		{"'*", "<b>"}, {"*'", "</b>"},
		{"'/", "<i>"}, {"/'", "</i>"},
		{"'_", "<u>"}, {"_'", "</u>"}
	}; // map_styles
	
	for (auto& x: map_styles) {
		s = replaceAll(s, x.first, x.second);
	} // for 

	return s;
} // replaceStyles

/*
typedef struct {
	string key;
	string val;
} Map;
*/

// replace [link->target] by <a href="target">link</a>
string replaceLinks(string& s) {
	// map<string,string> works greedly, why ?
	Map map_links[] = {
		{"\\[(.+?)->([^.]+?)\\]",	"<a href=\"#$2\">$1</a>"}
		,{"\\[(.+?)->(.+?\\..+?)\\]", "<a href=\"$2\">$1</a>"}
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
	s = regex_replace(s, e, "<a href=\"#$2\">$1</a>");
	e = "\\[(.+?)->(.+?\\..+?)\\]";
	s = regex_replace(s, e, "<a href=\"$2\">$1</a>");
	
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
	text = regex_replace(text, e, "<a href=\"#$2\">$1</a>");
	e = "\\[(.+?)->(.+?\\..+?)\\]";
	text = regex_replace(text, e, "<a href=\"$2\">$1</a>");
	*/
	cout << text << endl;

	END
	return 0;
}
#endif
