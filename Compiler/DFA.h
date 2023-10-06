#pragma once
#include "util.h"
#include "NFA.h"
#include "Node.h"

class DTran {
private:
	vector<string> input;
	map<DNode, map<string, DNode> > table;
	map<int, int> stateMapper; // 存储简化前后各状态的映射情况，其中简化前各状态的映射等于自己（并查集）
	void merge(int a, int b);
	int father(int x); // 维护stateMapper
public:
	void setInput(set<string> &in);
	DNode transition(DNode &now, string inputChar);
	void addTransition(DNode &from, string inputChar, DNode &to);
	bool testEqual(map<string, DNode> &t1, map<string, DNode> &t2); // 判断转换到的状态是否相同
	void simplify(); // 最简化
	void output();

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
	static int state;
	static int getState();
	DNode _start;
public:
	DTran transTable;

	DFA();
	DFA(NFA &nfa, set<string> &input);
	void createDFA(NFA& nfa, set<string>& input);
	void outputDFA(); // 输出转换表
	void simplify();
	DNode move(DNode &now, string input);
	DNode getStart();
};
