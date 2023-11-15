#pragma once
#include "util.h"
#include "io.h"
#include "Node.h"

class LLNRec
{
private:
	ifstream lexer;
	string begin;
	set<string> inputSign;
	stack<string> stk;
	map<string, vector<string> > grammar;
	map<string, set<string> > first;
	map<string, set<string> > follow;
	map<string, map<string, pair<string, vector<string> > > > predictTable; // predictTable[���ս��][�������] = ����ʽ(pair[��ߣ�vector[�ұ�]])

	int nodeCnt = 0;
	map<string, string> nodes;
	stack<string> nodeStk;

	typedef map<string, set<string> >::iterator it_mss;
	typedef map<string, vector<string> >::iterator it_msv;

	void initGrammar();
	vector<string> getRightItems(string &right);

	set<string> findFirst(string now);

	void initFirst();
	void initFollow();
	void buildTable();

	void initStk();
	void outputStk();
	int move(Token& t);

public:
	LLNRec();
	void showTable();
	void analyze(string& lexerOut);
	void analyze(vector<Token> &tokens);
};
