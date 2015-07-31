/*
project: qlam
file:		qlib.h
*/

#ifndef QLIB_H
#define QLIB_H

#include <string>
#include <iostream>
#include <map>
#include <regex>

using namespace std;

#define PROJECT "qlam"
#define VERSION 0.1
#define AUTHOR "2015 Grognon <contact@grognon.net>"
#define BEGIN cout << endl << PROJECT << " - " << VERSION << endl;
#define END cout << endl;


// basic map, map<string, string> work greedly
typedef struct {
	string key;
	string val;
} Map;


// replaceAll(txt, s, r): replace in txt all occurences of s by r
// return txt
string replaceAll(string&, const string&, const string&);

// replaceAll(txt, s, r): replace in txt all occurences of s by r
// return txt
string replaceAll(string&, const string&, const string&);

// replace [link->target] by <a href="target">link</a>
string replaceLinks(string&); 

#endif
