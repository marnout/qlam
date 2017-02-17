/*
project: Qlam
file:		qlam.cpp
compile
	g++ qlam.cpp -std=c++14 -Wfatal-errors -o qlam
version: 0.1.5
sam. juin  4 08:16:58 CEST 2016
*/

#include "qlam.h"

using namespace std;

// constructor ------------------------------------------------------------
Qlam::Qlam()
{
	name = "index";
	verbose = false;
}

Qlam::Qlam(const string& name, bool v=false) 
{
	this->name = name;
	verbose = v;
} // Qlam::Qlam


// destructor -------------------------------------------------------------
Qlam::~Qlam() 
{
	if(fqlm.is_open()) fqlm.close();
	if(fhtml.is_open()) fhtml.close();
} // Qlam::~Qlam

// article ----------------------------------------------------------------
void Qlam::article(string data)
{
	ifstream inc_file("qlm/" + data + ".qlm");
	smatch m;
	if(!inc_file.is_open()) {
		cout << MSG_OPEN << data << ".qlm" << " impossible\n";
		return;
	}
	while(getline(inc_file, line))
		if(regex_match(line, m, regex("^#title (.*)"))) break;
	if(regex_match(line, m,  regex("^#title (.*)"))) {
		fhtml << "<h4><a href=\"" << data << ".html\">";
		fhtml << m[1] << "</a></h4>\n"; 

	}
	while(getline(inc_file, line) && line.find("#substance")) continue;
	if(line.find("#substance") == 0) {
		while(getline(inc_file, line)) {
			if(line.empty()) break;
			fhtml << "<p class=\"substance\">" << styles(line) << "</p>\n";
		}
	}
	inc_file.close();
}
// code -------------------------------------------------------------------
void Qlam::code(string data) 
{
	if(data.empty())
		fhtml << "<pre>" << endl;
	else
		fhtml << "<pre class=\"" << data << "\">" << endl;
	while(getline(fqlm, line)) {
		if(line.empty()) break;
		//fhtml << "\t<p>" << line << "</p>" << endl;
		fhtml << raw(line) << endl;
	}
	fhtml << "</pre>" << endl;
} // code

// inchtml ----------------------------------------------------------------
void Qlam::inchtml(string data)
{
	ifstream inc_file(data + ".html");
	fhtml << "<!-- inchtml " << data << "-->\n";
	if(!inc_file.is_open()) {
		cout << MSG_OPEN << data << ".html" << " impossible\n";
		return;
	}	
	while(getline(inc_file, line))
		fhtml << line << endl;
	inc_file.close();
} //inchtml

// editorial ------------------------------------------------------------
void Qlam::editorial(string data)
{
	ifstream inc_file(data + ".html");
	if(!inc_file.is_open()) {
		cout << MSG_OPEN << data << ".html" << " impossible\n";
		return;
	}	
	while(getline(inc_file, line) && line != "<!--top-->") continue;
	while(getline(inc_file, line)) {
		if(line == "<!--end-->") break;
		fhtml << line << endl;
	}
	inc_file.close();
} // editorial

// head section of html file ----------------------------------------------
void Qlam::head()
{
	if(fqlm.is_open()) {
		if(verbose) cout << MSG_OPEN << "qlm/" << name << ".qlm\n";
		if(fhtml.is_open()) {
			if(verbose) cout << MSG_OPEN << name << ".html\n";
		} else {
			cout << "\n" << MSG_OPEN << name << ".html impossible\n";
			exit(EXIT_FAILURE);
		} // if html is open
	} else {
		cout << "\n" <<  MSG_OPEN << "qlm/" << name << ".qlm impossible\n";
		exit(EXIT_FAILURE);
	} // if qlm is open
		fhtml << "<!DOCTYPE html>\n<html>\n\n<head>\n";
	while(getline(fqlm, line)) {
		if(line.empty()) break;
				regex re; 	
		for (auto& x: map_head) {
			re = x.first;
			if(regex_match(line, re))
				fhtml << regex_replace(line, re, x.second) << endl;
		}; // for x
		if(line.at(0) == '<')
			fhtml << line << endl;
	} // while get not empty line
	fhtml << "</head>\n";
	
} // Qlam::head

