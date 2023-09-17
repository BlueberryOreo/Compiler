#pragma once
#include "util.h"
#include "Node.h"

class NFA
{
public:
	static int id;
	static int getId();

	static NFA meta(RNode *node);
	static NFA Concatenation(NFA &nfa1, NFA &nfa2);
	static NFA Union(NFA &nfa1, NFA &nfa2);
	static NFA Kleene(NFA &nfa);
public:
	ANode* start;
	ANode* end;
	static NFA MSE(RNode *now);
	void outputNFA();
};

