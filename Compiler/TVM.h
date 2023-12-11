#pragma once
#include "util.h"
#define MAX 1000

struct Code {
	vector<string> code;
	bool isLabel;

	Code(string c);

	string& operator[](int idx);
	void out();
};

class TVM
{
private:
	int PC;
	vector<Code> code;
	int data[MAX];
	int stk[MAX];
	int stktop;

	int push(int d);
	int get();
	int pop();

	int stoi(string &s);

	unordered_map<string, int> labels;

public:
	TVM();
	void reset();
	int analyze(Code &c);
	int run(const string &asmPath);
};

