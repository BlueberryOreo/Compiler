#pragma once
#include "util.h"
#include "NFA.h"
#include "Node.h"

class DTran {
	
	DNode transition(DNode &now, string inputChar);
	void addTransition(DNode &from, string inputChar, DNode &to);
};

class DNhash {
public:
	size_t operator()(const DNode& node) const;
};

class DStates {
private:
	unordered_set<DNode, DNhash> signedDNodes;
	unordered_set<DNode, DNhash> unsignedDNodes;
public:
	bool haveUnsigned();
	DNode getUnsigned();
	void addUnsigned(DNode node);
	void addSigned(DNode node);
};

// 确定有限自动机
class DFA
{
private:
	DStates ds;
	void createDFA(NFA &nfa);
	static int state;
	static int getState();
public:
	DFA(NFA &nfa, set<string> &input);
};

