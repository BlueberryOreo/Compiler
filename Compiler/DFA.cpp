#include "DFA.h"

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
		ds.addSigned(now);
		//cout << now.state << " " << now.getHash() << endl;
		for (s_Siterator it = input.begin(); it != input.end(); it ++) {
			DNode next(getState());
			now.move(*it, next);
			next.closure();
			DNode res = ds.get(next);
			if (res.empty()) {
				//cout << "doesn't find DNode " << next.state << " hash=" << next.getHash() << ": ";
				//for (s_Niterator it = next.NStates.begin(); it != next.NStates.end(); it ++) {
				//	cout << (*it)->id << " ";
				//}
				//cout << endl;
				ds.addUnsigned(next);
				res = next;
			}
			transTable.addTransition(now, *it, res);
		}
		//ds.outputUnsigned();
	}
	cout << "DFA construct end" << endl;
}

int DFA::getState()
{
	return state++;
}

DFA::DFA(NFA& nfa, set<string> &input)
{
	//cout << "here DFA:26" << endl;
	transTable.setInput(input);
	createDFA(nfa, input);
}

void DFA::outputDFA()
{
	// ±íÍ·
	cout << '\t';
	for (DTran::itercol it = transTable.inputBegin(); it != transTable.inputEnd(); it ++) {
		cout << *it << '\t';
	}
	cout << endl;

	for (DTran::iterrow row = transTable.rowBegin(); row != transTable.rowEnd(); row ++) {
		cout << row->first.state << '\t';
		for (DTran::itercol col = transTable.inputBegin(); col != transTable.inputEnd(); col ++) {
			cout << row->second[*col].state << '\t';
		}
		cout << endl;
	}
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

DNode DStates::get(DNode& node)
{
	iterator signedGet = signedDNodes.find(node);
	iterator unsignedGet = unsignedDNodes.find(node);
	if (signedGet != signedDNodes.end()) {
		return *signedGet;
	}
	else if (unsignedGet != unsignedDNodes.end()) {
		return *unsignedGet;
	}
	return DNode();
}

//void DStates::outputUnsigned()
//{
//	for (unordered_set<DNode>::iterator it = unsignedDNodes.begin(); it != unsignedDNodes.end(); it ++) {
//		cout << (*it).getHash() << " ";
//	}
//	cout << endl;
//}

void DTran::setInput(set<string>& in)
{
	for (s_Siterator it = in.begin(); it != in.end(); it ++) {
		input.push_back(*it);
	}
}

DNode DTran::transition(DNode& now, string inputChar)
{
	return table.find(now)->second.find(inputChar)->second;
	//return NULL;
}

void DTran::addTransition(DNode& from, string inputChar, DNode& to)
{
	table[from][inputChar] = to;
}

map<DNode, map<string, DNode>>::iterator DTran::rowBegin()
{
	return table.begin();
}

map<DNode, map<string, DNode>>::iterator DTran::rowEnd()
{
	return table.end();
}

vector<string>::iterator DTran::inputBegin()
{
	return input.begin();
}

vector<string>::iterator DTran::inputEnd()
{
	return input.end();
}
