/*
project: qlam
file:		qstr.cpp
*/

#include "qlam.h"

#include <string>
#include <iostream>
#include <map>
#include <regex>

using namespace std;

// constants --------------------------------------------------------------
// escape chars

string rplcEscChars(string s) {

	map <string,string> map_escChars = {
		{"\\`", "&grave;"}, 
		{"\\<", "&lt;"}, {"\\>", "&gt;"},
		{"\\[", "&#91;"}, {"\\]", "&#93;"},
		{"\\*", "&#42;"}, {"\\/", "&#47;"},
		{"\\_", "&#95;"}, {"\\#", "&num;"}
	}; // map€scChars
	
	for (auto& x: map_escChars) {
		regex find_re(x.first);
		s = regex_replace(s, find_re, x.second);
	} // for 

	return s;
}



int main() {
	BEGIN

	string text = "zob \* cul = \[ nuul \* z \]";
	cout << text << endl;
	text = rplcEscChars(text);
	cout << text << endl;
	END
	return 0;
}


