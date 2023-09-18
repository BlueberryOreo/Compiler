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

DState::DState(char state)
{
	this->state = state;
}

bool DState::have(ANode* node)
{
	return NStates.find(node) != NStates.end();
}

void DState::append(ANode* node)
{
	this->NStates.insert(node);
}

void DState::closure()
{
	stack<ANode*> stk;
	for (set<ANode*>::iterator it = this->NStates.begin(); it != this->NStates.end(); it ++) {
		stk.push(*it);
	}
	while (!stk.empty()) {
		ANode* now = stk.top();
		stk.pop();
		for (int i = 0; i < now->edges.size(); i++) {
			if (now->edges[i].data == E) {
				ANode* to = now->edges[i].to;
				if (!this->have(to)) {
					this->append(to);
					stk.push(to);
				}
			}
		}
	}
}

bool DScmp::operator()(const ANode* node1, const ANode* node2) const
{
	if (node1->id == node2->id) return false;
	return node1->id < node2->id;
}
