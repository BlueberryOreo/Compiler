#include "NFA.h"

int NFA::id = 1;

int NFA::getId()
{
	return id++;
}

NFA NFA::meta(RNode* node)
{
	NFA nfa(2);
	nfa.start = new NNode(getId());
	nfa.end = new NNode(getId());
	nfa.start->append(node->data, nfa.end);
	return nfa;
}

NFA NFA::Concatenation(NFA &nfa1, NFA &nfa2)
{
	NFA ret;
	for (int i = 0; i < nfa2.start->edges.size(); i ++) {
		// 将nfa2.start的所有边接到nfa1.end上
		nfa1.end->edges.push_back(nfa2.start->edges[i]);
	}
	//nfa2.end->id = nfa2.start->id;
	delete nfa2.start;
	ret.start = nfa1.start;
	ret.end = nfa2.end;
	ret.size = nfa1.size + nfa2.size - 1;
	return ret;
}

NFA NFA::Union(NFA& nfa1, NFA& nfa2)
{
	NFA ret(2);
	ret.start = new NNode(getId());
	ret.end = new NNode(getId());
	ret.start->append(E, nfa1.start);
	ret.start->append(E, nfa2.start);
	nfa1.end->append(E, ret.end);
	nfa2.end->append(E, ret.end);
	ret.size += nfa1.size + nfa2.size;
	return ret;
}

NFA NFA::Kleene(NFA& nfa)
{
	NFA ret(2);
	ret.start = new NNode(getId());
	ret.end = new NNode(getId());
	ret.start->append(E, nfa.start);
	nfa.end->append(E, nfa.start);
	ret.start->append(E, ret.end);
	nfa.end->append(E, ret.end);
	ret.size += nfa.size;
	return ret;
}

NFA::NFA(int size, NNode* start, NNode* end)
{
	this->size = size;
	this->start = start;
	this->end = end;
}

NFA NFA::MYT(RNode* now, set<string> &input)
{
	if (now->children.size() == 0) {
		// 是终结符，直接构造自动机
		if (now->data == "|" || now->data == "*" || now->data == "(" || now->data == ")") return NFA();
		//cout << "meta" << " " << now->data << endl;
		input.insert(now->data);
		return meta(now);
	}
	int childNum = now->children.size();
	vector<NFA> nfas;
	char type = 'c'; // 构造类型，c - 连接，u - 并，k - 闭包
	for (int i = 0; i < childNum; i ++) {
		RNode* child = now->children[i];
		if (child->data == "|") {
			type = 'u';
			continue;
		}
		else if (child->data == "*") {
			type = 'k';
			continue;
		}
		else if (child->data == "(" || child->data == ")") continue; // 是括号，略过
		nfas.push_back(MYT(child, input));
	}

	if (nfas.size() == 1 && type != 'k') return nfas[0];

	if (type == 'c') {
		//cout << type << " " << nfas.size() << endl;
		return Concatenation(nfas[0], nfas[1]);
	}
	else if (type == 'u') {
		//cout << type << " " << nfas.size() << endl;
		return Union(nfas[0], nfas[1]);
	}
	else {
		//cout << type << " " << nfas.size() << endl;
		return Kleene(nfas[0]);
	}
}

void NFA::outputNFA()
{
	queue<NNode*> q;
	q.push(start);
	map<int, bool> visited;
	while (!q.empty()) {
		NNode* now = q.front();
		q.pop();
		if (visited[now->id]) continue;
		visited[now->id] = 1;
		cout << now->id << " " << now->edges.size() << endl;
		for (int i = 0; i < now->edges.size(); i ++) {
			NNode* next = now->edges[i].to;
			if (visited[next->id]) continue;
			q.push(next);
		}
	}
}

void NFA::delNFA(NNode* delStart)
{
	if (!delStart) return;
	stack<NNode*> stk; // 用于删除的栈
	queue<NNode*> q;
	q.push(delStart);
	while (!q.empty()) {
		NNode* now = q.front();
		q.pop();
		if (now->id < 0) continue;
		now->id = -1;
		for (int i = 0; i < now->edges.size(); i ++) {
			NNode* next = now->edges[i].to;
			if (next->id < 0) continue;
			q.push(next);
		}
		stk.push(now);
	}
	while (stk.size() > 0) {
		NNode* top = stk.top();
		stk.pop();
		delete top;
	}
}
