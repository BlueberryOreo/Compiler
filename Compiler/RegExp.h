#pragma once
#include "util.h"
#include "Node.h"
#include "NFA.h"

using namespace std;

struct cmp {
	bool operator()(const RNode *a, const RNode *b) const;
};

class RegExp
{
private:
	RNode* root;
	NFA nfa;

	void deleteTree(RNode *now);
	RNode* searchNode(string s, RNode* now);

	void outputTree(RNode *now);
	
public:
	RegExp(string *defs = NULL, int size = 0);
	~RegExp();

	void readTree(); // 读取一棵树，按照父-子一行一行读入
};

