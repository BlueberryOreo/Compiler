#include "DFA.h"

char DFA::state = 'A';

void DFA::createDFA(NFA& nfa)
{
	
}

char DFA::getState()
{
	return state++;
}

DFA::DFA(NFA& nfa)
{
	createDFA(nfa);
}
