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
	map<string, string> regDef;
	RNode* root;
	NFA nfa;

	string singleword = "+-(){};,:|";
	string doubleword = "><=!/*";
	bool testWord(char c);
	vector<string> regLexer(string &s); // 正则表达式/定义式分词器

	void insertRegDef(string def);
	void deleteTree(RNode *now);
	string join(vector<string> &strs);
	RNode* searchNode(string s, RNode* now);

	void outputTree(RNode *now);
	
public:
	RegExp(string *defs = NULL, int size = 0);
	~RegExp();
	void initRegDef(string *defs = NULL, int size = 0);
	void createParseTree(string &regExp); // 构造语法分析树（移入-归约法）

	void readTree(); // 读取一棵树，按照父-子一行一行读入
};

