/*
project: qlam
file:		map.cpp
*/
#define PROJECT "qlam"
#define VERSION 0.1
#define AUTHOR "2015 Grognon <contact@grognon.net>"

#include <string>
#include <iostream>
#include <map>

using namespace std;

map <string,int> dict = {
	{"version", 1},
	{"no", 2},
	{"mounth", 6}
};
/* usage
	for (auto& x: dict) {
		cout << x.first << ": " << x.second << endl;
	}
	cout << "at version: " << dict.at("version") << endl;
*/

int main() {

	return 0;
};

