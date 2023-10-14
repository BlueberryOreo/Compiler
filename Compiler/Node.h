#pragma once
#include <string>
#include <vector>
#include <set>
#include <stack>

#ifndef m_iterator
#define m_iterator map<string, string>::iterator
#endif // !m_iterator

#ifndef s_Riterator
#define s_Riterator set<RNode*>::iterator
#endif // !s_iterator

#ifndef s_Niterator
#define s_Niterator set<NNode*>::iterator
#endif // !s_Aiterator


#ifndef s_Siterator
#define s_Siterator set<string>::iterator
#endif // !s_Siterator

#ifndef E
#define E "."
#endif // !E

using namespace std;

class NNode;

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
	RNode* father;
	vector<RNode*> children;
	RNode(string s, RNode* father=NULL/*, initializer_list<RNode*> args*/);
	//RNode(string s);
};

// 不确定自动机边
class NEdge : public Edge {
public:
	NNode* to;
	NEdge(string data="", NNode* toA=NULL);
};

// 不确定自动机节点
class NNode : public Node {
public:
	int id;
	vector<NEdge> edges;
	bool isEnd; // 是否是接受状态
	NNode(int id);
	void append(string edgeData, NNode*node);
};

struct DNcmp {
	bool operator() (const NNode*node1, const NNode*node2) const;
};

// 确定自动机状态节点
class DNode : public Node {
public:
	set<NNode*, DNcmp> NStates; // 按NNode的id去重
	int getHash() const;
	bool _empty; // 表示这个类是否是一个空的无意义的节点

public:
	bool isEnd; // 是否是接受状态
	//int indegree; // 入度

	int state;
	DNode();
	DNode(int state);
	bool have(NNode* node); // 查找该DFA状态内是否有NFA状态node
	void append(NNode* node); // 在该DFA状态内加入NFA状态node
	void closure(); // 计算当前DFA状态的epsilon闭包
	void move(string inputS, DNode& nextState); // 通过当前的状态以及输入符号找到下一个状态
	//bool operator==(const DNode& other) const; // 判断两个DFA状态是否相同（根据其包含的NFA状态）
	bool operator<(const DNode& other) const;
	int size(); // 一个DFA中包含的NFA节点个数
	//void setEmpty(bool e);
	//bool isDead();
	bool empty();

	struct Hash {
		size_t operator()(const DNode& node) const;
	};
	struct Equal {
		bool operator()(const DNode &n1, const DNode &n2) const;
	};
};