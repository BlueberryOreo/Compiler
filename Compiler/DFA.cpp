#include "DFA.h"

size_t DNhash::operator()(const DNode& node) const
{
	return hash<int>()(node.getHash());
}


int DFA::state = 0;

void DFA::createDFA(NFA& nfa, set<string> &input)
{
	DStates ds;
	DNode start(getState());
	start.append(nfa.start);
	start.closure();
	cout << "epsilon-closure(s0).size()=" << start.size() << endl;
	ds.addUnsigned(start);
	while (ds.haveUnsigned()) {
		DNode now = ds.getUnsigned();
		for (s_Siterator it = input.begin(); it != input.end(); it ++) {
			DNode next(getState());
			now.move(*it, next);
			next.closure();
			if (!ds.have(next)) {
				ds.addUnsigned(next);
			}
			transTable.addTransition(now, *it, next);
		}
	}
}

int DFA::getState()
{
	return state++;
}

DFA::DFA(NFA& nfa, set<string> &input)
{
	cout << "here DFA:26" << endl;
	transTable.setInput(input);
	createDFA(nfa, input);
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

bool DStates::have(DNode& node)
{
	return (signedDNodes.find(node) != signedDNodes.end()) && (unsignedDNodes.find(node) != unsignedDNodes.end());
}

void DTran::setInput(set<string>& in)
{
	int cnt = 0;
	for (s_Siterator it = in.begin(); it != in.end(); it ++) {
		this->input[*it] = cnt++;
	}
}

void DTran::addTransition(DNode& from, string inputChar, DNode& to)
{

}
