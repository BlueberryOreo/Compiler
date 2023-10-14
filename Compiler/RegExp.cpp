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

void RegExp::constructTree(const string& postReg)
{
	//cout << postReg << endl;
	int cntR = 0;
	for (int i = 0; i < postReg.size(); i++) {
		//cout << "r" << i << " " << postReg[i] << endl;
		if (postReg[i] == '|') {
			root->children.push_back(new RNode("|", root));
			swap(root->children[1], root->children[2]);
		}
		else {
			if (root) {
				RNode* newRoot = new RNode("r" + to_string(cntR++));
				newRoot->children.push_back(root);
				root->father = newRoot;
				root = newRoot;
			}
			
			if (postReg[i] == '*') {
				root->children.push_back(new RNode("*", root));
			}
			else if (postReg[i] == '.') {
				if (i + 1 >= postReg.size()) break;
				RNode* tmp = new RNode("r" + to_string(cntR++), root);
				tmp->children.push_back(new RNode(string(1, postReg[i + 1]), tmp));
				root->children.push_back(tmp);
				i++;
			}
			else {
				RNode* tmp = new RNode("r" + to_string(cntR++));
				tmp->children.push_back(new RNode(string(1, postReg[i]), tmp));
				if (root) {
					root->children.push_back(tmp);
				}
				else {
					swap(root, tmp);
					delete tmp;
				}
			}
		}

		//cout << root->data << " -> ";
		//for (auto c : root->children) cout << c->data << " ";
		//cout << endl;
	}
}

void RegExp::outputTree(RNode* now)
{
	if (now == NULL) return;
	//cout << now->data << " ";
	//cout << now->data << " " << now->children.size() << endl;
	for (int i = 0; i < now->children.size(); i ++) {
		RNode* child = now->children[i];
		//cout << now->data << " " << child->data << endl;
		outputTree(child);
	}
}

RegExp::RegExp(const string &name)
{
	//cout << infixToPostfix("(a|b)*abb") << endl;
	this->name = name;
	this->root = NULL; 
}

RegExp::~RegExp()
{
	if(root) deleteTree(root);
	NFA::delNFA(this->nfa.start);
}

void RegExp::init()
{
	//this->initRegDef(defs, size); 
	//if (file_path.size()) {
	//	int res = this->readFromFile(file_path);
	//	if (res) throw file_path + " not found";
	//}
	//else {
	//	this->readTree(); // 读入一棵树
	//}
	//this->constructTree(infixToPostfix("+|-|*|(|)|{|}|:|,|;"));
	//return;
	//this->outputTree(this->root);
	//cout << endl;
	//return;
	set<string> input; // 存储输入字符
	this->nfa = NFA::MYT(this->root, input);
	this->nfa.end->isEnd = true;
	//for (s_Siterator it = input.begin(); it != input.end(); it++) {
	//	cout << *it << " ";
	//}
	//cout << endl;
	////this->nfa.outputNFA();
	//cout << this->nfa.size << endl;
	//DFA dfa(nfa, input);
	dfa.createDFA(nfa, input);
	//cout << dfa.size() << endl;
	//dfa.outputDFA();
	dfa.simplify();
	dfa.outputDFA();
}

void RegExp::readTree()
{
	cout << "请输入一棵树，按照父-子节点，一行一行输入，父节点和子节点之间用空格分开。请从左往右输入" << endl;
	set<RNode*, cmp> nodes;
	string f, c;
	while (cin >> f >> c) {
		RNode* father = new RNode(f);
		RNode* child = new RNode(c);
		s_Riterator tmp = nodes.find(father);
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
	for (s_Riterator it = nodes.begin(); it != nodes.end(); it ++) {
		if ((*it)->father == NULL) {
			this->root = *it;
			break;
		}
	}
}

int RegExp::readFromFile(ifstream &ifs)
{

	set<RNode*, cmp> nodes;
	string f, c;
	while (ifs >> f >> c) {
		RNode* father = new RNode(f);
		RNode* child = new RNode(c);
		s_Riterator tmp = nodes.find(father);
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
	for (s_Riterator it = nodes.begin(); it != nodes.end(); it++) {
		if ((*it)->father == NULL) {
			this->root = *it;
			break;
		}
	}

	ifs.close();
	return 0;
}

bool RegExp::match(const string& pattern)
{
	DNode now = this->dfa.getStart();
	bool matched = false;
	int i;
	for (i = 0; i < pattern.size(); i ++) {
		DNode next = this->dfa.move(now, string(1, pattern[i]));
		if (next.empty() || dfa.isDead(next)) {
			//throw "Unexpected input char: " + pattern[i];
			return false;
		}
		//else if(next.isEnd){
		//	matched = true;
		//}
		else {
			now = next;
		}
	}
	if (!now.isEnd) return false;
	return true;
}

bool RegExp::matchNext(char c)
{
	if (pointer.empty()) pointer = dfa.getStart();
	pointer = dfa.move(pointer, string(1, c));
	if (pointer.empty() || dfa.isDead(pointer)) return false;
	if (pointer.isEnd) {
		pointer = dfa.getStart();
		return true;
	}
	return false;
}

string RegExp::getName()
{
	return this->name;
}

void RegExp::setName(const string &name)
{
	this->name = name;
}

bool cmp::operator()(const RNode* a, const RNode* b) const
{
	if (a->data == b->data) return false;
	return a->data < b->data;
}
