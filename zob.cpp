#include <iostream>
#include <string>
#include <regex>
#include <iterator>

using namespace std;
int main() {
	/*
	string s("test [link->target] end");
	cout << s << endl;
	regex e("\\[(.*)->(.+)\\]");
	s = regex_replace(s, e, "<A href=\"$2\">$1</A>");
	cout << s << endl;
	*/
	int n;
	cout << "Enter the startinf number > ";
	cin >> n;
	while (n > 0) {
		cout << n << endl;
		--n;
	}
	cout << "Fire!\n";
	return 0;
}
