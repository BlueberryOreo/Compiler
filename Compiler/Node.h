#pragma once
#include <string>
#include <vector>

#define E "epsilon"
using namespace std;

class ANode;

class Edge {
public:
	string data;
	Edge(string s="");
};

class Node {
public:
	string data;
	Node(string s="");
};

class RNode : public Node {
public:
	RNode* father = NULL;
	vector<RNode*> children;
	RNode(string s, initializer_list<RNode*> args);
	RNode(string s);
};

class AEdge : public Edge {
public:
	ANode* to;
	AEdge(string data="", ANode* toA=NULL);
};

class ANode : public Node {
public:
	int id;
	vector<AEdge> edges;
	ANode(int id);
	void append(string edgeData, ANode *node);
};