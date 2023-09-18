#pragma once
#include "util.h"
#include "NFA.h"
#include "Node.h"

// 确定有限自动机
class DFA
{
private:
	void createDFA(NFA &nfa);
	static char state;
	static char getState();
public:
	DFA(NFA &nfa, set<string> &input);
};