// body -------------------------------------------------------------------
void Qlam::body() 
{
	fhtml << "\n<body>\n<a id=\"top\"></a>\n";
	string cmd;
	while(getline(fqlm, line)) {
		if(line.empty()) continue;
		if(line.at(0) == '#') {
			for(auto& c: re_cmd) {
				re = c;
				if(regex_match(line, m, re)) {
					if(m[1] == "article") article(m[2]);
					else if(m[1] == "code") code(m[2]);
					else if(m[1] == "inchtml") inchtml(m[2]);
					else if(m[1] == "editorial") editorial(m[2]); 
					else if(m[1] == "js") js(m[2]); 
					else if(m[1] == "menu") menu(m[2]);
					else if(m[1] == "substance") substance(m[2]);
					else if(m[1] == "table") table(m[2]);
					continue;
				}
			} // cmd
			for (auto& x: map_body) {
				re = x.first;
				if(regex_match(line, re)) {
					line = regex_replace(line, re, x.second);
					fhtml << line << endl;
					continue;
				}
			}; // for x
		} else
		if(regex_match(line, m, regex("^(\t*)([\\+-]) (.*)"))) 
			list(m[1].length(), string(m[2]).at(0), m[3]);
		else if(regex_match(line, m, regex("^\\? (.*)"))) 
			dl(m[1]);
		//else if(regex_match(line, m, regex("\t(.+)")))
		//	fhtml << "<p class=\"note\">" << styles(m[1]) << "</p>\n";
		else if(regex_match(line, regex("\t*<.*")))
			fhtml << line << endl;

		else
			fhtml << "<p>" << styles(line) << "</p>" << endl;
	} // while getline
	fhtml << "<!--end-->\n";
	fhtml << "<a id=\"end\"></a>\n";
	if(footer) {
		fhtml << "<hr>\n";
		fhtml << "Site réalisé avec <a href=\"http://arad.free.fr/qlam/\">";
		fhtml << "Qlam</a> - ";
		fhtml << "</body>\n\n</html>";
	}
} // body

// js ---------------------------------------------------------------------
void Qlam::js(string data)
{
	if(data.empty()) {
		fhtml << "<script type=\"text/javascript\">" << endl;
		while(getline(fqlm, line)) {
			if(line.empty()) break;
			//fhtml << "\t<p>" << line << "</p>" << endl;
			fhtml << raw(line) << endl;
		}
		fhtml << "</script>" << endl;

	} else {
		fhtml << "<script type=\"text/javascript\">" << endl;
		ifstream inc_file("js/" + data + ".js");
		fhtml << "<!-- sript " << data << "-->\n";
		if(!inc_file.is_open()) {
			cout << MSG_OPEN << data << ".js" << " impossible\n";
			return;
		}	
		while(getline(inc_file, line))
			fhtml << "\t" << line << endl;
		inc_file.close();
		fhtml << "</script>" << endl;
	}

} // js

// list -------------------------------------------------------------------
void Qlam::list(int ntabs, char key, string data)
{
	fhtml << string(ntabs,'\t') << "<" << map_list[key] << ">\n";
	while(getline(fqlm, line)) {
		fhtml << string(ntabs+1,'\t') << "<li>" << styles(data) << "</li>\n";
		regex_match(line, m, regex("^(\t*)([\\+-]) (.*)"));
			if(string(m[2]).empty()) break;
			int n = m[1].length();
			char k = string(m[2]).at(0);
			if(ntabs < n) list(n, k, m[3]);
			if(ntabs > n) break;
			data = m[3];
	}
	fhtml << string(ntabs,'\t') << "</" << map_list[key] << ">\n";
} // list

void Qlam::dl(string data)
{
	fhtml << "<dl>\n";
	fhtml << "\t<dt>" << styles(data) << "</dt>\n";
	while(getline(fqlm, line)) {
		if(line.empty()) break;
		regex_match(line, m, regex("^(\t|\\? )(.*)")) ;
		data = m[2];
		if(m[1] == "\t")
			fhtml << "\t\t<dd>" << styles(data) << "</dd>\n";
		else if(m[1] == "? ")
			fhtml << "\t<dt>" << styles(data) << "</dt>\n";
		else break;
	} // getline
	fhtml << "</dl>\n";
} // dl

