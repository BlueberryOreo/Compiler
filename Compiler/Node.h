#pragma once
#include <string>
#include <vector>
#include <set>
#include <stack>

#define E "epsilon"
using namespace std;

class ANode;

// 边类
class Edge {
public:
	string data;
	Edge(string s="");
};

// 节点类
class Node {
public:
	string data;
	Node(string s="");
};

// 正则表达式语法分析树节点
class RNode : public Node {
public:
	RNode* father = NULL;
	vector<RNode*> children;
	RNode(string s, initializer_list<RNode*> args);
	RNode(string s);
};

// 自动机边
class AEdge : public Edge {
public:
	ANode* to;
	AEdge(string data="", ANode* toA=NULL);
};

// 自动机节点
class ANode : public Node {
public:
	int id;
	vector<AEdge> edges;
	ANode(int id);
	void append(string edgeData, ANode *node);
};

struct DScmp {
	bool operator() (const ANode *node1, const ANode *node2) const;
};

// 确定自动机状态节点
class DState : public Node {
public:
	char state;
	DState(char state);
	set<ANode*, DScmp> NStates; // 按ANode的id去重
	bool have(ANode *node); // 查找该DFA状态内是否有NFA状态node
	void append(ANode *node); // 在该DFA状态内加入NFA状态node
	void closure(); // 计算闭包
};