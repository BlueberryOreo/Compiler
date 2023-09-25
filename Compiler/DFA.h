#pragma once
#include "util.h"
#include "NFA.h"
#include "Node.h"

class DTran {
private:
	vector<string> input;
	map<DNode, map<string, DNode> > table;
public:
	void setInput(set<string> &in);
	DNode transition(DNode &now, string inputChar);
	void addTransition(DNode &from, string inputChar, DNode &to);

	typedef map<DNode, map<string, DNode> >::iterator iterrow;
	typedef vector<string>::iterator itercol;
	map<DNode, map<string, DNode> >::iterator rowBegin();
	map<DNode, map<string, DNode> >::iterator rowEnd();
	vector<string>::iterator inputBegin();
	vector<string>::iterator inputEnd();
};

// 存储标记和未标记的有限自动机状态
class DStates {
private:
	unordered_set<DNode, DNode::Hash, DNode::Equal> signedDNodes;
	unordered_set<DNode, DNode::Hash, DNode::Equal> unsignedDNodes;
public:
	typedef unordered_set<DNode>::iterator iterator;
	bool haveUnsigned();
	DNode getUnsigned();
	void addUnsigned(DNode node);
	void addSigned(DNode node);
	DNode get(DNode &node);
	//void outputUnsigned();
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
	void outputDFA(); // 输出转换表
	void simplify();
};
