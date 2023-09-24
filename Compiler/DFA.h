#pragma once
#include "util.h"
#include "NFA.h"
#include "Node.h"

class DTran {
private:
	map<string, int> input;
	map<DNode, map<string, DNode> > table;
public:
	void setInput(set<string> &in);
	DNode transition(DNode &now, string inputChar);
	void addTransition(DNode &from, string inputChar, DNode &to);
};

class DNhash {
public:
	size_t operator()(const DNode& node) const;
};

// 存储标记和未标记的有限自动机状态
class DStates {
private:
	unordered_set<DNode, DNhash> signedDNodes;
	unordered_set<DNode, DNhash> unsignedDNodes;
public:
	bool haveUnsigned();
	DNode getUnsigned();
	void addUnsigned(DNode node);
	void addSigned(DNode node);
	bool have(DNode &node);
};

// 确定有限自动机
class DFA
{
private:
	DTran transTable;
	void createDFA(NFA &nfa, set<string> &input);
	static int state;
	static int getState();
public:
	DFA(NFA &nfa, set<string> &input);
};
