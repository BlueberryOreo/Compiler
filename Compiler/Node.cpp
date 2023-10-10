#include "Node.h"

RNode::RNode(string s, RNode* father/*, initializer_list<RNode*> args*/): Node(s)
{
	this->father = father;
	//for (RNode* item : args) {
	//	this->children.push_back(item);
	//}
}

//RNode::RNode(string s): Node(s)
//{ // 无参构造函数
//}

NEdge::NEdge(string data, NNode* toA): Edge(data) {
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

NNode::NNode(int id)
{
	this->id = id;
	this->isEnd = false;
}

void NNode::append(string edgeData, NNode* node)
{
	NEdge edge(edgeData, node);
	edges.push_back(edge);
}

int DNode::getHash() const
{
	int value = 0;
	for (s_Niterator it = this->NStates.begin(); it != this->NStates.end(); it++) {
		value += (*it)->id;
	}
	return hash<int>()(value);
}

DNode::DNode()
{
	this->state = 0;
	this->_empty = true;
	this->isEnd = false;
	//this->indegree = 0;
}

DNode::DNode(int state)
{
	this->state = state;
	this->_empty = false;
	this->isEnd = false;
	//this->indegree = 0;
}

bool DNode::have(NNode* node)
{
	return NStates.find(node) != NStates.end();
}

void DNode::append(NNode* node)
{
	this->NStates.insert(node);
}

void DNode::closure()
{
	stack<NNode*> stk;
	for (set<NNode*>::iterator it = this->NStates.begin(); it != this->NStates.end(); it ++) {
		stk.push(*it);
	}
	while (!stk.empty()) {
		NNode* now = stk.top();
		stk.pop();
		this->isEnd |= now->isEnd;
		for (int i = 0; i < now->edges.size(); i++) {
			if (now->edges[i].data == E) {
				NNode* to = now->edges[i].to;
				if (!this->have(to)) {
					this->append(to);
					stk.push(to);
				}
			}
		}
	}
}

void DNode::move(string inputS, DNode& nextState)
{
	for (s_Niterator it = this->NStates.begin(); it != this->NStates.end(); it ++) {
		for (int i = 0; i < (*it)->edges.size(); i ++) {
			NEdge tmp = (*it)->edges[i];
			if (tmp.data == inputS) nextState.append(tmp.to);
		}
	}
}

//bool DNode::operator==(const DNode& other) const
//{
//	//for (s_Niterator it = this->NStates.begin(); it != this->NStates.end(); it ++) {
//	//	if (other.NStates.find(*it) == other.NStates.end()) {
//	//		return false;
//	//	}
//	//}
//	return getHash() == other.getHash();
//}

bool DNode::operator<(const DNode& other) const
{
	return this->state < other.state;
}

int DNode::size()
{
	return this->NStates.size();
}

bool DNode::empty()
{
	return this->_empty;
}

bool DNcmp::operator()(const NNode* node1, const NNode* node2) const
{
	if (node1->id == node2->id) return false;
	return node1->id < node2->id;
}

size_t DNode::Hash::operator()(const DNode& node) const
{
	return node.getHash();
}

bool DNode::Equal::operator()(const DNode& n1, const DNode& n2) const
{
	return n1.getHash() == n2.getHash();
}
