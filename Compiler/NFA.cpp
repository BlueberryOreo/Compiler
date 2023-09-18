#include "NFA.h"

int NFA::id = 0;

int NFA::getId()
{
	return id++;
}

NFA NFA::meta(RNode* node)
{
	NFA nfa{};
	nfa.start = new ANode(getId());
	nfa.end = new ANode(getId());
	nfa.start->append(node->data, nfa.end);
	return nfa;
}

NFA NFA::Concatenation(NFA &nfa1, NFA &nfa2)
{
	NFA ret{};
	for (int i = 0; i < nfa2.start->edges.size(); i ++) {
		// ��nfa2.start�����б߽ӵ�nfa1.end��
		nfa1.end->edges.push_back(nfa2.start->edges[i]);
	}
	//nfa2.end->id = nfa2.start->id;
	delete nfa2.start;
	ret.start = nfa1.start;
	ret.end = nfa2.end;
	return ret;
}

NFA NFA::Union(NFA& nfa1, NFA& nfa2)
{
	NFA ret{};
	ret.start = new ANode(getId());
	ret.end = new ANode(getId());
	ret.start->append(E, nfa1.start);
	ret.start->append(E, nfa2.start);
	nfa1.end->append(E, ret.end);
	nfa2.end->append(E, ret.end);
	return ret;
}

NFA NFA::Kleene(NFA& nfa)
{
	NFA ret{};
	ret.start = new ANode(getId());
	ret.end = new ANode(getId());
	ret.start->append(E, nfa.start);
	nfa.end->append(E, nfa.start);
	ret.start->append(E, ret.end);
	nfa.end->append(E, ret.end);
	return ret;
}

NFA NFA::MSE(RNode* now)
{
	if (now->children.size() == 0) {
		// ���ս����ֱ�ӹ����Զ���
		if (now->data == "|" || now->data == "*" || now->data == "(" || now->data == ")") return NFA();
		cout << "meta" << " " << now->data << endl;
		return meta(now);
	}
	int childNum = now->children.size();
	vector<NFA> nfas;
	char type = 'c'; // �������ͣ�c - ���ӣ�u - ����k - �հ�
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
		else if (child->data == "(" || child->data == ")") continue; // �����ţ��Թ�
		nfas.push_back(MSE(child));
	}

	if (nfas.size() == 1 && type != 'k') return nfas[0];

	if (type == 'c') {
		cout << type << " " << nfas.size() << endl;
		return Concatenation(nfas[0], nfas[1]);
	}
	else if (type == 'u') {
		cout << type << " " << nfas.size() << endl;
		return Union(nfas[0], nfas[1]);
	}
	else {
		cout << type << " " << nfas.size() << endl;
		return Kleene(nfas[0]);
	}
}

void NFA::outputNFA()
{
	queue<ANode*> q;
	q.push(start);
	map<int, int> visited;
	while (!q.empty()) {
		ANode* now = q.front();
		q.pop();
		if (visited[now->id]) continue;
		visited[now->id] = 1;
		cout << now->id << endl;
		for (int i = 0; i < now->edges.size(); i ++) {
			ANode* next = now->edges[i].to;
			if (visited[next->id]) continue;
			q.push(next);
		}
	}
}

void NFA::delNFA(ANode* delStart)
{
	stack<ANode*> stk; // ����ɾ����ջ
	queue<ANode*> q;
	q.push(delStart);
	while (!q.empty()) {
		ANode* now = q.front();
		q.pop();
		if (now->id < 0) continue;
		now->id = -1;
		for (int i = 0; i < now->edges.size(); i ++) {
			ANode* next = now->edges[i].to;
			if (next->id < 0) continue;
			q.push(next);
		}
		stk.push(now);
	}
	while (stk.size() > 0) {
		ANode* top = stk.top();
		stk.pop();
		delete top;
	}
}