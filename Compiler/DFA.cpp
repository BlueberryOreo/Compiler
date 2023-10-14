#include "DFA.h"

int DFA::state = 1;

void DFA::createDFA(NFA& nfa, set<string> &input)
{
	transTable.setInput(input);
	DStates ds;
	DNode start(getState());
	start.append(nfa.start);
	start.closure();
	//cout << "epsilon-closure(s0).size()=" << start.size() << endl;
	ds.addUnsigned(start);
	this->_start = start;
	while (ds.haveUnsigned()) {
		DNode now = ds.getUnsigned();
		ds.addSigned(now);
		//cout << "now.size() = " << now.size() << " now.state = " << now.state << " " << now.getHash() << endl;
		for (s_Siterator it = input.begin(); it != input.end(); it ++) {
			DNode next(getState());
			now.move(*it, next);
			next.closure();
			//cout << "next size = " << next.size() << " next.hash() = " << next.getHash() << endl;
			DNode res = ds.get(next);
			//cout << "res size = " << res.size() << endl;
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

DFA::DFA()
{ // 无参构造函数
}

DFA::DFA(NFA& nfa, set<string> &input)
{
	//cout << "here DFA:26" << endl;
	createDFA(nfa, input);
}

void DFA::outputDFA()
{
	transTable.output();
}

void DFA::simplify()
{
	transTable.simplify();
}

bool DFA::isDead(DNode& node)
{
	return transTable.isDead(node);
}

int DFA::size()
{
	return transTable.size();
}

DNode DFA::move(DNode& now, string input)
{
	return this->transTable.transition(now, input);
}

DNode DFA::getStart()
{
	return this->_start;
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
	//cout << "node.size() = " << node.size() << " signed.size() = " << signedDNodes.size() << " unsigned.size() = " << unsignedDNodes.size() << endl;
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

void DTran::merge(int a, int b)
{
	stateMapper[a] = father(b);
}

int DTran::father(int x)
{
	if (stateMapper[x] == x) return x;
	int tmpF = father(stateMapper[x]);
	stateMapper[x] = tmpF;
	return tmpF;
}

void DTran::setInput(set<string>& in)
{
	for (s_Siterator it = in.begin(); it != in.end(); it ++) {
		input.push_back(*it);
	}
}

DNode DTran::transition(DNode& now, string inputChar)
{
	iterrow row = table.find(states[father(now.state)]);
	string asterisk;
	if (('a' <= inputChar[0] && inputChar[0] <= 'z') || ('A' <= inputChar[0] && inputChar[0] <= 'Z')) asterisk = "\\w";
	if ('0' <= inputChar[0] && inputChar[0] <= '9') asterisk = "\\d";
	if (row->second.find(inputChar) != row->second.end()) {
		return states[father(table.find(now)->second.find(inputChar)->second.state)];
	}
	else if (row->second.find(asterisk) != row->second.end()) {
		return states[father(table.find(now)->second.find(asterisk)->second.state)];
	}
	else return DNode();
	
	//return NULL;
}

void DTran::addTransition(DNode& from, string inputChar, DNode& to)
{
	//to.indegree++;
	table[from][inputChar] = to;
	stateMapper[from.state] = from.state;
	stateMapper[to.state] = to.state;
	states[from.state] = from;
	states[to.state] = to;

}

bool DTran::testEqual(map<string, DNode>& t1, map<string, DNode>& t2)
{
	for (itercol it = input.begin(); it != input.end(); it ++) {
		if (father(t1[*it].state) != father(t2[*it].state)) {
			return false;
		}
	}
	return true;
}

bool DTran::isDead(DNode& node)
{
	iterrow it = table.find(node);
	for (int i = 0; i < input.size(); i++) {
		if (it->second[input[i]].state != node.state) return false;
	}
	return true;
}

void DTran::simplify()
{
	for (int i = 0; i < table.size(); i++) {
		cout << i << endl;
		bool flag = true;
		for (iterrow it = rowBegin(); it != rowEnd(); it ++) {
			//if (stateMapper[it->first.state] != it->first.state) continue; // 已经合并过
			int baseState = it->first.state;
			for (iterrow innerIt = rowBegin(); innerIt != rowEnd(); innerIt ++) {
				if (innerIt->first.state == baseState) continue;
				if (testEqual(it->second, innerIt->second) && (it->first.isEnd == innerIt->first.isEnd)) {
					// 二者的转换相同
					flag = false;
					merge(innerIt->first.state, baseState);
				}
			}
		}
		if (flag) break;
	}
}

void DTran::output()
{
	// 表头
	cout << string(27, '=') << endl;
	cout << setw(5) << "state";
	cout << setw(10) << "receive";
	cout << setw(12) << "move\n" << setw(15) << "";
	for (itercol it = inputBegin(); it != inputEnd(); it++) {
		cout << setw(6) << *it;
	}
	cout << endl;
	cout << string(27, '-') << endl;

	for (iterrow row = rowBegin(); row != rowEnd(); row++) {
		int fa = father(row->first.state);
		if (fa != row->first.state) continue;
		cout << setw(5) << row->first.state
			<< setw(10) << (row->first.isEnd ? "yes" : "no");
		for (itercol col = inputBegin(); col != inputEnd(); col++) {
			cout << setw(6) << father(row->second[*col].state);
		}

		cout << endl;
	}
	cout << string(27, '=') << endl;
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

int DTran::size()
{
	return table.size();
}
