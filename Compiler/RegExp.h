#pragma once
#include "util.h"
#include "Node.h"
#include "NFA.h"
#define m_iterator map<string, string>::iterator
#define s_iterator set<RNode*>::iterator
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

	void readTree(); // ��ȡһ���������ո�-��һ��һ�ж���
};
