#pragma once
#include "util.h"
#include "NFA.h"
#include "Node.h"

// ȷ�������Զ���
class DFA
{
private:
	void createDFA(NFA &nfa);
	static char state;
	static char getState();
public:
	DFA(NFA &nfa, set<string> &input);
};
