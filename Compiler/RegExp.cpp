#include "RegExp.h"

bool RegExp::testWord(char c) {
	if (('a' <= c && c <= 'z') ||
		('A' <= c && c <= 'Z') ||
		('0' <= c && c <= '9') ||
		c == '_')
		return true;
	return false;
}

vector<string> RegExp::regLexer(string& s)
{
	vector<string> ret;
	int i = 0;
	string element = "";
	for (; i < s.size() - 1; i++) {
		char now = s[i];
		if (now == ' ' || now == '\n' || now == '\t') {
			if (element.size()) ret.push_back(element), element = "";
			continue;
		}
		bool isSingleword = false, isDoubleword = false;
		for (int j = 0; j < max(singleword.size(), doubleword.size()); j++) {
			if (j < singleword.size() && now == singleword[j]) {
				isSingleword = true;
				break;
			}
			if (j < doubleword.size() && now == doubleword[j]) {
				isDoubleword = true;
				break;
			}
		}
		if (isSingleword) {
			if (element.size()) ret.push_back(element), element = "";
			ret.push_back(string(1, now));
			continue;
		}
		if (isDoubleword) {
			if (element.size()) ret.push_back(element), element = "";
			char next = s[i + 1];
			string dw(1, now);
			if ((now == '/' && next == '*') || (now == '*' && next == '/')) {
				dw.push_back(next);
				i++;
			}
			if (now != '/' && now != '*' && next == '=') {
				dw.push_back(next);
				i++;
			}
			ret.push_back(dw);
			continue;
		}
		if ('0' <= now && now <= '9') {
			if (element.size()) ret.push_back(element), element = "";
			ret.push_back(string(1, now));
			continue;
		}
		if (testWord(now)) {
			element.push_back(now);
		}
		else {
			throw "Word error in regular definition. Unexpected character found: " + now;
		}
	}
	if (i == s.size() - 1) {
		ret.push_back(string(1, s[i]));
	}
	if (element.size()) ret.push_back(element);
	return ret;
}

void RegExp::insertRegDef(string def)
{
	string left = "", right = "";
	bool isLeft = true;
	vector<string> processedDef;
	try {
		processedDef = regLexer(def);
	}
	catch (const char* msg) {
		throw msg;
	}
	for (string element : processedDef) {
		if (element == ":") {
			isLeft = false;
			continue;
		}
		if (isLeft) left += element;
		else right += element;
	}
	this->regDef.insert(pair<string, string>(right, left));
}

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

string RegExp::join(vector<string>& strs)
{
	string ret = "";
	for (string element : strs) {
		ret += element;
	}
	return ret;
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

void RegExp::initRegDef(string* defs, int size)
{
	if (defs == NULL) {
		cout << "Please type in the regular definitions [form => d:r]: (type in 'end' to finish)" << endl;
		string def;
		while (getline(cin, def)) {
			if (def == "end") break;
			insertRegDef(def);
		}
	}
	else {
		for (int i = 0; i < size; i ++) {
			insertRegDef(defs[i]);
		}
	}
	for (m_iterator it = regDef.begin(); it != regDef.end(); it ++) {
		cout << it->first << " " << it->second << endl;
	}
}

void RegExp::createParseTree(string& regExp)
{
	vector<string> stk;
	vector<string> processedExp;
	try {
		processedExp = regLexer(regExp);
	}
	catch (const char* msg) {
		throw msg;
	}
	for (string element : processedExp) {
		stk.push_back(element); // 移入
		m_iterator reduced = regDef.find(join(stk));
		if (reduced != regDef.end()) {
			// 可以归约

		}
	}
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
