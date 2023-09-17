#include "Node.h"

RNode::RNode(string s, initializer_list<RNode*> args): Node(s)
{
	for (RNode* item : args) {
		this->children.push_back(item);
	}
}

RNode::RNode(string s): Node(s)
{
}

AEdge::AEdge(string data, ANode* toA): Edge(data) {
	this->to = toA;
}

Node::Node(string s)
{
	this->data = s;
}

Edge::Edge(string s)
{
	this->data = s;
}

ANode::ANode(int id)
{
	this->id = id;
}

void ANode::append(string edgeData, ANode *node)
{
	AEdge edge(edgeData, node);
	edges.push_back(edge);
}
