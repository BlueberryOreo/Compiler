#pragma once
#include "util.h"

char getch(ifstream& ifs);
void testId(string& id);
void outputKw(string& kw);
void outputNum(string& num);
void outputSingleWord(char tmp);
void outputDoubleWord(string& dw);
void output(ofstream &ofs);
ifstream& operator>>(ifstream& fin, Token& t);
ostream& operator<<(ostream& out, Token& t);
ostream& operator<<(ostream& out, vector<string> &v);
ostream& operator<<(ostream& out, set<string>& s);
void output();

void error(int type, ifstream* ifs, int shift=0, string info="", int inputLine=-1);
