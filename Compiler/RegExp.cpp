#include "RegExp.h"

void RegExp::deleteTree(RNode* now)
{
	if (now->children.size() == 0) {
		// leaf node
		delete now;
		return;
	}
	for (int i = 0; i < now->children.size(); i ++) {
		deleteTree(now->children[i]);
	}
	delete now;
}

RNode* RegExp::searchNode(string s, RNode *now)
{
	if (s == now->data) return now;
	RNode* ret = nullptr;
	for (int i = 0; i < now->children.size(); i ++) {
		RNode* child = now->children[i];
		if (!ret) ret = searchNode(s, child);
		else break;
	}
	return ret;
}

void RegExp::outputTree(RNode* now)
{
	if (now == NULL) return;
	cout << now->data << " ";
	//cout << now->data << " " << now->children.size() << endl;
	for (int i = 0; i < now->children.size(); i ++) {
		RNode* child = now->children[i];
		outputTree(child);
	}
}

RegExp::RegExp(string *defs, int size)
{
	this->root = NULL; 
	//this->initRegDef(defs, size); 
	this->readTree();
	//this->outputTree(this->root);
	this->nfa = NFA::MSE(this->root);
	this->nfa.outputNFA();
}

RegExp::~RegExp()
{
	if(root) deleteTree(root);
	NFA::delNFA(this->nfa.start);
}

void RegExp::readTree()
{
	cout << "请输入一棵树，按照父-子节点，一行一行输入，父节点和子节点之间用空格分开" << endl;
	set<RNode*, cmp> nodes;
	string f, c;
	while (cin >> f >> c) {
		RNode* father = new RNode(f);
		RNode* child = new RNode(c);
		s_iterator tmp = nodes.find(father);
		if (tmp != nodes.end()) {
			delete father;
			father = *tmp;
		}
		else {
			nodes.insert(father);
		}
		if (nodes.find(child) == nodes.end()) {
			nodes.insert(child);
		}
		father->children.push_back(child);
		child->father = father;
	}
	for (s_iterator it = nodes.begin(); it != nodes.end(); it ++) {
		if ((*it)->father == NULL) {
			this->root = *it;
			break;
		}
	}
}

bool cmp::operator()(const RNode* a, const RNode* b) const
{
	if (a->data == b->data) return false;
	return a->data < b->data;
}