// table ------------------------------------------------------------------
void Qlam::table(string data)
{
	if(data.empty()) fhtml << "<table>\n";
	else fhtml << "<table class=\"" << data << "\">\n";
	getline(fqlm, line);
	line = styles(line);
	fhtml << "\t<tr>\n";
	fhtml << "\t<th>" << regex_replace(line, regex("\\|"), "</th><th>");
	fhtml << "</th>\n\t</tr>\n";
	while(getline(fqlm, line)) {
		if(line.empty()) break;
		line = styles(line);
		fhtml << "\t<tr>\n";
		fhtml << "\t<td>" << regex_replace(line, regex("\\|"), "</td><td>");
		fhtml << "</td>\n\t</tr>\n";
	}
	fhtml << "</table>\n";
} // table

// menu -------------------------------------------------------------------
void Qlam::menu(string data)
{
	fhtml << "<!-- Menu " << data << "-->\n";
	ifstream inc_file("qlm/" + data + ".qlm");
	if(inc_file.is_open()) {
		fhtml << "<ul id=\"menu\">\n";
		while(getline(inc_file, line)) {
			fhtml <<  "\t<li>\n\t" << styles(line) << "\n\t</li>\n";
		}
	fhtml << "</ul>\n";
	fhtml << "<!--top-->\n";
	inc_file.close();
	} else if(verbose)
		cout << MSG_OPEN << "qlm/" + data << ".qlm" << "impossible\n";

} // menu
// menu -------------------------------------------------------------------
void Qlam::substance(string cl="substance")
{
	if(cl.empty()) cl = "substance";
	fhtml << "<!-- Résumé -->\n";
	fhtml << "<div class=\"" << cl << "\">\n";
	do {
		getline(fqlm, line);
		if(line.empty()) break;
		fhtml << "\t<p>" << styles(line) << "</p>\n";
			
	} while(true);
	fhtml << "</div>\n";
} // substance

// qlm2html ---------------------------------------------------------------
void Qlam::to_html()
{
	fqlm.open("qlm/" + name + ".qlm");
	fhtml.open(name + ".html");
	cout << name << ".qlm --> " ;
	head();
	body();
	cout << name << ".html OK\n";
	if(fqlm.is_open()) fqlm.close();
	if(fhtml.is_open()) fhtml.close();

} // qlm2html

// main =====================================================================

string filename(string s)
{
	size_t p;
	p = s.find_last_of("/");
	if(p != string::npos) s = s.substr(p + 1);
	p = s.find_first_of(".");
	if(p != string::npos) s = s.substr(0, p);
	while(s.back() == ' ') s.pop_back();
	return s;
}

string filename_back(string s)
{
	size_t p;
	p = s.find_last_of(".");
	if(p != string::npos)
		return s.substr(p);
	else
		return "";
}

void site()
{
	DIR *dp;
	struct dirent *ep;
	char wd[] = "qlm/";
	string fname;
	size_t pos;
	Qlam qlm;
	if((dp = opendir(wd)) == NULL) exit(EXIT_FAILURE);
	while((ep = readdir(dp)) != NULL) {
		fname = string(ep->d_name);
		pos = fname.find_last_of(".");
		if(fname.substr(pos) == ".qlm") {
			qlm.name = fname.substr(0, pos);
			if(qlm.name != "menu" && qlm.name != "entete")
				qlm.to_html();
		}
	}
}

void help(string prog)
{
	cout << "Usage " << prog << "[options] fichier ...\n";
	cout << "Options : \n";
	cout << "\t-a convertit tous les fichiers qlm du site\n";
	cout << "\t-h cette aide\n";
	cout << "\t-v verbose\n";
}


int main(int argc, char * argv[]) 
{
	HEADER
	if (argc < 2) {
		cout << "Usage: " << argv[0] << "[-a] [-v] [FILE ...]";
		exit(EXIT_FAILURE);
	} // if no args

	char opt; // for getopt
	int index; // for non options arguments
	Qlam qlm;
	qlm.footer = true;
	// options
	while ((opt = getopt(argc, argv, "ahlv")) != -1) {
		switch (opt) {
			case 'a':
				site();
				break;
			case 'h':
				help(argv[0]);
				break;
			case 'l':
				qlm.footer = false;
				break;
			case 'v':
				qlm.verbose = true;
				break;
			default: /* '?' */
				cout << "Usage: " << argv[0];
				cout << "[-a] [-h] [-v] [FILE ...]\n";
				exit(EXIT_FAILURE);
		} // switch
	} // while getopt
	// non option arguments
	for(index = optind; index < argc; index++) {
		qlm.name = filename(argv[optind]);
		qlm.to_html();
	}

	return 0;
} // main

