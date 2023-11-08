#pragma once
#include "util.h"
#include "io.h"
#include "Node.h"

class LLNRec
{
private:
	ifstream lexer;
	string begin;
	map<string, vector<string> > gramma;
	map<string, set<string> > first;
	map<string, set<string> > follow;
	typedef map<string, set<string> >::iterator it_mss;
	typedef map<string, vector<string> >::iterator it_msv;

	void initGramma();

	set<string> findFirst(string now);

	void initFirst();
	void initFollow();

public:
	LLNRec(string& lexerOut);
};
