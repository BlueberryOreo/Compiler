#pragma once
#include "util.h"
#include "Node.h"
#include "NFA.h"
#include "DFA.h"

using namespace std;

struct cmp {
	bool operator()(const RNode *a, const RNode *b) const;
};

class RegExp
{
private:
	string name;
	RNode* root;
	NFA nfa;
	DFA dfa;

	DNode pointer;

	void deleteTree(RNode *now);
	RNode* searchNode(string s, RNode* now);
	void constructTree(const string &reg); // 已弃用

	void outputTree(RNode *now);
	
public:
	RegExp(const string &name = ""); // 创建一个正则表达式
	~RegExp();

	void init();
	void readTree(); // 读取一棵树，按照父-子一行一行读入
	int readFromFile(ifstream &ifs); // 从文件中读取一棵树
	bool match(const string &pattern);
	bool matchNext(char c);
	string getName();
	void setName(const string &name);
};

