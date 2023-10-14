#pragma once
#include "util.h"

char getch(ifstream& ifs);
void testId(string& id);
void outputKw(string& kw);
void outputNum(string& num);
void outputSingleWord(char tmp);
void outputDoubleWord(string& dw);
void output(ofstream &ofs);
void output();

void error(int type, ifstream* ifs, int shift=0, string info="");
