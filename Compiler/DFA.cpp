#include "DFA.h"

size_t DNhash::operator()(const DNode& node) const
{
	return hash<int>()(node.getHash());
}


int DFA::state = 0;

void DFA::createDFA(NFA& nfa)
{
	
}

int DFA::getState()
{
	return state++;
}

DFA::DFA(NFA& nfa, set<string> &input)
{
	createDFA(nfa);
}

bool DStates::haveUnsigned()
{
	return unsignedDNodes.size() > 0;
}

DNode DStates::getUnsigned()
{
	DNode ret = *(this->unsignedDNodes.begin());
	this->unsignedDNodes.erase(this->unsignedDNodes.begin());
	return ret;
}

void DStates::addUnsigned(DNode node)
{
	this->unsignedDNodes.insert(node);
}

void DStates::addSigned(DNode node)
{
	this->signedDNodes.insert(node);
}
